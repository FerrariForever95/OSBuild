// =============================================================================
//  zfs_lfs.c  —  ZENO OS private kernel filesystem implementation
//
//  Flash "zfs" partition  →  LittleFS2  →  this file  →  modzfs.c  →  Python
//
//  * Never touches MicroPython VFS.
//  * Never mounts to a filesystem path.
//  * All operations protected by a FreeRTOS mutex.
//
//  Key design notes:
//    - MicroPython's LittleFS2 does NOT have lfs2_file_open().
//      It uses lfs2_file_opencfg() which requires an lfs2_file_config struct
//      with per-file cache buffers.  We supply those from the heap.
//    - struct lfs2_config has no typedef in MicroPython's lfs2.h — we add
//      our own in zfs_lfs.h using a uniquely-named guard macro.
//    - zfs_lfs.c must NOT re-include lfs2.h directly; it arrives via zfs_lfs.h.
//
//  Target: ESP-IDF v5.5.x, MicroPython ESP32-S3 port.
// =============================================================================

/*
 * Include zfs_lfs.h FIRST — it pulls in lfs2_util.h then lfs2.h in the
 * correct order and adds the lfs2_config_t / lfs2_file_t / lfs2_dir_t
 * typedefs that the rest of this file uses.
 */
#include "zfs_lfs.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* ESP-IDF */
#include "esp_partition.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/* --------------------------------------------------------------------------
 * Constants
 * ----------------------------------------------------------------------- */

#define TAG             "ZFS"
#define ZFS_PART_LABEL  "zfs"

/* Must match ESP32 flash erase granularity (4 KB) */
#define ZFS_BLOCK_SIZE      4096u
#define ZFS_READ_SIZE       256u
#define ZFS_PROG_SIZE       256u
#define ZFS_LOOKAHEAD_SIZE  64u
#define ZFS_CACHE_SIZE      ZFS_PROG_SIZE   /* must equal prog_size */

/* --------------------------------------------------------------------------
 * Internal state
 *
 * cfg is typed as  struct lfs2_config  (plain struct tag — MicroPython's
 * lfs2.h has no typedef).  zfs_lfs.h adds  lfs2_config_t  as a typedef so
 * both spellings work here.
 * ----------------------------------------------------------------------- */

typedef struct {
    const esp_partition_t *part;
    lfs2_t                 lfs;
    struct lfs2_config     cfg;     /* plain struct tag — always safe */

    /* Static scratch buffers — lfs2_config_t points into these */
    uint8_t                read_buf[ZFS_CACHE_SIZE];
    uint8_t                prog_buf[ZFS_CACHE_SIZE];
    uint8_t                lookahead_buf[ZFS_LOOKAHEAD_SIZE];

    bool               mounted;
    SemaphoreHandle_t  lock;
} zfs_state_t;

static zfs_state_t s_zfs;   /* zero-initialised by C runtime */

/* --------------------------------------------------------------------------
 * Locking helpers
 * ----------------------------------------------------------------------- */

#define ZFS_LOCK()    xSemaphoreTake(s_zfs.lock, portMAX_DELAY)
#define ZFS_UNLOCK()  xSemaphoreGive(s_zfs.lock)

/* --------------------------------------------------------------------------
 * LittleFS2 error → zfs_err_t
 * ----------------------------------------------------------------------- */

static zfs_err_t _lfs_err(int rc) {
    switch (rc) {
        case LFS2_ERR_OK:          return ZFS_OK;
        case LFS2_ERR_IO:          return ZFS_ERR_IO;
        case LFS2_ERR_CORRUPT:     return ZFS_ERR_LFS;
        case LFS2_ERR_NOENT:       return ZFS_ERR_NOENT;
        case LFS2_ERR_EXIST:       return ZFS_ERR_EXIST;
        case LFS2_ERR_NOTDIR:      return ZFS_ERR_NOTDIR;
        case LFS2_ERR_ISDIR:       return ZFS_ERR_ISDIR;
        case LFS2_ERR_NOTEMPTY:    return ZFS_ERR_NOTEMPTY;
        case LFS2_ERR_BADF:        return ZFS_ERR_INVAL;
        case LFS2_ERR_NOMEM:       return ZFS_ERR_NOMEM;
        case LFS2_ERR_NAMETOOLONG: return ZFS_ERR_NAMETOOLONG;
        default:                   return ZFS_ERR_LFS;
    }
}

/* --------------------------------------------------------------------------
 * Block-device callbacks — called by LittleFS2 core
 * ----------------------------------------------------------------------- */

static int _lfs_read(const struct lfs2_config *c,
                     lfs2_block_t block, lfs2_off_t off,
                     void *buf, lfs2_size_t size)
{
    uint32_t addr = (uint32_t)(block * c->block_size + off);
    esp_err_t err = esp_partition_read(s_zfs.part, addr, buf, (size_t)size);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "read err addr=0x%"PRIx32" sz=%"PRIu32" esp=0x%x",
                 addr, (uint32_t)size, err);
        return LFS2_ERR_IO;
    }
    return LFS2_ERR_OK;
}

static int _lfs_prog(const struct lfs2_config *c,
                     lfs2_block_t block, lfs2_off_t off,
                     const void *buf, lfs2_size_t size)
{
    uint32_t addr = (uint32_t)(block * c->block_size + off);
    esp_err_t err = esp_partition_write(s_zfs.part, addr, buf, (size_t)size);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "write err addr=0x%"PRIx32" sz=%"PRIu32" esp=0x%x",
                 addr, (uint32_t)size, err);
        return LFS2_ERR_IO;
    }
    return LFS2_ERR_OK;
}

static int _lfs_erase(const struct lfs2_config *c, lfs2_block_t block)
{
    uint32_t addr = (uint32_t)(block * c->block_size);
    esp_err_t err = esp_partition_erase_range(s_zfs.part, addr,
                                              (size_t)c->block_size);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "erase err blk=%"PRIu32" esp=0x%x",
                 (uint32_t)block, err);
        return LFS2_ERR_IO;
    }
    return LFS2_ERR_OK;
}

static int _lfs_sync(const struct lfs2_config *c)
{
    (void)c;
    return LFS2_ERR_OK;  /* ESP-IDF writes are synchronous */
}

/* --------------------------------------------------------------------------
 * One-time initialisation  (idempotent — safe to call repeatedly)
 *
 * IMPORTANT: this must be called at the top of EVERY public entry point,
 * before ZFS_LOCK(), not just mount()/format(). s_zfs is a zero-initialised
 * static, so on a fresh boot s_zfs.lock is NULL. If any public function
 * calls ZFS_LOCK() (xSemaphoreTake) before the mutex has been created —
 * e.g. calling zfs.info() or zfs.listdir() before zfs.mount() — it takes a
 * NULL semaphore handle and crashes (LoadProhibited / null-deref).
 * ----------------------------------------------------------------------- */

static zfs_err_t _zfs_init_once(void)
{
    /* Create mutex on first call */
    if (s_zfs.lock == NULL) {
        s_zfs.lock = xSemaphoreCreateMutex();
        if (!s_zfs.lock) {
            ESP_LOGE(TAG, "mutex alloc failed");
            return ZFS_ERR_NOMEM;
        }
    }

    /* Locate partition on first call */
    if (s_zfs.part == NULL) {
        s_zfs.part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA,
                                              ESP_PARTITION_SUBTYPE_ANY,
                                              ZFS_PART_LABEL);
        if (!s_zfs.part) {
            ESP_LOGE(TAG, "partition \"%s\" not found", ZFS_PART_LABEL);
            return ZFS_ERR_NO_PART;
        }
        ESP_LOGI(TAG, "partition found: offset=0x%"PRIx32" size=0x%"PRIx32,
                 s_zfs.part->address, s_zfs.part->size);
    }

    /* Fill lfs2_config (idempotent) */
    struct lfs2_config *c = &s_zfs.cfg;
    memset(c, 0, sizeof(*c));

    c->read        = _lfs_read;
    c->prog        = _lfs_prog;
    c->erase       = _lfs_erase;
    c->sync        = _lfs_sync;

    c->read_size      = ZFS_READ_SIZE;
    c->prog_size      = ZFS_PROG_SIZE;
    c->block_size     = ZFS_BLOCK_SIZE;
    c->block_count    = s_zfs.part->size / ZFS_BLOCK_SIZE;
    c->cache_size     = ZFS_CACHE_SIZE;
    c->lookahead_size = ZFS_LOOKAHEAD_SIZE;
    c->block_cycles   = 500;   /* wear-levelling interval */

    c->read_buffer      = s_zfs.read_buf;
    c->prog_buffer      = s_zfs.prog_buf;
    c->lookahead_buffer = s_zfs.lookahead_buf;

    return ZFS_OK;
}

/* --------------------------------------------------------------------------
 * Helper: open a file using lfs2_file_opencfg()
 *
 * MicroPython's LittleFS2 does NOT have lfs2_file_open().
 * lfs2_file_opencfg() is the correct API — it requires a per-file
 * lfs2_file_config with a caller-supplied cache buffer.
 *
 * Caller must call lfs2_file_close() when done.
 * out_fcfg must remain valid until lfs2_file_close().
 * out_filebuf must be ZFS_CACHE_SIZE bytes allocated by the caller.
 * ----------------------------------------------------------------------- */

static int _open_file(lfs2_file_t *f, struct lfs2_file_config *fcfg,
                      uint8_t *filebuf,
                      const char *path, int flags)
{
    memset(fcfg, 0, sizeof(*fcfg));
    fcfg->buffer = filebuf;
    return lfs2_file_opencfg(&s_zfs.lfs, f, path, flags, fcfg);
}

/* --------------------------------------------------------------------------
 * Public API — Lifecycle
 * ----------------------------------------------------------------------- */

zfs_err_t zfs_lfs_mount(void)
{
    zfs_err_t rc = _zfs_init_once();
    if (rc != ZFS_OK) return rc;

    ZFS_LOCK();
    if (s_zfs.mounted) { ZFS_UNLOCK(); return ZFS_ERR_ALREADY_MNT; }

    int lrc = lfs2_mount(&s_zfs.lfs, &s_zfs.cfg);
    if (lrc < 0) {
        ESP_LOGE(TAG, "lfs2_mount failed (%d) — may need format", lrc);
        ZFS_UNLOCK();
        return _lfs_err(lrc);
    }

    s_zfs.mounted = true;
    ESP_LOGI(TAG, "mounted: %"PRIu32" blocks x %"PRIu32" B",
             (uint32_t)s_zfs.cfg.block_count,
             (uint32_t)s_zfs.cfg.block_size);
    ZFS_UNLOCK();
    return ZFS_OK;
}

zfs_err_t zfs_lfs_umount(void)
{
    zfs_err_t rc = _zfs_init_once();
    if (rc != ZFS_OK) return rc;

    ZFS_LOCK();
    if (!s_zfs.mounted) { ZFS_UNLOCK(); return ZFS_ERR_NOT_MOUNTED; }

    int lrc = lfs2_unmount(&s_zfs.lfs);
    s_zfs.mounted = false;
    ZFS_UNLOCK();

    if (lrc < 0) return _lfs_err(lrc);
    ESP_LOGI(TAG, "unmounted");
    return ZFS_OK;
}

zfs_err_t zfs_lfs_format(void)
{
    zfs_err_t rc = _zfs_init_once();
    if (rc != ZFS_OK) return rc;

    ZFS_LOCK();
    if (s_zfs.mounted) {
        lfs2_unmount(&s_zfs.lfs);
        s_zfs.mounted = false;
    }

    int lrc = lfs2_format(&s_zfs.lfs, &s_zfs.cfg);
    ZFS_UNLOCK();

    if (lrc < 0) {
        ESP_LOGE(TAG, "lfs2_format failed (%d)", lrc);
        return _lfs_err(lrc);
    }
    ESP_LOGI(TAG, "format complete");
    return ZFS_OK;
}

zfs_err_t zfs_lfs_info(zfs_info_t *out)
{
    if (!out) return ZFS_ERR_INVAL;

    zfs_err_t rc = _zfs_init_once();
    if (rc != ZFS_OK) return rc;

    ZFS_LOCK();
    out->mounted          = s_zfs.mounted;
    out->block_size       = (uint32_t)s_zfs.cfg.block_size;
    out->block_count      = (uint32_t)s_zfs.cfg.block_count;
    out->blocks_used      = 0;
    out->partition_offset = s_zfs.part ? s_zfs.part->address : 0u;
    out->partition_size   = s_zfs.part ? s_zfs.part->size    : 0u;

    if (s_zfs.mounted) {
        lfs2_ssize_t used = lfs2_fs_size(&s_zfs.lfs);
        if (used >= 0) out->blocks_used = (uint32_t)used;
    }
    ZFS_UNLOCK();
    return ZFS_OK;
}

/* --------------------------------------------------------------------------
 * Public API — File I/O
 * ----------------------------------------------------------------------- */

zfs_err_t zfs_lfs_write(const char *path, const uint8_t *data, size_t len)
{
    if (!path || (!data && len > 0)) return ZFS_ERR_INVAL;

    zfs_err_t irc = _zfs_init_once();
    if (irc != ZFS_OK) return irc;

    ZFS_LOCK();
    if (!s_zfs.mounted) { ZFS_UNLOCK(); return ZFS_ERR_NOT_MOUNTED; }

    /* Per-file cache buffer required by lfs2_file_opencfg */
    uint8_t *filebuf = (uint8_t *)malloc(ZFS_CACHE_SIZE);
    if (!filebuf) { ZFS_UNLOCK(); return ZFS_ERR_NOMEM; }

    lfs2_file_t            f;
    struct lfs2_file_config fcfg;
    int lrc = _open_file(&f, &fcfg, filebuf, path,
                         LFS2_O_WRONLY | LFS2_O_CREAT | LFS2_O_TRUNC);
    if (lrc < 0) {
        free(filebuf);
        ZFS_UNLOCK();
        return _lfs_err(lrc);
    }

    zfs_err_t rc = ZFS_OK;
    if (len > 0) {
        lfs2_ssize_t written = lfs2_file_write(&s_zfs.lfs, &f, data,
                                               (lfs2_size_t)len);
        if (written < 0) {
            rc = _lfs_err((int)written);
        } else if ((size_t)written != len) {
            rc = ZFS_ERR_IO;
        }
    }

    int crc = lfs2_file_close(&s_zfs.lfs, &f);
    if (rc == ZFS_OK && crc < 0) rc = _lfs_err(crc);

    free(filebuf);
    ZFS_UNLOCK();
    return rc;
}

zfs_err_t zfs_lfs_read(const char *path, uint8_t **out_data, size_t *out_len)
{
    if (!path || !out_data || !out_len) return ZFS_ERR_INVAL;
    *out_data = NULL;
    *out_len  = 0;

    zfs_err_t irc = _zfs_init_once();
    if (irc != ZFS_OK) return irc;

    ZFS_LOCK();
    if (!s_zfs.mounted) { ZFS_UNLOCK(); return ZFS_ERR_NOT_MOUNTED; }

    /* Stat to get size and confirm it is a regular file */
    struct lfs2_info info;
    int lrc = lfs2_stat(&s_zfs.lfs, path, &info);
    if (lrc < 0) { ZFS_UNLOCK(); return _lfs_err(lrc); }
    if (info.type != LFS2_TYPE_REG) { ZFS_UNLOCK(); return ZFS_ERR_ISDIR; }

    size_t   fsize   = (size_t)info.size;
    uint8_t *databuf = (uint8_t *)malloc(fsize + 1);  /* +1 for NUL */
    if (!databuf) { ZFS_UNLOCK(); return ZFS_ERR_NOMEM; }

    uint8_t *filebuf = (uint8_t *)malloc(ZFS_CACHE_SIZE);
    if (!filebuf) { free(databuf); ZFS_UNLOCK(); return ZFS_ERR_NOMEM; }

    lfs2_file_t            f;
    struct lfs2_file_config fcfg;
    lrc = _open_file(&f, &fcfg, filebuf, path, LFS2_O_RDONLY);
    if (lrc < 0) {
        free(filebuf); free(databuf);
        ZFS_UNLOCK();
        return _lfs_err(lrc);
    }

    zfs_err_t rc = ZFS_OK;
    if (fsize > 0) {
        lfs2_ssize_t got = lfs2_file_read(&s_zfs.lfs, &f, databuf,
                                          (lfs2_size_t)fsize);
        if (got < 0) {
            rc = _lfs_err((int)got);
            free(databuf);
            databuf = NULL;
        } else {
            databuf[got] = '\0';
            *out_len = (size_t)got;
        }
    } else {
        databuf[0] = '\0';
        *out_len   = 0;
    }

    lfs2_file_close(&s_zfs.lfs, &f);
    free(filebuf);
    ZFS_UNLOCK();

    if (rc == ZFS_OK) *out_data = databuf;
    return rc;
}

zfs_err_t zfs_lfs_delete(const char *path)
{
    if (!path) return ZFS_ERR_INVAL;

    zfs_err_t irc = _zfs_init_once();
    if (irc != ZFS_OK) return irc;

    ZFS_LOCK();
    if (!s_zfs.mounted) { ZFS_UNLOCK(); return ZFS_ERR_NOT_MOUNTED; }
    int lrc = lfs2_remove(&s_zfs.lfs, path);
    ZFS_UNLOCK();
    return (lrc < 0) ? _lfs_err(lrc) : ZFS_OK;
}

zfs_err_t zfs_lfs_exists(const char *path, bool *out)
{
    if (!path || !out) return ZFS_ERR_INVAL;

    zfs_err_t irc = _zfs_init_once();
    if (irc != ZFS_OK) return irc;

    ZFS_LOCK();
    if (!s_zfs.mounted) { ZFS_UNLOCK(); return ZFS_ERR_NOT_MOUNTED; }
    struct lfs2_info info;
    int lrc = lfs2_stat(&s_zfs.lfs, path, &info);
    ZFS_UNLOCK();
    if (lrc == LFS2_ERR_NOENT) { *out = false; return ZFS_OK; }
    if (lrc < 0)               { return _lfs_err(lrc); }
    *out = true;
    return ZFS_OK;
}

zfs_err_t zfs_lfs_rename(const char *oldpath, const char *newpath)
{
    if (!oldpath || !newpath) return ZFS_ERR_INVAL;

    zfs_err_t irc = _zfs_init_once();
    if (irc != ZFS_OK) return irc;

    ZFS_LOCK();
    if (!s_zfs.mounted) { ZFS_UNLOCK(); return ZFS_ERR_NOT_MOUNTED; }
    int lrc = lfs2_rename(&s_zfs.lfs, oldpath, newpath);
    ZFS_UNLOCK();
    return (lrc < 0) ? _lfs_err(lrc) : ZFS_OK;
}

/* --------------------------------------------------------------------------
 * Public API — Directory
 * ----------------------------------------------------------------------- */

zfs_err_t zfs_lfs_mkdir(const char *path)
{
    if (!path) return ZFS_ERR_INVAL;

    zfs_err_t irc = _zfs_init_once();
    if (irc != ZFS_OK) return irc;

    ZFS_LOCK();
    if (!s_zfs.mounted) { ZFS_UNLOCK(); return ZFS_ERR_NOT_MOUNTED; }
    int lrc = lfs2_mkdir(&s_zfs.lfs, path);
    ZFS_UNLOCK();
    return (lrc < 0) ? _lfs_err(lrc) : ZFS_OK;
}

zfs_err_t zfs_lfs_rmdir(const char *path)
{
    if (!path) return ZFS_ERR_INVAL;

    zfs_err_t irc = _zfs_init_once();
    if (irc != ZFS_OK) return irc;

    ZFS_LOCK();
    if (!s_zfs.mounted) { ZFS_UNLOCK(); return ZFS_ERR_NOT_MOUNTED; }
    /* lfs2_remove handles empty directories */
    int lrc = lfs2_remove(&s_zfs.lfs, path);
    ZFS_UNLOCK();
    return (lrc < 0) ? _lfs_err(lrc) : ZFS_OK;
}

zfs_err_t zfs_lfs_listdir(const char *path,
                           zfs_listdir_cb_t cb, void *userdata)
{
    if (!path || !cb) return ZFS_ERR_INVAL;

    zfs_err_t irc = _zfs_init_once();
    if (irc != ZFS_OK) return irc;

    ZFS_LOCK();
    if (!s_zfs.mounted) { ZFS_UNLOCK(); return ZFS_ERR_NOT_MOUNTED; }

    lfs2_dir_t dir;
    int lrc = lfs2_dir_open(&s_zfs.lfs, &dir, path);
    if (lrc < 0) { ZFS_UNLOCK(); return _lfs_err(lrc); }

    zfs_err_t    rc = ZFS_OK;
    struct lfs2_info info;

    while (true) {
        lrc = lfs2_dir_read(&s_zfs.lfs, &dir, &info);
        if (lrc == 0) break;   /* end of directory */
        if (lrc < 0) { rc = _lfs_err(lrc); break; }

        /* skip "." and ".." */
        if (strcmp(info.name, ".") == 0 || strcmp(info.name, "..") == 0)
            continue;

        zfs_dirent_t ent;
        memset(&ent, 0, sizeof(ent));
        strncpy(ent.name, info.name, ZFS_NAME_MAX);
        ent.name[ZFS_NAME_MAX] = '\0';
        ent.type = (uint8_t)info.type;
        ent.size = (info.type == LFS2_TYPE_REG) ? (uint32_t)info.size : 0u;

        if (cb(&ent, userdata) != 0) break;
    }

    lfs2_dir_close(&s_zfs.lfs, &dir);
    ZFS_UNLOCK();
    return rc;
}

/* --------------------------------------------------------------------------
 * Error string
 * ----------------------------------------------------------------------- */

const char *zfs_lfs_strerror(zfs_err_t err)
{
    switch (err) {
        case ZFS_OK:              return "ok";
        case ZFS_ERR_NOT_MOUNTED: return "not mounted";
        case ZFS_ERR_ALREADY_MNT: return "already mounted";
        case ZFS_ERR_NO_PART:     return "partition not found";
        case ZFS_ERR_LFS:         return "littlefs error";
        case ZFS_ERR_IO:          return "flash I/O error";
        case ZFS_ERR_INVAL:       return "invalid argument";
        case ZFS_ERR_NOENT:       return "no such file or directory";
        case ZFS_ERR_EXIST:       return "already exists";
        case ZFS_ERR_NOMEM:       return "out of memory";
        case ZFS_ERR_NOTDIR:      return "not a directory";
        case ZFS_ERR_ISDIR:       return "is a directory";
        case ZFS_ERR_NOTEMPTY:    return "directory not empty";
        case ZFS_ERR_NAMETOOLONG: return "name too long";
        default:                  return "unknown error";
    }
}
