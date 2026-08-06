//
// Content for MICROPY_MODULE_FROZEN_STR
//
#include <stdint.h>
#define MP_FROZEN_STR_NAMES \

const uint32_t mp_frozen_str_sizes[] = { 0 };
const char mp_frozen_str_content[] = {
"\0"
};

//
// Content for MICROPY_MODULE_FROZEN_MPY
//
#include "py/mpconfig.h"
#include "py/objint.h"
#include "py/objstr.h"
#include "py/emitglue.h"
#include "py/nativeglue.h"

#if MICROPY_LONGINT_IMPL != 2
#error "incompatible MICROPY_LONGINT_IMPL"
#endif

#if MPZ_DIG_SIZE != 16
#error "incompatible MPZ_DIG_SIZE"
#endif

#if MICROPY_PY_BUILTINS_FLOAT
typedef struct _mp_obj_float_t {
    mp_obj_base_t base;
    mp_float_t value;
} mp_obj_float_t;
#endif

#if MICROPY_PY_BUILTINS_COMPLEX
typedef struct _mp_obj_complex_t {
    mp_obj_base_t base;
    mp_float_t real;
    mp_float_t imag;
} mp_obj_complex_t;
#endif

enum {
    MP_QSTR__0x0a__bracket_open_recover_bracket_close__space_Interrupted_dot_ = MP_QSTRnumber_of,
    MP_QSTR__0x0a__bracket_open_recovery_bracket_close__space_Exiting_dot_,
    MP_QSTR__0x0a__bracket_open_recovery_bracket_close__space_Interrupted_dot_,
    MP_QSTR__0x0a__brace_open__colon__comma_d_brace_close_k_space_free,
    MP_QSTR__0x0a__brace_open__brace_close_,
    MP_QSTR__0x0d__0x0a__0x0d__0x0a_,
    MP_QSTR__0x1b__bracket_open_2J_0x1b__bracket_open_H,
    MP_QSTR__space__space__space__space__space__space_,
    MP_QSTR__space__space__space__space__space__space__space__space__space__space__percent_s,
    MP_QSTR__space__space__space__space__lt_dir_gt__space__percent_s,
    MP_QSTR__space__space__brace_open__brace_close__brace_open__colon_28_brace_close__space__brace_open__colon__gt_10_brace_close_,
    MP_QSTR__space__space__brace_open__brace_close__brace_open__brace_close_,
    MP_QSTR__bang_BH,
    MP_QSTR__bang_H,
    MP_QSTR__bang_I,
    MP_QSTR__hash_,
    MP_QSTR__percent__space_9d_space__percent_s,
    MP_QSTR__paren_open_Empty_space_line_space_to_space_quit_paren_close_,
    MP_QSTR__hyphen_,
    MP_QSTR__hyphen_l,
    MP_QSTR__hyphen_r,
    MP_QSTR__hyphen_rf,
    MP_QSTR__dot_,
    MP_QSTR__dot__dot_,
    MP_QSTR__dot__slash_boot_dot_py,
    MP_QSTR__dot__slash_webrepl_cfg_dot_py,
    MP_QSTR__dot_json,
    MP_QSTR__dot_mpy,
    MP_QSTR__dot_py,
    MP_QSTR__dot_tmp,
    MP_QSTR__slash__hyphen__slash_raw_slash_,
    MP_QSTR__slash_rom,
    MP_QSTR_0_0x0d__0x0a__0x0d__0x0a_,
    MP_QSTR_0_dot_0_dot_0_dot_0,
    MP_QSTR_0_dot_1_dot_0,
    MP_QSTR_0_dot_1_dot_1,
    MP_QSTR_0_dot_10_dot_2,
    MP_QSTR_0_dot_2_dot_1,
    MP_QSTR_0_dot_4_dot_2,
    MP_QSTR_0_dot_6_dot_1,
    MP_QSTR_0_dot_8_dot_0,
    MP_QSTR_1_dot_0_dot_2,
    MP_QSTR_1_dot_7_dot_0,
    MP_QSTR__colon_,
    MP_QSTR__lt_DIR_gt__space_,
    MP_QSTR__gt__space_,
    MP_QSTR_AIOESPNow,
    MP_QSTR_APA106,
    MP_QSTR_Authorization,
    MP_QSTR_BASE,
    MP_QSTR_Basic_space__brace_open__brace_close_,
    MP_QSTR_CLEAR,
    MP_QSTR_CONFIG,
    MP_QSTR_CRC_space_error,
    MP_QSTR_Confirm_space_password_colon__space_,
    MP_QSTR_Connection,
    MP_QSTR_Content_hyphen_Length,
    MP_QSTR_Content_hyphen_Type,
    MP_QSTR_Copied_space__brace_open__brace_close__space__hyphen__gt__space__brace_open__brace_close_,
    MP_QSTR_Copying_colon_,
    MP_QSTR_Counter,
    MP_QSTR_Created_colon_,
    MP_QSTR_D,
    MP_QSTR_DEBUG,
    MP_QSTR_DELAY,
    MP_QSTR_DELETE,
    MP_QSTR_DHT11,
    MP_QSTR_DHT22,
    MP_QSTR_DHTBase,
    MP_QSTR_DOWN,
    MP_QSTR_DS18X20,
    MP_QSTR_Directory_space_not_space_found_colon__space__brace_open__brace_close_,
    MP_QSTR_Done,
    MP_QSTR_Downloading_space__brace_open__brace_close__space_to_space__brace_open__brace_close_,
    MP_QSTR_E,
    MP_QSTR_ESPNow,
    MP_QSTR_Encoder,
    MP_QSTR_Error,
    MP_QSTR_Event,
    MP_QSTR_Exists_colon_,
    MP_QSTR_FALLING,
    MP_QSTR_File_space_not_space_found_colon__space__brace_open__brace_close__space__brace_open__brace_close_,
    MP_QSTR_Free_space__space__colon__space__brace_open__brace_close_,
    MP_QSTR_GET,
    MP_QSTR_HEAD,
    MP_QSTR_Host,
    MP_QSTR_IOQueue,
    MP_QSTR_Installing_space__brace_open__brace_close__space_to_space__brace_open__brace_close_,
    MP_QSTR_Invalid_space_password_space_length,
    MP_QSTR_LS,
    MP_QSTR_Lock,
    MP_QSTR_Lock_space_not_space_acquired,
    MP_QSTR_Loop,
    MP_QSTR_MATCH_ROM,
    MP_QSTR_MQTTClient,
    MP_QSTR_MQTTException,
    MP_QSTR_Man,
    MP_QSTR_Moved_space__brace_open__brace_close__space__hyphen__gt__space__brace_open__brace_close_,
    MP_QSTR_NeoPixel,
    MP_QSTR_Not_space_a_space_directory_colon__space__brace_open__brace_close_,
    MP_QSTR_NotADirectoryError,
    MP_QSTR_ORDER,
    MP_QSTR_OneWire,
    MP_QSTR_OneWireError,
    MP_QSTR_PASS,
    MP_QSTR_PASS_space__equals__space__percent_r_0x0a_,
    MP_QSTR_PATCH,
    MP_QSTR_PCNT_space_in_space_use,
    MP_QSTR_POST,
    MP_QSTR_PROMPT,
    MP_QSTR_PUT,
    MP_QSTR_PWD,
    MP_QSTR_Package_space_not_space_found_colon_,
    MP_QSTR_PackageManager,
    MP_QSTR_Passwords_space_do_space_not_space_match,
    MP_QSTR_Path_space__space__colon__space__brace_open__brace_close_,
    MP_QSTR_Path_space__colon__space__brace_open__brace_close_,
    MP_QSTR_Performing_space_initial_space_setup,
    MP_QSTR_RAM_space_free_space__space__colon__space__brace_open__brace_close_,
    MP_QSTR_RAM_space_total_space__colon__space__brace_open__brace_close_,
    MP_QSTR_RAM_space_used_space__space__colon__space__brace_open__brace_close_,
    MP_QSTR_RC,
    MP_QSTR_RISING,
    MP_QSTR_Recovery,
    MP_QSTR_RecoveryShell,
    MP_QSTR_Removed_colon_,
    MP_QSTR_Response,
    MP_QSTR_SEARCH_ROM,
    MP_QSTR_SKIP_ROM,
    MP_QSTR_Sec_hyphen_WebSocket_hyphen_Key_colon_,
    MP_QSTR_Server,
    MP_QSTR_Services,
    MP_QSTR_SingletonGenerator,
    MP_QSTR_Size_space__colon__space__brace_open__brace_close_,
    MP_QSTR_Stream,
    MP_QSTR_StreamReader,
    MP_QSTR_StreamWriter,
    MP_QSTR_SysPathManager,
    MP_QSTR_ThreadSafeFlag,
    MP_QSTR_TimeoutError,
    MP_QSTR_Total_space__colon__space__brace_open__brace_close_,
    MP_QSTR_Touched_colon_,
    MP_QSTR_Transfer_hyphen_Encoding,
    MP_QSTR_Type_space__colon__space__brace_open__brace_close_,
    MP_QSTR_UP,
    MP_QSTR_Unsupported_space_,
    MP_QSTR_Unsupported_space_epoch_colon__space__brace_open__brace_close_,
    MP_QSTR_Unsupported_space_protocol_colon__space_,
    MP_QSTR_Usage_colon__space_cat_space__lt_file_gt_,
    MP_QSTR_Usage_colon__space_cp_space__lt_src_gt__space__lt_dst_gt_,
    MP_QSTR_Usage_colon__space_mkdir_space__lt_dir_gt_,
    MP_QSTR_Usage_colon__space_mv_space__lt_src_gt__space__lt_dst_gt_,
    MP_QSTR_Usage_colon__space_rm_space__bracket_open__hyphen_r_bracket_close__space__lt_path_gt_,
    MP_QSTR_Usage_colon__space_rmdir_space__lt_dir_gt_,
    MP_QSTR_Usage_colon__space_stat_space__lt_path_gt_,
    MP_QSTR_Usage_colon__space_touch_space__lt_file_gt_,
    MP_QSTR_Used_space__space__colon__space__brace_open__brace_close_,
    MP_QSTR__bracket_open_cd_bracket_close__space_No_space_such_space_path_colon__space__brace_open__brace_close_,
    MP_QSTR__bracket_open_cd_bracket_close__space_Not_space_a_space_directory_colon__space__brace_open__brace_close_,
    MP_QSTR__bracket_open_cp_bracket_close__space__brace_open__brace_close_,
    MP_QSTR__bracket_open_mv_bracket_close__space__brace_open__brace_close_,
    MP_QSTR__bracket_open_pkg_bracket_close__space__squot__brace_open__brace_close__squot__space_failed_colon__space__brace_open__brace_close_,
    MP_QSTR__bracket_open_recovery_bracket_close__space_Bye_dot_,
    MP_QSTR__bracket_open_recovery_bracket_close__space_Rebooting_dot__dot__dot_,
    MP_QSTR__bracket_open_rm_bracket_close__space_No_space_such_space_path_colon__space__brace_open__brace_close_,
    MP_QSTR__bracket_open_rmdir_bracket_close__space__brace_open__brace_close_,
    MP_QSTR__bracket_open_stat_bracket_close__space_No_space_such_space_path_colon__space__brace_open__brace_close_,
    MP_QSTR__bracket_open_touch_bracket_close__space__brace_open__brace_close_,
    MP_QSTR__CounterBase,
    MP_QSTR__INSTANCES,
    MP_QSTR__PCNT,
    MP_QSTR__Remove,
    MP_QSTR___version__,
    MP_QSTR__abs,
    MP_QSTR__attrs,
    MP_QSTR__boot_dot_py,
    MP_QSTR__cached,
    MP_QSTR__check_exists,
    MP_QSTR__chunk,
    MP_QSTR__cmd_cat,
    MP_QSTR__cmd_cd,
    MP_QSTR__cmd_cp,
    MP_QSTR__cmd_df,
    MP_QSTR__cmd_exit,
    MP_QSTR__cmd_free,
    MP_QSTR__cmd_help,
    MP_QSTR__cmd_ls,
    MP_QSTR__cmd_mkdir,
    MP_QSTR__cmd_mv,
    MP_QSTR__cmd_pkg,
    MP_QSTR__cmd_pwd,
    MP_QSTR__cmd_reboot,
    MP_QSTR__cmd_recover,
    MP_QSTR__cmd_rm,
    MP_QSTR__cmd_rmdir,
    MP_QSTR__cmd_stat,
    MP_QSTR__cmd_touch,
    MP_QSTR__configure,
    MP_QSTR__context,
    MP_QSTR__data,
    MP_QSTR__dequeue,
    MP_QSTR__dispatch,
    MP_QSTR__download_file,
    MP_QSTR__enqueue,
    MP_QSTR__ensure_path_exists,
    MP_QSTR__exc_context,
    MP_QSTR__exc_handler,
    MP_QSTR__exists,
    MP_QSTR__human_size,
    MP_QSTR__install_json,
    MP_QSTR__install_package,
    MP_QSTR__io_queue,
    MP_QSTR__is_dir,
    MP_QSTR__join,
    MP_QSTR__listdir_safe,
    MP_QSTR__mkdirs,
    MP_QSTR__none_tuple,
    MP_QSTR__normalize_path,
    MP_QSTR__offset,
    MP_QSTR__overflow,
    MP_QSTR__overflows,
    MP_QSTR__ow,
    MP_QSTR__path,
    MP_QSTR__pcnt,
    MP_QSTR__phases,
    MP_QSTR__pm,
    MP_QSTR__promote_to_task,
    MP_QSTR__recv_len,
    MP_QSTR__rewrite_url,
    MP_QSTR__rm_recursive,
    MP_QSTR__run,
    MP_QSTR__search_rom,
    MP_QSTR__send_str,
    MP_QSTR__serve,
    MP_QSTR__size,
    MP_QSTR__stop_task,
    MP_QSTR__stopper,
    MP_QSTR__try_get_package_manager,
    MP_QSTR__try_import_recovery,
    MP_QSTR__try_import_services,
    MP_QSTR__unit_id,
    MP_QSTR_a,
    MP_QSTR_accept_conn,
    MP_QSTR_accept_handler,
    MP_QSTR_aclose,
    MP_QSTR_action,
    MP_QSTR_aioespnow_dot_py,
    MP_QSTR_airecv,
    MP_QSTR_allowed_mip_url_prefixes,
    MP_QSTR_apa106_dot_py,
    MP_QSTR_application_slash_json,
    MP_QSTR_arecv,
    MP_QSTR_ascii,
    MP_QSTR_asend,
    MP_QSTR_asyncio,
    MP_QSTR_asyncio_slash___init___dot_py,
    MP_QSTR_asyncio_slash_core_dot_py,
    MP_QSTR_asyncio_slash_event_dot_py,
    MP_QSTR_asyncio_slash_funcs_dot_py,
    MP_QSTR_asyncio_slash_lock_dot_py,
    MP_QSTR_asyncio_slash_stream_dot_py,
    MP_QSTR_attempts,
    MP_QSTR_attr,
    MP_QSTR_auth,
    MP_QSTR_aw,
    MP_QSTR_awrite,
    MP_QSTR_awritestr,
    MP_QSTR_backlog,
    MP_QSTR_base_path,
    MP_QSTR_bdev,
    MP_QSTR_boot_dot_py,
    MP_QSTR_bpp,
    MP_QSTR_branch,
    MP_QSTR_cadata,
    MP_QSTR_cafile,
    MP_QSTR_call_exception_handler,
    MP_QSTR_can_squot_t_space_gather,
    MP_QSTR_cat,
    MP_QSTR_cb,
    MP_QSTR_cert,
    MP_QSTR_cert_reqs,
    MP_QSTR_certfile,
    MP_QSTR_change_daemon,
    MP_QSTR_check_bootsec,
    MP_QSTR_check_msg,
    MP_QSTR_checksum_space_error,
    MP_QSTR_choices,
    MP_QSTR_chunked,
    MP_QSTR_cl,
    MP_QSTR_clean_session,
    MP_QSTR_client_s,
    MP_QSTR_cls,
    MP_QSTR_content,
    MP_QSTR_context,
    MP_QSTR_convert_temp,
    MP_QSTR_core,
    MP_QSTR_core_dot_py,
    MP_QSTR_coro_equals_,
    MP_QSTR_coroutine_space_expected,
    MP_QSTR_cp,
    MP_QSTR_create_task,
    MP_QSTR_current_path,
    MP_QSTR_current_task,
    MP_QSTR_d,
    MP_QSTR_default_exception_handler,
    MP_QSTR_delay,
    MP_QSTR_deps,
    MP_QSTR_df,
    MP_QSTR_dht_dot_py,
    MP_QSTR_diff,
    MP_QSTR_direction,
    MP_QSTR_directory,
    MP_QSTR_disabled,
    MP_QSTR_do_handshake,
    MP_QSTR_drain,
    MP_QSTR_ds18x20_dot_py,
    MP_QSTR_dt,
    MP_QSTR_edge,
    MP_QSTR_enabled,
    MP_QSTR_er,
    MP_QSTR_espnow,
    MP_QSTR_espnow_dot_py,
    MP_QSTR_event,
    MP_QSTR_exc,
    MP_QSTR_exc_type,
    MP_QSTR_exception,
    MP_QSTR_f,
    MP_QSTR_ffat,
    MP_QSTR_filter_ns,
    MP_QSTR_flashbdev,
    MP_QSTR_flashbdev_dot_py,
    MP_QSTR_fname,
    MP_QSTR_folder_name,
    MP_QSTR_free,
    MP_QSTR_fs_corrupted,
    MP_QSTR_funcs,
    MP_QSTR_future,
    MP_QSTR_future_colon_,
    MP_QSTR_gather,
    MP_QSTR_get_daemon_status,
    MP_QSTR_get_event_loop,
    MP_QSTR_get_exception_handler,
    MP_QSTR_get_extra_info,
    MP_QSTR_getpass,
    MP_QSTR_github_colon_,
    MP_QSTR_gitlab_colon_,
    MP_QSTR_hashes,
    MP_QSTR_head,
    MP_QSTR_headers,
    MP_QSTR_host,
    MP_QSTR_http_colon_,
    MP_QSTR_http_colon__slash__slash_,
    MP_QSTR_https_colon_,
    MP_QSTR_https_colon__slash__slash_,
    MP_QSTR_https_colon__slash__slash_gitlab_dot_com_slash_,
    MP_QSTR_humidity,
    MP_QSTR_i,
    MP_QSTR_idx,
    MP_QSTR_import_space_webrepl,
    MP_QSTR_import_from,
    MP_QSTR_in_reconnect,
    MP_QSTR_inisetup,
    MP_QSTR_inisetup_dot_py,
    MP_QSTR_input_choice,
    MP_QSTR_input_pass,
    MP_QSTR_install,
    MP_QSTR_install_package,
    MP_QSTR_irecv,
    MP_QSTR_is_set,
    MP_QSTR_keepalive,
    MP_QSTR_keyfile,
    MP_QSTR_l_rom,
    MP_QSTR_latest,
    MP_QSTR_listen_s,
    MP_QSTR_listen_sock,
    MP_QSTR_ls,
    MP_QSTR_lw_msg,
    MP_QSTR_lw_qos,
    MP_QSTR_lw_retain,
    MP_QSTR_lw_topic,
    MP_QSTR_machine_dot_py,
    MP_QSTR_main_task,
    MP_QSTR_man,
    MP_QSTR_measure,
    MP_QSTR_message,
    MP_QSTR_method,
    MP_QSTR_mip_slash___init___dot_py,
    MP_QSTR_module_name,
    MP_QSTR_mpy,
    MP_QSTR_mqtt_space_reconnect_colon__space__percent_r,
    MP_QSTR_mqtt_colon__space__percent_r,
    MP_QSTR_msg,
    MP_QSTR_mv,
    MP_QSTR_n,
    MP_QSTR_neopixel,
    MP_QSTR_neopixel_dot_py,
    MP_QSTR_new_event_loop,
    MP_QSTR_newfile,
    MP_QSTR_no_space_running_space_event_space_loop,
    MP_QSTR_not_space_initialised,
    MP_QSTR_ntptime_dot_py,
    MP_QSTR_onewire_dot_py,
    MP_QSTR_open_connection,
    MP_QSTR_out_buf,
    MP_QSTR_ow,
    MP_QSTR_package,
    MP_QSTR_package_dot_json,
    MP_QSTR_package_json_url,
    MP_QSTR_parent,
    MP_QSTR_parse_headers,
    MP_QSTR_patch,
    MP_QSTR_pcnt,
    MP_QSTR_peername,
    MP_QSTR_phase_a,
    MP_QSTR_phase_b,
    MP_QSTR_phases,
    MP_QSTR_pid,
    MP_QSTR_ping,
    MP_QSTR_pkg,
    MP_QSTR_poller,
    MP_QSTR_pool_dot_ntp_dot_org,
    MP_QSTR_port,
    MP_QSTR_post,
    MP_QSTR_prompt,
    MP_QSTR_pswd,
    MP_QSTR_publish,
    MP_QSTR_pwd,
    MP_QSTR_py,
    MP_QSTR_pyb,
    MP_QSTR_pyboard,
    MP_QSTR_qos,
    MP_QSTR_queue_read,
    MP_QSTR_queue_write,
    MP_QSTR_quit,
    MP_QSTR_raw,
    MP_QSTR_read_scratch,
    MP_QSTR_read_temp,
    MP_QSTR_readexactly,
    MP_QSTR_reason,
    MP_QSTR_reboot,
    MP_QSTR_reconnect,
    MP_QSTR_recover,
    MP_QSTR_recovery,
    MP_QSTR_recovery_dot_py,
    MP_QSTR_recursive,
    MP_QSTR_rejected,
    MP_QSTR_req,
    MP_QSTR_request,
    MP_QSTR_requesting,
    MP_QSTR_requests,
    MP_QSTR_requests_slash___init___dot_py,
    MP_QSTR_required,
    MP_QSTR_respkey_colon_,
    MP_QSTR_retain,
    MP_QSTR_return_exceptions,
    MP_QSTR_rm,
    MP_QSTR_rm_space_of_space__squot__percent_s_squot__space_failed,
    MP_QSTR_rom,
    MP_QSTR_run_forever,
    MP_QSTR_run_until_complete,
    MP_QSTR_running,
    MP_QSTR_runq_len,
    MP_QSTR_rwb,
    MP_QSTR_s,
    MP_QSTR_select_rom,
    MP_QSTR_send_html,
    MP_QSTR_server,
    MP_QSTR_server_handshake,
    MP_QSTR_set_callback,
    MP_QSTR_set_exception_handler,
    MP_QSTR_set_last_will,
    MP_QSTR_settime,
    MP_QSTR_setup,
    MP_QSTR_setup_conn,
    MP_QSTR_sgen,
    MP_QSTR_short_hash,
    MP_QSTR_simple,
    MP_QSTR_sock,
    MP_QSTR_src,
    MP_QSTR_ssl,
    MP_QSTR_ssl_dot_py,
    MP_QSTR_ssl_params,
    MP_QSTR_start_foreground,
    MP_QSTR_start_server,
    MP_QSTR_static_host,
    MP_QSTR_status_code,
    MP_QSTR_stream_awrite,
    MP_QSTR_subscribe,
    MP_QSTR_sz,
    MP_QSTR_t,
    MP_QSTR_task,
    MP_QSTR_tb,
    MP_QSTR_temperature,
    MP_QSTR_ticks,
    MP_QSTR_timing,
    MP_QSTR_topic,
    MP_QSTR_touch,
    MP_QSTR_uasyncio_dot_py,
    MP_QSTR_umqtt_slash_robust_dot_py,
    MP_QSTR_umqtt_slash_simple_dot_py,
    MP_QSTR_uninstall,
    MP_QSTR_unit_id,
    MP_QSTR_unsubscribe,
    MP_QSTR_upysh_dot_py,
    MP_QSTR_urequests_dot_py,
    MP_QSTR_url,
    MP_QSTR_urls,
    MP_QSTR_v,
    MP_QSTR_val,
    MP_QSTR_verify,
    MP_QSTR_wait,
    MP_QSTR_wait_closed,
    MP_QSTR_wait_for,
    MP_QSTR_wait_for_ms,
    MP_QSTR_wait_io_event,
    MP_QSTR_wait_msg,
    MP_QSTR_waiter,
    MP_QSTR_waiting,
    MP_QSTR_waitq_len,
    MP_QSTR_webrepl,
    MP_QSTR_webrepl_dot_py,
    MP_QSTR_webrepl_dot_start_paren_open__paren_close_,
    MP_QSTR_webrepl_cfg,
    MP_QSTR_webrepl_setup_dot_py,
    MP_QSTR_write_scratch,
    MP_QSTR__brace_open__colon__dot_2f_brace_close__space_KB,
    MP_QSTR__brace_open__colon__dot_2f_brace_close__space_MB,
    MP_QSTR__brace_open__brace_close__space_B,
    MP_QSTR__brace_open__brace_close__slash_file_slash__brace_open__brace_close__slash__brace_open__brace_close_,
    MP_QSTR__brace_open__brace_close__slash_package_slash__brace_open__brace_close__slash__brace_open__brace_close__slash__brace_open__brace_close__dot_json,
    MP_QSTR__brace_open__brace_close__slash__brace_open__brace_close_,
    MP_QSTR__brace_open__brace_close__colon__brace_open__brace_close__gt__space_,
};

const qstr_hash_t mp_qstr_frozen_const_hashes[] = {
    42299,
    13380,
    62594,
    27620,
    59913,
    40133,
    21,
    51205,
    47859,
    49998,
    54422,
    17989,
    12110,
    26188,
    26189,
    46470,
    28331,
    39613,
    46472,
    26340,
    26362,
    18012,
    46475,
    26309,
    27925,
    32613,
    16755,
    7567,
    14722,
    31778,
    47299,
    63706,
    1365,
    64203,
    26900,
    26901,
    58982,
    23830,
    29971,
    27922,
    17693,
    27926,
    28947,
    46495,
    33496,
    26843,
    57554,
    20130,
    40382,
    9712,
    57753,
    40860,
    48783,
    53071,
    25106,
    42451,
    45021,
    47385,
    53026,
    31962,
    49969,
    60031,
    46561,
    27956,
    33968,
    11292,
    32861,
    33021,
    28456,
    18231,
    45825,
    43539,
    51237,
    17354,
    46560,
    43861,
    20977,
    4573,
    35561,
    1983,
    26882,
    11802,
    18797,
    48915,
    32237,
    64005,
    26002,
    40031,
    22679,
    29530,
    59534,
    43943,
    59673,
    20921,
    2816,
    62926,
    50119,
    36803,
    33641,
    11108,
    28288,
    26955,
    55400,
    23888,
    61364,
    64660,
    55275,
    32488,
    6461,
    481,
    3220,
    3398,
    20518,
    45964,
    36388,
    51156,
    16052,
    7181,
    47443,
    49701,
    31456,
    29844,
    2861,
    45894,
    27608,
    36509,
    5830,
    28580,
    4555,
    35752,
    2976,
    19403,
    12601,
    6044,
    55353,
    39580,
    35638,
    15458,
    9558,
    19302,
    39387,
    54043,
    53440,
    36161,
    29600,
    18832,
    17597,
    38204,
    20808,
    19976,
    43806,
    53568,
    17747,
    61895,
    64871,
    60507,
    24222,
    25913,
    31109,
    8310,
    40766,
    39878,
    23184,
    52303,
    24385,
    42213,
    41228,
    27872,
    19515,
    62464,
    35187,
    37244,
    38975,
    15242,
    24346,
    15787,
    6226,
    7715,
    49025,
    62937,
    29704,
    29724,
    29741,
    56879,
    31515,
    55614,
    29488,
    5686,
    29524,
    19795,
    19404,
    1550,
    32981,
    30448,
    55151,
    9469,
    25770,
    50578,
    7717,
    30442,
    49770,
    48522,
    53815,
    26816,
    9133,
    13092,
    52355,
    45562,
    60255,
    55644,
    7934,
    43474,
    12320,
    63064,
    14985,
    35984,
    6295,
    61421,
    31159,
    15110,
    39861,
    8066,
    33911,
    35123,
    37926,
    7911,
    24914,
    35040,
    37796,
    27904,
    58803,
    32507,
    14476,
    55469,
    44511,
    5648,
    12837,
    24037,
    33627,
    59862,
    31182,
    46532,
    48726,
    36290,
    24658,
    60251,
    37845,
    38415,
    57627,
    57253,
    48680,
    39142,
    22356,
    24376,
    44357,
    63415,
    61558,
    25345,
    46624,
    16486,
    35185,
    26331,
    48310,
    47277,
    28211,
    49657,
    2796,
    43370,
    45858,
    12400,
    47476,
    18887,
    22033,
    26615,
    19745,
    19688,
    4535,
    19379,
    28388,
    45349,
    15087,
    53475,
    54096,
    28927,
    39173,
    53040,
    39069,
    3583,
    28394,
    35073,
    18128,
    19545,
    55724,
    55546,
    15425,
    56254,
    24921,
    21001,
    50473,
    28406,
    21139,
    61950,
    8734,
    46529,
    45889,
    19536,
    60551,
    28039,
    56666,
    64712,
    29728,
    40460,
    18645,
    32646,
    18805,
    26790,
    28053,
    27334,
    40672,
    28082,
    35349,
    20626,
    27081,
    9435,
    52220,
    60370,
    46531,
    1904,
    35911,
    62912,
    59367,
    21988,
    3499,
    15089,
    25555,
    27400,
    14592,
    22842,
    2184,
    49387,
    19971,
    60604,
    16583,
    58434,
    22202,
    31274,
    34369,
    24045,
    43945,
    13605,
    3847,
    59623,
    63796,
    4820,
    1,
    44924,
    46540,
    21584,
    42115,
    47313,
    26196,
    46092,
    7083,
    17127,
    59261,
    56068,
    9505,
    23790,
    58562,
    1385,
    62580,
    41766,
    27646,
    59904,
    32199,
    28314,
    17400,
    21612,
    34404,
    58496,
    12615,
    31740,
    17319,
    44573,
    48238,
    22682,
    29955,
    12455,
    17857,
    17533,
    63572,
    18044,
    28350,
    46539,
    41065,
    49582,
    3209,
    41535,
    49107,
    24341,
    16733,
    55311,
    25560,
    30085,
    28541,
    51135,
    23177,
    21804,
    53657,
    44963,
    40587,
    51660,
    8320,
    57844,
    57847,
    33497,
    29688,
    62037,
    29625,
    42221,
    33769,
    55388,
    55357,
    40417,
    34485,
    27228,
    28710,
    28684,
    29166,
    38486,
    30696,
    52601,
    8726,
    63836,
    35809,
    36436,
    12004,
    23753,
    59329,
    44772,
    4460,
    51263,
    53350,
    19713,
    3711,
    8523,
    35683,
    45172,
    56532,
    48775,
    26741,
    19500,
    47804,
    11104,
    47572,
    28890,
    26822,
    35765,
    25036,
    37871,
    59746,
    52773,
    34978,
    46550,
    20450,
    3579,
    10432,
    50376,
    43409,
    5288,
    6291,
    35858,
    706,
    59505,
    32154,
    17434,
    19467,
    40913,
    36103,
    36329,
    48942,
    30954,
    31393,
    63679,
    44450,
    62819,
    59482,
    2061,
    28908,
    46545,
    40488,
    28563,
    57065,
    2115,
    9013,
    55556,
    58752,
    55575,
    33545,
    40458,
    64735,
    56209,
    41174,
    50373,
    36405,
    25486,
    54589,
    46547,
    31486,
    722,
    21902,
    29411,
    40746,
    25035,
    6948,
    43752,
    60665,
    31214,
    21895,
    25086,
    20377,
    30641,
    803,
    59393,
    8251,
    27530,
    27852,
    31361,
    38218,
    18473,
    60938,
    17921,
};

const qstr_len_t mp_qstr_frozen_const_lengths[] = {
    23,
    20,
    24,
    12,
    3,
    4,
    7,
    6,
    12,
    12,
    16,
    6,
    3,
    2,
    2,
    1,
    7,
    20,
    1,
    2,
    2,
    3,
    1,
    2,
    9,
    16,
    5,
    4,
    3,
    4,
    7,
    4,
    5,
    7,
    5,
    5,
    6,
    5,
    5,
    5,
    5,
    5,
    5,
    1,
    6,
    2,
    9,
    6,
    13,
    4,
    8,
    5,
    6,
    9,
    18,
    10,
    14,
    12,
    15,
    8,
    7,
    8,
    1,
    5,
    5,
    6,
    5,
    5,
    7,
    4,
    7,
    23,
    4,
    20,
    1,
    6,
    7,
    5,
    5,
    7,
    7,
    21,
    10,
    3,
    4,
    4,
    7,
    19,
    23,
    2,
    4,
    17,
    4,
    9,
    10,
    13,
    3,
    14,
    8,
    19,
    18,
    5,
    7,
    12,
    4,
    10,
    5,
    11,
    4,
    6,
    3,
    3,
    18,
    14,
    22,
    10,
    9,
    24,
    14,
    14,
    14,
    2,
    6,
    8,
    13,
    8,
    8,
    10,
    8,
    18,
    6,
    8,
    18,
    9,
    6,
    12,
    12,
    14,
    14,
    12,
    10,
    8,
    17,
    9,
    2,
    12,
    21,
    22,
    17,
    21,
    18,
    21,
    21,
    18,
    18,
    19,
    10,
    21,
    24,
    7,
    7,
    21,
    15,
    23,
    21,
    10,
    23,
    10,
    12,
    10,
    5,
    7,
    11,
    4,
    6,
    8,
    7,
    13,
    6,
    8,
    7,
    7,
    7,
    9,
    9,
    9,
    7,
    10,
    7,
    8,
    8,
    11,
    12,
    7,
    10,
    9,
    10,
    10,
    8,
    5,
    8,
    9,
    14,
    8,
    19,
    12,
    12,
    7,
    11,
    13,
    16,
    9,
    7,
    5,
    13,
    7,
    11,
    15,
    7,
    9,
    10,
    3,
    5,
    5,
    7,
    3,
    16,
    9,
    12,
    13,
    4,
    11,
    9,
    6,
    5,
    10,
    8,
    24,
    20,
    20,
    8,
    1,
    11,
    14,
    6,
    6,
    12,
    6,
    24,
    9,
    16,
    5,
    5,
    5,
    7,
    19,
    15,
    16,
    16,
    15,
    17,
    8,
    4,
    4,
    2,
    6,
    9,
    7,
    9,
    4,
    7,
    3,
    6,
    6,
    6,
    22,
    12,
    3,
    2,
    4,
    9,
    8,
    13,
    13,
    9,
    14,
    7,
    7,
    2,
    13,
    8,
    3,
    7,
    7,
    12,
    4,
    7,
    5,
    18,
    2,
    11,
    12,
    12,
    1,
    25,
    5,
    4,
    2,
    6,
    4,
    9,
    9,
    8,
    12,
    5,
    10,
    2,
    4,
    7,
    2,
    6,
    9,
    5,
    3,
    8,
    9,
    1,
    4,
    9,
    9,
    12,
    5,
    11,
    4,
    12,
    5,
    6,
    7,
    6,
    17,
    14,
    21,
    14,
    7,
    7,
    7,
    6,
    4,
    7,
    4,
    5,
    7,
    6,
    8,
    19,
    8,
    1,
    3,
    14,
    11,
    12,
    8,
    11,
    12,
    10,
    7,
    15,
    5,
    6,
    9,
    7,
    5,
    6,
    8,
    11,
    2,
    6,
    6,
    9,
    8,
    10,
    9,
    3,
    7,
    7,
    6,
    15,
    11,
    3,
    18,
    8,
    3,
    2,
    1,
    8,
    11,
    14,
    7,
    21,
    15,
    10,
    10,
    15,
    7,
    2,
    7,
    12,
    16,
    6,
    13,
    5,
    4,
    8,
    7,
    7,
    6,
    3,
    4,
    3,
    6,
    12,
    4,
    4,
    6,
    4,
    7,
    3,
    2,
    3,
    7,
    3,
    10,
    11,
    4,
    3,
    12,
    9,
    11,
    6,
    6,
    9,
    7,
    8,
    11,
    9,
    8,
    3,
    7,
    10,
    8,
    20,
    8,
    8,
    6,
    17,
    2,
    17,
    3,
    11,
    18,
    7,
    8,
    3,
    1,
    10,
    9,
    6,
    16,
    12,
    21,
    13,
    7,
    5,
    10,
    4,
    10,
    6,
    4,
    3,
    3,
    6,
    10,
    16,
    12,
    11,
    11,
    13,
    9,
    2,
    1,
    4,
    2,
    11,
    5,
    6,
    5,
    5,
    11,
    15,
    15,
    9,
    7,
    11,
    8,
    12,
    3,
    4,
    1,
    3,
    6,
    4,
    11,
    8,
    11,
    13,
    8,
    6,
    7,
    9,
    7,
    10,
    15,
    11,
    16,
    13,
    9,
    9,
    4,
    13,
    24,
    5,
    7,
};

extern const qstr_pool_t mp_qstr_const_pool;
const qstr_pool_t mp_qstr_frozen_const_pool = {
    &mp_qstr_const_pool, // previous pool
    MP_QSTRnumber_of, // previous pool size
    true, // is_sorted
    10, // allocated entries
    532, // used entries
    (qstr_hash_t *)mp_qstr_frozen_const_hashes,
    (qstr_len_t *)mp_qstr_frozen_const_lengths,
    {
        "\x0a\x5b\x72\x65\x63\x6f\x76\x65\x72\x5d\x20\x49\x6e\x74\x65\x72\x72\x75\x70\x74\x65\x64\x2e",
        "\x0a\x5b\x72\x65\x63\x6f\x76\x65\x72\x79\x5d\x20\x45\x78\x69\x74\x69\x6e\x67\x2e",
        "\x0a\x5b\x72\x65\x63\x6f\x76\x65\x72\x79\x5d\x20\x49\x6e\x74\x65\x72\x72\x75\x70\x74\x65\x64\x2e",
        "\x0a\x7b\x3a\x2c\x64\x7d\x6b\x20\x66\x72\x65\x65",
        "\x0a\x7b\x7d",
        "\x0d\x0a\x0d\x0a",
        "\x1b\x5b\x32\x4a\x1b\x5b\x48",
        "      ",
        "          %s",
        "    <dir> %s",
        "  {}{:28} {:>10}",
        "  {}{}",
        "!BH",
        "!H",
        "!I",
        "#",
        "% 9d %s",
        "(Empty line to quit)",
        "-",
        "-l",
        "-r",
        "-rf",
        ".",
        "..",
        "./boot.py",
        "./webrepl_cfg.py",
        ".json",
        ".mpy",
        ".py",
        ".tmp",
        "/-/raw/",
        "/rom",
        "\x30\x0d\x0a\x0d\x0a",
        "0.0.0.0",
        "0.1.0",
        "0.1.1",
        "0.10.2",
        "0.2.1",
        "0.4.2",
        "0.6.1",
        "0.8.0",
        "1.0.2",
        "1.7.0",
        ":",
        "<DIR> ",
        "> ",
        "AIOESPNow",
        "APA106",
        "Authorization",
        "BASE",
        "Basic {}",
        "CLEAR",
        "CONFIG",
        "CRC error",
        "Confirm password: ",
        "Connection",
        "Content-Length",
        "Content-Type",
        "Copied {} -> {}",
        "Copying:",
        "Counter",
        "Created:",
        "D",
        "DEBUG",
        "DELAY",
        "DELETE",
        "DHT11",
        "DHT22",
        "DHTBase",
        "DOWN",
        "DS18X20",
        "Directory not found: {}",
        "Done",
        "Downloading {} to {}",
        "E",
        "ESPNow",
        "Encoder",
        "Error",
        "Event",
        "Exists:",
        "FALLING",
        "File not found: {} {}",
        "Free  : {}",
        "GET",
        "HEAD",
        "Host",
        "IOQueue",
        "Installing {} to {}",
        "Invalid password length",
        "LS",
        "Lock",
        "Lock not acquired",
        "Loop",
        "MATCH_ROM",
        "MQTTClient",
        "MQTTException",
        "Man",
        "Moved {} -> {}",
        "NeoPixel",
        "Not a directory: {}",
        "NotADirectoryError",
        "ORDER",
        "OneWire",
        "OneWireError",
        "PASS",
        "\x50\x41\x53\x53\x20\x3d\x20\x25\x72\x0a",
        "PATCH",
        "PCNT in use",
        "POST",
        "PROMPT",
        "PUT",
        "PWD",
        "Package not found:",
        "PackageManager",
        "Passwords do not match",
        "Path  : {}",
        "Path : {}",
        "Performing initial setup",
        "RAM free  : {}",
        "RAM total : {}",
        "RAM used  : {}",
        "RC",
        "RISING",
        "Recovery",
        "RecoveryShell",
        "Removed:",
        "Response",
        "SEARCH_ROM",
        "SKIP_ROM",
        "Sec-WebSocket-Key:",
        "Server",
        "Services",
        "SingletonGenerator",
        "Size : {}",
        "Stream",
        "StreamReader",
        "StreamWriter",
        "SysPathManager",
        "ThreadSafeFlag",
        "TimeoutError",
        "Total : {}",
        "Touched:",
        "Transfer-Encoding",
        "Type : {}",
        "UP",
        "Unsupported ",
        "Unsupported epoch: {}",
        "Unsupported protocol: ",
        "Usage: cat <file>",
        "Usage: cp <src> <dst>",
        "Usage: mkdir <dir>",
        "Usage: mv <src> <dst>",
        "Usage: rm [-r] <path>",
        "Usage: rmdir <dir>",
        "Usage: stat <path>",
        "Usage: touch <file>",
        "Used  : {}",
        "[cd] No such path: {}",
        "[cd] Not a directory: {}",
        "[cp] {}",
        "[mv] {}",
        "[pkg] '{}' failed: {}",
        "[recovery] Bye.",
        "[recovery] Rebooting...",
        "[rm] No such path: {}",
        "[rmdir] {}",
        "[stat] No such path: {}",
        "[touch] {}",
        "_CounterBase",
        "_INSTANCES",
        "_PCNT",
        "_Remove",
        "__version__",
        "_abs",
        "_attrs",
        "_boot.py",
        "_cached",
        "_check_exists",
        "_chunk",
        "_cmd_cat",
        "_cmd_cd",
        "_cmd_cp",
        "_cmd_df",
        "_cmd_exit",
        "_cmd_free",
        "_cmd_help",
        "_cmd_ls",
        "_cmd_mkdir",
        "_cmd_mv",
        "_cmd_pkg",
        "_cmd_pwd",
        "_cmd_reboot",
        "_cmd_recover",
        "_cmd_rm",
        "_cmd_rmdir",
        "_cmd_stat",
        "_cmd_touch",
        "_configure",
        "_context",
        "_data",
        "_dequeue",
        "_dispatch",
        "_download_file",
        "_enqueue",
        "_ensure_path_exists",
        "_exc_context",
        "_exc_handler",
        "_exists",
        "_human_size",
        "_install_json",
        "_install_package",
        "_io_queue",
        "_is_dir",
        "_join",
        "_listdir_safe",
        "_mkdirs",
        "_none_tuple",
        "_normalize_path",
        "_offset",
        "_overflow",
        "_overflows",
        "_ow",
        "_path",
        "_pcnt",
        "_phases",
        "_pm",
        "_promote_to_task",
        "_recv_len",
        "_rewrite_url",
        "_rm_recursive",
        "_run",
        "_search_rom",
        "_send_str",
        "_serve",
        "_size",
        "_stop_task",
        "_stopper",
        "_try_get_package_manager",
        "_try_import_recovery",
        "_try_import_services",
        "_unit_id",
        "a",
        "accept_conn",
        "accept_handler",
        "aclose",
        "action",
        "aioespnow.py",
        "airecv",
        "allowed_mip_url_prefixes",
        "apa106.py",
        "application/json",
        "arecv",
        "ascii",
        "asend",
        "asyncio",
        "asyncio/__init__.py",
        "asyncio/core.py",
        "asyncio/event.py",
        "asyncio/funcs.py",
        "asyncio/lock.py",
        "asyncio/stream.py",
        "attempts",
        "attr",
        "auth",
        "aw",
        "awrite",
        "awritestr",
        "backlog",
        "base_path",
        "bdev",
        "boot.py",
        "bpp",
        "branch",
        "cadata",
        "cafile",
        "call_exception_handler",
        "can't gather",
        "cat",
        "cb",
        "cert",
        "cert_reqs",
        "certfile",
        "change_daemon",
        "check_bootsec",
        "check_msg",
        "checksum error",
        "choices",
        "chunked",
        "cl",
        "clean_session",
        "client_s",
        "cls",
        "content",
        "context",
        "convert_temp",
        "core",
        "core.py",
        "coro=",
        "coroutine expected",
        "cp",
        "create_task",
        "current_path",
        "current_task",
        "d",
        "default_exception_handler",
        "delay",
        "deps",
        "df",
        "dht.py",
        "diff",
        "direction",
        "directory",
        "disabled",
        "do_handshake",
        "drain",
        "ds18x20.py",
        "dt",
        "edge",
        "enabled",
        "er",
        "espnow",
        "espnow.py",
        "event",
        "exc",
        "exc_type",
        "exception",
        "f",
        "ffat",
        "filter_ns",
        "flashbdev",
        "flashbdev.py",
        "fname",
        "folder_name",
        "free",
        "fs_corrupted",
        "funcs",
        "future",
        "future:",
        "gather",
        "get_daemon_status",
        "get_event_loop",
        "get_exception_handler",
        "get_extra_info",
        "getpass",
        "github:",
        "gitlab:",
        "hashes",
        "head",
        "headers",
        "host",
        "http:",
        "http://",
        "https:",
        "https://",
        "https://gitlab.com/",
        "humidity",
        "i",
        "idx",
        "import webrepl",
        "import_from",
        "in_reconnect",
        "inisetup",
        "inisetup.py",
        "input_choice",
        "input_pass",
        "install",
        "install_package",
        "irecv",
        "is_set",
        "keepalive",
        "keyfile",
        "l_rom",
        "latest",
        "listen_s",
        "listen_sock",
        "ls",
        "lw_msg",
        "lw_qos",
        "lw_retain",
        "lw_topic",
        "machine.py",
        "main_task",
        "man",
        "measure",
        "message",
        "method",
        "mip/__init__.py",
        "module_name",
        "mpy",
        "mqtt reconnect: %r",
        "mqtt: %r",
        "msg",
        "mv",
        "n",
        "neopixel",
        "neopixel.py",
        "new_event_loop",
        "newfile",
        "no running event loop",
        "not initialised",
        "ntptime.py",
        "onewire.py",
        "open_connection",
        "out_buf",
        "ow",
        "package",
        "package.json",
        "package_json_url",
        "parent",
        "parse_headers",
        "patch",
        "pcnt",
        "peername",
        "phase_a",
        "phase_b",
        "phases",
        "pid",
        "ping",
        "pkg",
        "poller",
        "pool.ntp.org",
        "port",
        "post",
        "prompt",
        "pswd",
        "publish",
        "pwd",
        "py",
        "pyb",
        "pyboard",
        "qos",
        "queue_read",
        "queue_write",
        "quit",
        "raw",
        "read_scratch",
        "read_temp",
        "readexactly",
        "reason",
        "reboot",
        "reconnect",
        "recover",
        "recovery",
        "recovery.py",
        "recursive",
        "rejected",
        "req",
        "request",
        "requesting",
        "requests",
        "requests/__init__.py",
        "required",
        "respkey:",
        "retain",
        "return_exceptions",
        "rm",
        "rm of '%s' failed",
        "rom",
        "run_forever",
        "run_until_complete",
        "running",
        "runq_len",
        "rwb",
        "s",
        "select_rom",
        "send_html",
        "server",
        "server_handshake",
        "set_callback",
        "set_exception_handler",
        "set_last_will",
        "settime",
        "setup",
        "setup_conn",
        "sgen",
        "short_hash",
        "simple",
        "sock",
        "src",
        "ssl",
        "ssl.py",
        "ssl_params",
        "start_foreground",
        "start_server",
        "static_host",
        "status_code",
        "stream_awrite",
        "subscribe",
        "sz",
        "t",
        "task",
        "tb",
        "temperature",
        "ticks",
        "timing",
        "topic",
        "touch",
        "uasyncio.py",
        "umqtt/robust.py",
        "umqtt/simple.py",
        "uninstall",
        "unit_id",
        "unsubscribe",
        "upysh.py",
        "urequests.py",
        "url",
        "urls",
        "v",
        "val",
        "verify",
        "wait",
        "wait_closed",
        "wait_for",
        "wait_for_ms",
        "wait_io_event",
        "wait_msg",
        "waiter",
        "waiting",
        "waitq_len",
        "webrepl",
        "webrepl.py",
        "webrepl.start()",
        "webrepl_cfg",
        "webrepl_setup.py",
        "write_scratch",
        "{:.2f} KB",
        "{:.2f} MB",
        "{} B",
        "{}/file/{}/{}",
        "{}/package/{}/{}/{}.json",
        "{}/{}",
        "{}:{}> ",
    },
};

////////////////////////////////////////////////////////////////////////////////
// frozen module flashbdev
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/flashbdev.mpy
// - frozen file name: flashbdev.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file flashbdev.py, scope flashbdev__lt_module_gt_
static const byte fun_data_flashbdev__lt_module_gt_[77] = {
    0x30,0x0a, // prelude
    0x01, // names: <module>
    0x6c,0x31,0x24,0x36, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING 'Partition'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'esp32'
    0x1c,0x02, // IMPORT_FROM 'Partition'
    0x16,0x02, // STORE_NAME 'Partition'
    0x59, // POP_TOP
    0x11,0x02, // LOAD_NAME 'Partition'
    0x14,0x04, // LOAD_METHOD 'find'
    0x11,0x02, // LOAD_NAME 'Partition'
    0x13,0x05, // LOAD_ATTR 'TYPE_DATA'
    0x10,0x06, // LOAD_CONST_STRING 'label'
    0x10,0x07, // LOAD_CONST_STRING 'vfs'
    0x36,0x82,0x01, // CALL_METHOD 257
    0x16,0x0a, // STORE_NAME 'bdev'
    0x11,0x0a, // LOAD_NAME 'bdev'
    0x43,0x56, // POP_JUMP_IF_TRUE 22
    0x11,0x02, // LOAD_NAME 'Partition'
    0x14,0x04, // LOAD_METHOD 'find'
    0x11,0x02, // LOAD_NAME 'Partition'
    0x13,0x05, // LOAD_ATTR 'TYPE_DATA'
    0x10,0x06, // LOAD_CONST_STRING 'label'
    0x10,0x08, // LOAD_CONST_STRING 'ffat'
    0x10,0x09, // LOAD_CONST_STRING 'block_size'
    0x22,0x84,0x00, // LOAD_CONST_SMALL_INT 512
    0x36,0x84,0x01, // CALL_METHOD 513
    0x16,0x0a, // STORE_NAME 'bdev'
    0x11,0x0a, // LOAD_NAME 'bdev'
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x11,0x0a, // LOAD_NAME 'bdev'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x42,0x41, // JUMP 1
    0x51, // LOAD_CONST_NONE
    0x16,0x0a, // STORE_NAME 'bdev'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_flashbdev__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_flashbdev__lt_module_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 77,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_flashbdev__lt_module_gt_ + 3,
        .line_info_top = fun_data_flashbdev__lt_module_gt_ + 7,
        .opcodes = fun_data_flashbdev__lt_module_gt_ + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_flashbdev__lt_module_gt_ fun_data_flashbdev__lt_module_gt_[0]
#endif

static const qstr_short_t const_qstr_table_data_flashbdev[11] = {
    MP_QSTR_flashbdev_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_Partition,
    MP_QSTR_esp32,
    MP_QSTR_find,
    MP_QSTR_TYPE_DATA,
    MP_QSTR_label,
    MP_QSTR_vfs,
    MP_QSTR_ffat,
    MP_QSTR_block_size,
    MP_QSTR_bdev,
};

static const mp_frozen_module_t frozen_module_flashbdev = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_flashbdev,
        .obj_table = NULL,
    },
    .proto_fun = &proto_fun_flashbdev__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module apa106
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/apa106.mpy
// - frozen file name: apa106.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file apa106.py, scope apa106__lt_module_gt_
static const byte fun_data_apa106__lt_module_gt_[30] = {
    0x18,0x06, // prelude
    0x01, // names: <module>
    0x60,0x6c, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING 'NeoPixel'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'neopixel'
    0x1c,0x02, // IMPORT_FROM 'NeoPixel'
    0x16,0x02, // STORE_NAME 'NeoPixel'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'APA106'
    0x11,0x02, // LOAD_NAME 'NeoPixel'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x04, // STORE_NAME 'APA106'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of apa106__lt_module_gt_
// frozen bytecode for file apa106.py, scope apa106_APA106
static const byte fun_data_apa106_APA106[19] = {
    0x00,0x06, // prelude
    0x04, // names: APA106
    0x88,0x07, // code info
    0x11,0x05, // LOAD_NAME '__name__'
    0x16,0x06, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'APA106'
    0x16,0x07, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x16,0x08, // STORE_NAME 'ORDER'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_apa106_APA106 = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_apa106_APA106,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 19,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_apa106_APA106 + 3,
        .line_info_top = fun_data_apa106_APA106 + 5,
        .opcodes = fun_data_apa106_APA106 + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_apa106_APA106 fun_data_apa106_APA106[0]
#endif

static const mp_raw_code_t *const children_apa106__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_apa106_APA106,
};

static const mp_raw_code_truncated_t proto_fun_apa106__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_apa106__lt_module_gt_,
    .children = (void *)&children_apa106__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 30,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_apa106__lt_module_gt_ + 3,
        .line_info_top = fun_data_apa106__lt_module_gt_ + 5,
        .opcodes = fun_data_apa106__lt_module_gt_ + 5,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_apa106[9] = {
    MP_QSTR_apa106_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_NeoPixel,
    MP_QSTR_neopixel,
    MP_QSTR_APA106,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_ORDER,
};

// constants
static const mp_rom_obj_tuple_t const_obj_apa106_0 = {{&mp_type_tuple}, 4, {
    MP_ROM_INT(0),
    MP_ROM_INT(1),
    MP_ROM_INT(2),
    MP_ROM_INT(3),
}};

// constant table
static const mp_rom_obj_t const_obj_table_data_apa106[1] = {
    MP_ROM_PTR(&const_obj_apa106_0),
};

static const mp_frozen_module_t frozen_module_apa106 = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_apa106,
        .obj_table = (mp_obj_t *)&const_obj_table_data_apa106,
    },
    .proto_fun = &proto_fun_apa106__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module espnow
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/espnow.mpy
// - frozen file name: espnow.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file espnow.py, scope espnow__lt_module_gt_
static const byte fun_data_espnow__lt_module_gt_[26] = {
    0x18,0x06, // prelude
    0x01, // names: <module>
    0x60,0x68, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING '*'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME '_espnow'
    0x69, // IMPORT_STAR
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'ESPNow'
    0x11,0x0f, // LOAD_NAME 'ESPNowBase'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x04, // STORE_NAME 'ESPNow'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of espnow__lt_module_gt_
// frozen bytecode for file espnow.py, scope espnow_ESPNow
static const byte fun_data_espnow_ESPNow[76] = {
    0x18,0x19, // prelude
    0x04, // names: ESPNow
    0x88,0x08,0x2b,0x44,0x66,0x68,0x20,0x68,0x20,0x66,0x64,0x00, // code info
    0x11,0x10, // LOAD_NAME '__name__'
    0x16,0x11, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'ESPNow'
    0x16,0x12, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x11,0x13, // LOAD_NAME 'bytearray'
    0x11,0x14, // LOAD_NAME 'MAX_DATA_LEN'
    0x34,0x01, // CALL_FUNCTION 1
    0x2b,0x02, // BUILD_LIST 2
    0x16,0x08, // STORE_NAME '_data'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x16,0x09, // STORE_NAME '_none_tuple'
    0xb0, // LOAD_FAST 0
    0x20,0x00,0x01, // MAKE_CLOSURE 0
    0x16,0x05, // STORE_NAME '__init__'
    0x51, // LOAD_CONST_NONE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x06, // STORE_NAME 'irecv'
    0x51, // LOAD_CONST_NONE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x02, // MAKE_FUNCTION_DEFARGS 2
    0x16,0x0a, // STORE_NAME 'recv'
    0xb0, // LOAD_FAST 0
    0x20,0x03,0x01, // MAKE_CLOSURE 3
    0x16,0x0b, // STORE_NAME 'irq'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x0c, // STORE_NAME '__iter__'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x0d, // STORE_NAME '__next__'
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
// child of espnow_ESPNow
// frozen bytecode for file espnow.py, scope espnow_ESPNow___init__
static const byte fun_data_espnow_ESPNow___init__[19] = {
    0x22,0x0a, // prelude
    0x05,0x02,0x15, // names: __init__, *, self
    0x80,0x0c, // code info
    0x12,0x16, // LOAD_GLOBAL 'super'
    0x25,0x00, // LOAD_DEREF 0
    0xb1, // LOAD_FAST 1
    0x15,0x05, // LOAD_SUPER_METHOD '__init__'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_espnow_ESPNow___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_espnow_ESPNow___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 19,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 5,
        .line_info = fun_data_espnow_ESPNow___init__ + 5,
        .line_info_top = fun_data_espnow_ESPNow___init__ + 7,
        .opcodes = fun_data_espnow_ESPNow___init__ + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_espnow_ESPNow___init__ fun_data_espnow_ESPNow___init__[0]
#endif

// child of espnow_ESPNow
// frozen bytecode for file espnow.py, scope espnow_ESPNow_irecv
static const byte fun_data_espnow_ESPNow_irecv[30] = {
    0xb2,0x01,0x0c, // prelude
    0x06,0x15,0x17, // names: irecv, self, timeout_ms
    0x80,0x0f,0x2a, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x07, // LOAD_METHOD 'recvinto'
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR '_data'
    0xb1, // LOAD_FAST 1
    0x36,0x02, // CALL_METHOD 2
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR '_data'
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR '_none_tuple'
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_espnow_ESPNow_irecv = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_espnow_ESPNow_irecv,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 30,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_espnow_ESPNow_irecv + 6,
        .line_info_top = fun_data_espnow_ESPNow_irecv + 9,
        .opcodes = fun_data_espnow_ESPNow_irecv + 9,
    },
    #endif
    #endif
};
#else
#define proto_fun_espnow_ESPNow_irecv fun_data_espnow_ESPNow_irecv[0]
#endif

// child of espnow_ESPNow
// frozen bytecode for file espnow.py, scope espnow_ESPNow_recv
static const byte fun_data_espnow_ESPNow_recv[34] = {
    0xb2,0x01,0x0c, // prelude
    0x0a,0x15,0x17, // names: recv, self, timeout_ms
    0x80,0x13,0x2a, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x07, // LOAD_METHOD 'recvinto'
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR '_data'
    0xb1, // LOAD_FAST 1
    0x36,0x02, // CALL_METHOD 2
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x32,0x00, // MAKE_FUNCTION 0
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR '_data'
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR '_none_tuple'
    0x63, // RETURN_VALUE
};
// child of espnow_ESPNow_recv
// frozen bytecode for file espnow.py, scope espnow_ESPNow_recv__lt_listcomp_gt_
static const byte fun_data_espnow_ESPNow_recv__lt_listcomp_gt_[23] = {
    0x41,0x08, // prelude
    0x0e,0x02, // names: <listcomp>, *
    0x80,0x14, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x0a, // FOR_ITER 10
    0xc1, // STORE_FAST 1
    0x12,0x19, // LOAD_GLOBAL 'bytes'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x2f,0x14, // STORE_COMP 20
    0x42,0x34, // JUMP -12
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_espnow_ESPNow_recv__lt_listcomp_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_espnow_ESPNow_recv__lt_listcomp_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 23,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_espnow_ESPNow_recv__lt_listcomp_gt_ + 4,
        .line_info_top = fun_data_espnow_ESPNow_recv__lt_listcomp_gt_ + 6,
        .opcodes = fun_data_espnow_ESPNow_recv__lt_listcomp_gt_ + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_espnow_ESPNow_recv__lt_listcomp_gt_ fun_data_espnow_ESPNow_recv__lt_listcomp_gt_[0]
#endif

static const mp_raw_code_t *const children_espnow_ESPNow_recv[] = {
    (const mp_raw_code_t *)&proto_fun_espnow_ESPNow_recv__lt_listcomp_gt_,
};

static const mp_raw_code_truncated_t proto_fun_espnow_ESPNow_recv = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_espnow_ESPNow_recv,
    .children = (void *)&children_espnow_ESPNow_recv,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 34,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_espnow_ESPNow_recv + 6,
        .line_info_top = fun_data_espnow_ESPNow_recv + 9,
        .opcodes = fun_data_espnow_ESPNow_recv + 9,
    },
    #endif
    #endif
};

// child of espnow_ESPNow
// frozen bytecode for file espnow.py, scope espnow_ESPNow_irq
static const byte fun_data_espnow_ESPNow_irq[22] = {
    0x33,0x0c, // prelude
    0x0b,0x02,0x15,0x18, // names: irq, *, self, callback
    0x80,0x17, // code info
    0x12,0x16, // LOAD_GLOBAL 'super'
    0x25,0x00, // LOAD_DEREF 0
    0xb1, // LOAD_FAST 1
    0x15,0x0b, // LOAD_SUPER_METHOD 'irq'
    0xb2, // LOAD_FAST 2
    0xb1, // LOAD_FAST 1
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_espnow_ESPNow_irq = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_espnow_ESPNow_irq,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 22,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_espnow_ESPNow_irq + 6,
        .line_info_top = fun_data_espnow_ESPNow_irq + 8,
        .opcodes = fun_data_espnow_ESPNow_irq + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_espnow_ESPNow_irq fun_data_espnow_ESPNow_irq[0]
#endif

// child of espnow_ESPNow
// frozen bytecode for file espnow.py, scope espnow_ESPNow___iter__
static const byte fun_data_espnow_ESPNow___iter__[8] = {
    0x09,0x08, // prelude
    0x0c,0x15, // names: __iter__, self
    0x80,0x1a, // code info
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_espnow_ESPNow___iter__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_espnow_ESPNow___iter__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 8,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_espnow_ESPNow___iter__ + 4,
        .line_info_top = fun_data_espnow_ESPNow___iter__ + 6,
        .opcodes = fun_data_espnow_ESPNow___iter__ + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_espnow_ESPNow___iter__ fun_data_espnow_ESPNow___iter__[0]
#endif

// child of espnow_ESPNow
// frozen bytecode for file espnow.py, scope espnow_ESPNow___next__
static const byte fun_data_espnow_ESPNow___next__[12] = {
    0x11,0x08, // prelude
    0x0d,0x15, // names: __next__, self
    0x80,0x1d, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x06, // LOAD_METHOD 'irecv'
    0x36,0x00, // CALL_METHOD 0
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_espnow_ESPNow___next__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_espnow_ESPNow___next__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 12,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_espnow_ESPNow___next__ + 4,
        .line_info_top = fun_data_espnow_ESPNow___next__ + 6,
        .opcodes = fun_data_espnow_ESPNow___next__ + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_espnow_ESPNow___next__ fun_data_espnow_ESPNow___next__[0]
#endif

static const mp_raw_code_t *const children_espnow_ESPNow[] = {
    (const mp_raw_code_t *)&proto_fun_espnow_ESPNow___init__,
    (const mp_raw_code_t *)&proto_fun_espnow_ESPNow_irecv,
    (const mp_raw_code_t *)&proto_fun_espnow_ESPNow_recv,
    (const mp_raw_code_t *)&proto_fun_espnow_ESPNow_irq,
    (const mp_raw_code_t *)&proto_fun_espnow_ESPNow___iter__,
    (const mp_raw_code_t *)&proto_fun_espnow_ESPNow___next__,
};

static const mp_raw_code_truncated_t proto_fun_espnow_ESPNow = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_espnow_ESPNow,
    .children = (void *)&children_espnow_ESPNow,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 76,
    .n_children = 6,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_espnow_ESPNow + 3,
        .line_info_top = fun_data_espnow_ESPNow + 14,
        .opcodes = fun_data_espnow_ESPNow + 15,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_espnow__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_espnow_ESPNow,
};

static const mp_raw_code_truncated_t proto_fun_espnow__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_espnow__lt_module_gt_,
    .children = (void *)&children_espnow__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 26,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_espnow__lt_module_gt_ + 3,
        .line_info_top = fun_data_espnow__lt_module_gt_ + 5,
        .opcodes = fun_data_espnow__lt_module_gt_ + 5,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_espnow[26] = {
    MP_QSTR_espnow_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR__star_,
    MP_QSTR__espnow,
    MP_QSTR_ESPNow,
    MP_QSTR___init__,
    MP_QSTR_irecv,
    MP_QSTR_recvinto,
    MP_QSTR__data,
    MP_QSTR__none_tuple,
    MP_QSTR_recv,
    MP_QSTR_irq,
    MP_QSTR___iter__,
    MP_QSTR___next__,
    MP_QSTR__lt_listcomp_gt_,
    MP_QSTR_ESPNowBase,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_bytearray,
    MP_QSTR_MAX_DATA_LEN,
    MP_QSTR_self,
    MP_QSTR_super,
    MP_QSTR_timeout_ms,
    MP_QSTR_callback,
    MP_QSTR_bytes,
};

// constants
static const mp_rom_obj_tuple_t const_obj_espnow_0 = {{&mp_type_tuple}, 2, {
    MP_ROM_NONE,
    MP_ROM_NONE,
}};

// constant table
static const mp_rom_obj_t const_obj_table_data_espnow[1] = {
    MP_ROM_PTR(&const_obj_espnow_0),
};

static const mp_frozen_module_t frozen_module_espnow = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_espnow,
        .obj_table = (mp_obj_t *)&const_obj_table_data_espnow,
    },
    .proto_fun = &proto_fun_espnow__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module machine
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/machine.mpy
// - frozen file name: machine.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file machine.py, scope machine__lt_module_gt_
static const byte fun_data_machine__lt_module_gt_[127] = {
    0x24,0x28, // prelude
    0x01, // names: <module>
    0x46,0x26,0x26,0x22,0x49,0x26,0x22,0x63,0x2c,0x46,0x2a,0x40,0x89,0x74,0x8b,0x11,0x8b,0x20,0x62, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'sys'
    0x16,0x02, // STORE_NAME 'sys'
    0x11,0x02, // LOAD_NAME 'sys'
    0x13,0x03, // LOAD_ATTR 'path'
    0x16,0x33, // STORE_NAME '_path'
    0x2a,0x00, // BUILD_TUPLE 0
    0x11,0x02, // LOAD_NAME 'sys'
    0x18,0x03, // STORE_ATTR 'path'
    0x49,0x09, // SETUP_FINALLY 9
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x04, // LOAD_CONST_STRING '*'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x05, // IMPORT_NAME 'machine'
    0x69, // IMPORT_STAR
    0x51, // LOAD_CONST_NONE
    0x11,0x33, // LOAD_NAME '_path'
    0x11,0x02, // LOAD_NAME 'sys'
    0x18,0x03, // STORE_ATTR 'path'
    0x19,0x33, // DELETE_NAME '_path'
    0x19,0x02, // DELETE_NAME 'sys'
    0x5d, // END_FINALLY
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x06, // LOAD_CONST_STRING 'const'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x07, // IMPORT_NAME 'micropython'
    0x1c,0x06, // IMPORT_FROM 'const'
    0x16,0x06, // STORE_NAME 'const'
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x08, // IMPORT_NAME 'esp32'
    0x16,0x08, // STORE_NAME 'esp32'
    0x11,0x34, // LOAD_NAME 'hasattr'
    0x11,0x08, // LOAD_NAME 'esp32'
    0x10,0x09, // LOAD_CONST_STRING 'PCNT'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x61, // POP_JUMP_IF_FALSE 33
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x0a, // LOAD_CONST_STRING '_CounterBase'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x0a, // STORE_NAME '_CounterBase'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x0b, // LOAD_CONST_STRING 'Counter'
    0x11,0x0a, // LOAD_NAME '_CounterBase'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x0b, // STORE_NAME 'Counter'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x0c, // LOAD_CONST_STRING 'Encoder'
    0x11,0x0a, // LOAD_NAME '_CounterBase'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x0c, // STORE_NAME 'Encoder'
    0x19,0x0a, // DELETE_NAME '_CounterBase'
    0x19,0x08, // DELETE_NAME 'esp32'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of machine__lt_module_gt_
// frozen bytecode for file machine.py, scope machine__CounterBase
static const byte fun_data_machine__CounterBase[73] = {
    0x18,0x20, // prelude
    0x0a, // names: _CounterBase
    0x88,0x13,0x46,0x64,0x60,0x84,0x11,0x8b,0x2b,0x84,0x08,0x64,0x20,0x64,0x60, // code info
    0x11,0x35, // LOAD_NAME '__name__'
    0x16,0x36, // STORE_NAME '__module__'
    0x10,0x0a, // LOAD_CONST_STRING '_CounterBase'
    0x16,0x37, // STORE_NAME '__qualname__'
    0x11,0x08, // LOAD_NAME 'esp32'
    0x13,0x09, // LOAD_ATTR 'PCNT'
    0x16,0x0e, // STORE_NAME '_PCNT'
    0x2c,0x00, // BUILD_MAP 0
    0x16,0x12, // STORE_NAME '_INSTANCES'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x11, // STORE_NAME '__new__'
    0x53, // LOAD_NULL
    0x2c,0x00, // BUILD_MAP 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x0d, // LOAD_CONST_STRING 'filter_ns'
    0x62, // STORE_MAP
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x14, // STORE_NAME '__init__'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x20, // STORE_NAME '_overflow'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x1a, // STORE_NAME 'init'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x25, // STORE_NAME 'deinit'
    0x51, // LOAD_CONST_NONE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x05, // MAKE_FUNCTION_DEFARGS 5
    0x16,0x1c, // STORE_NAME 'value'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of machine__CounterBase
// frozen bytecode for file machine.py, scope machine__CounterBase___new__
static const byte fun_data_machine__CounterBase___new__[61] = {
    0xba,0x80,0xc0,0x40,0x16, // prelude
    0x11,0x3a,0x3b, // names: __new__, cls, unit_id
    0x80,0x1d,0x49,0x63,0x28,0x69,0x28,0x66, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x12, // LOAD_ATTR '_INSTANCES'
    0x14,0x13, // LOAD_METHOD 'get'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x44,0x51, // POP_JUMP_IF_FALSE 17
    0x12,0x3c, // LOAD_GLOBAL 'isinstance'
    0xb4, // LOAD_FAST 4
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0x12,0x3d, // LOAD_GLOBAL 'ValueError'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x42,0x4e, // JUMP 14
    0x12,0x3e, // LOAD_GLOBAL 'object'
    0x14,0x11, // LOAD_METHOD '__new__'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0xb0, // LOAD_FAST 0
    0x13,0x12, // LOAD_ATTR '_INSTANCES'
    0xb1, // LOAD_FAST 1
    0x56, // STORE_SUBSCR
    0xb4, // LOAD_FAST 4
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_machine__CounterBase___new__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_machine__CounterBase___new__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 61,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 6,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 17,
        .line_info = fun_data_machine__CounterBase___new__ + 8,
        .line_info_top = fun_data_machine__CounterBase___new__ + 16,
        .opcodes = fun_data_machine__CounterBase___new__ + 16,
    },
    #endif
    #endif
};
#else
#define proto_fun_machine__CounterBase___new__ fun_data_machine__CounterBase___new__[0]
#endif

// child of machine__CounterBase
// frozen bytecode for file machine.py, scope machine__CounterBase___init__
static const byte fun_data_machine__CounterBase___init__[157] = {
    0xe2,0x88,0xc0,0xc0,0x40,0x28, // prelude
    0x14,0x3f,0x3b,0x0d, // names: __init__, self, unit_id, filter_ns
    0x80,0x2d,0x44,0x49,0x7e,0x60,0x40,0x62,0x6b,0x27,0x7c,0x80,0x0a,0x24,0x64,0x76, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x15, // STORE_ATTR '_unit_id'
    0x12,0x34, // LOAD_GLOBAL 'hasattr'
    0xb0, // LOAD_FAST 0
    0x10,0x16, // LOAD_CONST_STRING '_pcnt'
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x58, // POP_JUMP_IF_TRUE 24
    0xb0, // LOAD_FAST 0
    0x14,0x0e, // LOAD_METHOD '_PCNT'
    0xb1, // LOAD_FAST 1
    0x10,0x17, // LOAD_CONST_STRING 'min'
    0x22,0xfe,0x86,0x00, // LOAD_CONST_SMALL_INT -32000
    0x10,0x18, // LOAD_CONST_STRING 'max'
    0x22,0x81,0xfa,0x00, // LOAD_CONST_SMALL_INT 32000
    0x36,0x84,0x01, // CALL_METHOD 513
    0xb0, // LOAD_FAST 0
    0x18,0x16, // STORE_ATTR '_pcnt'
    0x42,0x48, // JUMP 8
    0xb3, // LOAD_FAST 3
    0x43,0x45, // POP_JUMP_IF_TRUE 5
    0xb4, // LOAD_FAST 4
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD '_configure'
    0xb3, // LOAD_FAST 3
    0x53, // LOAD_NULL
    0xb4, // LOAD_FAST 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0x37,0x82,0x01, // CALL_METHOD_VAR_KW 257
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR '_pcnt'
    0x14,0x1a, // LOAD_METHOD 'init'
    0x10,0x1b, // LOAD_CONST_STRING 'filter'
    0x12,0x17, // LOAD_GLOBAL 'min'
    0x12,0x18, // LOAD_GLOBAL 'max'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb2, // LOAD_FAST 2
    0x22,0x80,0x50, // LOAD_CONST_SMALL_INT 80
    0xf4, // BINARY_OP 29 __mul__
    0x22,0x87,0x68, // LOAD_CONST_SMALL_INT 1000
    0xf6, // BINARY_OP 31 __floordiv__
    0x34,0x02, // CALL_FUNCTION 2
    0x22,0x87,0x7f, // LOAD_CONST_SMALL_INT 1023
    0x34,0x02, // CALL_FUNCTION 2
    0x10,0x1c, // LOAD_CONST_STRING 'value'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x84,0x00, // CALL_METHOD 512
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x1d, // STORE_ATTR '_overflows'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x1e, // STORE_ATTR '_offset'
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR '_pcnt'
    0x14,0x1f, // LOAD_METHOD 'irq'
    0xb0, // LOAD_FAST 0
    0x13,0x20, // LOAD_ATTR '_overflow'
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR '_PCNT'
    0x13,0x21, // LOAD_ATTR 'IRQ_MIN'
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR '_PCNT'
    0x13,0x22, // LOAD_ATTR 'IRQ_MAX'
    0xed, // BINARY_OP 22 __or__
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR '_pcnt'
    0x14,0x23, // LOAD_METHOD 'start'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_machine__CounterBase___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_machine__CounterBase___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 157,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 0,
        .scope_flags = 14,
        .n_pos_args = 2,
        .n_kwonly_args = 1,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 20,
        .line_info = fun_data_machine__CounterBase___init__ + 10,
        .line_info_top = fun_data_machine__CounterBase___init__ + 26,
        .opcodes = fun_data_machine__CounterBase___init__ + 26,
    },
    #endif
    #endif
};
#else
#define proto_fun_machine__CounterBase___init__ fun_data_machine__CounterBase___init__[0]
#endif

// child of machine__CounterBase
// frozen bytecode for file machine.py, scope machine__CounterBase__overflow
static const byte fun_data_machine__CounterBase__overflow[63] = {
    0x2a,0x12, // prelude
    0x20,0x3f,0x40, // names: _overflow, self, pcnt
    0x80,0x58,0x2a,0x29,0x2b,0x29, // code info
    0xb1, // LOAD_FAST 1
    0x14,0x1f, // LOAD_METHOD 'irq'
    0x36,0x00, // CALL_METHOD 0
    0x14,0x24, // LOAD_METHOD 'flags'
    0x36,0x00, // CALL_METHOD 0
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR '_PCNT'
    0x13,0x21, // LOAD_ATTR 'IRQ_MIN'
    0xef, // BINARY_OP 24 __and__
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x1d, // LOAD_ATTR '_overflows'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe6, // BINARY_OP 15 __isub__
    0x5a, // ROT_TWO
    0x18,0x1d, // STORE_ATTR '_overflows'
    0x42,0x54, // JUMP 20
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR '_PCNT'
    0x13,0x22, // LOAD_ATTR 'IRQ_MAX'
    0xef, // BINARY_OP 24 __and__
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x1d, // LOAD_ATTR '_overflows'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x1d, // STORE_ATTR '_overflows'
    0x42,0x40, // JUMP 0
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_machine__CounterBase__overflow = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_machine__CounterBase__overflow,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 63,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 32,
        .line_info = fun_data_machine__CounterBase__overflow + 5,
        .line_info_top = fun_data_machine__CounterBase__overflow + 11,
        .opcodes = fun_data_machine__CounterBase__overflow + 11,
    },
    #endif
    #endif
};
#else
#define proto_fun_machine__CounterBase__overflow fun_data_machine__CounterBase__overflow[0]
#endif

// child of machine__CounterBase
// frozen bytecode for file machine.py, scope machine__CounterBase_init
static const byte fun_data_machine__CounterBase_init[25] = {
    0xc9,0x80,0xc0,0x40,0x08, // prelude
    0x1a,0x3f, // names: init, self
    0x80,0x60, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x14, // LOAD_METHOD '__init__'
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR '_unit_id'
    0xb1, // LOAD_FAST 1
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x82, // LOAD_CONST_SMALL_INT 2
    0x37,0x82,0x02, // CALL_METHOD_VAR_KW 258
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_machine__CounterBase_init = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_machine__CounterBase_init,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 25,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 6,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 26,
        .line_info = fun_data_machine__CounterBase_init + 7,
        .line_info_top = fun_data_machine__CounterBase_init + 9,
        .opcodes = fun_data_machine__CounterBase_init + 9,
    },
    #endif
    #endif
};
#else
#define proto_fun_machine__CounterBase_init fun_data_machine__CounterBase_init[0]
#endif

// child of machine__CounterBase
// frozen bytecode for file machine.py, scope machine__CounterBase_deinit
static const byte fun_data_machine__CounterBase_deinit[32] = {
    0x19,0x0c, // prelude
    0x25,0x3f, // names: deinit, self
    0x80,0x64,0x29,0x28, // code info
    0x12,0x34, // LOAD_GLOBAL 'hasattr'
    0xb0, // LOAD_FAST 0
    0x10,0x16, // LOAD_CONST_STRING '_pcnt'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR '_pcnt'
    0x14,0x25, // LOAD_METHOD 'deinit'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0x5a, // ROT_TWO
    0x18,0x16, // STORE_ATTR '_pcnt'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_machine__CounterBase_deinit = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_machine__CounterBase_deinit,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 32,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 37,
        .line_info = fun_data_machine__CounterBase_deinit + 4,
        .line_info_top = fun_data_machine__CounterBase_deinit + 8,
        .opcodes = fun_data_machine__CounterBase_deinit + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_machine__CounterBase_deinit fun_data_machine__CounterBase_deinit[0]
#endif

// child of machine__CounterBase
// frozen bytecode for file machine.py, scope machine__CounterBase_value
static const byte fun_data_machine__CounterBase_value[98] = {
    0xba,0x01,0x28, // prelude
    0x1c,0x3f,0x1c, // names: value, self, value
    0x80,0x6a,0x29,0x67,0x20,0x20,0x24,0x68,0x20,0x27,0x64,0x20,0x6d,0x60,0x26,0x29,0x46, // code info
    0x12,0x34, // LOAD_GLOBAL 'hasattr'
    0xb0, // LOAD_FAST 0
    0x10,0x16, // LOAD_CONST_STRING '_pcnt'
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0x12,0x41, // LOAD_GLOBAL 'RuntimeError'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb0, // LOAD_FAST 0
    0x13,0x1d, // LOAD_ATTR '_overflows'
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR '_pcnt'
    0x14,0x1c, // LOAD_METHOD 'value'
    0x36,0x00, // CALL_METHOD 0
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x1d, // LOAD_ATTR '_overflows'
    0xb2, // LOAD_FAST 2
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x42,0x42, // JUMP 2
    0x42,0x29, // JUMP -23
    0xb2, // LOAD_FAST 2
    0x22,0x81,0xfa,0x00, // LOAD_CONST_SMALL_INT 32000
    0xf4, // BINARY_OP 29 __mul__
    0xb3, // LOAD_FAST 3
    0xf2, // BINARY_OP 27 __add__
    0xb0, // LOAD_FAST 0
    0x13,0x1e, // LOAD_ATTR '_offset'
    0xf2, // BINARY_OP 27 __add__
    0xc4, // STORE_FAST 4
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x1d, // LOAD_ATTR '_overflows'
    0xb2, // LOAD_FAST 2
    0xe6, // BINARY_OP 15 __isub__
    0x5a, // ROT_TWO
    0x18,0x1d, // STORE_ATTR '_overflows'
    0xb1, // LOAD_FAST 1
    0xb3, // LOAD_FAST 3
    0xf3, // BINARY_OP 28 __sub__
    0xb0, // LOAD_FAST 0
    0x18,0x1e, // STORE_ATTR '_offset'
    0xb4, // LOAD_FAST 4
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_machine__CounterBase_value = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_machine__CounterBase_value,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 98,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 28,
        .line_info = fun_data_machine__CounterBase_value + 6,
        .line_info_top = fun_data_machine__CounterBase_value + 23,
        .opcodes = fun_data_machine__CounterBase_value + 23,
    },
    #endif
    #endif
};
#else
#define proto_fun_machine__CounterBase_value fun_data_machine__CounterBase_value[0]
#endif

static const mp_raw_code_t *const children_machine__CounterBase[] = {
    (const mp_raw_code_t *)&proto_fun_machine__CounterBase___new__,
    (const mp_raw_code_t *)&proto_fun_machine__CounterBase___init__,
    (const mp_raw_code_t *)&proto_fun_machine__CounterBase__overflow,
    (const mp_raw_code_t *)&proto_fun_machine__CounterBase_init,
    (const mp_raw_code_t *)&proto_fun_machine__CounterBase_deinit,
    (const mp_raw_code_t *)&proto_fun_machine__CounterBase_value,
};

static const mp_raw_code_truncated_t proto_fun_machine__CounterBase = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_machine__CounterBase,
    .children = (void *)&children_machine__CounterBase,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 73,
    .n_children = 6,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_machine__CounterBase + 3,
        .line_info_top = fun_data_machine__CounterBase + 18,
        .opcodes = fun_data_machine__CounterBase + 18,
    },
    #endif
    #endif
};

// child of machine__lt_module_gt_
// frozen bytecode for file machine.py, scope machine_Counter
static const byte fun_data_machine_Counter[52] = {
    0x08,0x0e, // prelude
    0x0b, // names: Counter
    0x88,0x88,0x23,0x23,0x28,0x68, // code info
    0x11,0x35, // LOAD_NAME '__name__'
    0x16,0x36, // STORE_NAME '__module__'
    0x10,0x0b, // LOAD_CONST_STRING 'Counter'
    0x16,0x37, // STORE_NAME '__qualname__'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x16,0x29, // STORE_NAME 'RISING'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x16,0x2c, // STORE_NAME 'FALLING'
    0x11,0x0a, // LOAD_NAME '_CounterBase'
    0x13,0x0e, // LOAD_ATTR '_PCNT'
    0x13,0x0f, // LOAD_ATTR 'INCREMENT'
    0x16,0x38, // STORE_NAME 'UP'
    0x11,0x0a, // LOAD_NAME '_CounterBase'
    0x13,0x0e, // LOAD_ATTR '_PCNT'
    0x13,0x10, // LOAD_ATTR 'DECREMENT'
    0x16,0x39, // STORE_NAME 'DOWN'
    0x11,0x29, // LOAD_NAME 'RISING'
    0x11,0x38, // LOAD_NAME 'UP'
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x19, // STORE_NAME '_configure'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of machine_Counter
// frozen bytecode for file machine.py, scope machine_Counter__configure
static const byte fun_data_machine_Counter__configure[66] = {
    0xf0,0x84,0x01,0x12, // prelude
    0x19,0x3f,0x42,0x43,0x44, // names: _configure, self, src, edge, direction
    0x80,0x90,0x6d,0x32, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR '_pcnt'
    0x14,0x1a, // LOAD_METHOD 'init'
    0x10,0x26, // LOAD_CONST_STRING 'channel'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x27, // LOAD_CONST_STRING 'pin'
    0xb1, // LOAD_FAST 1
    0x10,0x28, // LOAD_CONST_STRING 'rising'
    0xb2, // LOAD_FAST 2
    0x12,0x0b, // LOAD_GLOBAL 'Counter'
    0x13,0x29, // LOAD_ATTR 'RISING'
    0xef, // BINARY_OP 24 __and__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0xb3, // LOAD_FAST 3
    0x42,0x45, // JUMP 5
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR '_PCNT'
    0x13,0x2a, // LOAD_ATTR 'IGNORE'
    0x10,0x2b, // LOAD_CONST_STRING 'falling'
    0xb2, // LOAD_FAST 2
    0x12,0x0b, // LOAD_GLOBAL 'Counter'
    0x13,0x2c, // LOAD_ATTR 'FALLING'
    0xef, // BINARY_OP 24 __and__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0xb3, // LOAD_FAST 3
    0x42,0x45, // JUMP 5
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR '_PCNT'
    0x13,0x2a, // LOAD_ATTR 'IGNORE'
    0x36,0x88,0x00, // CALL_METHOD 1024
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_machine_Counter__configure = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_machine_Counter__configure,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 66,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 15,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 25,
        .line_info = fun_data_machine_Counter__configure + 9,
        .line_info_top = fun_data_machine_Counter__configure + 13,
        .opcodes = fun_data_machine_Counter__configure + 13,
    },
    #endif
    #endif
};
#else
#define proto_fun_machine_Counter__configure fun_data_machine_Counter__configure[0]
#endif

static const mp_raw_code_t *const children_machine_Counter[] = {
    (const mp_raw_code_t *)&proto_fun_machine_Counter__configure,
};

static const mp_raw_code_truncated_t proto_fun_machine_Counter = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_machine_Counter,
    .children = (void *)&children_machine_Counter,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 52,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_machine_Counter + 3,
        .line_info_top = fun_data_machine_Counter + 9,
        .opcodes = fun_data_machine_Counter + 9,
    },
    #endif
    #endif
};

// child of machine__lt_module_gt_
// frozen bytecode for file machine.py, scope machine_Encoder
static const byte fun_data_machine_Encoder[29] = {
    0x08,0x0a, // prelude
    0x0c, // names: Encoder
    0x88,0x99,0x88,0x1b, // code info
    0x11,0x35, // LOAD_NAME '__name__'
    0x16,0x36, // STORE_NAME '__module__'
    0x10,0x0c, // LOAD_CONST_STRING 'Encoder'
    0x16,0x37, // STORE_NAME '__qualname__'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x19, // STORE_NAME '_configure'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x2d, // STORE_NAME 'phases'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of machine_Encoder
// frozen bytecode for file machine.py, scope machine_Encoder__configure
static const byte fun_data_machine_Encoder__configure[162] = {
    0x90,0x15,0x28, // prelude
    0x19,0x3f,0x45,0x46,0x2d, // names: _configure, self, phase_a, phase_b, phases
    0x80,0x9a,0x27,0x47,0x6d,0x27,0x4a,0x55,0x45,0x6d,0x27,0x4a,0x6b,0x20,0x36, // code info
    0xb3, // LOAD_FAST 3
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xdd, // BINARY_OP 6 <in>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x3d, // LOAD_GLOBAL 'ValueError'
    0x10,0x2d, // LOAD_CONST_STRING 'phases'
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR '_pcnt'
    0x14,0x1a, // LOAD_METHOD 'init'
    0x10,0x26, // LOAD_CONST_STRING 'channel'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x27, // LOAD_CONST_STRING 'pin'
    0xb1, // LOAD_FAST 1
    0x10,0x2b, // LOAD_CONST_STRING 'falling'
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR '_PCNT'
    0x13,0x0f, // LOAD_ATTR 'INCREMENT'
    0x10,0x28, // LOAD_CONST_STRING 'rising'
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR '_PCNT'
    0x13,0x10, // LOAD_ATTR 'DECREMENT'
    0x10,0x2e, // LOAD_CONST_STRING 'mode_pin'
    0xb2, // LOAD_FAST 2
    0x10,0x2f, // LOAD_CONST_STRING 'mode_low'
    0xb3, // LOAD_FAST 3
    0x81, // LOAD_CONST_SMALL_INT 1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR '_PCNT'
    0x13,0x30, // LOAD_ATTR 'HOLD'
    0x42,0x45, // JUMP 5
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR '_PCNT'
    0x13,0x31, // LOAD_ATTR 'REVERSE'
    0x36,0x8c,0x00, // CALL_METHOD 1536
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0x84, // LOAD_CONST_SMALL_INT 4
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x69, // POP_JUMP_IF_FALSE 41
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR '_pcnt'
    0x14,0x1a, // LOAD_METHOD 'init'
    0x10,0x26, // LOAD_CONST_STRING 'channel'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x27, // LOAD_CONST_STRING 'pin'
    0xb2, // LOAD_FAST 2
    0x10,0x2b, // LOAD_CONST_STRING 'falling'
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR '_PCNT'
    0x13,0x10, // LOAD_ATTR 'DECREMENT'
    0x10,0x28, // LOAD_CONST_STRING 'rising'
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR '_PCNT'
    0x13,0x0f, // LOAD_ATTR 'INCREMENT'
    0x10,0x2e, // LOAD_CONST_STRING 'mode_pin'
    0xb1, // LOAD_FAST 1
    0x10,0x2f, // LOAD_CONST_STRING 'mode_low'
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR '_PCNT'
    0x13,0x31, // LOAD_ATTR 'REVERSE'
    0x36,0x8c,0x00, // CALL_METHOD 1536
    0x59, // POP_TOP
    0x42,0x56, // JUMP 22
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR '_pcnt'
    0x14,0x1a, // LOAD_METHOD 'init'
    0x10,0x26, // LOAD_CONST_STRING 'channel'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x27, // LOAD_CONST_STRING 'pin'
    0x51, // LOAD_CONST_NONE
    0x10,0x28, // LOAD_CONST_STRING 'rising'
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR '_PCNT'
    0x13,0x2a, // LOAD_ATTR 'IGNORE'
    0x36,0x86,0x00, // CALL_METHOD 768
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x32, // STORE_ATTR '_phases'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_machine_Encoder__configure = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_machine_Encoder__configure,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 162,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 19,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 25,
        .line_info = fun_data_machine_Encoder__configure + 8,
        .line_info_top = fun_data_machine_Encoder__configure + 23,
        .opcodes = fun_data_machine_Encoder__configure + 23,
    },
    #endif
    #endif
};
#else
#define proto_fun_machine_Encoder__configure fun_data_machine_Encoder__configure[0]
#endif

// child of machine_Encoder
// frozen bytecode for file machine.py, scope machine_Encoder_phases
static const byte fun_data_machine_Encoder_phases[10] = {
    0x09,0x08, // prelude
    0x2d,0x3f, // names: phases, self
    0x80,0xb5, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x32, // LOAD_ATTR '_phases'
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_machine_Encoder_phases = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_machine_Encoder_phases,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 10,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 45,
        .line_info = fun_data_machine_Encoder_phases + 4,
        .line_info_top = fun_data_machine_Encoder_phases + 6,
        .opcodes = fun_data_machine_Encoder_phases + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_machine_Encoder_phases fun_data_machine_Encoder_phases[0]
#endif

static const mp_raw_code_t *const children_machine_Encoder[] = {
    (const mp_raw_code_t *)&proto_fun_machine_Encoder__configure,
    (const mp_raw_code_t *)&proto_fun_machine_Encoder_phases,
};

static const mp_raw_code_truncated_t proto_fun_machine_Encoder = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_machine_Encoder,
    .children = (void *)&children_machine_Encoder,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 29,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_machine_Encoder + 3,
        .line_info_top = fun_data_machine_Encoder + 7,
        .opcodes = fun_data_machine_Encoder + 7,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_machine__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_machine__CounterBase,
    (const mp_raw_code_t *)&proto_fun_machine_Counter,
    (const mp_raw_code_t *)&proto_fun_machine_Encoder,
};

static const mp_raw_code_truncated_t proto_fun_machine__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_machine__lt_module_gt_,
    .children = (void *)&children_machine__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 127,
    .n_children = 3,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_machine__lt_module_gt_ + 3,
        .line_info_top = fun_data_machine__lt_module_gt_ + 22,
        .opcodes = fun_data_machine__lt_module_gt_ + 22,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_machine[71] = {
    MP_QSTR_machine_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_sys,
    MP_QSTR_path,
    MP_QSTR__star_,
    MP_QSTR_machine,
    MP_QSTR_const,
    MP_QSTR_micropython,
    MP_QSTR_esp32,
    MP_QSTR_PCNT,
    MP_QSTR__CounterBase,
    MP_QSTR_Counter,
    MP_QSTR_Encoder,
    MP_QSTR_filter_ns,
    MP_QSTR__PCNT,
    MP_QSTR_INCREMENT,
    MP_QSTR_DECREMENT,
    MP_QSTR___new__,
    MP_QSTR__INSTANCES,
    MP_QSTR_get,
    MP_QSTR___init__,
    MP_QSTR__unit_id,
    MP_QSTR__pcnt,
    MP_QSTR_min,
    MP_QSTR_max,
    MP_QSTR__configure,
    MP_QSTR_init,
    MP_QSTR_filter,
    MP_QSTR_value,
    MP_QSTR__overflows,
    MP_QSTR__offset,
    MP_QSTR_irq,
    MP_QSTR__overflow,
    MP_QSTR_IRQ_MIN,
    MP_QSTR_IRQ_MAX,
    MP_QSTR_start,
    MP_QSTR_flags,
    MP_QSTR_deinit,
    MP_QSTR_channel,
    MP_QSTR_pin,
    MP_QSTR_rising,
    MP_QSTR_RISING,
    MP_QSTR_IGNORE,
    MP_QSTR_falling,
    MP_QSTR_FALLING,
    MP_QSTR_phases,
    MP_QSTR_mode_pin,
    MP_QSTR_mode_low,
    MP_QSTR_HOLD,
    MP_QSTR_REVERSE,
    MP_QSTR__phases,
    MP_QSTR__path,
    MP_QSTR_hasattr,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_UP,
    MP_QSTR_DOWN,
    MP_QSTR_cls,
    MP_QSTR_unit_id,
    MP_QSTR_isinstance,
    MP_QSTR_ValueError,
    MP_QSTR_object,
    MP_QSTR_self,
    MP_QSTR_pcnt,
    MP_QSTR_RuntimeError,
    MP_QSTR_src,
    MP_QSTR_edge,
    MP_QSTR_direction,
    MP_QSTR_phase_a,
    MP_QSTR_phase_b,
};

// constants
static const mp_rom_obj_tuple_t const_obj_machine_2 = {{&mp_type_tuple}, 3, {
    MP_ROM_INT(1),
    MP_ROM_INT(2),
    MP_ROM_INT(4),
}};

// constant table
static const mp_rom_obj_t const_obj_table_data_machine[3] = {
    MP_ROM_QSTR(MP_QSTR_PCNT_space_in_space_use),
    MP_ROM_QSTR(MP_QSTR_not_space_initialised),
    MP_ROM_PTR(&const_obj_machine_2),
};

static const mp_frozen_module_t frozen_module_machine = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_machine,
        .obj_table = (mp_obj_t *)&const_obj_table_data_machine,
    },
    .proto_fun = &proto_fun_machine__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module _boot
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/_boot.mpy
// - frozen file name: _boot.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file _boot.py, scope _boot__lt_module_gt_
static const byte fun_data__boot__lt_module_gt_[86] = {
    0x2c,0x12, // prelude
    0x01, // names: <module>
    0x26,0x26,0x4c,0x22,0x24,0x74,0x46,0x4a, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'gc'
    0x16,0x02, // STORE_NAME 'gc'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'vfs'
    0x16,0x03, // STORE_NAME 'vfs'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x04, // LOAD_CONST_STRING 'bdev'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x05, // IMPORT_NAME 'flashbdev'
    0x1c,0x04, // IMPORT_FROM 'bdev'
    0x16,0x04, // STORE_NAME 'bdev'
    0x59, // POP_TOP
    0x48,0x11, // SETUP_EXCEPT 17
    0x11,0x04, // LOAD_NAME 'bdev'
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0x11,0x03, // LOAD_NAME 'vfs'
    0x14,0x06, // LOAD_METHOD 'mount'
    0x11,0x04, // LOAD_NAME 'bdev'
    0x10,0x07, // LOAD_CONST_STRING '/'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x4a,0x17, // POP_EXCEPT_JUMP 23
    0x57, // DUP_TOP
    0x11,0x0b, // LOAD_NAME 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x08, // IMPORT_NAME 'inisetup'
    0x16,0x08, // STORE_NAME 'inisetup'
    0x11,0x08, // LOAD_NAME 'inisetup'
    0x14,0x09, // LOAD_METHOD 'setup'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x11,0x02, // LOAD_NAME 'gc'
    0x14,0x0a, // LOAD_METHOD 'collect'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun__boot__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data__boot__lt_module_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 86,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data__boot__lt_module_gt_ + 3,
        .line_info_top = fun_data__boot__lt_module_gt_ + 11,
        .opcodes = fun_data__boot__lt_module_gt_ + 11,
    },
    #endif
    #endif
};
#else
#define proto_fun__boot__lt_module_gt_ fun_data__boot__lt_module_gt_[0]
#endif

static const qstr_short_t const_qstr_table_data__boot[12] = {
    MP_QSTR__boot_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_gc,
    MP_QSTR_vfs,
    MP_QSTR_bdev,
    MP_QSTR_flashbdev,
    MP_QSTR_mount,
    MP_QSTR__slash_,
    MP_QSTR_inisetup,
    MP_QSTR_setup,
    MP_QSTR_collect,
    MP_QSTR_OSError,
};

static const mp_frozen_module_t frozen_module__boot = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data__boot,
        .obj_table = NULL,
    },
    .proto_fun = &proto_fun__boot__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module inisetup
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/inisetup.mpy
// - frozen file name: inisetup.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file inisetup.py, scope inisetup__lt_module_gt_
static const byte fun_data_inisetup__lt_module_gt_[41] = {
    0x08,0x0e, // prelude
    0x01, // names: <module>
    0x26,0x6c,0x84,0x0d,0x84,0x13, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'vfs'
    0x16,0x02, // STORE_NAME 'vfs'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x03, // LOAD_CONST_STRING 'bdev'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x04, // IMPORT_NAME 'flashbdev'
    0x1c,0x03, // IMPORT_FROM 'bdev'
    0x16,0x03, // STORE_NAME 'bdev'
    0x59, // POP_TOP
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x05, // STORE_NAME 'check_bootsec'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x08, // STORE_NAME 'fs_corrupted'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0d, // STORE_NAME 'setup'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of inisetup__lt_module_gt_
// frozen bytecode for file inisetup.py, scope inisetup_check_bootsec
static const byte fun_data_inisetup_check_bootsec[72] = {
    0x40,0x18, // prelude
    0x05, // names: check_bootsec
    0x60,0x40,0x2d,0x29,0x22,0x25,0x27,0x22,0x28,0x23,0x22, // code info
    0x12,0x18, // LOAD_GLOBAL 'bytearray'
    0x12,0x03, // LOAD_GLOBAL 'bdev'
    0x14,0x06, // LOAD_METHOD 'ioctl'
    0x85, // LOAD_CONST_SMALL_INT 5
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0xc0, // STORE_FAST 0
    0x12,0x03, // LOAD_GLOBAL 'bdev'
    0x14,0x07, // LOAD_METHOD 'readblocks'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x52, // LOAD_CONST_TRUE
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x12, // FOR_ITER 18
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x22,0x81,0x7f, // LOAD_CONST_SMALL_INT 255
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x50, // LOAD_CONST_FALSE
    0xc1, // STORE_FAST 1
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x42,0x42, // JUMP 2
    0x42,0x2c, // JUMP -20
    0xb1, // LOAD_FAST 1
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
    0x12,0x08, // LOAD_GLOBAL 'fs_corrupted'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_inisetup_check_bootsec = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_inisetup_check_bootsec,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 72,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 5,
        .line_info = fun_data_inisetup_check_bootsec + 3,
        .line_info_top = fun_data_inisetup_check_bootsec + 14,
        .opcodes = fun_data_inisetup_check_bootsec + 14,
    },
    #endif
    #endif
};
#else
#define proto_fun_inisetup_check_bootsec fun_data_inisetup_check_bootsec[0]
#endif

// child of inisetup__lt_module_gt_
// frozen bytecode for file inisetup.py, scope inisetup_fs_corrupted
static const byte fun_data_inisetup_fs_corrupted[47] = {
    0x20,0x14, // prelude
    0x08, // names: fs_corrupted
    0x80,0x12,0x25,0x65,0x47,0x20,0x22,0x85,0x07, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x09, // IMPORT_NAME 'time'
    0xc0, // STORE_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x0a, // IMPORT_NAME 'micropython'
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x0b, // LOAD_METHOD 'kbd_intr'
    0x83, // LOAD_CONST_SMALL_INT 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x19, // LOAD_GLOBAL 'print'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x0c, // LOAD_METHOD 'sleep'
    0x83, // LOAD_CONST_SMALL_INT 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x30, // JUMP -16
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_inisetup_fs_corrupted = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_inisetup_fs_corrupted,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 47,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 8,
        .line_info = fun_data_inisetup_fs_corrupted + 3,
        .line_info_top = fun_data_inisetup_fs_corrupted + 12,
        .opcodes = fun_data_inisetup_fs_corrupted + 12,
    },
    #endif
    #endif
};
#else
#define proto_fun_inisetup_fs_corrupted fun_data_inisetup_fs_corrupted[0]
#endif

// child of inisetup__lt_module_gt_
// frozen bytecode for file inisetup.py, scope inisetup_setup
static const byte fun_data_inisetup_setup[134] = {
    0x34,0x20, // prelude
    0x0d, // names: setup
    0x80,0x25,0x25,0x27,0x2d,0x2b,0x2b,0x2d,0x2b,0x2b,0x2a,0x2b,0x23,0x88,0x08, // code info
    0x12,0x05, // LOAD_GLOBAL 'check_bootsec'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x12,0x19, // LOAD_GLOBAL 'print'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x03, // LOAD_GLOBAL 'bdev'
    0x14,0x0e, // LOAD_METHOD 'info'
    0x36,0x00, // CALL_METHOD 0
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0x10,0x02, // LOAD_CONST_STRING 'vfs'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x56, // POP_JUMP_IF_FALSE 22
    0x12,0x02, // LOAD_GLOBAL 'vfs'
    0x13,0x0f, // LOAD_ATTR 'VfsLfs2'
    0x14,0x10, // LOAD_METHOD 'mkfs'
    0x12,0x03, // LOAD_GLOBAL 'bdev'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x02, // LOAD_GLOBAL 'vfs'
    0x14,0x0f, // LOAD_METHOD 'VfsLfs2'
    0x12,0x03, // LOAD_GLOBAL 'bdev'
    0x36,0x01, // CALL_METHOD 1
    0xc0, // STORE_FAST 0
    0x42,0x63, // JUMP 35
    0x12,0x03, // LOAD_GLOBAL 'bdev'
    0x14,0x0e, // LOAD_METHOD 'info'
    0x36,0x00, // CALL_METHOD 0
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0x10,0x11, // LOAD_CONST_STRING 'ffat'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x56, // POP_JUMP_IF_FALSE 22
    0x12,0x02, // LOAD_GLOBAL 'vfs'
    0x13,0x12, // LOAD_ATTR 'VfsFat'
    0x14,0x10, // LOAD_METHOD 'mkfs'
    0x12,0x03, // LOAD_GLOBAL 'bdev'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x02, // LOAD_GLOBAL 'vfs'
    0x14,0x12, // LOAD_METHOD 'VfsFat'
    0x12,0x03, // LOAD_GLOBAL 'bdev'
    0x36,0x01, // CALL_METHOD 1
    0xc0, // STORE_FAST 0
    0x42,0x40, // JUMP 0
    0x12,0x02, // LOAD_GLOBAL 'vfs'
    0x14,0x13, // LOAD_METHOD 'mount'
    0xb0, // LOAD_FAST 0
    0x10,0x14, // LOAD_CONST_STRING '/'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x12,0x1a, // LOAD_GLOBAL 'open'
    0x10,0x15, // LOAD_CONST_STRING 'boot.py'
    0x10,0x16, // LOAD_CONST_STRING 'w'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x0a, // SETUP_WITH 10
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x17, // LOAD_METHOD 'write'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_inisetup_setup = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_inisetup_setup,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 134,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_inisetup_setup + 3,
        .line_info_top = fun_data_inisetup_setup + 18,
        .opcodes = fun_data_inisetup_setup + 18,
    },
    #endif
    #endif
};
#else
#define proto_fun_inisetup_setup fun_data_inisetup_setup[0]
#endif

static const mp_raw_code_t *const children_inisetup__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_inisetup_check_bootsec,
    (const mp_raw_code_t *)&proto_fun_inisetup_fs_corrupted,
    (const mp_raw_code_t *)&proto_fun_inisetup_setup,
};

static const mp_raw_code_truncated_t proto_fun_inisetup__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_inisetup__lt_module_gt_,
    .children = (void *)&children_inisetup__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 41,
    .n_children = 3,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_inisetup__lt_module_gt_ + 3,
        .line_info_top = fun_data_inisetup__lt_module_gt_ + 9,
        .opcodes = fun_data_inisetup__lt_module_gt_ + 9,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_inisetup[27] = {
    MP_QSTR_inisetup_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_vfs,
    MP_QSTR_bdev,
    MP_QSTR_flashbdev,
    MP_QSTR_check_bootsec,
    MP_QSTR_ioctl,
    MP_QSTR_readblocks,
    MP_QSTR_fs_corrupted,
    MP_QSTR_time,
    MP_QSTR_micropython,
    MP_QSTR_kbd_intr,
    MP_QSTR_sleep,
    MP_QSTR_setup,
    MP_QSTR_info,
    MP_QSTR_VfsLfs2,
    MP_QSTR_mkfs,
    MP_QSTR_ffat,
    MP_QSTR_VfsFat,
    MP_QSTR_mount,
    MP_QSTR__slash_,
    MP_QSTR_boot_dot_py,
    MP_QSTR_w,
    MP_QSTR_write,
    MP_QSTR_bytearray,
    MP_QSTR_print,
    MP_QSTR_open,
};

// constants
static const mp_obj_str_t const_obj_inisetup_0 = {{&mp_type_str}, 55993, 258, (const byte*)"\x54\x68\x65\x20\x66\x69\x6c\x65\x73\x79\x73\x74\x65\x6d\x20\x61\x70\x70\x65\x61\x72\x73\x20\x74\x6f\x20\x62\x65\x20\x63\x6f\x72\x72\x75\x70\x74\x65\x64\x2e\x20\x49\x66\x20\x79\x6f\x75\x20\x68\x61\x64\x20\x69\x6d\x70\x6f\x72\x74\x61\x6e\x74\x20\x64\x61\x74\x61\x20\x74\x68\x65\x72\x65\x2c\x20\x79\x6f\x75\x0a\x6d\x61\x79\x20\x77\x61\x6e\x74\x20\x74\x6f\x20\x6d\x61\x6b\x65\x20\x61\x20\x66\x6c\x61\x73\x68\x20\x73\x6e\x61\x70\x73\x68\x6f\x74\x20\x74\x6f\x20\x74\x72\x79\x20\x74\x6f\x20\x72\x65\x63\x6f\x76\x65\x72\x20\x69\x74\x2e\x20\x4f\x74\x68\x65\x72\x77\x69\x73\x65\x2c\x20\x70\x65\x72\x66\x6f\x72\x6d\x0a\x66\x61\x63\x74\x6f\x72\x79\x20\x72\x65\x70\x72\x6f\x67\x72\x61\x6d\x6d\x69\x6e\x67\x20\x6f\x66\x20\x4d\x69\x63\x72\x6f\x50\x79\x74\x68\x6f\x6e\x20\x66\x69\x72\x6d\x77\x61\x72\x65\x20\x28\x63\x6f\x6d\x70\x6c\x65\x74\x65\x6c\x79\x20\x65\x72\x61\x73\x65\x20\x66\x6c\x61\x73\x68\x2c\x20\x66\x6f\x6c\x6c\x6f\x77\x65\x64\x0a\x62\x79\x20\x66\x69\x72\x6d\x77\x61\x72\x65\x20\x70\x72\x6f\x67\x72\x61\x6d\x6d\x69\x6e\x67\x29\x2e\x0a"};
static const mp_obj_str_t const_obj_inisetup_2 = {{&mp_type_str}, 18582, 139, (const byte*)"\x23\x20\x54\x68\x69\x73\x20\x66\x69\x6c\x65\x20\x69\x73\x20\x65\x78\x65\x63\x75\x74\x65\x64\x20\x6f\x6e\x20\x65\x76\x65\x72\x79\x20\x62\x6f\x6f\x74\x20\x28\x69\x6e\x63\x6c\x75\x64\x69\x6e\x67\x20\x77\x61\x6b\x65\x2d\x62\x6f\x6f\x74\x20\x66\x72\x6f\x6d\x20\x64\x65\x65\x70\x73\x6c\x65\x65\x70\x29\x0a\x23\x69\x6d\x70\x6f\x72\x74\x20\x65\x73\x70\x0a\x23\x65\x73\x70\x2e\x6f\x73\x64\x65\x62\x75\x67\x28\x4e\x6f\x6e\x65\x29\x0a\x23\x69\x6d\x70\x6f\x72\x74\x20\x77\x65\x62\x72\x65\x70\x6c\x0a\x23\x77\x65\x62\x72\x65\x70\x6c\x2e\x73\x74\x61\x72\x74\x28\x29\x0a"};

// constant table
static const mp_rom_obj_t const_obj_table_data_inisetup[3] = {
    MP_ROM_PTR(&const_obj_inisetup_0),
    MP_ROM_QSTR(MP_QSTR_Performing_space_initial_space_setup),
    MP_ROM_PTR(&const_obj_inisetup_2),
};

static const mp_frozen_module_t frozen_module_inisetup = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_inisetup,
        .obj_table = (mp_obj_t *)&const_obj_table_data_inisetup,
    },
    .proto_fun = &proto_fun_inisetup__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module recovery
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/recovery.mpy
// - frozen file name: recovery.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file recovery.py, scope recovery__lt_module_gt_
static const byte fun_data_recovery__lt_module_gt_[132] = {
    0x10,0x48, // prelude
    0x01, // names: <module>
    0x80,0x20,0x26,0x26,0x86,0x07,0x84,0x0d,0x64,0x40,0x84,0x08,0x84,0x07,0x84,0x08,0x84,0x08,0x84,0x0c,0x84,0x0f,0x84,0x0c,0x84,0x09,0x84,0x0f,0x84,0x13,0x99,0x6b,0x64,0x20,0x28, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'os'
    0x16,0x02, // STORE_NAME 'os'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'sys'
    0x16,0x03, // STORE_NAME 'sys'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x04, // IMPORT_NAME 'time'
    0x16,0x04, // STORE_NAME 'time'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x07, // STORE_NAME '_normalize_path'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x0f, // STORE_NAME '_join'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x11, // STORE_NAME '_exists'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x13, // STORE_NAME '_is_dir'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x14, // STORE_NAME '_size'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x15, // STORE_NAME '_human_size'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x1a, // STORE_NAME '_mkdirs'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x1c, // STORE_NAME '_rm_recursive'
    0x32,0x08, // MAKE_FUNCTION 8
    0x16,0x1f, // STORE_NAME '_listdir_safe'
    0x32,0x09, // MAKE_FUNCTION 9
    0x16,0x21, // STORE_NAME '_try_import_recovery'
    0x32,0x0a, // MAKE_FUNCTION 10
    0x16,0x23, // STORE_NAME '_try_import_services'
    0x32,0x0b, // MAKE_FUNCTION 11
    0x16,0x27, // STORE_NAME '_try_get_package_manager'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x0c, // MAKE_FUNCTION 12
    0x10,0x05, // LOAD_CONST_STRING 'RecoveryShell'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x05, // STORE_NAME 'RecoveryShell'
    0x32,0x0d, // MAKE_FUNCTION 13
    0x16,0x2a, // STORE_NAME 'run'
    0x11,0x81,0x09, // LOAD_NAME '__name__'
    0x10,0x06, // LOAD_CONST_STRING '__main__'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0x11,0x2a, // LOAD_NAME 'run'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of recovery__lt_module_gt_
// frozen bytecode for file recovery.py, scope recovery__normalize_path
static const byte fun_data_recovery__normalize_path[85] = {
    0x49,0x1c, // prelude
    0x07,0x81,0x0a, // names: _normalize_path, path
    0x80,0x2a,0x23,0x2f,0x26,0x22,0x26,0x23,0x26,0x22,0x29, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x45,0x02, // JUMP_IF_TRUE_OR_POP 2
    0x10,0x08, // LOAD_CONST_STRING ''
    0x14,0x09, // LOAD_METHOD 'split'
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x5f, // GET_ITER_STACK
    0x4b,0x23, // FOR_ITER 35
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xdd, // BINARY_OP 6 <in>
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x42,0x35, // JUMP -11
    0xb2, // LOAD_FAST 2
    0x10,0x0b, // LOAD_CONST_STRING '..'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0xb1, // LOAD_FAST 1
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0xb1, // LOAD_FAST 1
    0x14,0x0c, // LOAD_METHOD 'pop'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x42,0x24, // JUMP -28
    0xb1, // LOAD_FAST 1
    0x14,0x0d, // LOAD_METHOD 'append'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x1b, // JUMP -37
    0xb1, // LOAD_FAST 1
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x14,0x0e, // LOAD_METHOD 'join'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xf2, // BINARY_OP 27 __add__
    0x63, // RETURN_VALUE
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery__normalize_path = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery__normalize_path,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 85,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_recovery__normalize_path + 5,
        .line_info_top = fun_data_recovery__normalize_path + 16,
        .opcodes = fun_data_recovery__normalize_path + 16,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery__normalize_path fun_data_recovery__normalize_path[0]
#endif

// child of recovery__lt_module_gt_
// frozen bytecode for file recovery.py, scope recovery__join
static const byte fun_data_recovery__join[41] = {
    0x22,0x10, // prelude
    0x0f,0x81,0x0b,0x81,0x0c, // names: _join, parent, name
    0x80,0x37,0x30, // code info
    0xb0, // LOAD_FAST 0
    0x45,0x02, // JUMP_IF_TRUE_OR_POP 2
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x14,0x10, // LOAD_METHOD 'rstrip'
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x45,0x02, // JUMP_IF_TRUE_OR_POP 2
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xc0, // STORE_FAST 0
    0xb0, // LOAD_FAST 0
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xf2, // BINARY_OP 27 __add__
    0xb1, // LOAD_FAST 1
    0xf2, // BINARY_OP 27 __add__
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery__join = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery__join,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 41,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 15,
        .line_info = fun_data_recovery__join + 7,
        .line_info_top = fun_data_recovery__join + 10,
        .opcodes = fun_data_recovery__join + 10,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery__join fun_data_recovery__join[0]
#endif

// child of recovery__lt_module_gt_
// frozen bytecode for file recovery.py, scope recovery__exists
static const byte fun_data_recovery__exists[35] = {
    0x35,0x10, // prelude
    0x11,0x81,0x0a, // names: _exists, path
    0x80,0x3c,0x22,0x28,0x4a, // code info
    0x48,0x0a, // SETUP_EXCEPT 10
    0x12,0x02, // LOAD_GLOBAL 'os'
    0x14,0x12, // LOAD_METHOD 'stat'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
    0x57, // DUP_TOP
    0x12,0x81,0x0d, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery__exists = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery__exists,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 35,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 17,
        .line_info = fun_data_recovery__exists + 5,
        .line_info_top = fun_data_recovery__exists + 10,
        .opcodes = fun_data_recovery__exists + 10,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery__exists fun_data_recovery__exists[0]
#endif

// child of recovery__lt_module_gt_
// frozen bytecode for file recovery.py, scope recovery__is_dir
static const byte fun_data_recovery__is_dir[44] = {
    0x35,0x0e, // prelude
    0x13,0x81,0x0a, // names: _is_dir, path
    0x80,0x44,0x22,0x5c, // code info
    0x48,0x14, // SETUP_EXCEPT 20
    0x12,0x81,0x0e, // LOAD_GLOBAL 'bool'
    0x12,0x02, // LOAD_GLOBAL 'os'
    0x14,0x12, // LOAD_METHOD 'stat'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x22,0x81,0x80,0x00, // LOAD_CONST_SMALL_INT 16384
    0xef, // BINARY_OP 24 __and__
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
    0x57, // DUP_TOP
    0x12,0x81,0x0d, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery__is_dir = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery__is_dir,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 44,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 19,
        .line_info = fun_data_recovery__is_dir + 5,
        .line_info_top = fun_data_recovery__is_dir + 9,
        .opcodes = fun_data_recovery__is_dir + 9,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery__is_dir fun_data_recovery__is_dir[0]
#endif

// child of recovery__lt_module_gt_
// frozen bytecode for file recovery.py, scope recovery__size
static const byte fun_data_recovery__size[68] = {
    0x3d,0x12, // prelude
    0x14,0x81,0x0a, // names: _size, path
    0x80,0x4b,0x22,0x28,0x1f,0x4b, // code info
    0x48,0x2a, // SETUP_EXCEPT 42
    0x12,0x02, // LOAD_GLOBAL 'os'
    0x14,0x12, // LOAD_METHOD 'stat'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0xc1, // STORE_FAST 1
    0x12,0x81,0x0e, // LOAD_GLOBAL 'bool'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x22,0x81,0x80,0x00, // LOAD_CONST_SMALL_INT 16384
    0xef, // BINARY_OP 24 __and__
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x63, // RETURN_VALUE
    0x12,0x81,0x0f, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x86, // LOAD_CONST_SMALL_INT 6
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb1, // LOAD_FAST 1
    0x86, // LOAD_CONST_SMALL_INT 6
    0x55, // LOAD_SUBSCR
    0x42,0x41, // JUMP 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x63, // RETURN_VALUE
    0x57, // DUP_TOP
    0x12,0x81,0x0d, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x63, // RETURN_VALUE
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery__size = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery__size,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 68,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 20,
        .line_info = fun_data_recovery__size + 5,
        .line_info_top = fun_data_recovery__size + 11,
        .opcodes = fun_data_recovery__size + 11,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery__size fun_data_recovery__size[0]
#endif

// child of recovery__lt_module_gt_
// frozen bytecode for file recovery.py, scope recovery__human_size
static const byte fun_data_recovery__human_size[61] = {
    0x21,0x12, // prelude
    0x15,0x81,0x10, // names: _human_size, n
    0x80,0x53,0x27,0x28,0x29,0x2c, // code info
    0xb0, // LOAD_FAST 0
    0x22,0x88,0x00, // LOAD_CONST_SMALL_INT 1024
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x10,0x16, // LOAD_CONST_STRING '{} B'
    0x14,0x17, // LOAD_METHOD 'format'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x22,0x80,0xc0,0x80,0x00, // LOAD_CONST_SMALL_INT 1048576
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0x10,0x18, // LOAD_CONST_STRING '{:.2f} KB'
    0x14,0x17, // LOAD_METHOD 'format'
    0xb0, // LOAD_FAST 0
    0x22,0x88,0x00, // LOAD_CONST_SMALL_INT 1024
    0xf7, // BINARY_OP 32 __truediv__
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
    0x10,0x19, // LOAD_CONST_STRING '{:.2f} MB'
    0x14,0x17, // LOAD_METHOD 'format'
    0xb0, // LOAD_FAST 0
    0x22,0x80,0xc0,0x80,0x00, // LOAD_CONST_SMALL_INT 1048576
    0xf7, // BINARY_OP 32 __truediv__
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery__human_size = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery__human_size,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 61,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 21,
        .line_info = fun_data_recovery__human_size + 5,
        .line_info_top = fun_data_recovery__human_size + 11,
        .opcodes = fun_data_recovery__human_size + 11,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery__human_size fun_data_recovery__human_size[0]
#endif

// child of recovery__lt_module_gt_
// frozen bytecode for file recovery.py, scope recovery__mkdirs
static const byte fun_data_recovery__mkdirs[77] = {
    0x65,0x18, // prelude
    0x1a,0x81,0x0a, // names: _mkdirs, path
    0x80,0x5b,0x20,0x23,0x33,0x27,0x27,0x22,0x52, // code info
    0x10,0x08, // LOAD_CONST_STRING ''
    0xc1, // STORE_FAST 1
    0x32,0x00, // MAKE_FUNCTION 0
    0xb0, // LOAD_FAST 0
    0x45,0x02, // JUMP_IF_TRUE_OR_POP 2
    0x10,0x08, // LOAD_CONST_STRING ''
    0x14,0x09, // LOAD_METHOD 'split'
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x5f, // GET_ITER_STACK
    0x4b,0x28, // FOR_ITER 40
    0xc2, // STORE_FAST 2
    0xb1, // LOAD_FAST 1
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xb2, // LOAD_FAST 2
    0xf2, // BINARY_OP 27 __add__
    0xe5, // BINARY_OP 14 __iadd__
    0xc1, // STORE_FAST 1
    0x12,0x11, // LOAD_GLOBAL '_exists'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x57, // POP_JUMP_IF_TRUE 23
    0x48,0x0a, // SETUP_EXCEPT 10
    0x12,0x02, // LOAD_GLOBAL 'os'
    0x14,0x1b, // LOAD_METHOD 'mkdir'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0x0b, // POP_EXCEPT_JUMP 11
    0x57, // DUP_TOP
    0x12,0x81,0x0d, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x42,0x16, // JUMP -42
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of recovery__mkdirs
// frozen bytecode for file recovery.py, scope recovery__mkdirs__lt_listcomp_gt_
static const byte fun_data_recovery__mkdirs__lt_listcomp_gt_[23] = {
    0x39,0x0a, // prelude
    0x2b,0x81,0x17, // names: <listcomp>, *
    0x80,0x5d, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x09, // FOR_ITER 9
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x44,0x3a, // POP_JUMP_IF_FALSE -6
    0xb1, // LOAD_FAST 1
    0x2f,0x14, // STORE_COMP 20
    0x42,0x35, // JUMP -11
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery__mkdirs__lt_listcomp_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery__mkdirs__lt_listcomp_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 23,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 43,
        .line_info = fun_data_recovery__mkdirs__lt_listcomp_gt_ + 5,
        .line_info_top = fun_data_recovery__mkdirs__lt_listcomp_gt_ + 7,
        .opcodes = fun_data_recovery__mkdirs__lt_listcomp_gt_ + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery__mkdirs__lt_listcomp_gt_ fun_data_recovery__mkdirs__lt_listcomp_gt_[0]
#endif

static const mp_raw_code_t *const children_recovery__mkdirs[] = {
    (const mp_raw_code_t *)&proto_fun_recovery__mkdirs__lt_listcomp_gt_,
};

static const mp_raw_code_truncated_t proto_fun_recovery__mkdirs = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery__mkdirs,
    .children = (void *)&children_recovery__mkdirs,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 77,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 26,
        .line_info = fun_data_recovery__mkdirs + 5,
        .line_info_top = fun_data_recovery__mkdirs + 14,
        .opcodes = fun_data_recovery__mkdirs + 14,
    },
    #endif
    #endif
};

// child of recovery__lt_module_gt_
// frozen bytecode for file recovery.py, scope recovery__rm_recursive
static const byte fun_data_recovery__rm_recursive[144] = {
    0xd1,0x02,0x1a, // prelude
    0x1c,0x81,0x0a, // names: _rm_recursive, path
    0x80,0x67,0x28,0x2d,0x2d,0x22,0x54,0x59,0x22,0x54, // code info
    0x12,0x13, // LOAD_GLOBAL '_is_dir'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0xc9,0x80, // POP_JUMP_IF_FALSE 73
    0x12,0x1f, // LOAD_GLOBAL '_listdir_safe'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x45,0x02, // JUMP_IF_TRUE_OR_POP 2
    0x2b,0x00, // BUILD_LIST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x0e, // FOR_ITER 14
    0xc1, // STORE_FAST 1
    0x12,0x1c, // LOAD_GLOBAL '_rm_recursive'
    0x12,0x0f, // LOAD_GLOBAL '_join'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x30, // JUMP -16
    0x48,0x0a, // SETUP_EXCEPT 10
    0x12,0x02, // LOAD_GLOBAL 'os'
    0x14,0x1d, // LOAD_METHOD 'rmdir'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0x21, // POP_EXCEPT_JUMP 33
    0x57, // DUP_TOP
    0x12,0x81,0x0d, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0xc2, // STORE_FAST 2
    0x49,0x0f, // SETUP_FINALLY 15
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x14,0x17, // LOAD_METHOD 'format'
    0xb0, // LOAD_FAST 0
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc2, // STORE_FAST 2
    0x28,0x02, // DELETE_FAST 2
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x42,0x6d, // JUMP 45
    0x48,0x0a, // SETUP_EXCEPT 10
    0x12,0x02, // LOAD_GLOBAL 'os'
    0x14,0x1e, // LOAD_METHOD 'remove'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0x21, // POP_EXCEPT_JUMP 33
    0x57, // DUP_TOP
    0x12,0x81,0x0d, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0xc2, // STORE_FAST 2
    0x49,0x0f, // SETUP_FINALLY 15
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x14,0x17, // LOAD_METHOD 'format'
    0xb0, // LOAD_FAST 0
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc2, // STORE_FAST 2
    0x28,0x02, // DELETE_FAST 2
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery__rm_recursive = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery__rm_recursive,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 144,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 28,
        .line_info = fun_data_recovery__rm_recursive + 6,
        .line_info_top = fun_data_recovery__rm_recursive + 16,
        .opcodes = fun_data_recovery__rm_recursive + 16,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery__rm_recursive fun_data_recovery__rm_recursive[0]
#endif

// child of recovery__lt_module_gt_
// frozen bytecode for file recovery.py, scope recovery__listdir_safe
static const byte fun_data_recovery__listdir_safe[57] = {
    0xc9,0x02,0x10, // prelude
    0x1f,0x81,0x0a, // names: _listdir_safe, path
    0x80,0x76,0x22,0x52,0x2e, // code info
    0x48,0x08, // SETUP_EXCEPT 8
    0x12,0x02, // LOAD_GLOBAL 'os'
    0x14,0x20, // LOAD_METHOD 'listdir'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
    0x57, // DUP_TOP
    0x12,0x81,0x0d, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x5a, // POP_JUMP_IF_FALSE 26
    0xc1, // STORE_FAST 1
    0x49,0x10, // SETUP_FINALLY 16
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x14,0x17, // LOAD_METHOD 'format'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0xc1, // STORE_FAST 1
    0x28,0x01, // DELETE_FAST 1
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery__listdir_safe = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery__listdir_safe,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 57,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 31,
        .line_info = fun_data_recovery__listdir_safe + 6,
        .line_info_top = fun_data_recovery__listdir_safe + 11,
        .opcodes = fun_data_recovery__listdir_safe + 11,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery__listdir_safe fun_data_recovery__listdir_safe[0]
#endif

// child of recovery__lt_module_gt_
// frozen bytecode for file recovery.py, scope recovery__try_import_recovery
static const byte fun_data_recovery__try_import_recovery[60] = {
    0xc0,0x02,0x0e, // prelude
    0x21, // names: _try_import_recovery
    0x80,0x82,0x22,0x2b,0x4c,0x2d, // code info
    0x48,0x0d, // SETUP_EXCEPT 13
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x22, // LOAD_CONST_STRING 'Recovery'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x22, // IMPORT_NAME 'Recovery'
    0x1c,0x22, // IMPORT_FROM 'Recovery'
    0xc0, // STORE_FAST 0
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
    0x57, // DUP_TOP
    0x12,0x81,0x12, // LOAD_GLOBAL 'ImportError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0xc1, // STORE_FAST 1
    0x49,0x0f, // SETUP_FINALLY 15
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x14,0x17, // LOAD_METHOD 'format'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0xc1, // STORE_FAST 1
    0x28,0x01, // DELETE_FAST 1
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery__try_import_recovery = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery__try_import_recovery,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 60,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 33,
        .line_info = fun_data_recovery__try_import_recovery + 4,
        .line_info_top = fun_data_recovery__try_import_recovery + 10,
        .opcodes = fun_data_recovery__try_import_recovery + 10,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery__try_import_recovery fun_data_recovery__try_import_recovery[0]
#endif

// child of recovery__lt_module_gt_
// frozen bytecode for file recovery.py, scope recovery__try_import_services
static const byte fun_data_recovery__try_import_services[88] = {
    0xc8,0x02,0x14, // prelude
    0x23, // names: _try_import_services
    0x80,0x8b,0x60,0x22,0x35,0x2a,0x25,0x4c,0x4d, // code info
    0x48,0x26, // SETUP_EXCEPT 38
    0x32,0x00, // MAKE_FUNCTION 0
    0x12,0x81,0x02, // LOAD_GLOBAL 'list'
    0x12,0x03, // LOAD_GLOBAL 'sys'
    0x13,0x24, // LOAD_ATTR 'modules'
    0x14,0x25, // LOAD_METHOD 'keys'
    0x36,0x00, // CALL_METHOD 0
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x5f, // GET_ITER_STACK
    0x4b,0x0b, // FOR_ITER 11
    0xc0, // STORE_FAST 0
    0x12,0x03, // LOAD_GLOBAL 'sys'
    0x13,0x24, // LOAD_ATTR 'modules'
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0x5b, // ROT_THREE
    0x56, // STORE_SUBSCR
    0x42,0x33, // JUMP -13
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x26, // IMPORT_NAME 'Services'
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
    0x57, // DUP_TOP
    0x12,0x81,0x13, // LOAD_GLOBAL 'Exception'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0xc2, // STORE_FAST 2
    0x49,0x0f, // SETUP_FINALLY 15
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x14,0x17, // LOAD_METHOD 'format'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0xc2, // STORE_FAST 2
    0x28,0x02, // DELETE_FAST 2
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of recovery__try_import_services
// frozen bytecode for file recovery.py, scope recovery__try_import_services__lt_listcomp_gt_
static const byte fun_data_recovery__try_import_services__lt_listcomp_gt_[26] = {
    0x41,0x0a, // prelude
    0x2b,0x81,0x17, // names: <listcomp>, *
    0x80,0x8f, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x0c, // FOR_ITER 12
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x10,0x26, // LOAD_CONST_STRING 'Services'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x37, // POP_JUMP_IF_FALSE -9
    0xb1, // LOAD_FAST 1
    0x2f,0x14, // STORE_COMP 20
    0x42,0x32, // JUMP -14
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery__try_import_services__lt_listcomp_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery__try_import_services__lt_listcomp_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 26,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 43,
        .line_info = fun_data_recovery__try_import_services__lt_listcomp_gt_ + 5,
        .line_info_top = fun_data_recovery__try_import_services__lt_listcomp_gt_ + 7,
        .opcodes = fun_data_recovery__try_import_services__lt_listcomp_gt_ + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery__try_import_services__lt_listcomp_gt_ fun_data_recovery__try_import_services__lt_listcomp_gt_[0]
#endif

static const mp_raw_code_t *const children_recovery__try_import_services[] = {
    (const mp_raw_code_t *)&proto_fun_recovery__try_import_services__lt_listcomp_gt_,
};

static const mp_raw_code_truncated_t proto_fun_recovery__try_import_services = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery__try_import_services,
    .children = (void *)&children_recovery__try_import_services,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 88,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 35,
        .line_info = fun_data_recovery__try_import_services + 4,
        .line_info_top = fun_data_recovery__try_import_services + 13,
        .opcodes = fun_data_recovery__try_import_services + 13,
    },
    #endif
    #endif
};

// child of recovery__lt_module_gt_
// frozen bytecode for file recovery.py, scope recovery__try_get_package_manager
static const byte fun_data_recovery__try_get_package_manager[94] = {
    0xc8,0x02,0x1a, // prelude
    0x27, // names: _try_get_package_manager
    0x80,0x9a,0x25,0x25,0x22,0x2a,0x25,0x28,0x22,0x22,0x4e,0x2d, // code info
    0x12,0x23, // LOAD_GLOBAL '_try_import_services'
    0x34,0x00, // CALL_FUNCTION 0
    0xc0, // STORE_FAST 0
    0xb0, // LOAD_FAST 0
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x81,0x14, // LOAD_GLOBAL 'getattr'
    0xb0, // LOAD_FAST 0
    0x10,0x28, // LOAD_CONST_STRING 'PackageManager'
    0x51, // LOAD_CONST_NONE
    0x34,0x03, // CALL_FUNCTION 3
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x48,0x04, // SETUP_EXCEPT 4
    0xb1, // LOAD_FAST 1
    0x34,0x00, // CALL_FUNCTION 0
    0x63, // RETURN_VALUE
    0x57, // DUP_TOP
    0x12,0x81,0x13, // LOAD_GLOBAL 'Exception'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0xc2, // STORE_FAST 2
    0x49,0x0f, // SETUP_FINALLY 15
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x07, // LOAD_CONST_OBJ 7
    0x14,0x17, // LOAD_METHOD 'format'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0xc2, // STORE_FAST 2
    0x28,0x02, // DELETE_FAST 2
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery__try_get_package_manager = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery__try_get_package_manager,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 94,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 39,
        .line_info = fun_data_recovery__try_get_package_manager + 4,
        .line_info_top = fun_data_recovery__try_get_package_manager + 16,
        .opcodes = fun_data_recovery__try_get_package_manager + 16,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery__try_get_package_manager fun_data_recovery__try_get_package_manager[0]
#endif

// child of recovery__lt_module_gt_
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell
static const byte fun_data_recovery_RecoveryShell[152] = {
    0x00,0x5a, // prelude
    0x05, // names: RecoveryShell
    0x8b,0xae,0x44,0x84,0x08,0x84,0x0d,0x84,0x19,0x84,0x23,0x84,0x15,0x84,0x16,0x84,0x0d,0x64,0x84,0x0b,0x84,0x09,0x84,0x12,0x84,0x10,0x84,0x11,0x84,0x0b,0x84,0x10,0x84,0x0e,0x84,0x0e,0x84,0x0d,0x84,0x34,0x84,0x0e,0x84,0x09, // code info
    0x11,0x81,0x09, // LOAD_NAME '__name__'
    0x16,0x81,0x15, // STORE_NAME '__module__'
    0x10,0x05, // LOAD_CONST_STRING 'RecoveryShell'
    0x16,0x81,0x16, // STORE_NAME '__qualname__'
    0x10,0x29, // LOAD_CONST_STRING 'recovery'
    0x16,0x34, // STORE_NAME 'PROMPT'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x2c, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x30, // STORE_NAME '_abs'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x2a, // STORE_NAME 'run'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x36, // STORE_NAME '_dispatch'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x37, // STORE_NAME '_cmd_help'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x39, // STORE_NAME '_cmd_ls'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x3b, // STORE_NAME '_cmd_cd'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x3d, // STORE_NAME '_cmd_pwd'
    0x32,0x08, // MAKE_FUNCTION 8
    0x16,0x3f, // STORE_NAME '_cmd_cat'
    0x32,0x09, // MAKE_FUNCTION 9
    0x16,0x41, // STORE_NAME '_cmd_mkdir'
    0x32,0x0a, // MAKE_FUNCTION 10
    0x16,0x42, // STORE_NAME '_cmd_rmdir'
    0x32,0x0b, // MAKE_FUNCTION 11
    0x16,0x43, // STORE_NAME '_cmd_rm'
    0x32,0x0c, // MAKE_FUNCTION 12
    0x16,0x45, // STORE_NAME '_cmd_cp'
    0x32,0x0d, // MAKE_FUNCTION 13
    0x16,0x47, // STORE_NAME '_cmd_mv'
    0x32,0x0e, // MAKE_FUNCTION 14
    0x16,0x49, // STORE_NAME '_cmd_touch'
    0x32,0x0f, // MAKE_FUNCTION 15
    0x16,0x4b, // STORE_NAME '_cmd_stat'
    0x32,0x10, // MAKE_FUNCTION 16
    0x16,0x4c, // STORE_NAME '_cmd_df'
    0x32,0x11, // MAKE_FUNCTION 17
    0x16,0x4e, // STORE_NAME '_cmd_free'
    0x32,0x12, // MAKE_FUNCTION 18
    0x16,0x50, // STORE_NAME '_cmd_pkg'
    0x32,0x13, // MAKE_FUNCTION 19
    0x16,0x52, // STORE_NAME '_cmd_recover'
    0x32,0x14, // MAKE_FUNCTION 20
    0x16,0x54, // STORE_NAME '_cmd_reboot'
    0x32,0x15, // MAKE_FUNCTION 21
    0x16,0x56, // STORE_NAME '_cmd_exit'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell___init__
static const byte fun_data_recovery_RecoveryShell___init__[24] = {
    0x11,0x0e, // prelude
    0x2c,0x81,0x18, // names: __init__, self
    0x80,0xb1,0x25,0x24, // code info
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xb0, // LOAD_FAST 0
    0x18,0x2d, // STORE_ATTR 'current_path'
    0x50, // LOAD_CONST_FALSE
    0xb0, // LOAD_FAST 0
    0x18,0x2e, // STORE_ATTR 'running'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x2f, // STORE_ATTR '_pm'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 24,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 44,
        .line_info = fun_data_recovery_RecoveryShell___init__ + 5,
        .line_info_top = fun_data_recovery_RecoveryShell___init__ + 9,
        .opcodes = fun_data_recovery_RecoveryShell___init__ + 9,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell___init__ fun_data_recovery_RecoveryShell___init__[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__abs
static const byte fun_data_recovery_RecoveryShell__abs[68] = {
    0x32,0x1a, // prelude
    0x30,0x81,0x18,0x81,0x0a, // names: _abs, self, path
    0x80,0xb9,0x23,0x24,0x29,0x44,0x37,0x24, // code info
    0xb1, // LOAD_FAST 1
    0x43,0x44, // POP_JUMP_IF_TRUE 4
    0xb0, // LOAD_FAST 0
    0x13,0x2d, // LOAD_ATTR 'current_path'
    0x63, // RETURN_VALUE
    0xb1, // LOAD_FAST 1
    0x14,0x31, // LOAD_METHOD 'startswith'
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0xb1, // LOAD_FAST 1
    0xc2, // STORE_FAST 2
    0x42,0x5b, // JUMP 27
    0xb0, // LOAD_FAST 0
    0x13,0x2d, // LOAD_ATTR 'current_path'
    0x14,0x32, // LOAD_METHOD 'endswith'
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb0, // LOAD_FAST 0
    0x13,0x2d, // LOAD_ATTR 'current_path'
    0x42,0x46, // JUMP 6
    0xb0, // LOAD_FAST 0
    0x13,0x2d, // LOAD_ATTR 'current_path'
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xf2, // BINARY_OP 27 __add__
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0xb1, // LOAD_FAST 1
    0xf2, // BINARY_OP 27 __add__
    0xc2, // STORE_FAST 2
    0x12,0x07, // LOAD_GLOBAL '_normalize_path'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__abs = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__abs,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 68,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 48,
        .line_info = fun_data_recovery_RecoveryShell__abs + 7,
        .line_info_top = fun_data_recovery_RecoveryShell__abs + 15,
        .opcodes = fun_data_recovery_RecoveryShell__abs + 15,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__abs fun_data_recovery_RecoveryShell__abs[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell_run
static const byte fun_data_recovery_RecoveryShell_run[188] = {
    0xc9,0x02,0x2c, // prelude
    0x2a,0x81,0x18, // names: run, self
    0x80,0xc6,0x28,0x28,0x48,0x24,0x23,0x22,0x39,0x2c,0x28,0x45,0x23,0x43,0x22,0x2a,0x2c,0x74,0x20, // code info
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x08, // LOAD_CONST_OBJ 8
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x0a, // LOAD_CONST_OBJ 10
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x52, // LOAD_CONST_TRUE
    0xb0, // LOAD_FAST 0
    0x18,0x2e, // STORE_ATTR 'running'
    0x42,0xfc,0x80, // JUMP 124
    0x48,0x18, // SETUP_EXCEPT 24
    0x12,0x81,0x19, // LOAD_GLOBAL 'input'
    0x10,0x33, // LOAD_CONST_STRING '{}:{}> '
    0x14,0x17, // LOAD_METHOD 'format'
    0xb0, // LOAD_FAST 0
    0x13,0x34, // LOAD_ATTR 'PROMPT'
    0xb0, // LOAD_FAST 0
    0x13,0x2d, // LOAD_ATTR 'current_path'
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x14,0x35, // LOAD_METHOD 'strip'
    0x36,0x00, // CALL_METHOD 0
    0xc1, // STORE_FAST 1
    0x4a,0x1a, // POP_EXCEPT_JUMP 26
    0x57, // DUP_TOP
    0x12,0x81,0x1a, // LOAD_GLOBAL 'KeyboardInterrupt'
    0x12,0x81,0x1b, // LOAD_GLOBAL 'EOFError'
    0x2a,0x02, // BUILD_TUPLE 2
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x40,0xd0,0x80,0x01, // UNWIND_JUMP 80
    0x5d, // END_FINALLY
    0xb1, // LOAD_FAST 1
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x42,0xc2,0x80, // JUMP 66
    0x48,0x09, // SETUP_EXCEPT 9
    0xb0, // LOAD_FAST 0
    0x14,0x36, // LOAD_METHOD '_dispatch'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0x37, // POP_EXCEPT_JUMP 55
    0x57, // DUP_TOP
    0x12,0x81,0x1a, // LOAD_GLOBAL 'KeyboardInterrupt'
    0x12,0x81,0x1b, // LOAD_GLOBAL 'EOFError'
    0x2a,0x02, // BUILD_TUPLE 2
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x0c, // LOAD_CONST_OBJ 12
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x20, // POP_EXCEPT_JUMP 32
    0x57, // DUP_TOP
    0x12,0x81,0x13, // LOAD_GLOBAL 'Exception'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x58, // POP_JUMP_IF_FALSE 24
    0xc2, // STORE_FAST 2
    0x49,0x0e, // SETUP_FINALLY 14
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x0d, // LOAD_CONST_OBJ 13
    0x14,0x17, // LOAD_METHOD 'format'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc2, // STORE_FAST 2
    0x28,0x02, // DELETE_FAST 2
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xb0, // LOAD_FAST 0
    0x13,0x2e, // LOAD_ATTR 'running'
    0x43,0xfe,0x7e, // POP_JUMP_IF_TRUE -130
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell_run = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell_run,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 188,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 42,
        .line_info = fun_data_recovery_RecoveryShell_run + 6,
        .line_info_top = fun_data_recovery_RecoveryShell_run + 25,
        .opcodes = fun_data_recovery_RecoveryShell_run + 25,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell_run fun_data_recovery_RecoveryShell_run[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__dispatch
static const byte fun_data_recovery_RecoveryShell__dispatch[204] = {
    0x52,0x42, // prelude
    0x36,0x81,0x18,0x81,0x1c, // names: _dispatch, self, raw
    0x80,0xdf,0x26,0x4c,0x22,0x26,0x26,0x26,0x26,0x26,0x26,0x26,0x26,0x26,0x26,0x26,0x26,0x26,0x26,0x26,0x26,0x26,0x26,0x67,0x27,0x25,0x2d,0x22, // code info
    0xb1, // LOAD_FAST 1
    0x14,0x09, // LOAD_METHOD 'split'
    0x36,0x00, // CALL_METHOD 0
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb2, // LOAD_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x5a, // ROT_TWO
    0xc3, // STORE_FAST 3
    0xc4, // STORE_FAST 4
    0x2c,0x13, // BUILD_MAP 19
    0xb0, // LOAD_FAST 0
    0x13,0x37, // LOAD_ATTR '_cmd_help'
    0x10,0x38, // LOAD_CONST_STRING 'help'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x39, // LOAD_ATTR '_cmd_ls'
    0x10,0x3a, // LOAD_CONST_STRING 'ls'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x3b, // LOAD_ATTR '_cmd_cd'
    0x10,0x3c, // LOAD_CONST_STRING 'cd'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x3d, // LOAD_ATTR '_cmd_pwd'
    0x10,0x3e, // LOAD_CONST_STRING 'pwd'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x3f, // LOAD_ATTR '_cmd_cat'
    0x10,0x40, // LOAD_CONST_STRING 'cat'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x41, // LOAD_ATTR '_cmd_mkdir'
    0x10,0x1b, // LOAD_CONST_STRING 'mkdir'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x42, // LOAD_ATTR '_cmd_rmdir'
    0x10,0x1d, // LOAD_CONST_STRING 'rmdir'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x43, // LOAD_ATTR '_cmd_rm'
    0x10,0x44, // LOAD_CONST_STRING 'rm'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x45, // LOAD_ATTR '_cmd_cp'
    0x10,0x46, // LOAD_CONST_STRING 'cp'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x47, // LOAD_ATTR '_cmd_mv'
    0x10,0x48, // LOAD_CONST_STRING 'mv'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x49, // LOAD_ATTR '_cmd_touch'
    0x10,0x4a, // LOAD_CONST_STRING 'touch'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x4b, // LOAD_ATTR '_cmd_stat'
    0x10,0x12, // LOAD_CONST_STRING 'stat'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x4c, // LOAD_ATTR '_cmd_df'
    0x10,0x4d, // LOAD_CONST_STRING 'df'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x4e, // LOAD_ATTR '_cmd_free'
    0x10,0x4f, // LOAD_CONST_STRING 'free'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x50, // LOAD_ATTR '_cmd_pkg'
    0x10,0x51, // LOAD_CONST_STRING 'pkg'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x52, // LOAD_ATTR '_cmd_recover'
    0x10,0x53, // LOAD_CONST_STRING 'recover'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x54, // LOAD_ATTR '_cmd_reboot'
    0x10,0x55, // LOAD_CONST_STRING 'reboot'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x56, // LOAD_ATTR '_cmd_exit'
    0x10,0x57, // LOAD_CONST_STRING 'exit'
    0x62, // STORE_MAP
    0xb0, // LOAD_FAST 0
    0x13,0x56, // LOAD_ATTR '_cmd_exit'
    0x10,0x58, // LOAD_CONST_STRING 'quit'
    0x62, // STORE_MAP
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x14,0x59, // LOAD_METHOD 'get'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0xc6, // STORE_FAST 6
    0xb6, // LOAD_FAST 6
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x0e, // LOAD_CONST_OBJ 14
    0x14,0x17, // LOAD_METHOD 'format'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb6, // LOAD_FAST 6
    0xb4, // LOAD_FAST 4
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__dispatch = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__dispatch,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 204,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 54,
        .line_info = fun_data_recovery_RecoveryShell__dispatch + 7,
        .line_info_top = fun_data_recovery_RecoveryShell__dispatch + 35,
        .opcodes = fun_data_recovery_RecoveryShell__dispatch + 35,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__dispatch fun_data_recovery_RecoveryShell__dispatch[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_help
static const byte fun_data_recovery_RecoveryShell__cmd_help[181] = {
    0x1a,0x32, // prelude
    0x37,0x81,0x18,0x81,0x1d, // names: _cmd_help, self, args
    0x90,0x02,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28, // code info
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x0f, // LOAD_CONST_OBJ 15
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x10, // LOAD_CONST_OBJ 16
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x11, // LOAD_CONST_OBJ 17
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x12, // LOAD_CONST_OBJ 18
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x13, // LOAD_CONST_OBJ 19
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x14, // LOAD_CONST_OBJ 20
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x15, // LOAD_CONST_OBJ 21
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x16, // LOAD_CONST_OBJ 22
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x17, // LOAD_CONST_OBJ 23
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x18, // LOAD_CONST_OBJ 24
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x19, // LOAD_CONST_OBJ 25
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x1a, // LOAD_CONST_OBJ 26
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x1b, // LOAD_CONST_OBJ 27
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x1c, // LOAD_CONST_OBJ 28
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x1d, // LOAD_CONST_OBJ 29
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x1e, // LOAD_CONST_OBJ 30
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x1f, // LOAD_CONST_OBJ 31
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x20, // LOAD_CONST_OBJ 32
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x21, // LOAD_CONST_OBJ 33
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_help = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_help,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 181,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 55,
        .line_info = fun_data_recovery_RecoveryShell__cmd_help + 7,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_help + 27,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_help + 27,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_help fun_data_recovery_RecoveryShell__cmd_help[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_ls
static const byte fun_data_recovery_RecoveryShell__cmd_ls[186] = {
    0xa2,0x10,0x2e, // prelude
    0x39,0x81,0x18,0x81,0x1d, // names: _cmd_ls, self, args
    0x90,0x17,0x25,0x26,0x51,0x26,0x25,0x42,0x2d,0x2a,0x2a,0x27,0x26,0x23,0x2a,0x2a,0x51,0x39, // code info
    0x10,0x5a, // LOAD_CONST_STRING '-l'
    0xb1, // LOAD_FAST 1
    0xdd, // BINARY_OP 6 <in>
    0xc2, // STORE_FAST 2
    0x32,0x00, // MAKE_FUNCTION 0
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0xb0, // LOAD_FAST 0
    0x14,0x30, // LOAD_METHOD '_abs'
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x42,0x43, // JUMP 3
    0xb0, // LOAD_FAST 0
    0x13,0x2d, // LOAD_ATTR 'current_path'
    0xc4, // STORE_FAST 4
    0x12,0x1f, // LOAD_GLOBAL '_listdir_safe'
    0xb4, // LOAD_FAST 4
    0x34,0x01, // CALL_FUNCTION 1
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x5b, // LOAD_CONST_STRING '\n{}'
    0x14,0x17, // LOAD_METHOD 'format'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x5c, // LOAD_CONST_STRING '-'
    0xa8, // LOAD_CONST_SMALL_INT 40
    0xf4, // BINARY_OP 29 __mul__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x1e, // LOAD_GLOBAL 'sorted'
    0xb5, // LOAD_FAST 5
    0x34,0x01, // CALL_FUNCTION 1
    0x5f, // GET_ITER_STACK
    0x4b,0x4f, // FOR_ITER 79
    0xc6, // STORE_FAST 6
    0x12,0x0f, // LOAD_GLOBAL '_join'
    0xb4, // LOAD_FAST 4
    0xb6, // LOAD_FAST 6
    0x34,0x02, // CALL_FUNCTION 2
    0xc7, // STORE_FAST 7
    0x12,0x13, // LOAD_GLOBAL '_is_dir'
    0xb7, // LOAD_FAST 7
    0x34,0x01, // CALL_FUNCTION 1
    0xc8, // STORE_FAST 8
    0xb2, // LOAD_FAST 2
    0x44,0x65, // POP_JUMP_IF_FALSE 37
    0xb8, // LOAD_FAST 8
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x10,0x5d, // LOAD_CONST_STRING '<DIR> '
    0x42,0x42, // JUMP 2
    0x10,0x5e, // LOAD_CONST_STRING '      '
    0xc9, // STORE_FAST 9
    0x12,0x15, // LOAD_GLOBAL '_human_size'
    0x12,0x14, // LOAD_GLOBAL '_size'
    0xb7, // LOAD_FAST 7
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0xca, // STORE_FAST 10
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x22, // LOAD_CONST_OBJ 34
    0x14,0x17, // LOAD_METHOD 'format'
    0xb9, // LOAD_FAST 9
    0xb6, // LOAD_FAST 6
    0xba, // LOAD_FAST 10
    0x36,0x03, // CALL_METHOD 3
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x56, // JUMP 22
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x5f, // LOAD_CONST_STRING '  {}{}'
    0x14,0x17, // LOAD_METHOD 'format'
    0xb6, // LOAD_FAST 6
    0xb8, // LOAD_FAST 8
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x42,0x42, // JUMP 2
    0x10,0x08, // LOAD_CONST_STRING ''
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0xaf,0x7f, // JUMP -81
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of recovery_RecoveryShell__cmd_ls
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_ls__lt_listcomp_gt_
static const byte fun_data_recovery_RecoveryShell__cmd_ls__lt_listcomp_gt_[29] = {
    0x49,0x0a, // prelude
    0x2b,0x81,0x17, // names: <listcomp>, *
    0x90,0x18, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x0f, // FOR_ITER 15
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x31, // LOAD_METHOD 'startswith'
    0x10,0x5c, // LOAD_CONST_STRING '-'
    0x36,0x01, // CALL_METHOD 1
    0x43,0x34, // POP_JUMP_IF_TRUE -12
    0xb1, // LOAD_FAST 1
    0x2f,0x14, // STORE_COMP 20
    0x42,0x2f, // JUMP -17
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_ls__lt_listcomp_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_ls__lt_listcomp_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 29,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 43,
        .line_info = fun_data_recovery_RecoveryShell__cmd_ls__lt_listcomp_gt_ + 5,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_ls__lt_listcomp_gt_ + 7,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_ls__lt_listcomp_gt_ + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_ls__lt_listcomp_gt_ fun_data_recovery_RecoveryShell__cmd_ls__lt_listcomp_gt_[0]
#endif

static const mp_raw_code_t *const children_recovery_RecoveryShell__cmd_ls[] = {
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_ls__lt_listcomp_gt_,
};

static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_ls = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_ls,
    .children = (void *)&children_recovery_RecoveryShell__cmd_ls,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 186,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 21,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 57,
        .line_info = fun_data_recovery_RecoveryShell__cmd_ls + 8,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_ls + 26,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_ls + 26,
    },
    #endif
    #endif
};

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_cd
static const byte fun_data_recovery_RecoveryShell__cmd_cd[88] = {
    0x32,0x22, // prelude
    0x3b,0x81,0x18,0x81,0x1d, // names: _cmd_cd, self, args
    0x90,0x2d,0x23,0x25,0x22,0x29,0x27,0x2d,0x22,0x27,0x2d,0x22, // code info
    0xb1, // LOAD_FAST 1
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xb0, // LOAD_FAST 0
    0x18,0x2d, // STORE_ATTR 'current_path'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x14,0x30, // LOAD_METHOD '_abs'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0x12,0x11, // LOAD_GLOBAL '_exists'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x4f, // POP_JUMP_IF_TRUE 15
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x23, // LOAD_CONST_OBJ 35
    0x14,0x17, // LOAD_METHOD 'format'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x13, // LOAD_GLOBAL '_is_dir'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x4f, // POP_JUMP_IF_TRUE 15
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x24, // LOAD_CONST_OBJ 36
    0x14,0x17, // LOAD_METHOD 'format'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x2d, // STORE_ATTR 'current_path'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_cd = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_cd,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 88,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 59,
        .line_info = fun_data_recovery_RecoveryShell__cmd_cd + 7,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_cd + 19,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_cd + 19,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_cd fun_data_recovery_RecoveryShell__cmd_cd[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_pwd
static const byte fun_data_recovery_RecoveryShell__cmd_pwd[20] = {
    0x1a,0x0e, // prelude
    0x3d,0x81,0x18,0x81,0x1d, // names: _cmd_pwd, self, args
    0x90,0x3a, // code info
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0xb0, // LOAD_FAST 0
    0x13,0x2d, // LOAD_ATTR 'current_path'
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_pwd = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_pwd,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 20,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 61,
        .line_info = fun_data_recovery_RecoveryShell__cmd_pwd + 7,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_pwd + 9,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_pwd + 9,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_pwd fun_data_recovery_RecoveryShell__cmd_pwd[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_cat
static const byte fun_data_recovery_RecoveryShell__cmd_cat[103] = {
    0xe2,0x02,0x1c, // prelude
    0x3f,0x81,0x18,0x81,0x1d, // names: _cmd_cat, self, args
    0x90,0x3d,0x23,0x28,0x22,0x29,0x22,0x2b,0x5a, // code info
    0xb1, // LOAD_FAST 1
    0x43,0x4a, // POP_JUMP_IF_TRUE 10
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x25, // LOAD_CONST_OBJ 37
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x14,0x30, // LOAD_METHOD '_abs'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0x48,0x1b, // SETUP_EXCEPT 27
    0x12,0x81,0x1f, // LOAD_GLOBAL 'open'
    0xb2, // LOAD_FAST 2
    0x10,0x60, // LOAD_CONST_STRING 'r'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x0d, // SETUP_WITH 13
    0xc3, // STORE_FAST 3
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0xb3, // LOAD_FAST 3
    0x14,0x61, // LOAD_METHOD 'read'
    0x36,0x00, // CALL_METHOD 0
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x4a,0x21, // POP_EXCEPT_JUMP 33
    0x57, // DUP_TOP
    0x12,0x81,0x0d, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0xc4, // STORE_FAST 4
    0x49,0x0f, // SETUP_FINALLY 15
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x26, // LOAD_CONST_OBJ 38
    0x14,0x17, // LOAD_METHOD 'format'
    0xb2, // LOAD_FAST 2
    0xb4, // LOAD_FAST 4
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc4, // STORE_FAST 4
    0x28,0x04, // DELETE_FAST 4
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_cat = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_cat,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 103,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 63,
        .line_info = fun_data_recovery_RecoveryShell__cmd_cat + 8,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_cat + 17,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_cat + 17,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_cat fun_data_recovery_RecoveryShell__cmd_cat[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_mkdir
static const byte fun_data_recovery_RecoveryShell__cmd_mkdir[59] = {
    0x52,0x1a, // prelude
    0x41,0x81,0x18,0x81,0x1d, // names: _cmd_mkdir, self, args
    0x90,0x48,0x23,0x28,0x22,0x25,0x27,0x26, // code info
    0xb1, // LOAD_FAST 1
    0x43,0x4a, // POP_JUMP_IF_TRUE 10
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x27, // LOAD_CONST_OBJ 39
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb1, // LOAD_FAST 1
    0x5f, // GET_ITER_STACK
    0x4b,0x19, // FOR_ITER 25
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x14,0x30, // LOAD_METHOD '_abs'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0x12,0x1a, // LOAD_GLOBAL '_mkdirs'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x62, // LOAD_CONST_STRING 'Created:'
    0xb3, // LOAD_FAST 3
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x42,0x25, // JUMP -27
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_mkdir = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_mkdir,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 59,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 65,
        .line_info = fun_data_recovery_RecoveryShell__cmd_mkdir + 7,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_mkdir + 15,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_mkdir + 15,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_mkdir fun_data_recovery_RecoveryShell__cmd_mkdir[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_rmdir
static const byte fun_data_recovery_RecoveryShell__cmd_rmdir[147] = {
    0xda,0x02,0x2a, // prelude
    0x42,0x81,0x18,0x81,0x1d, // names: _cmd_rmdir, self, args
    0x90,0x51,0x23,0x28,0x22,0x29,0x27,0x2d,0x22,0x26,0x23,0x2d,0x22,0x22,0x28,0x55, // code info
    0xb1, // LOAD_FAST 1
    0x43,0x4a, // POP_JUMP_IF_TRUE 10
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x28, // LOAD_CONST_OBJ 40
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x14,0x30, // LOAD_METHOD '_abs'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0x12,0x13, // LOAD_GLOBAL '_is_dir'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x4f, // POP_JUMP_IF_TRUE 15
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x29, // LOAD_CONST_OBJ 41
    0x14,0x17, // LOAD_METHOD 'format'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x1f, // LOAD_GLOBAL '_listdir_safe'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x2a, // LOAD_CONST_OBJ 42
    0x14,0x17, // LOAD_METHOD 'format'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x48,0x13, // SETUP_EXCEPT 19
    0x12,0x02, // LOAD_GLOBAL 'os'
    0x14,0x1d, // LOAD_METHOD 'rmdir'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x63, // LOAD_CONST_STRING 'Removed:'
    0xb2, // LOAD_FAST 2
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x4a,0x20, // POP_EXCEPT_JUMP 32
    0x57, // DUP_TOP
    0x12,0x81,0x0d, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x58, // POP_JUMP_IF_FALSE 24
    0xc4, // STORE_FAST 4
    0x49,0x0e, // SETUP_FINALLY 14
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x64, // LOAD_CONST_STRING '[rmdir] {}'
    0x14,0x17, // LOAD_METHOD 'format'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc4, // STORE_FAST 4
    0x28,0x04, // DELETE_FAST 4
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_rmdir = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_rmdir,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 147,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 66,
        .line_info = fun_data_recovery_RecoveryShell__cmd_rmdir + 8,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_rmdir + 24,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_rmdir + 24,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_rmdir fun_data_recovery_RecoveryShell__cmd_rmdir[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_rm
static const byte fun_data_recovery_RecoveryShell__cmd_rm[125] = {
    0x42,0x28, // prelude
    0x43,0x81,0x18,0x81,0x1d, // names: _cmd_rm, self, args
    0x90,0x63,0x2b,0x26,0x23,0x28,0x22,0x29,0x27,0x2d,0x22,0x2a,0x2d,0x22,0x26, // code info
    0x10,0x65, // LOAD_CONST_STRING '-r'
    0xb1, // LOAD_FAST 1
    0xdd, // BINARY_OP 6 <in>
    0x45,0x04, // JUMP_IF_TRUE_OR_POP 4
    0x10,0x66, // LOAD_CONST_STRING '-rf'
    0xb1, // LOAD_FAST 1
    0xdd, // BINARY_OP 6 <in>
    0xc2, // STORE_FAST 2
    0x32,0x00, // MAKE_FUNCTION 0
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x43,0x4a, // POP_JUMP_IF_TRUE 10
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x2b, // LOAD_CONST_OBJ 43
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x14,0x30, // LOAD_METHOD '_abs'
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xc4, // STORE_FAST 4
    0x12,0x11, // LOAD_GLOBAL '_exists'
    0xb4, // LOAD_FAST 4
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x4f, // POP_JUMP_IF_TRUE 15
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x2c, // LOAD_CONST_OBJ 44
    0x14,0x17, // LOAD_METHOD 'format'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x13, // LOAD_GLOBAL '_is_dir'
    0xb4, // LOAD_FAST 4
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0xb2, // LOAD_FAST 2
    0x43,0x4f, // POP_JUMP_IF_TRUE 15
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x2d, // LOAD_CONST_OBJ 45
    0x14,0x17, // LOAD_METHOD 'format'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x1c, // LOAD_GLOBAL '_rm_recursive'
    0xb4, // LOAD_FAST 4
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x63, // LOAD_CONST_STRING 'Removed:'
    0xb4, // LOAD_FAST 4
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of recovery_RecoveryShell__cmd_rm
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_rm__lt_listcomp_gt_
static const byte fun_data_recovery_RecoveryShell__cmd_rm__lt_listcomp_gt_[29] = {
    0x49,0x0a, // prelude
    0x2b,0x81,0x17, // names: <listcomp>, *
    0x90,0x64, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x0f, // FOR_ITER 15
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x31, // LOAD_METHOD 'startswith'
    0x10,0x5c, // LOAD_CONST_STRING '-'
    0x36,0x01, // CALL_METHOD 1
    0x43,0x34, // POP_JUMP_IF_TRUE -12
    0xb1, // LOAD_FAST 1
    0x2f,0x14, // STORE_COMP 20
    0x42,0x2f, // JUMP -17
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_rm__lt_listcomp_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_rm__lt_listcomp_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 29,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 43,
        .line_info = fun_data_recovery_RecoveryShell__cmd_rm__lt_listcomp_gt_ + 5,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_rm__lt_listcomp_gt_ + 7,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_rm__lt_listcomp_gt_ + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_rm__lt_listcomp_gt_ fun_data_recovery_RecoveryShell__cmd_rm__lt_listcomp_gt_[0]
#endif

static const mp_raw_code_t *const children_recovery_RecoveryShell__cmd_rm[] = {
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_rm__lt_listcomp_gt_,
};

static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_rm = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_rm,
    .children = (void *)&children_recovery_RecoveryShell__cmd_rm,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 125,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 67,
        .line_info = fun_data_recovery_RecoveryShell__cmd_rm + 7,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_rm + 22,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_rm + 22,
    },
    #endif
    #endif
};

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_cp
static const byte fun_data_recovery_RecoveryShell__cmd_cp[172] = {
    0xf2,0x02,0x28, // prelude
    0x45,0x81,0x18,0x81,0x1d, // names: _cmd_cp, self, args
    0x90,0x73,0x2a,0x28,0x22,0x33,0x27,0x28,0x22,0x22,0x2b,0x29,0x2b,0x2a,0x5a, // code info
    0x12,0x81,0x0f, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x2e, // LOAD_CONST_OBJ 46
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x14,0x30, // LOAD_METHOD '_abs'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xb0, // LOAD_FAST 0
    0x14,0x30, // LOAD_METHOD '_abs'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x5a, // ROT_TWO
    0xc2, // STORE_FAST 2
    0xc3, // STORE_FAST 3
    0x12,0x13, // LOAD_GLOBAL '_is_dir'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x2f, // LOAD_CONST_OBJ 47
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x48,0x39, // SETUP_EXCEPT 57
    0x12,0x81,0x1f, // LOAD_GLOBAL 'open'
    0xb2, // LOAD_FAST 2
    0x10,0x67, // LOAD_CONST_STRING 'rb'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x08, // SETUP_WITH 8
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x14,0x61, // LOAD_METHOD 'read'
    0x36,0x00, // CALL_METHOD 0
    0xc5, // STORE_FAST 5
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x12,0x81,0x1f, // LOAD_GLOBAL 'open'
    0xb3, // LOAD_FAST 3
    0x10,0x68, // LOAD_CONST_STRING 'wb'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x09, // SETUP_WITH 9
    0xc6, // STORE_FAST 6
    0xb6, // LOAD_FAST 6
    0x14,0x69, // LOAD_METHOD 'write'
    0xb5, // LOAD_FAST 5
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x30, // LOAD_CONST_OBJ 48
    0x14,0x17, // LOAD_METHOD 'format'
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x20, // POP_EXCEPT_JUMP 32
    0x57, // DUP_TOP
    0x12,0x81,0x0d, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x58, // POP_JUMP_IF_FALSE 24
    0xc7, // STORE_FAST 7
    0x49,0x0e, // SETUP_FINALLY 14
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x6a, // LOAD_CONST_STRING '[cp] {}'
    0x14,0x17, // LOAD_METHOD 'format'
    0xb7, // LOAD_FAST 7
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc7, // STORE_FAST 7
    0x28,0x07, // DELETE_FAST 7
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_cp = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_cp,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 172,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 15,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 69,
        .line_info = fun_data_recovery_RecoveryShell__cmd_cp + 8,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_cp + 23,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_cp + 23,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_cp fun_data_recovery_RecoveryShell__cmd_cp[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_mv
static const byte fun_data_recovery_RecoveryShell__cmd_mv[117] = {
    0xda,0x02,0x1c, // prelude
    0x47,0x81,0x18,0x81,0x1d, // names: _cmd_mv, self, args
    0x90,0x84,0x2a,0x28,0x22,0x33,0x22,0x29,0x5a, // code info
    0x12,0x81,0x0f, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x31, // LOAD_CONST_OBJ 49
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x14,0x30, // LOAD_METHOD '_abs'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xb0, // LOAD_FAST 0
    0x14,0x30, // LOAD_METHOD '_abs'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x5a, // ROT_TWO
    0xc2, // STORE_FAST 2
    0xc3, // STORE_FAST 3
    0x48,0x19, // SETUP_EXCEPT 25
    0x12,0x02, // LOAD_GLOBAL 'os'
    0x14,0x6b, // LOAD_METHOD 'rename'
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x32, // LOAD_CONST_OBJ 50
    0x14,0x17, // LOAD_METHOD 'format'
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x20, // POP_EXCEPT_JUMP 32
    0x57, // DUP_TOP
    0x12,0x81,0x0d, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x58, // POP_JUMP_IF_FALSE 24
    0xc4, // STORE_FAST 4
    0x49,0x0e, // SETUP_FINALLY 14
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x6c, // LOAD_CONST_STRING '[mv] {}'
    0x14,0x17, // LOAD_METHOD 'format'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc4, // STORE_FAST 4
    0x28,0x04, // DELETE_FAST 4
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_mv = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_mv,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 117,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 71,
        .line_info = fun_data_recovery_RecoveryShell__cmd_mv + 8,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_mv + 17,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_mv + 17,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_mv fun_data_recovery_RecoveryShell__cmd_mv[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_touch
static const byte fun_data_recovery_RecoveryShell__cmd_touch[127] = {
    0xd2,0x02,0x24, // prelude
    0x49,0x81,0x18,0x81,0x1d, // names: _cmd_touch, self, args
    0x90,0x8f,0x23,0x28,0x22,0x29,0x22,0x27,0x2b,0x45,0x2b,0x23,0x55, // code info
    0xb1, // LOAD_FAST 1
    0x43,0x4a, // POP_JUMP_IF_TRUE 10
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x33, // LOAD_CONST_OBJ 51
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x14,0x30, // LOAD_METHOD '_abs'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0x48,0x30, // SETUP_EXCEPT 48
    0x12,0x11, // LOAD_GLOBAL '_exists'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0x12,0x81,0x1f, // LOAD_GLOBAL 'open'
    0xb2, // LOAD_FAST 2
    0x10,0x6d, // LOAD_CONST_STRING 'a'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x02, // SETUP_WITH 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x42,0x4e, // JUMP 14
    0x12,0x81,0x1f, // LOAD_GLOBAL 'open'
    0xb2, // LOAD_FAST 2
    0x10,0x6e, // LOAD_CONST_STRING 'w'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x02, // SETUP_WITH 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x6f, // LOAD_CONST_STRING 'Touched:'
    0xb2, // LOAD_FAST 2
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x4a,0x20, // POP_EXCEPT_JUMP 32
    0x57, // DUP_TOP
    0x12,0x81,0x0d, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x58, // POP_JUMP_IF_FALSE 24
    0xc3, // STORE_FAST 3
    0x49,0x0e, // SETUP_FINALLY 14
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x70, // LOAD_CONST_STRING '[touch] {}'
    0x14,0x17, // LOAD_METHOD 'format'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc3, // STORE_FAST 3
    0x28,0x03, // DELETE_FAST 3
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_touch = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_touch,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 127,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 73,
        .line_info = fun_data_recovery_RecoveryShell__cmd_touch + 8,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_touch + 21,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_touch + 21,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_touch fun_data_recovery_RecoveryShell__cmd_touch[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_stat
static const byte fun_data_recovery_RecoveryShell__cmd_stat[130] = {
    0x4a,0x24, // prelude
    0x4b,0x81,0x18,0x81,0x1d, // names: _cmd_stat, self, args
    0x90,0x9f,0x23,0x28,0x22,0x29,0x27,0x2d,0x22,0x26,0x2d,0x35,0x23, // code info
    0xb1, // LOAD_FAST 1
    0x43,0x4a, // POP_JUMP_IF_TRUE 10
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x34, // LOAD_CONST_OBJ 52
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x14,0x30, // LOAD_METHOD '_abs'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0x12,0x11, // LOAD_GLOBAL '_exists'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x4f, // POP_JUMP_IF_TRUE 15
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x35, // LOAD_CONST_OBJ 53
    0x14,0x17, // LOAD_METHOD 'format'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x13, // LOAD_GLOBAL '_is_dir'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x71, // LOAD_CONST_STRING 'Path : {}'
    0x14,0x17, // LOAD_METHOD 'format'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x72, // LOAD_CONST_STRING 'Type : {}'
    0x14,0x17, // LOAD_METHOD 'format'
    0xb3, // LOAD_FAST 3
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x10,0x73, // LOAD_CONST_STRING 'directory'
    0x42,0x42, // JUMP 2
    0x10,0x74, // LOAD_CONST_STRING 'file'
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0x43,0x55, // POP_JUMP_IF_TRUE 21
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x75, // LOAD_CONST_STRING 'Size : {}'
    0x14,0x17, // LOAD_METHOD 'format'
    0x12,0x15, // LOAD_GLOBAL '_human_size'
    0x12,0x14, // LOAD_GLOBAL '_size'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_stat = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_stat,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 130,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 75,
        .line_info = fun_data_recovery_RecoveryShell__cmd_stat + 7,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_stat + 20,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_stat + 20,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_stat fun_data_recovery_RecoveryShell__cmd_stat[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_df
static const byte fun_data_recovery_RecoveryShell__cmd_df[167] = {
    0xfa,0x02,0x22, // prelude
    0x4c,0x81,0x18,0x81,0x1d, // names: _cmd_df, self, args
    0x90,0xad,0x30,0x22,0x28,0x28,0x28,0x24,0x2d,0x31,0x31,0x5d, // code info
    0xb1, // LOAD_FAST 1
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0xb0, // LOAD_FAST 0
    0x14,0x30, // LOAD_METHOD '_abs'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x42,0x42, // JUMP 2
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xc2, // STORE_FAST 2
    0x48,0x5e, // SETUP_EXCEPT 94
    0x12,0x02, // LOAD_GLOBAL 'os'
    0x14,0x76, // LOAD_METHOD 'statvfs'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xf4, // BINARY_OP 29 __mul__
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xf4, // BINARY_OP 29 __mul__
    0xc5, // STORE_FAST 5
    0xb4, // LOAD_FAST 4
    0xb5, // LOAD_FAST 5
    0xf3, // BINARY_OP 28 __sub__
    0xc6, // STORE_FAST 6
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x77, // LOAD_CONST_STRING 'Path  : {}'
    0x14,0x17, // LOAD_METHOD 'format'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x78, // LOAD_CONST_STRING 'Total : {}'
    0x14,0x17, // LOAD_METHOD 'format'
    0x12,0x15, // LOAD_GLOBAL '_human_size'
    0xb4, // LOAD_FAST 4
    0x34,0x01, // CALL_FUNCTION 1
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x79, // LOAD_CONST_STRING 'Used  : {}'
    0x14,0x17, // LOAD_METHOD 'format'
    0x12,0x15, // LOAD_GLOBAL '_human_size'
    0xb6, // LOAD_FAST 6
    0x34,0x01, // CALL_FUNCTION 1
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x10,0x7a, // LOAD_CONST_STRING 'Free  : {}'
    0x14,0x17, // LOAD_METHOD 'format'
    0x12,0x15, // LOAD_GLOBAL '_human_size'
    0xb5, // LOAD_FAST 5
    0x34,0x01, // CALL_FUNCTION 1
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x21, // POP_EXCEPT_JUMP 33
    0x57, // DUP_TOP
    0x12,0x81,0x0d, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0xc7, // STORE_FAST 7
    0x49,0x0f, // SETUP_FINALLY 15
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x36, // LOAD_CONST_OBJ 54
    0x14,0x17, // LOAD_METHOD 'format'
    0xb2, // LOAD_FAST 2
    0xb7, // LOAD_FAST 7
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc7, // STORE_FAST 7
    0x28,0x07, // DELETE_FAST 7
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_df = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_df,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 167,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 16,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 76,
        .line_info = fun_data_recovery_RecoveryShell__cmd_df + 8,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_df + 20,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_df + 20,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_df fun_data_recovery_RecoveryShell__cmd_df[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_free
static const byte fun_data_recovery_RecoveryShell__cmd_free[135] = {
    0xea,0x02,0x20, // prelude
    0x4e,0x81,0x18,0x81,0x1d, // names: _cmd_free, self, args
    0x90,0xbb,0x22,0x25,0x26,0x26,0x26,0x24,0x31,0x31,0x5d, // code info
    0x48,0x50, // SETUP_EXCEPT 80
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x7b, // IMPORT_NAME 'gc'
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x14,0x7c, // LOAD_METHOD 'collect'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x14,0x7d, // LOAD_METHOD 'mem_free'
    0x36,0x00, // CALL_METHOD 0
    0xc3, // STORE_FAST 3
    0xb2, // LOAD_FAST 2
    0x14,0x7e, // LOAD_METHOD 'mem_alloc'
    0x36,0x00, // CALL_METHOD 0
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0xf2, // BINARY_OP 27 __add__
    0xc5, // STORE_FAST 5
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x37, // LOAD_CONST_OBJ 55
    0x14,0x17, // LOAD_METHOD 'format'
    0x12,0x15, // LOAD_GLOBAL '_human_size'
    0xb5, // LOAD_FAST 5
    0x34,0x01, // CALL_FUNCTION 1
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x38, // LOAD_CONST_OBJ 56
    0x14,0x17, // LOAD_METHOD 'format'
    0x12,0x15, // LOAD_GLOBAL '_human_size'
    0xb4, // LOAD_FAST 4
    0x34,0x01, // CALL_FUNCTION 1
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x39, // LOAD_CONST_OBJ 57
    0x14,0x17, // LOAD_METHOD 'format'
    0x12,0x15, // LOAD_GLOBAL '_human_size'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x20, // POP_EXCEPT_JUMP 32
    0x57, // DUP_TOP
    0x12,0x81,0x13, // LOAD_GLOBAL 'Exception'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x58, // POP_JUMP_IF_FALSE 24
    0xc6, // STORE_FAST 6
    0x49,0x0e, // SETUP_FINALLY 14
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x3a, // LOAD_CONST_OBJ 58
    0x14,0x17, // LOAD_METHOD 'format'
    0xb6, // LOAD_FAST 6
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc6, // STORE_FAST 6
    0x28,0x06, // DELETE_FAST 6
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_free = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_free,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 135,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 78,
        .line_info = fun_data_recovery_RecoveryShell__cmd_free + 8,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_free + 19,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_free + 19,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_free fun_data_recovery_RecoveryShell__cmd_free[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_pkg
static const byte fun_data_recovery_RecoveryShell__cmd_pkg[398] = {
    0xea,0x02,0x54, // prelude
    0x50,0x81,0x18,0x81,0x1d, // names: _cmd_pkg, self, args
    0x90,0xc8,0x23,0x28,0x42,0x4c,0x27,0x47,0x29,0x23,0x29,0x2e,0x2a,0x2f,0x27,0x35,0x27,0x2b,0x2a,0x2e,0x27,0x4b,0x54,0x37,0x62,0x60,0x2b,0x25,0x25,0x27,0x28,0x22,0x27,0x28,0x22,0x58,0x5b, // code info
    0xb1, // LOAD_FAST 1
    0x43,0x4a, // POP_JUMP_IF_TRUE 10
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x3b, // LOAD_CONST_OBJ 59
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x5a, // ROT_TWO
    0xc2, // STORE_FAST 2
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x2f, // LOAD_ATTR '_pm'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x27, // LOAD_GLOBAL '_try_get_package_manager'
    0x34,0x00, // CALL_FUNCTION 0
    0xb0, // LOAD_FAST 0
    0x18,0x2f, // STORE_ATTR '_pm'
    0xb0, // LOAD_FAST 0
    0x13,0x2f, // LOAD_ATTR '_pm'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0xb8,0x81, // POP_JUMP_IF_FALSE 184
    0x48,0x92,0x01, // SETUP_EXCEPT 146
    0xb2, // LOAD_FAST 2
    0x10,0x7f, // LOAD_CONST_STRING 'install'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x51, // POP_JUMP_IF_FALSE 17
    0xb3, // LOAD_FAST 3
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0xb0, // LOAD_FAST 0
    0x13,0x2f, // LOAD_ATTR '_pm'
    0x14,0x7f, // LOAD_METHOD 'install'
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0xf9,0x80, // JUMP 121
    0xb2, // LOAD_FAST 2
    0x10,0x81,0x00, // LOAD_CONST_STRING 'uninstall'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0xb3, // LOAD_FAST 3
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0xb0, // LOAD_FAST 0
    0x13,0x2f, // LOAD_ATTR '_pm'
    0x14,0x81,0x00, // LOAD_METHOD 'uninstall'
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0xe0,0x80, // JUMP 96
    0xb2, // LOAD_FAST 2
    0x10,0x81,0x01, // LOAD_CONST_STRING 'update'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x55, // POP_JUMP_IF_FALSE 21
    0xb0, // LOAD_FAST 0
    0x13,0x2f, // LOAD_ATTR '_pm'
    0x14,0x81,0x01, // LOAD_METHOD 'update'
    0xb3, // LOAD_FAST 3
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x42,0x41, // JUMP 1
    0x51, // LOAD_CONST_NONE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0xc4,0x80, // JUMP 68
    0xb2, // LOAD_FAST 2
    0x10,0x81,0x02, // LOAD_CONST_STRING 'list'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0xb0, // LOAD_FAST 0
    0x13,0x2f, // LOAD_ATTR '_pm'
    0x14,0x81,0x02, // LOAD_METHOD 'list'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x42,0x72, // JUMP 50
    0xb2, // LOAD_FAST 2
    0x10,0x81,0x03, // LOAD_CONST_STRING 'info'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x51, // POP_JUMP_IF_FALSE 17
    0xb3, // LOAD_FAST 3
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0xb0, // LOAD_FAST 0
    0x13,0x2f, // LOAD_ATTR '_pm'
    0x14,0x81,0x03, // LOAD_METHOD 'info'
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x5a, // JUMP 26
    0xb2, // LOAD_FAST 2
    0x10,0x81,0x04, // LOAD_CONST_STRING 'verify'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0xb0, // LOAD_FAST 0
    0x13,0x2f, // LOAD_ATTR '_pm'
    0x14,0x81,0x04, // LOAD_METHOD 'verify'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x42,0x48, // JUMP 8
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x3b, // LOAD_CONST_OBJ 59
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x21, // POP_EXCEPT_JUMP 33
    0x57, // DUP_TOP
    0x12,0x81,0x13, // LOAD_GLOBAL 'Exception'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0xc4, // STORE_FAST 4
    0x49,0x0f, // SETUP_FINALLY 15
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x3c, // LOAD_CONST_OBJ 60
    0x14,0x17, // LOAD_METHOD 'format'
    0xb2, // LOAD_FAST 2
    0xb4, // LOAD_FAST 4
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc4, // STORE_FAST 4
    0x28,0x04, // DELETE_FAST 4
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb2, // LOAD_FAST 2
    0x23,0x3d, // LOAD_CONST_OBJ 61
    0xdd, // BINARY_OP 6 <in>
    0x44,0xe3,0x80, // POP_JUMP_IF_FALSE 99
    0xb3, // LOAD_FAST 3
    0x44,0xdf,0x80, // POP_JUMP_IF_FALSE 95
    0x12,0x21, // LOAD_GLOBAL '_try_import_recovery'
    0x34,0x00, // CALL_FUNCTION 0
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x51, // POP_JUMP_IF_FALSE 17
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x3e, // LOAD_CONST_OBJ 62
    0x14,0x17, // LOAD_METHOD 'format'
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x3f, // LOAD_CONST_OBJ 63
    0x14,0x17, // LOAD_METHOD 'format'
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x48,0x0e, // SETUP_EXCEPT 14
    0xb5, // LOAD_FAST 5
    0x34,0x00, // CALL_FUNCTION 0
    0x14,0x81,0x05, // LOAD_METHOD 'install_package'
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0x23, // POP_EXCEPT_JUMP 35
    0x57, // DUP_TOP
    0x12,0x81,0x13, // LOAD_GLOBAL 'Exception'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x5b, // POP_JUMP_IF_FALSE 27
    0xc4, // STORE_FAST 4
    0x49,0x11, // SETUP_FINALLY 17
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x40, // LOAD_CONST_OBJ 64
    0x14,0x17, // LOAD_METHOD 'format'
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb4, // LOAD_FAST 4
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc4, // STORE_FAST 4
    0x28,0x04, // DELETE_FAST 4
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x42,0x4d, // JUMP 13
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x41, // LOAD_CONST_OBJ 65
    0x14,0x17, // LOAD_METHOD 'format'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_pkg = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_pkg,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 398,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 80,
        .line_info = fun_data_recovery_RecoveryShell__cmd_pkg + 8,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_pkg + 45,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_pkg + 45,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_pkg fun_data_recovery_RecoveryShell__cmd_pkg[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_recover
static const byte fun_data_recovery_RecoveryShell__cmd_recover[104] = {
    0xda,0x02,0x1e, // prelude
    0x52,0x81,0x18,0x81,0x1d, // names: _cmd_recover, self, args
    0x90,0xfc,0x25,0x25,0x48,0x22,0x22,0x24,0x50,0x54, // code info
    0x12,0x21, // LOAD_GLOBAL '_try_import_recovery'
    0x34,0x00, // CALL_FUNCTION 0
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x42, // LOAD_CONST_OBJ 66
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x48,0x0c, // SETUP_EXCEPT 12
    0xb2, // LOAD_FAST 2
    0x34,0x00, // CALL_FUNCTION 0
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x14,0x2a, // LOAD_METHOD 'run'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x4a,0x32, // POP_EXCEPT_JUMP 50
    0x57, // DUP_TOP
    0x12,0x81,0x1a, // LOAD_GLOBAL 'KeyboardInterrupt'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0x59, // POP_TOP
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x43, // LOAD_CONST_OBJ 67
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x20, // POP_EXCEPT_JUMP 32
    0x57, // DUP_TOP
    0x12,0x81,0x13, // LOAD_GLOBAL 'Exception'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x58, // POP_JUMP_IF_FALSE 24
    0xc4, // STORE_FAST 4
    0x49,0x0e, // SETUP_FINALLY 14
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x44, // LOAD_CONST_OBJ 68
    0x14,0x17, // LOAD_METHOD 'format'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc4, // STORE_FAST 4
    0x28,0x04, // DELETE_FAST 4
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_recover = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_recover,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 104,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 82,
        .line_info = fun_data_recovery_RecoveryShell__cmd_recover + 8,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_recover + 18,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_recover + 18,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_recover fun_data_recovery_RecoveryShell__cmd_recover[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_reboot
static const byte fun_data_recovery_RecoveryShell__cmd_reboot[85] = {
    0xd2,0x02,0x18, // prelude
    0x54,0x81,0x18,0x81,0x1d, // names: _cmd_reboot, self, args
    0xa0,0x0a,0x22,0x26,0x28,0x2b,0x53, // code info
    0x48,0x22, // SETUP_EXCEPT 34
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x81,0x06, // IMPORT_NAME 'machine'
    0xc2, // STORE_FAST 2
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x45, // LOAD_CONST_OBJ 69
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x04, // LOAD_GLOBAL 'time'
    0x14,0x81,0x07, // LOAD_METHOD 'sleep_ms'
    0x22,0x81,0x48, // LOAD_CONST_SMALL_INT 200
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x14,0x81,0x08, // LOAD_METHOD 'reset'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x4a,0x20, // POP_EXCEPT_JUMP 32
    0x57, // DUP_TOP
    0x12,0x81,0x13, // LOAD_GLOBAL 'Exception'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x58, // POP_JUMP_IF_FALSE 24
    0xc3, // STORE_FAST 3
    0x49,0x0e, // SETUP_FINALLY 14
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x46, // LOAD_CONST_OBJ 70
    0x14,0x17, // LOAD_METHOD 'format'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc3, // STORE_FAST 3
    0x28,0x03, // DELETE_FAST 3
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_reboot = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_reboot,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 85,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 84,
        .line_info = fun_data_recovery_RecoveryShell__cmd_reboot + 8,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_reboot + 15,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_reboot + 15,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_reboot fun_data_recovery_RecoveryShell__cmd_reboot[0]
#endif

// child of recovery_RecoveryShell
// frozen bytecode for file recovery.py, scope recovery_RecoveryShell__cmd_exit
static const byte fun_data_recovery_RecoveryShell__cmd_exit[24] = {
    0x1a,0x10, // prelude
    0x56,0x81,0x18,0x81,0x1d, // names: _cmd_exit, self, args
    0xa0,0x13,0x24, // code info
    0x50, // LOAD_CONST_FALSE
    0xb0, // LOAD_FAST 0
    0x18,0x2e, // STORE_ATTR 'running'
    0x12,0x81,0x11, // LOAD_GLOBAL 'print'
    0x23,0x47, // LOAD_CONST_OBJ 71
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell__cmd_exit = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell__cmd_exit,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 24,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 86,
        .line_info = fun_data_recovery_RecoveryShell__cmd_exit + 7,
        .line_info_top = fun_data_recovery_RecoveryShell__cmd_exit + 10,
        .opcodes = fun_data_recovery_RecoveryShell__cmd_exit + 10,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_RecoveryShell__cmd_exit fun_data_recovery_RecoveryShell__cmd_exit[0]
#endif

static const mp_raw_code_t *const children_recovery_RecoveryShell[] = {
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell___init__,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__abs,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell_run,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__dispatch,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_help,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_ls,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_cd,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_pwd,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_cat,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_mkdir,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_rmdir,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_rm,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_cp,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_mv,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_touch,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_stat,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_df,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_free,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_pkg,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_recover,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_reboot,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell__cmd_exit,
};

static const mp_raw_code_truncated_t proto_fun_recovery_RecoveryShell = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_RecoveryShell,
    .children = (void *)&children_recovery_RecoveryShell,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 152,
    .n_children = 22,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 5,
        .line_info = fun_data_recovery_RecoveryShell + 3,
        .line_info_top = fun_data_recovery_RecoveryShell + 47,
        .opcodes = fun_data_recovery_RecoveryShell + 47,
    },
    #endif
    #endif
};

// child of recovery__lt_module_gt_
// frozen bytecode for file recovery.py, scope recovery_run
static const byte fun_data_recovery_run[16] = {
    0x08,0x06, // prelude
    0x2a, // names: run
    0xa0,0x18, // code info
    0x12,0x05, // LOAD_GLOBAL 'RecoveryShell'
    0x34,0x00, // CALL_FUNCTION 0
    0x14,0x2a, // LOAD_METHOD 'run'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_recovery_run = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery_run,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 16,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 42,
        .line_info = fun_data_recovery_run + 3,
        .line_info_top = fun_data_recovery_run + 5,
        .opcodes = fun_data_recovery_run + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_recovery_run fun_data_recovery_run[0]
#endif

static const mp_raw_code_t *const children_recovery__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_recovery__normalize_path,
    (const mp_raw_code_t *)&proto_fun_recovery__join,
    (const mp_raw_code_t *)&proto_fun_recovery__exists,
    (const mp_raw_code_t *)&proto_fun_recovery__is_dir,
    (const mp_raw_code_t *)&proto_fun_recovery__size,
    (const mp_raw_code_t *)&proto_fun_recovery__human_size,
    (const mp_raw_code_t *)&proto_fun_recovery__mkdirs,
    (const mp_raw_code_t *)&proto_fun_recovery__rm_recursive,
    (const mp_raw_code_t *)&proto_fun_recovery__listdir_safe,
    (const mp_raw_code_t *)&proto_fun_recovery__try_import_recovery,
    (const mp_raw_code_t *)&proto_fun_recovery__try_import_services,
    (const mp_raw_code_t *)&proto_fun_recovery__try_get_package_manager,
    (const mp_raw_code_t *)&proto_fun_recovery_RecoveryShell,
    (const mp_raw_code_t *)&proto_fun_recovery_run,
};

static const mp_raw_code_truncated_t proto_fun_recovery__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_recovery__lt_module_gt_,
    .children = (void *)&children_recovery__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 132,
    .n_children = 14,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_recovery__lt_module_gt_ + 3,
        .line_info_top = fun_data_recovery__lt_module_gt_ + 38,
        .opcodes = fun_data_recovery__lt_module_gt_ + 38,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_recovery[160] = {
    MP_QSTR_recovery_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_os,
    MP_QSTR_sys,
    MP_QSTR_time,
    MP_QSTR_RecoveryShell,
    MP_QSTR___main__,
    MP_QSTR__normalize_path,
    MP_QSTR_,
    MP_QSTR_split,
    MP_QSTR__slash_,
    MP_QSTR__dot__dot_,
    MP_QSTR_pop,
    MP_QSTR_append,
    MP_QSTR_join,
    MP_QSTR__join,
    MP_QSTR_rstrip,
    MP_QSTR__exists,
    MP_QSTR_stat,
    MP_QSTR__is_dir,
    MP_QSTR__size,
    MP_QSTR__human_size,
    MP_QSTR__brace_open__brace_close__space_B,
    MP_QSTR_format,
    MP_QSTR__brace_open__colon__dot_2f_brace_close__space_KB,
    MP_QSTR__brace_open__colon__dot_2f_brace_close__space_MB,
    MP_QSTR__mkdirs,
    MP_QSTR_mkdir,
    MP_QSTR__rm_recursive,
    MP_QSTR_rmdir,
    MP_QSTR_remove,
    MP_QSTR__listdir_safe,
    MP_QSTR_listdir,
    MP_QSTR__try_import_recovery,
    MP_QSTR_Recovery,
    MP_QSTR__try_import_services,
    MP_QSTR_modules,
    MP_QSTR_keys,
    MP_QSTR_Services,
    MP_QSTR__try_get_package_manager,
    MP_QSTR_PackageManager,
    MP_QSTR_recovery,
    MP_QSTR_run,
    MP_QSTR__lt_listcomp_gt_,
    MP_QSTR___init__,
    MP_QSTR_current_path,
    MP_QSTR_running,
    MP_QSTR__pm,
    MP_QSTR__abs,
    MP_QSTR_startswith,
    MP_QSTR_endswith,
    MP_QSTR__brace_open__brace_close__colon__brace_open__brace_close__gt__space_,
    MP_QSTR_PROMPT,
    MP_QSTR_strip,
    MP_QSTR__dispatch,
    MP_QSTR__cmd_help,
    MP_QSTR_help,
    MP_QSTR__cmd_ls,
    MP_QSTR_ls,
    MP_QSTR__cmd_cd,
    MP_QSTR_cd,
    MP_QSTR__cmd_pwd,
    MP_QSTR_pwd,
    MP_QSTR__cmd_cat,
    MP_QSTR_cat,
    MP_QSTR__cmd_mkdir,
    MP_QSTR__cmd_rmdir,
    MP_QSTR__cmd_rm,
    MP_QSTR_rm,
    MP_QSTR__cmd_cp,
    MP_QSTR_cp,
    MP_QSTR__cmd_mv,
    MP_QSTR_mv,
    MP_QSTR__cmd_touch,
    MP_QSTR_touch,
    MP_QSTR__cmd_stat,
    MP_QSTR__cmd_df,
    MP_QSTR_df,
    MP_QSTR__cmd_free,
    MP_QSTR_free,
    MP_QSTR__cmd_pkg,
    MP_QSTR_pkg,
    MP_QSTR__cmd_recover,
    MP_QSTR_recover,
    MP_QSTR__cmd_reboot,
    MP_QSTR_reboot,
    MP_QSTR__cmd_exit,
    MP_QSTR_exit,
    MP_QSTR_quit,
    MP_QSTR_get,
    MP_QSTR__hyphen_l,
    MP_QSTR__0x0a__brace_open__brace_close_,
    MP_QSTR__hyphen_,
    MP_QSTR__lt_DIR_gt__space_,
    MP_QSTR__space__space__space__space__space__space_,
    MP_QSTR__space__space__brace_open__brace_close__brace_open__brace_close_,
    MP_QSTR_r,
    MP_QSTR_read,
    MP_QSTR_Created_colon_,
    MP_QSTR_Removed_colon_,
    MP_QSTR__bracket_open_rmdir_bracket_close__space__brace_open__brace_close_,
    MP_QSTR__hyphen_r,
    MP_QSTR__hyphen_rf,
    MP_QSTR_rb,
    MP_QSTR_wb,
    MP_QSTR_write,
    MP_QSTR__bracket_open_cp_bracket_close__space__brace_open__brace_close_,
    MP_QSTR_rename,
    MP_QSTR__bracket_open_mv_bracket_close__space__brace_open__brace_close_,
    MP_QSTR_a,
    MP_QSTR_w,
    MP_QSTR_Touched_colon_,
    MP_QSTR__bracket_open_touch_bracket_close__space__brace_open__brace_close_,
    MP_QSTR_Path_space__colon__space__brace_open__brace_close_,
    MP_QSTR_Type_space__colon__space__brace_open__brace_close_,
    MP_QSTR_directory,
    MP_QSTR_file,
    MP_QSTR_Size_space__colon__space__brace_open__brace_close_,
    MP_QSTR_statvfs,
    MP_QSTR_Path_space__space__colon__space__brace_open__brace_close_,
    MP_QSTR_Total_space__colon__space__brace_open__brace_close_,
    MP_QSTR_Used_space__space__colon__space__brace_open__brace_close_,
    MP_QSTR_Free_space__space__colon__space__brace_open__brace_close_,
    MP_QSTR_gc,
    MP_QSTR_collect,
    MP_QSTR_mem_free,
    MP_QSTR_mem_alloc,
    MP_QSTR_install,
    MP_QSTR_uninstall,
    MP_QSTR_update,
    MP_QSTR_list,
    MP_QSTR_info,
    MP_QSTR_verify,
    MP_QSTR_install_package,
    MP_QSTR_machine,
    MP_QSTR_sleep_ms,
    MP_QSTR_reset,
    MP_QSTR___name__,
    MP_QSTR_path,
    MP_QSTR_parent,
    MP_QSTR_name,
    MP_QSTR_OSError,
    MP_QSTR_bool,
    MP_QSTR_len,
    MP_QSTR_n,
    MP_QSTR_print,
    MP_QSTR_ImportError,
    MP_QSTR_Exception,
    MP_QSTR_getattr,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR__star_,
    MP_QSTR_self,
    MP_QSTR_input,
    MP_QSTR_KeyboardInterrupt,
    MP_QSTR_EOFError,
    MP_QSTR_raw,
    MP_QSTR_args,
    MP_QSTR_sorted,
    MP_QSTR_open,
};

// constants
static const mp_rom_obj_tuple_t const_obj_recovery_0 = {{&mp_type_tuple}, 2, {
    MP_ROM_QSTR(MP_QSTR_),
    MP_ROM_QSTR(MP_QSTR__dot_),
}};
static const mp_obj_str_t const_obj_recovery_1 = {{&mp_type_str}, 4237, 40, (const byte*)"\x5b\x72\x6d\x5d\x20\x43\x6f\x75\x6c\x64\x20\x6e\x6f\x74\x20\x72\x65\x6d\x6f\x76\x65\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x79\x20\x27\x7b\x7d\x27\x3a\x20\x7b\x7d"};
static const mp_obj_str_t const_obj_recovery_2 = {{&mp_type_str}, 2660, 30, (const byte*)"\x5b\x72\x6d\x5d\x20\x43\x6f\x75\x6c\x64\x20\x6e\x6f\x74\x20\x72\x65\x6d\x6f\x76\x65\x20\x27\x7b\x7d\x27\x3a\x20\x7b\x7d"};
static const mp_obj_str_t const_obj_recovery_3 = {{&mp_type_str}, 9373, 25, (const byte*)"\x5b\x6c\x73\x5d\x20\x43\x61\x6e\x6e\x6f\x74\x20\x6c\x69\x73\x74\x20\x27\x7b\x7d\x27\x3a\x20\x7b\x7d"};
static const mp_obj_str_t const_obj_recovery_4 = {{&mp_type_str}, 61214, 41, (const byte*)"\x5b\x72\x65\x63\x6f\x76\x65\x72\x5d\x20\x52\x65\x63\x6f\x76\x65\x72\x79\x20\x6d\x6f\x64\x75\x6c\x65\x20\x75\x6e\x61\x76\x61\x69\x6c\x61\x62\x6c\x65\x3a\x20\x7b\x7d"};
static const mp_obj_str_t const_obj_recovery_5 = {{&mp_type_str}, 8358, 86, (const byte*)"\x5b\x70\x6b\x67\x5d\x20\x53\x65\x72\x76\x69\x63\x65\x73\x2e\x70\x79\x20\x75\x6e\x61\x76\x61\x69\x6c\x61\x62\x6c\x65\x20\x28\x7b\x7d\x29\x2e\x20\x27\x70\x6b\x67\x27\x20\x6e\x65\x65\x64\x73\x20\x53\x65\x72\x76\x69\x63\x65\x73\x20\x74\x6f\x20\x77\x6f\x72\x6b\x3b\x20\x74\x72\x79\x20\x27\x72\x65\x63\x6f\x76\x65\x72\x27\x20\x66\x69\x72\x73\x74\x2e"};
static const mp_obj_str_t const_obj_recovery_6 = {{&mp_type_str}, 52808, 71, (const byte*)"\x5b\x70\x6b\x67\x5d\x20\x53\x65\x72\x76\x69\x63\x65\x73\x2e\x70\x79\x20\x6c\x6f\x61\x64\x65\x64\x2c\x20\x62\x75\x74\x20\x68\x61\x73\x20\x6e\x6f\x20\x50\x61\x63\x6b\x61\x67\x65\x4d\x61\x6e\x61\x67\x65\x72\x20\x2d\x2d\x20\x63\x61\x6e\x27\x74\x20\x72\x75\x6e\x20\x27\x70\x6b\x67\x27\x2e"};
static const mp_obj_str_t const_obj_recovery_7 = {{&mp_type_str}, 13230, 40, (const byte*)"\x5b\x70\x6b\x67\x5d\x20\x43\x6f\x75\x6c\x64\x20\x6e\x6f\x74\x20\x73\x74\x61\x72\x74\x20\x50\x61\x63\x6b\x61\x67\x65\x4d\x61\x6e\x61\x67\x65\x72\x3a\x20\x7b\x7d"};
static const mp_obj_str_t const_obj_recovery_8 = {{&mp_type_str}, 65392, 31, (const byte*)"\x0a\x3d\x3d\x3d\x20\x5a\x65\x6e\x6f\x20\x4f\x53\x20\x52\x65\x63\x6f\x76\x65\x72\x79\x20\x53\x68\x65\x6c\x6c\x20\x3d\x3d\x3d"};
static const mp_obj_str_t const_obj_recovery_9 = {{&mp_type_str}, 60273, 63, (const byte*)"\x4d\x69\x6e\x69\x6d\x61\x6c\x20\x73\x68\x65\x6c\x6c\x20\x2d\x2d\x20\x6c\x73\x2f\x63\x64\x2f\x63\x61\x74\x2f\x70\x6b\x67\x2f\x72\x65\x63\x6f\x76\x65\x72\x20\x61\x6e\x64\x20\x62\x61\x73\x69\x63\x20\x66\x69\x6c\x65\x20\x6f\x70\x73\x20\x6f\x6e\x6c\x79\x2e"};
static const mp_obj_str_t const_obj_recovery_10 = {{&mp_type_str}, 53185, 50, (const byte*)"\x54\x79\x70\x65\x20\x27\x68\x65\x6c\x70\x27\x20\x66\x6f\x72\x20\x74\x68\x65\x20\x63\x6f\x6d\x6d\x61\x6e\x64\x20\x6c\x69\x73\x74\x2c\x20\x27\x65\x78\x69\x74\x27\x20\x74\x6f\x20\x71\x75\x69\x74\x2e\x0a"};
static const mp_obj_str_t const_obj_recovery_13 = {{&mp_type_str}, 34872, 29, (const byte*)"\x5b\x72\x65\x63\x6f\x76\x65\x72\x79\x5d\x20\x43\x6f\x6d\x6d\x61\x6e\x64\x20\x66\x61\x69\x6c\x65\x64\x3a\x20\x7b\x7d"};
static const mp_obj_str_t const_obj_recovery_14 = {{&mp_type_str}, 23647, 48, (const byte*)"\x55\x6e\x6b\x6e\x6f\x77\x6e\x20\x63\x6f\x6d\x6d\x61\x6e\x64\x3a\x20\x27\x7b\x7d\x27\x2e\x20\x54\x79\x70\x65\x20\x27\x68\x65\x6c\x70\x27\x20\x66\x6f\x72\x20\x74\x68\x65\x20\x6c\x69\x73\x74\x2e"};
static const mp_obj_str_t const_obj_recovery_15 = {{&mp_type_str}, 60110, 39, (const byte*)"\x0a\x52\x65\x63\x6f\x76\x65\x72\x79\x20\x73\x68\x65\x6c\x6c\x20\x2d\x2d\x20\x6d\x69\x6e\x69\x6d\x61\x6c\x20\x63\x6f\x6d\x6d\x61\x6e\x64\x20\x73\x65\x74\x3a"};
static const mp_obj_str_t const_obj_recovery_16 = {{&mp_type_str}, 36679, 47, (const byte*)"\x20\x20\x6c\x73\x20\x5b\x2d\x6c\x5d\x20\x5b\x70\x61\x74\x68\x5d\x20\x20\x20\x20\x20\x20\x20\x20\x4c\x69\x73\x74\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x79\x20\x63\x6f\x6e\x74\x65\x6e\x74\x73"};
static const mp_obj_str_t const_obj_recovery_17 = {{&mp_type_str}, 25149, 55, (const byte*)"\x20\x20\x63\x64\x20\x5b\x70\x61\x74\x68\x5d\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x43\x68\x61\x6e\x67\x65\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x79\x20\x28\x6e\x6f\x20\x61\x72\x67\x73\x20\x2d\x3e\x20\x2f\x29"};
static const mp_obj_str_t const_obj_recovery_18 = {{&mp_type_str}, 33015, 47, (const byte*)"\x20\x20\x70\x77\x64\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x50\x72\x69\x6e\x74\x20\x63\x75\x72\x72\x65\x6e\x74\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x79"};
static const mp_obj_str_t const_obj_recovery_19 = {{&mp_type_str}, 23450, 43, (const byte*)"\x20\x20\x63\x61\x74\x20\x3c\x66\x69\x6c\x65\x3e\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x50\x72\x69\x6e\x74\x20\x66\x69\x6c\x65\x20\x63\x6f\x6e\x74\x65\x6e\x74\x73"};
static const mp_obj_str_t const_obj_recovery_20 = {{&mp_type_str}, 48296, 56, (const byte*)"\x20\x20\x6d\x6b\x64\x69\x72\x20\x3c\x64\x69\x72\x3e\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x43\x72\x65\x61\x74\x65\x20\x61\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x79\x20\x28\x61\x6e\x64\x20\x70\x61\x72\x65\x6e\x74\x73\x29"};
static const mp_obj_str_t const_obj_recovery_21 = {{&mp_type_str}, 8589, 49, (const byte*)"\x20\x20\x72\x6d\x64\x69\x72\x20\x3c\x64\x69\x72\x3e\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x52\x65\x6d\x6f\x76\x65\x20\x61\x6e\x20\x65\x6d\x70\x74\x79\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x79"};
static const mp_obj_str_t const_obj_recovery_22 = {{&mp_type_str}, 57375, 37, (const byte*)"\x20\x20\x72\x6d\x20\x3c\x66\x69\x6c\x65\x3e\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x52\x65\x6d\x6f\x76\x65\x20\x61\x20\x66\x69\x6c\x65"};
static const mp_obj_str_t const_obj_recovery_23 = {{&mp_type_str}, 1657, 55, (const byte*)"\x20\x20\x72\x6d\x20\x2d\x72\x20\x3c\x70\x61\x74\x68\x3e\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x52\x65\x6d\x6f\x76\x65\x20\x61\x20\x66\x69\x6c\x65\x20\x6f\x72\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x79\x20\x74\x72\x65\x65"};
static const mp_obj_str_t const_obj_recovery_24 = {{&mp_type_str}, 40181, 35, (const byte*)"\x20\x20\x63\x70\x20\x3c\x73\x72\x63\x3e\x20\x3c\x64\x73\x74\x3e\x20\x20\x20\x20\x20\x20\x20\x20\x43\x6f\x70\x79\x20\x61\x20\x66\x69\x6c\x65"};
static const mp_obj_str_t const_obj_recovery_25 = {{&mp_type_str}, 14946, 55, (const byte*)"\x20\x20\x6d\x76\x20\x3c\x73\x72\x63\x3e\x20\x3c\x64\x73\x74\x3e\x20\x20\x20\x20\x20\x20\x20\x20\x52\x65\x6e\x61\x6d\x65\x2f\x6d\x6f\x76\x65\x20\x61\x20\x66\x69\x6c\x65\x20\x6f\x72\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x79"};
static const mp_obj_str_t const_obj_recovery_26 = {{&mp_type_str}, 37340, 44, (const byte*)"\x20\x20\x74\x6f\x75\x63\x68\x20\x3c\x66\x69\x6c\x65\x3e\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x43\x72\x65\x61\x74\x65\x20\x61\x6e\x20\x65\x6d\x70\x74\x79\x20\x66\x69\x6c\x65"};
static const mp_obj_str_t const_obj_recovery_27 = {{&mp_type_str}, 50142, 49, (const byte*)"\x20\x20\x73\x74\x61\x74\x20\x3c\x70\x61\x74\x68\x3e\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x53\x68\x6f\x77\x20\x73\x69\x7a\x65\x2f\x74\x79\x70\x65\x20\x66\x6f\x72\x20\x61\x20\x70\x61\x74\x68"};
static const mp_obj_str_t const_obj_recovery_28 = {{&mp_type_str}, 40052, 55, (const byte*)"\x20\x20\x64\x66\x20\x5b\x70\x61\x74\x68\x5d\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x53\x68\x6f\x77\x20\x66\x69\x6c\x65\x73\x79\x73\x74\x65\x6d\x20\x66\x72\x65\x65\x2f\x75\x73\x65\x64\x20\x73\x70\x61\x63\x65"};
static const mp_obj_str_t const_obj_recovery_29 = {{&mp_type_str}, 42471, 57, (const byte*)"\x20\x20\x66\x72\x65\x65\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x53\x68\x6f\x77\x20\x52\x41\x4d\x20\x66\x72\x65\x65\x2f\x75\x73\x65\x64\x20\x28\x69\x66\x20\x61\x76\x61\x69\x6c\x61\x62\x6c\x65\x29"};
static const mp_obj_str_t const_obj_recovery_30 = {{&mp_type_str}, 43754, 59, (const byte*)"\x20\x20\x70\x6b\x67\x20\x3c\x61\x72\x67\x73\x2e\x2e\x2e\x3e\x20\x20\x20\x20\x20\x20\x20\x20\x20\x50\x61\x63\x6b\x61\x67\x65\x20\x6d\x61\x6e\x61\x67\x65\x72\x20\x28\x6e\x65\x65\x64\x73\x20\x53\x65\x72\x76\x69\x63\x65\x73\x2e\x70\x79\x29"};
static const mp_obj_str_t const_obj_recovery_31 = {{&mp_type_str}, 47950, 64, (const byte*)"\x20\x20\x72\x65\x63\x6f\x76\x65\x72\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x52\x65\x62\x75\x69\x6c\x64\x20\x63\x6f\x72\x65\x20\x70\x61\x63\x6b\x61\x67\x65\x73\x20\x66\x72\x6f\x6d\x20\x70\x6b\x67\x74\x61\x62\x6c\x65\x2e\x6a\x73\x6f\x6e"};
static const mp_obj_str_t const_obj_recovery_32 = {{&mp_type_str}, 25328, 40, (const byte*)"\x20\x20\x72\x65\x62\x6f\x6f\x74\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x52\x65\x73\x65\x74\x20\x74\x68\x65\x20\x64\x65\x76\x69\x63\x65"};
static const mp_obj_str_t const_obj_recovery_33 = {{&mp_type_str}, 18374, 49, (const byte*)"\x20\x20\x65\x78\x69\x74\x20\x2f\x20\x71\x75\x69\x74\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x4c\x65\x61\x76\x65\x20\x74\x68\x65\x20\x72\x65\x63\x6f\x76\x65\x72\x79\x20\x73\x68\x65\x6c\x6c\x0a"};
static const mp_obj_str_t const_obj_recovery_38 = {{&mp_type_str}, 1433, 29, (const byte*)"\x5b\x63\x61\x74\x5d\x20\x43\x6f\x75\x6c\x64\x20\x6e\x6f\x74\x20\x72\x65\x61\x64\x20\x27\x7b\x7d\x27\x3a\x20\x7b\x7d"};
static const mp_obj_str_t const_obj_recovery_41 = {{&mp_type_str}, 29346, 27, (const byte*)"\x5b\x72\x6d\x64\x69\x72\x5d\x20\x4e\x6f\x74\x20\x61\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x79\x3a\x20\x7b\x7d"};
static const mp_obj_str_t const_obj_recovery_42 = {{&mp_type_str}, 13014, 31, (const byte*)"\x5b\x72\x6d\x64\x69\x72\x5d\x20\x44\x69\x72\x65\x63\x74\x6f\x72\x79\x20\x6e\x6f\x74\x20\x65\x6d\x70\x74\x79\x3a\x20\x7b\x7d"};
static const mp_obj_str_t const_obj_recovery_45 = {{&mp_type_str}, 38221, 48, (const byte*)"\x5b\x72\x6d\x5d\x20\x49\x73\x20\x61\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x79\x20\x28\x75\x73\x65\x20\x27\x72\x6d\x20\x2d\x72\x27\x20\x6f\x72\x20\x27\x72\x6d\x64\x69\x72\x27\x29\x3a\x20\x7b\x7d"};
static const mp_obj_str_t const_obj_recovery_47 = {{&mp_type_str}, 26067, 65, (const byte*)"\x5b\x63\x70\x5d\x20\x44\x69\x72\x65\x63\x74\x6f\x72\x79\x20\x63\x6f\x70\x79\x20\x6e\x6f\x74\x20\x73\x75\x70\x70\x6f\x72\x74\x65\x64\x20\x69\x6e\x20\x72\x65\x63\x6f\x76\x65\x72\x79\x20\x73\x68\x65\x6c\x6c\x20\x28\x66\x69\x6c\x65\x73\x20\x6f\x6e\x6c\x79\x29\x2e"};
static const mp_obj_str_t const_obj_recovery_54 = {{&mp_type_str}, 56429, 28, (const byte*)"\x5b\x64\x66\x5d\x20\x43\x6f\x75\x6c\x64\x20\x6e\x6f\x74\x20\x73\x74\x61\x74\x20\x27\x7b\x7d\x27\x3a\x20\x7b\x7d"};
static const mp_obj_str_t const_obj_recovery_58 = {{&mp_type_str}, 13702, 34, (const byte*)"\x5b\x66\x72\x65\x65\x5d\x20\x4d\x65\x6d\x6f\x72\x79\x20\x69\x6e\x66\x6f\x20\x75\x6e\x61\x76\x61\x69\x6c\x61\x62\x6c\x65\x3a\x20\x7b\x7d"};
static const mp_obj_str_t const_obj_recovery_59 = {{&mp_type_str}, 41000, 61, (const byte*)"\x55\x73\x61\x67\x65\x3a\x20\x70\x6b\x67\x20\x3c\x69\x6e\x73\x74\x61\x6c\x6c\x7c\x75\x6e\x69\x6e\x73\x74\x61\x6c\x6c\x7c\x75\x70\x64\x61\x74\x65\x7c\x6c\x69\x73\x74\x7c\x69\x6e\x66\x6f\x7c\x76\x65\x72\x69\x66\x79\x3e\x20\x5b\x6e\x61\x6d\x65\x5d"};
static const mp_rom_obj_tuple_t const_obj_recovery_61 = {{&mp_type_tuple}, 2, {
    MP_ROM_QSTR(MP_QSTR_install),
    MP_ROM_QSTR(MP_QSTR_update),
}};
static const mp_obj_str_t const_obj_recovery_62 = {{&mp_type_str}, 7513, 85, (const byte*)"\x5b\x70\x6b\x67\x5d\x20\x43\x61\x6e\x6e\x6f\x74\x20\x69\x6e\x73\x74\x61\x6c\x6c\x20\x27\x7b\x7d\x27\x20\x77\x69\x74\x68\x6f\x75\x74\x20\x53\x65\x72\x76\x69\x63\x65\x73\x2e\x70\x79\x20\x6f\x72\x20\x52\x65\x63\x6f\x76\x65\x72\x79\x2e\x70\x79\x20\x2d\x2d\x20\x6e\x65\x69\x74\x68\x65\x72\x20\x69\x73\x20\x61\x76\x61\x69\x6c\x61\x62\x6c\x65\x2e"};
static const mp_obj_str_t const_obj_recovery_63 = {{&mp_type_str}, 2343, 85, (const byte*)"\x5b\x70\x6b\x67\x5d\x20\x53\x65\x72\x76\x69\x63\x65\x73\x2e\x70\x79\x20\x75\x6e\x61\x76\x61\x69\x6c\x61\x62\x6c\x65\x20\x2d\x2d\x20\x69\x6e\x73\x74\x61\x6c\x6c\x69\x6e\x67\x20\x27\x7b\x7d\x27\x20\x64\x69\x72\x65\x63\x74\x6c\x79\x20\x66\x72\x6f\x6d\x20\x70\x6b\x67\x74\x61\x62\x6c\x65\x2e\x6a\x73\x6f\x6e\x20\x69\x6e\x73\x74\x65\x61\x64\x2e"};
static const mp_obj_str_t const_obj_recovery_64 = {{&mp_type_str}, 21283, 39, (const byte*)"\x5b\x70\x6b\x67\x5d\x20\x44\x69\x72\x65\x63\x74\x20\x69\x6e\x73\x74\x61\x6c\x6c\x20\x6f\x66\x20\x27\x7b\x7d\x27\x20\x66\x61\x69\x6c\x65\x64\x3a\x20\x7b\x7d"};
static const mp_obj_str_t const_obj_recovery_65 = {{&mp_type_str}, 34679, 177, (const byte*)"\x5b\x70\x6b\x67\x5d\x20\x27\x7b\x7d\x27\x20\x6e\x65\x65\x64\x73\x20\x53\x65\x72\x76\x69\x63\x65\x73\x2e\x70\x79\x2c\x20\x77\x68\x69\x63\x68\x20\x69\x73\x20\x75\x6e\x61\x76\x61\x69\x6c\x61\x62\x6c\x65\x2e\x20\x54\x72\x79\x20\x27\x70\x6b\x67\x20\x69\x6e\x73\x74\x61\x6c\x6c\x20\x3c\x6e\x61\x6d\x65\x3e\x27\x20\x28\x77\x6f\x72\x6b\x73\x20\x73\x74\x61\x6e\x64\x61\x6c\x6f\x6e\x65\x20\x76\x69\x61\x20\x52\x65\x63\x6f\x76\x65\x72\x79\x29\x2c\x20\x6f\x72\x20\x27\x72\x65\x63\x6f\x76\x65\x72\x27\x20\x74\x6f\x20\x72\x65\x73\x74\x6f\x72\x65\x20\x63\x6f\x72\x65\x20\x70\x61\x63\x6b\x61\x67\x65\x73\x20\x69\x6e\x63\x6c\x75\x64\x69\x6e\x67\x20\x53\x65\x72\x76\x69\x63\x65\x73\x2e\x70\x79\x20\x69\x74\x73\x65\x6c\x66\x2e"};
static const mp_obj_str_t const_obj_recovery_66 = {{&mp_type_str}, 20301, 113, (const byte*)"\x5b\x72\x65\x63\x6f\x76\x65\x72\x5d\x20\x4e\x6f\x74\x68\x69\x6e\x67\x20\x74\x6f\x20\x72\x75\x6e\x20\x2d\x2d\x20\x52\x65\x63\x6f\x76\x65\x72\x79\x2e\x70\x79\x20\x69\x73\x20\x6e\x6f\x74\x20\x69\x6d\x70\x6f\x72\x74\x61\x62\x6c\x65\x20\x28\x6d\x69\x73\x73\x69\x6e\x67\x20\x66\x72\x6f\x6d\x20\x66\x69\x6c\x65\x73\x79\x73\x74\x65\x6d\x20\x61\x6e\x64\x20\x6e\x6f\x74\x20\x66\x72\x6f\x7a\x65\x6e\x20\x69\x6e\x74\x6f\x20\x66\x69\x72\x6d\x77\x61\x72\x65\x29\x2e"};
static const mp_obj_str_t const_obj_recovery_68 = {{&mp_type_str}, 30309, 29, (const byte*)"\x5b\x72\x65\x63\x6f\x76\x65\x72\x5d\x20\x52\x65\x63\x6f\x76\x65\x72\x79\x20\x66\x61\x69\x6c\x65\x64\x3a\x20\x7b\x7d"};
static const mp_obj_str_t const_obj_recovery_70 = {{&mp_type_str}, 3567, 28, (const byte*)"\x5b\x72\x65\x62\x6f\x6f\x74\x5d\x20\x43\x6f\x75\x6c\x64\x20\x6e\x6f\x74\x20\x72\x65\x73\x65\x74\x3a\x20\x7b\x7d"};

// constant table
static const mp_rom_obj_t const_obj_table_data_recovery[72] = {
    MP_ROM_PTR(&const_obj_recovery_0),
    MP_ROM_PTR(&const_obj_recovery_1),
    MP_ROM_PTR(&const_obj_recovery_2),
    MP_ROM_PTR(&const_obj_recovery_3),
    MP_ROM_PTR(&const_obj_recovery_4),
    MP_ROM_PTR(&const_obj_recovery_5),
    MP_ROM_PTR(&const_obj_recovery_6),
    MP_ROM_PTR(&const_obj_recovery_7),
    MP_ROM_PTR(&const_obj_recovery_8),
    MP_ROM_PTR(&const_obj_recovery_9),
    MP_ROM_PTR(&const_obj_recovery_10),
    MP_ROM_QSTR(MP_QSTR__0x0a__bracket_open_recovery_bracket_close__space_Exiting_dot_),
    MP_ROM_QSTR(MP_QSTR__0x0a__bracket_open_recovery_bracket_close__space_Interrupted_dot_),
    MP_ROM_PTR(&const_obj_recovery_13),
    MP_ROM_PTR(&const_obj_recovery_14),
    MP_ROM_PTR(&const_obj_recovery_15),
    MP_ROM_PTR(&const_obj_recovery_16),
    MP_ROM_PTR(&const_obj_recovery_17),
    MP_ROM_PTR(&const_obj_recovery_18),
    MP_ROM_PTR(&const_obj_recovery_19),
    MP_ROM_PTR(&const_obj_recovery_20),
    MP_ROM_PTR(&const_obj_recovery_21),
    MP_ROM_PTR(&const_obj_recovery_22),
    MP_ROM_PTR(&const_obj_recovery_23),
    MP_ROM_PTR(&const_obj_recovery_24),
    MP_ROM_PTR(&const_obj_recovery_25),
    MP_ROM_PTR(&const_obj_recovery_26),
    MP_ROM_PTR(&const_obj_recovery_27),
    MP_ROM_PTR(&const_obj_recovery_28),
    MP_ROM_PTR(&const_obj_recovery_29),
    MP_ROM_PTR(&const_obj_recovery_30),
    MP_ROM_PTR(&const_obj_recovery_31),
    MP_ROM_PTR(&const_obj_recovery_32),
    MP_ROM_PTR(&const_obj_recovery_33),
    MP_ROM_QSTR(MP_QSTR__space__space__brace_open__brace_close__brace_open__colon_28_brace_close__space__brace_open__colon__gt_10_brace_close_),
    MP_ROM_QSTR(MP_QSTR__bracket_open_cd_bracket_close__space_No_space_such_space_path_colon__space__brace_open__brace_close_),
    MP_ROM_QSTR(MP_QSTR__bracket_open_cd_bracket_close__space_Not_space_a_space_directory_colon__space__brace_open__brace_close_),
    MP_ROM_QSTR(MP_QSTR_Usage_colon__space_cat_space__lt_file_gt_),
    MP_ROM_PTR(&const_obj_recovery_38),
    MP_ROM_QSTR(MP_QSTR_Usage_colon__space_mkdir_space__lt_dir_gt_),
    MP_ROM_QSTR(MP_QSTR_Usage_colon__space_rmdir_space__lt_dir_gt_),
    MP_ROM_PTR(&const_obj_recovery_41),
    MP_ROM_PTR(&const_obj_recovery_42),
    MP_ROM_QSTR(MP_QSTR_Usage_colon__space_rm_space__bracket_open__hyphen_r_bracket_close__space__lt_path_gt_),
    MP_ROM_QSTR(MP_QSTR__bracket_open_rm_bracket_close__space_No_space_such_space_path_colon__space__brace_open__brace_close_),
    MP_ROM_PTR(&const_obj_recovery_45),
    MP_ROM_QSTR(MP_QSTR_Usage_colon__space_cp_space__lt_src_gt__space__lt_dst_gt_),
    MP_ROM_PTR(&const_obj_recovery_47),
    MP_ROM_QSTR(MP_QSTR_Copied_space__brace_open__brace_close__space__hyphen__gt__space__brace_open__brace_close_),
    MP_ROM_QSTR(MP_QSTR_Usage_colon__space_mv_space__lt_src_gt__space__lt_dst_gt_),
    MP_ROM_QSTR(MP_QSTR_Moved_space__brace_open__brace_close__space__hyphen__gt__space__brace_open__brace_close_),
    MP_ROM_QSTR(MP_QSTR_Usage_colon__space_touch_space__lt_file_gt_),
    MP_ROM_QSTR(MP_QSTR_Usage_colon__space_stat_space__lt_path_gt_),
    MP_ROM_QSTR(MP_QSTR__bracket_open_stat_bracket_close__space_No_space_such_space_path_colon__space__brace_open__brace_close_),
    MP_ROM_PTR(&const_obj_recovery_54),
    MP_ROM_QSTR(MP_QSTR_RAM_space_total_space__colon__space__brace_open__brace_close_),
    MP_ROM_QSTR(MP_QSTR_RAM_space_used_space__space__colon__space__brace_open__brace_close_),
    MP_ROM_QSTR(MP_QSTR_RAM_space_free_space__space__colon__space__brace_open__brace_close_),
    MP_ROM_PTR(&const_obj_recovery_58),
    MP_ROM_PTR(&const_obj_recovery_59),
    MP_ROM_QSTR(MP_QSTR__bracket_open_pkg_bracket_close__space__squot__brace_open__brace_close__squot__space_failed_colon__space__brace_open__brace_close_),
    MP_ROM_PTR(&const_obj_recovery_61),
    MP_ROM_PTR(&const_obj_recovery_62),
    MP_ROM_PTR(&const_obj_recovery_63),
    MP_ROM_PTR(&const_obj_recovery_64),
    MP_ROM_PTR(&const_obj_recovery_65),
    MP_ROM_PTR(&const_obj_recovery_66),
    MP_ROM_QSTR(MP_QSTR__0x0a__bracket_open_recover_bracket_close__space_Interrupted_dot_),
    MP_ROM_PTR(&const_obj_recovery_68),
    MP_ROM_QSTR(MP_QSTR__bracket_open_recovery_bracket_close__space_Rebooting_dot__dot__dot_),
    MP_ROM_PTR(&const_obj_recovery_70),
    MP_ROM_QSTR(MP_QSTR__bracket_open_recovery_bracket_close__space_Bye_dot_),
};

static const mp_frozen_module_t frozen_module_recovery = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_recovery,
        .obj_table = (mp_obj_t *)&const_obj_table_data_recovery,
    },
    .proto_fun = &proto_fun_recovery__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module core
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/core.mpy
// - frozen file name: core.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file core.py, scope core__lt_module_gt_
static const byte fun_data_core__lt_module_gt_[43] = {
    0x18,0x0e, // prelude
    0x01, // names: <module>
    0x80,0x11,0x26,0x66,0x8b,0x09, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'sys'
    0x16,0x02, // STORE_NAME 'sys'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'os'
    0x16,0x03, // STORE_NAME 'os'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'NotADirectoryError'
    0x11,0x15, // LOAD_NAME 'OSError'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x04, // STORE_NAME 'NotADirectoryError'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x05, // LOAD_CONST_STRING 'SysPathManager'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x05, // STORE_NAME 'SysPathManager'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of core__lt_module_gt_
// frozen bytecode for file core.py, scope core_NotADirectoryError
static const byte fun_data_core_NotADirectoryError[17] = {
    0x00,0x0a, // prelude
    0x04, // names: NotADirectoryError
    0x88,0x16,0x60,0x40, // code info
    0x11,0x16, // LOAD_NAME '__name__'
    0x16,0x17, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'NotADirectoryError'
    0x16,0x18, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_core_NotADirectoryError = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_core_NotADirectoryError,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 17,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_core_NotADirectoryError + 3,
        .line_info_top = fun_data_core_NotADirectoryError + 7,
        .opcodes = fun_data_core_NotADirectoryError + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_core_NotADirectoryError fun_data_core_NotADirectoryError[0]
#endif

// child of core__lt_module_gt_
// frozen bytecode for file core.py, scope core_SysPathManager
static const byte fun_data_core_SysPathManager[25] = {
    0x08,0x0a, // prelude
    0x05, // names: SysPathManager
    0x88,0x1f,0x80,0x09, // code info
    0x11,0x16, // LOAD_NAME '__name__'
    0x16,0x17, // STORE_NAME '__module__'
    0x10,0x05, // LOAD_CONST_STRING 'SysPathManager'
    0x16,0x18, // STORE_NAME '__qualname__'
    0x11,0x19, // LOAD_NAME 'staticmethod'
    0x32,0x00, // MAKE_FUNCTION 0
    0x34,0x01, // CALL_FUNCTION 1
    0x16,0x06, // STORE_NAME 'import_from'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of core_SysPathManager
// frozen bytecode for file core.py, scope core_SysPathManager_import_from
static const byte fun_data_core_SysPathManager_import_from[225] = {
    0x83,0x12,0x35, // prelude
    0x06,0x1a,0x1b,0x1c, // names: import_from, base_path, folder_name, module_name
    0x80,0x2a,0x80,0x13,0x3a,0x29,0x65,0x20,0x22,0x51,0x2d,0x2e,0x23,0x6c,0x29,0x6a,0x60,0x56,0x4a,0x22,0x50,0x22,0x02, // code info
    0x10,0x07, // LOAD_CONST_STRING '/'
    0x14,0x08, // LOAD_METHOD 'join'
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x09, // LOAD_CONST_STRING '{}/{}'
    0x14,0x0a, // LOAD_METHOD 'format'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0x36,0x02, // CALL_METHOD 2
    0x14,0x0b, // LOAD_METHOD 'split'
    0x10,0x07, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x5e, // GET_ITER
    0x34,0x01, // CALL_FUNCTION 1
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x14,0x0c, // LOAD_METHOD 'startswith'
    0x10,0x07, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0x10,0x07, // LOAD_CONST_STRING '/'
    0xb3, // LOAD_FAST 3
    0xf2, // BINARY_OP 27 __add__
    0xc3, // STORE_FAST 3
    0x48,0x0a, // SETUP_EXCEPT 10
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x0d, // LOAD_METHOD 'stat'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0xc4, // STORE_FAST 4
    0x4a,0x14, // POP_EXCEPT_JUMP 20
    0x57, // DUP_TOP
    0x12,0x15, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x59, // POP_TOP
    0x12,0x04, // LOAD_GLOBAL 'NotADirectoryError'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x14,0x0a, // LOAD_METHOD 'format'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x5d, // END_FINALLY
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x22,0x83,0xe0,0x00, // LOAD_CONST_SMALL_INT 61440
    0xef, // BINARY_OP 24 __and__
    0x22,0x81,0x80,0x00, // LOAD_CONST_SMALL_INT 16384
    0xd9, // BINARY_OP 2 __eq__
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x43,0x4c, // POP_JUMP_IF_TRUE 12
    0x12,0x04, // LOAD_GLOBAL 'NotADirectoryError'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x14,0x0a, // LOAD_METHOD 'format'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb3, // LOAD_FAST 3
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x0e, // LOAD_ATTR 'path'
    0xdd, // BINARY_OP 6 <in>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x0e, // LOAD_ATTR 'path'
    0x14,0x0f, // LOAD_METHOD 'append'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x20,0x01,0x01, // MAKE_CLOSURE 1
    0x12,0x1d, // LOAD_GLOBAL 'list'
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x10, // LOAD_ATTR 'modules'
    0x14,0x11, // LOAD_METHOD 'keys'
    0x36,0x00, // CALL_METHOD 0
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x5f, // GET_ITER_STACK
    0x4b,0x0b, // FOR_ITER 11
    0xc6, // STORE_FAST 6
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x10, // LOAD_ATTR 'modules'
    0xb6, // LOAD_FAST 6
    0x53, // LOAD_NULL
    0x5b, // ROT_THREE
    0x56, // STORE_SUBSCR
    0x42,0x33, // JUMP -13
    0x48,0x07, // SETUP_EXCEPT 7
    0x12,0x1e, // LOAD_GLOBAL '__import__'
    0x25,0x02, // LOAD_DEREF 2
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
    0x57, // DUP_TOP
    0x12,0x1f, // LOAD_GLOBAL 'ImportError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0xc7, // STORE_FAST 7
    0x49,0x0f, // SETUP_FINALLY 15
    0x12,0x1f, // LOAD_GLOBAL 'ImportError'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x14,0x0a, // LOAD_METHOD 'format'
    0x25,0x02, // LOAD_DEREF 2
    0xb3, // LOAD_FAST 3
    0xb7, // LOAD_FAST 7
    0x36,0x03, // CALL_METHOD 3
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0xc7, // STORE_FAST 7
    0x28,0x07, // DELETE_FAST 7
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of core_SysPathManager_import_from
// frozen bytecode for file core.py, scope core_SysPathManager_import_from__lt_genexpr_gt_
static const byte fun_data_core_SysPathManager_import_from__lt_genexpr_gt_[24] = {
    0xb1,0x40,0x08, // prelude
    0x12,0x20, // names: <genexpr>, *
    0x80,0x3d, // code info
    0x53, // LOAD_NULL
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0x53, // LOAD_NULL
    0x4b,0x09, // FOR_ITER 9
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x44,0x3a, // POP_JUMP_IF_FALSE -6
    0xb1, // LOAD_FAST 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x42,0x35, // JUMP -11
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_core_SysPathManager_import_from__lt_genexpr_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_core_SysPathManager_import_from__lt_genexpr_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 24,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 18,
        .line_info = fun_data_core_SysPathManager_import_from__lt_genexpr_gt_ + 5,
        .line_info_top = fun_data_core_SysPathManager_import_from__lt_genexpr_gt_ + 7,
        .opcodes = fun_data_core_SysPathManager_import_from__lt_genexpr_gt_ + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_core_SysPathManager_import_from__lt_genexpr_gt_ fun_data_core_SysPathManager_import_from__lt_genexpr_gt_[0]
#endif

// child of core_SysPathManager_import_from
// frozen bytecode for file core.py, scope core_SysPathManager_import_from__lt_listcomp_gt_
static const byte fun_data_core_SysPathManager_import_from__lt_listcomp_gt_[39] = {
    0x5a,0x0c, // prelude
    0x13,0x20,0x20, // names: <listcomp>, *, *
    0x80,0x53,0x27, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb1, // LOAD_FAST 1
    0x5f, // GET_ITER_STACK
    0x4b,0x18, // FOR_ITER 24
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x25,0x00, // LOAD_DEREF 0
    0xd9, // BINARY_OP 2 __eq__
    0x43,0x4c, // POP_JUMP_IF_TRUE 12
    0xb2, // LOAD_FAST 2
    0x14,0x0c, // LOAD_METHOD 'startswith'
    0x25,0x00, // LOAD_DEREF 0
    0x10,0x14, // LOAD_CONST_STRING '.'
    0xf2, // BINARY_OP 27 __add__
    0x36,0x01, // CALL_METHOD 1
    0x44,0x2b, // POP_JUMP_IF_FALSE -21
    0xb2, // LOAD_FAST 2
    0x2f,0x14, // STORE_COMP 20
    0x42,0x26, // JUMP -26
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_core_SysPathManager_import_from__lt_listcomp_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_core_SysPathManager_import_from__lt_listcomp_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 39,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 19,
        .line_info = fun_data_core_SysPathManager_import_from__lt_listcomp_gt_ + 5,
        .line_info_top = fun_data_core_SysPathManager_import_from__lt_listcomp_gt_ + 8,
        .opcodes = fun_data_core_SysPathManager_import_from__lt_listcomp_gt_ + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_core_SysPathManager_import_from__lt_listcomp_gt_ fun_data_core_SysPathManager_import_from__lt_listcomp_gt_[0]
#endif

static const mp_raw_code_t *const children_core_SysPathManager_import_from[] = {
    (const mp_raw_code_t *)&proto_fun_core_SysPathManager_import_from__lt_genexpr_gt_,
    (const mp_raw_code_t *)&proto_fun_core_SysPathManager_import_from__lt_listcomp_gt_,
};

static const mp_raw_code_truncated_t proto_fun_core_SysPathManager_import_from = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_core_SysPathManager_import_from,
    .children = (void *)&children_core_SysPathManager_import_from,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 225,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 17,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_core_SysPathManager_import_from + 7,
        .line_info_top = fun_data_core_SysPathManager_import_from + 29,
        .opcodes = fun_data_core_SysPathManager_import_from + 30,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_core_SysPathManager[] = {
    (const mp_raw_code_t *)&proto_fun_core_SysPathManager_import_from,
};

static const mp_raw_code_truncated_t proto_fun_core_SysPathManager = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_core_SysPathManager,
    .children = (void *)&children_core_SysPathManager,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 25,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 5,
        .line_info = fun_data_core_SysPathManager + 3,
        .line_info_top = fun_data_core_SysPathManager + 7,
        .opcodes = fun_data_core_SysPathManager + 7,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_core__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_core_NotADirectoryError,
    (const mp_raw_code_t *)&proto_fun_core_SysPathManager,
};

static const mp_raw_code_truncated_t proto_fun_core__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_core__lt_module_gt_,
    .children = (void *)&children_core__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 43,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_core__lt_module_gt_ + 3,
        .line_info_top = fun_data_core__lt_module_gt_ + 9,
        .opcodes = fun_data_core__lt_module_gt_ + 9,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_core[33] = {
    MP_QSTR_core_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_sys,
    MP_QSTR_os,
    MP_QSTR_NotADirectoryError,
    MP_QSTR_SysPathManager,
    MP_QSTR_import_from,
    MP_QSTR__slash_,
    MP_QSTR_join,
    MP_QSTR__brace_open__brace_close__slash__brace_open__brace_close_,
    MP_QSTR_format,
    MP_QSTR_split,
    MP_QSTR_startswith,
    MP_QSTR_stat,
    MP_QSTR_path,
    MP_QSTR_append,
    MP_QSTR_modules,
    MP_QSTR_keys,
    MP_QSTR__lt_genexpr_gt_,
    MP_QSTR__lt_listcomp_gt_,
    MP_QSTR__dot_,
    MP_QSTR_OSError,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_staticmethod,
    MP_QSTR_base_path,
    MP_QSTR_folder_name,
    MP_QSTR_module_name,
    MP_QSTR_list,
    MP_QSTR___import__,
    MP_QSTR_ImportError,
    MP_QSTR__star_,
};

// constants
static const mp_obj_str_t const_obj_core_2 = {{&mp_type_str}, 15908, 35, (const byte*)"\x43\x6f\x75\x6c\x64\x20\x6e\x6f\x74\x20\x66\x69\x6e\x64\x20\x27\x7b\x7d\x27\x20\x69\x6e\x73\x69\x64\x65\x20\x27\x7b\x7d\x27\x3a\x20\x7b\x7d"};

// constant table
static const mp_rom_obj_t const_obj_table_data_core[3] = {
    MP_ROM_QSTR(MP_QSTR_Directory_space_not_space_found_colon__space__brace_open__brace_close_),
    MP_ROM_QSTR(MP_QSTR_Not_space_a_space_directory_colon__space__brace_open__brace_close_),
    MP_ROM_PTR(&const_obj_core_2),
};

static const mp_frozen_module_t frozen_module_core = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_core,
        .obj_table = (mp_obj_t *)&const_obj_table_data_core,
    },
    .proto_fun = &proto_fun_core__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module asyncio___init__
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/asyncio/__init__.mpy
// - frozen file name: asyncio/__init__.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file asyncio/__init__.py, scope asyncio___init____lt_module_gt_
static const byte fun_data_asyncio___init____lt_module_gt_[75] = {
    0x10,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x02, // LOAD_CONST_STRING '*'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'core'
    0x69, // IMPORT_STAR
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x16,0x0e, // STORE_NAME '__version__'
    0x2c,0x0a, // BUILD_MAP 10
    0x10,0x04, // LOAD_CONST_STRING 'funcs'
    0x10,0x05, // LOAD_CONST_STRING 'wait_for'
    0x62, // STORE_MAP
    0x10,0x04, // LOAD_CONST_STRING 'funcs'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x62, // STORE_MAP
    0x10,0x04, // LOAD_CONST_STRING 'funcs'
    0x10,0x06, // LOAD_CONST_STRING 'gather'
    0x62, // STORE_MAP
    0x10,0x07, // LOAD_CONST_STRING 'event'
    0x10,0x08, // LOAD_CONST_STRING 'Event'
    0x62, // STORE_MAP
    0x10,0x07, // LOAD_CONST_STRING 'event'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x62, // STORE_MAP
    0x10,0x09, // LOAD_CONST_STRING 'lock'
    0x10,0x0a, // LOAD_CONST_STRING 'Lock'
    0x62, // STORE_MAP
    0x10,0x0b, // LOAD_CONST_STRING 'stream'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x62, // STORE_MAP
    0x10,0x0b, // LOAD_CONST_STRING 'stream'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x62, // STORE_MAP
    0x10,0x0b, // LOAD_CONST_STRING 'stream'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x62, // STORE_MAP
    0x10,0x0b, // LOAD_CONST_STRING 'stream'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x62, // STORE_MAP
    0x16,0x0f, // STORE_NAME '_attrs'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x0c, // STORE_NAME '__getattr__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio___init____lt_module_gt_
// frozen bytecode for file asyncio/__init__.py, scope asyncio___init_____getattr__
static const byte fun_data_asyncio___init_____getattr__[51] = {
    0x49,0x04, // prelude
    0x0c,0x10, // names: __getattr__, attr
     // code info
    0x12,0x0f, // LOAD_GLOBAL '_attrs'
    0x14,0x0d, // LOAD_METHOD 'get'
    0xb0, // LOAD_FAST 0
    0x51, // LOAD_CONST_NONE
    0x36,0x02, // CALL_METHOD 2
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x12,0x11, // LOAD_GLOBAL 'AttributeError'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x12,0x12, // LOAD_GLOBAL 'getattr'
    0x12,0x13, // LOAD_GLOBAL '__import__'
    0xb1, // LOAD_FAST 1
    0x12,0x14, // LOAD_GLOBAL 'globals'
    0x34,0x00, // CALL_FUNCTION 0
    0x51, // LOAD_CONST_NONE
    0x52, // LOAD_CONST_TRUE
    0x81, // LOAD_CONST_SMALL_INT 1
    0x34,0x05, // CALL_FUNCTION 5
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x12,0x14, // LOAD_GLOBAL 'globals'
    0x34,0x00, // CALL_FUNCTION 0
    0xb0, // LOAD_FAST 0
    0x56, // STORE_SUBSCR
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio___init_____getattr__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio___init_____getattr__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 51,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_asyncio___init_____getattr__ + 4,
        .line_info_top = fun_data_asyncio___init_____getattr__ + 4,
        .opcodes = fun_data_asyncio___init_____getattr__ + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio___init_____getattr__ fun_data_asyncio___init_____getattr__[0]
#endif

static const mp_raw_code_t *const children_asyncio___init____lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_asyncio___init_____getattr__,
};

static const mp_raw_code_truncated_t proto_fun_asyncio___init____lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio___init____lt_module_gt_,
    .children = (void *)&children_asyncio___init____lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 75,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_asyncio___init____lt_module_gt_ + 3,
        .line_info_top = fun_data_asyncio___init____lt_module_gt_ + 3,
        .opcodes = fun_data_asyncio___init____lt_module_gt_ + 3,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_asyncio___init__[21] = {
    MP_QSTR_asyncio_slash___init___dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR__star_,
    MP_QSTR_core,
    MP_QSTR_funcs,
    MP_QSTR_wait_for,
    MP_QSTR_gather,
    MP_QSTR_event,
    MP_QSTR_Event,
    MP_QSTR_lock,
    MP_QSTR_Lock,
    MP_QSTR_stream,
    MP_QSTR___getattr__,
    MP_QSTR_get,
    MP_QSTR___version__,
    MP_QSTR__attrs,
    MP_QSTR_attr,
    MP_QSTR_AttributeError,
    MP_QSTR_getattr,
    MP_QSTR___import__,
    MP_QSTR_globals,
};

// constants
static const mp_rom_obj_tuple_t const_obj_asyncio___init___0 = {{&mp_type_tuple}, 3, {
    MP_ROM_INT(3),
    MP_ROM_INT(0),
    MP_ROM_INT(0),
}};

// constant table
static const mp_rom_obj_t const_obj_table_data_asyncio___init__[7] = {
    MP_ROM_PTR(&const_obj_asyncio___init___0),
    MP_ROM_QSTR(MP_QSTR_wait_for_ms),
    MP_ROM_QSTR(MP_QSTR_ThreadSafeFlag),
    MP_ROM_QSTR(MP_QSTR_open_connection),
    MP_ROM_QSTR(MP_QSTR_start_server),
    MP_ROM_QSTR(MP_QSTR_StreamReader),
    MP_ROM_QSTR(MP_QSTR_StreamWriter),
};

static const mp_frozen_module_t frozen_module_asyncio___init__ = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_asyncio___init__,
        .obj_table = (mp_obj_t *)&const_obj_table_data_asyncio___init__,
    },
    .proto_fun = &proto_fun_asyncio___init____lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module asyncio_core
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/asyncio/core.mpy
// - frozen file name: asyncio/core.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file asyncio/core.py, scope asyncio_core__lt_module_gt_
static const byte fun_data_asyncio_core__lt_module_gt_[218] = {
    0x2c,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING 'ticks_ms'
    0x10,0x03, // LOAD_CONST_STRING 'ticks_diff'
    0x10,0x04, // LOAD_CONST_STRING 'ticks_add'
    0x2a,0x03, // BUILD_TUPLE 3
    0x1b,0x05, // IMPORT_NAME 'time'
    0x1c,0x02, // IMPORT_FROM 'ticks_ms'
    0x16,0x49, // STORE_NAME 'ticks'
    0x1c,0x03, // IMPORT_FROM 'ticks_diff'
    0x16,0x03, // STORE_NAME 'ticks_diff'
    0x1c,0x04, // IMPORT_FROM 'ticks_add'
    0x16,0x04, // STORE_NAME 'ticks_add'
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x06, // IMPORT_NAME 'sys'
    0x16,0x06, // STORE_NAME 'sys'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x07, // IMPORT_NAME 'select'
    0x16,0x07, // STORE_NAME 'select'
    0x48,0x14, // SETUP_EXCEPT 20
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x08, // LOAD_CONST_STRING 'TaskQueue'
    0x10,0x09, // LOAD_CONST_STRING 'Task'
    0x2a,0x02, // BUILD_TUPLE 2
    0x1b,0x0a, // IMPORT_NAME '_asyncio'
    0x1c,0x08, // IMPORT_FROM 'TaskQueue'
    0x16,0x08, // STORE_NAME 'TaskQueue'
    0x1c,0x09, // IMPORT_FROM 'Task'
    0x16,0x09, // STORE_NAME 'Task'
    0x59, // POP_TOP
    0x4a,0x16, // POP_EXCEPT_JUMP 22
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x08, // LOAD_CONST_STRING 'TaskQueue'
    0x10,0x09, // LOAD_CONST_STRING 'Task'
    0x2a,0x02, // BUILD_TUPLE 2
    0x1b,0x0b, // IMPORT_NAME 'task'
    0x1c,0x08, // IMPORT_FROM 'TaskQueue'
    0x16,0x08, // STORE_NAME 'TaskQueue'
    0x1c,0x09, // IMPORT_FROM 'Task'
    0x16,0x09, // STORE_NAME 'Task'
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x0c, // LOAD_CONST_STRING 'CancelledError'
    0x11,0x4a, // LOAD_NAME 'BaseException'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x0c, // STORE_NAME 'CancelledError'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x0d, // LOAD_CONST_STRING 'TimeoutError'
    0x11,0x4b, // LOAD_NAME 'Exception'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x0d, // STORE_NAME 'TimeoutError'
    0x2c,0x03, // BUILD_MAP 3
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x10,0x0e, // LOAD_CONST_STRING 'message'
    0x62, // STORE_MAP
    0x51, // LOAD_CONST_NONE
    0x10,0x0f, // LOAD_CONST_STRING 'exception'
    0x62, // STORE_MAP
    0x51, // LOAD_CONST_NONE
    0x10,0x10, // LOAD_CONST_STRING 'future'
    0x62, // STORE_MAP
    0x16,0x4c, // STORE_NAME '_exc_context'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x11, // LOAD_CONST_STRING 'SingletonGenerator'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x11, // STORE_NAME 'SingletonGenerator'
    0x11,0x11, // LOAD_NAME 'SingletonGenerator'
    0x34,0x00, // CALL_FUNCTION 0
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x03, // MAKE_FUNCTION_DEFARGS 3
    0x16,0x14, // STORE_NAME 'sleep_ms'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x16, // STORE_NAME 'sleep'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x05, // MAKE_FUNCTION 5
    0x10,0x12, // LOAD_CONST_STRING 'IOQueue'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x12, // STORE_NAME 'IOQueue'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x17, // STORE_NAME '_promote_to_task'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x18, // STORE_NAME 'create_task'
    0x51, // LOAD_CONST_NONE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x08, // MAKE_FUNCTION_DEFARGS 8
    0x16,0x1b, // STORE_NAME 'run_until_complete'
    0x32,0x09, // MAKE_FUNCTION 9
    0x16,0x26, // STORE_NAME 'run'
    0x32,0x0a, // MAKE_FUNCTION 10
    0x16,0x27, // STORE_NAME '_stopper'
    0x51, // LOAD_CONST_NONE
    0x17,0x4d, // STORE_GLOBAL 'cur_task'
    0x51, // LOAD_CONST_NONE
    0x17,0x4e, // STORE_GLOBAL '_stop_task'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x0b, // MAKE_FUNCTION 11
    0x10,0x13, // LOAD_CONST_STRING 'Loop'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x13, // STORE_NAME 'Loop'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x0c, // MAKE_FUNCTION_DEFARGS 12
    0x16,0x28, // STORE_NAME 'get_event_loop'
    0x32,0x0d, // MAKE_FUNCTION 13
    0x16,0x29, // STORE_NAME 'current_task'
    0x32,0x0e, // MAKE_FUNCTION 14
    0x16,0x2a, // STORE_NAME 'new_event_loop'
    0x11,0x2a, // LOAD_NAME 'new_event_loop'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_CancelledError
static const byte fun_data_asyncio_core_CancelledError[13] = {
    0x00,0x02, // prelude
    0x0c, // names: CancelledError
     // code info
    0x11,0x4f, // LOAD_NAME '__name__'
    0x16,0x50, // STORE_NAME '__module__'
    0x10,0x0c, // LOAD_CONST_STRING 'CancelledError'
    0x16,0x51, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_CancelledError = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_CancelledError,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 13,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_asyncio_core_CancelledError + 3,
        .line_info_top = fun_data_asyncio_core_CancelledError + 3,
        .opcodes = fun_data_asyncio_core_CancelledError + 3,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_CancelledError fun_data_asyncio_core_CancelledError[0]
#endif

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_TimeoutError
static const byte fun_data_asyncio_core_TimeoutError[13] = {
    0x00,0x02, // prelude
    0x0d, // names: TimeoutError
     // code info
    0x11,0x4f, // LOAD_NAME '__name__'
    0x16,0x50, // STORE_NAME '__module__'
    0x10,0x0d, // LOAD_CONST_STRING 'TimeoutError'
    0x16,0x51, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_TimeoutError = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_TimeoutError,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 13,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_asyncio_core_TimeoutError + 3,
        .line_info_top = fun_data_asyncio_core_TimeoutError + 3,
        .opcodes = fun_data_asyncio_core_TimeoutError + 3,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_TimeoutError fun_data_asyncio_core_TimeoutError[0]
#endif

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_SingletonGenerator
static const byte fun_data_asyncio_core_SingletonGenerator[25] = {
    0x00,0x02, // prelude
    0x11, // names: SingletonGenerator
     // code info
    0x11,0x4f, // LOAD_NAME '__name__'
    0x16,0x50, // STORE_NAME '__module__'
    0x10,0x11, // LOAD_CONST_STRING 'SingletonGenerator'
    0x16,0x51, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x2b, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x2d, // STORE_NAME '__iter__'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x2e, // STORE_NAME '__next__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_core_SingletonGenerator
// frozen bytecode for file asyncio/core.py, scope asyncio_core_SingletonGenerator___init__
static const byte fun_data_asyncio_core_SingletonGenerator___init__[17] = {
    0x11,0x04, // prelude
    0x2b,0x63, // names: __init__, self
     // code info
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x15, // STORE_ATTR 'state'
    0x12,0x5d, // LOAD_GLOBAL 'StopIteration'
    0x34,0x00, // CALL_FUNCTION 0
    0xb0, // LOAD_FAST 0
    0x18,0x2c, // STORE_ATTR 'exc'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_SingletonGenerator___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_SingletonGenerator___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 17,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 43,
        .line_info = fun_data_asyncio_core_SingletonGenerator___init__ + 4,
        .line_info_top = fun_data_asyncio_core_SingletonGenerator___init__ + 4,
        .opcodes = fun_data_asyncio_core_SingletonGenerator___init__ + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_SingletonGenerator___init__ fun_data_asyncio_core_SingletonGenerator___init__[0]
#endif

// child of asyncio_core_SingletonGenerator
// frozen bytecode for file asyncio/core.py, scope asyncio_core_SingletonGenerator___iter__
static const byte fun_data_asyncio_core_SingletonGenerator___iter__[6] = {
    0x09,0x04, // prelude
    0x2d,0x63, // names: __iter__, self
     // code info
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_SingletonGenerator___iter__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_SingletonGenerator___iter__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 6,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 45,
        .line_info = fun_data_asyncio_core_SingletonGenerator___iter__ + 4,
        .line_info_top = fun_data_asyncio_core_SingletonGenerator___iter__ + 4,
        .opcodes = fun_data_asyncio_core_SingletonGenerator___iter__ + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_SingletonGenerator___iter__ fun_data_asyncio_core_SingletonGenerator___iter__[0]
#endif

// child of asyncio_core_SingletonGenerator
// frozen bytecode for file asyncio/core.py, scope asyncio_core_SingletonGenerator___next__
static const byte fun_data_asyncio_core_SingletonGenerator___next__[42] = {
    0x21,0x04, // prelude
    0x2e,0x63, // names: __next__, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0x12,0x5b, // LOAD_GLOBAL '_task_queue'
    0x14,0x1a, // LOAD_METHOD 'push'
    0x12,0x4d, // LOAD_GLOBAL 'cur_task'
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'state'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x15, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x13,0x2c, // LOAD_ATTR 'exc'
    0x18,0x2f, // STORE_ATTR '__traceback__'
    0xb0, // LOAD_FAST 0
    0x13,0x2c, // LOAD_ATTR 'exc'
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_SingletonGenerator___next__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_SingletonGenerator___next__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 42,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 46,
        .line_info = fun_data_asyncio_core_SingletonGenerator___next__ + 4,
        .line_info_top = fun_data_asyncio_core_SingletonGenerator___next__ + 4,
        .opcodes = fun_data_asyncio_core_SingletonGenerator___next__ + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_SingletonGenerator___next__ fun_data_asyncio_core_SingletonGenerator___next__[0]
#endif

static const mp_raw_code_t *const children_asyncio_core_SingletonGenerator[] = {
    (const mp_raw_code_t *)&proto_fun_asyncio_core_SingletonGenerator___init__,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_SingletonGenerator___iter__,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_SingletonGenerator___next__,
};

static const mp_raw_code_truncated_t proto_fun_asyncio_core_SingletonGenerator = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_SingletonGenerator,
    .children = (void *)&children_asyncio_core_SingletonGenerator,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 25,
    .n_children = 3,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 17,
        .line_info = fun_data_asyncio_core_SingletonGenerator + 3,
        .line_info_top = fun_data_asyncio_core_SingletonGenerator + 3,
        .opcodes = fun_data_asyncio_core_SingletonGenerator + 3,
    },
    #endif
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_sleep_ms
static const byte fun_data_asyncio_core_sleep_ms[25] = {
    0xb2,0x01,0x06, // prelude
    0x14,0x52,0x53, // names: sleep_ms, t, sgen
     // code info
    0x12,0x04, // LOAD_GLOBAL 'ticks_add'
    0x12,0x49, // LOAD_GLOBAL 'ticks'
    0x34,0x00, // CALL_FUNCTION 0
    0x12,0x54, // LOAD_GLOBAL 'max'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x02, // CALL_FUNCTION 2
    0xb1, // LOAD_FAST 1
    0x18,0x15, // STORE_ATTR 'state'
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_sleep_ms = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_sleep_ms,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 25,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 20,
        .line_info = fun_data_asyncio_core_sleep_ms + 6,
        .line_info_top = fun_data_asyncio_core_sleep_ms + 6,
        .opcodes = fun_data_asyncio_core_sleep_ms + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_sleep_ms fun_data_asyncio_core_sleep_ms[0]
#endif

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_sleep
static const byte fun_data_asyncio_core_sleep[18] = {
    0x21,0x04, // prelude
    0x16,0x52, // names: sleep, t
     // code info
    0x12,0x14, // LOAD_GLOBAL 'sleep_ms'
    0x12,0x55, // LOAD_GLOBAL 'int'
    0xb0, // LOAD_FAST 0
    0x22,0x87,0x68, // LOAD_CONST_SMALL_INT 1000
    0xf4, // BINARY_OP 29 __mul__
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_sleep = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_sleep,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 18,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 22,
        .line_info = fun_data_asyncio_core_sleep + 4,
        .line_info_top = fun_data_asyncio_core_sleep + 4,
        .opcodes = fun_data_asyncio_core_sleep + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_sleep fun_data_asyncio_core_sleep[0]
#endif

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_IOQueue
static const byte fun_data_asyncio_core_IOQueue[41] = {
    0x00,0x02, // prelude
    0x12, // names: IOQueue
     // code info
    0x11,0x4f, // LOAD_NAME '__name__'
    0x16,0x50, // STORE_NAME '__module__'
    0x10,0x12, // LOAD_CONST_STRING 'IOQueue'
    0x16,0x51, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x2b, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x32, // STORE_NAME '_enqueue'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x37, // STORE_NAME '_dequeue'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x39, // STORE_NAME 'queue_read'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x3a, // STORE_NAME 'queue_write'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x3b, // STORE_NAME 'remove'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x1f, // STORE_NAME 'wait_io_event'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_core_IOQueue
// frozen bytecode for file asyncio/core.py, scope asyncio_core_IOQueue___init__
static const byte fun_data_asyncio_core_IOQueue___init__[20] = {
    0x11,0x04, // prelude
    0x2b,0x63, // names: __init__, self
     // code info
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x14,0x30, // LOAD_METHOD 'poll'
    0x36,0x00, // CALL_METHOD 0
    0xb0, // LOAD_FAST 0
    0x18,0x31, // STORE_ATTR 'poller'
    0x2c,0x00, // BUILD_MAP 0
    0xb0, // LOAD_FAST 0
    0x18,0x1e, // STORE_ATTR 'map'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_IOQueue___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_IOQueue___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 20,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 43,
        .line_info = fun_data_asyncio_core_IOQueue___init__ + 4,
        .line_info_top = fun_data_asyncio_core_IOQueue___init__ + 4,
        .opcodes = fun_data_asyncio_core_IOQueue___init__ + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_IOQueue___init__ fun_data_asyncio_core_IOQueue___init__[0]
#endif

// child of asyncio_core_IOQueue
// frozen bytecode for file asyncio/core.py, scope asyncio_core_IOQueue__enqueue
static const byte fun_data_asyncio_core_IOQueue__enqueue[105] = {
    0x4b,0x08, // prelude
    0x32,0x63,0x64,0x65, // names: _enqueue, self, s, idx
     // code info
    0x12,0x66, // LOAD_GLOBAL 'id'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x13,0x1e, // LOAD_ATTR 'map'
    0xdd, // BINARY_OP 6 <in>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x6f, // POP_JUMP_IF_FALSE 47
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xb1, // LOAD_FAST 1
    0x2b,0x03, // BUILD_LIST 3
    0xc3, // STORE_FAST 3
    0x12,0x4d, // LOAD_GLOBAL 'cur_task'
    0xb3, // LOAD_FAST 3
    0xb2, // LOAD_FAST 2
    0x56, // STORE_SUBSCR
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x1e, // LOAD_ATTR 'map'
    0x12,0x66, // LOAD_GLOBAL 'id'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x56, // STORE_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x31, // LOAD_ATTR 'poller'
    0x14,0x33, // LOAD_METHOD 'register'
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x13,0x34, // LOAD_ATTR 'POLLIN'
    0x42,0x44, // JUMP 4
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x13,0x35, // LOAD_ATTR 'POLLOUT'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x42,0x61, // JUMP 33
    0xb0, // LOAD_FAST 0
    0x13,0x1e, // LOAD_ATTR 'map'
    0x12,0x66, // LOAD_GLOBAL 'id'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x55, // LOAD_SUBSCR
    0xc4, // STORE_FAST 4
    0x12,0x4d, // LOAD_GLOBAL 'cur_task'
    0xb4, // LOAD_FAST 4
    0xb2, // LOAD_FAST 2
    0x56, // STORE_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x31, // LOAD_ATTR 'poller'
    0x14,0x36, // LOAD_METHOD 'modify'
    0xb1, // LOAD_FAST 1
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x13,0x34, // LOAD_ATTR 'POLLIN'
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x13,0x35, // LOAD_ATTR 'POLLOUT'
    0xed, // BINARY_OP 22 __or__
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x12,0x4d, // LOAD_GLOBAL 'cur_task'
    0x18,0x21, // STORE_ATTR 'data'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_IOQueue__enqueue = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_IOQueue__enqueue,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 105,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 50,
        .line_info = fun_data_asyncio_core_IOQueue__enqueue + 6,
        .line_info_top = fun_data_asyncio_core_IOQueue__enqueue + 6,
        .opcodes = fun_data_asyncio_core_IOQueue__enqueue + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_IOQueue__enqueue fun_data_asyncio_core_IOQueue__enqueue[0]
#endif

// child of asyncio_core_IOQueue
// frozen bytecode for file asyncio/core.py, scope asyncio_core_IOQueue__dequeue
static const byte fun_data_asyncio_core_IOQueue__dequeue[27] = {
    0x22,0x06, // prelude
    0x37,0x63,0x64, // names: _dequeue, self, s
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x1e, // LOAD_ATTR 'map'
    0x12,0x66, // LOAD_GLOBAL 'id'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x53, // LOAD_NULL
    0x5b, // ROT_THREE
    0x56, // STORE_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x31, // LOAD_ATTR 'poller'
    0x14,0x38, // LOAD_METHOD 'unregister'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_IOQueue__dequeue = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_IOQueue__dequeue,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 27,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 55,
        .line_info = fun_data_asyncio_core_IOQueue__dequeue + 5,
        .line_info_top = fun_data_asyncio_core_IOQueue__dequeue + 5,
        .opcodes = fun_data_asyncio_core_IOQueue__dequeue + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_IOQueue__dequeue fun_data_asyncio_core_IOQueue__dequeue[0]
#endif

// child of asyncio_core_IOQueue
// frozen bytecode for file asyncio/core.py, scope asyncio_core_IOQueue_queue_read
static const byte fun_data_asyncio_core_IOQueue_queue_read[15] = {
    0x2a,0x06, // prelude
    0x39,0x63,0x64, // names: queue_read, self, s
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x32, // LOAD_METHOD '_enqueue'
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_IOQueue_queue_read = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_IOQueue_queue_read,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 15,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 57,
        .line_info = fun_data_asyncio_core_IOQueue_queue_read + 5,
        .line_info_top = fun_data_asyncio_core_IOQueue_queue_read + 5,
        .opcodes = fun_data_asyncio_core_IOQueue_queue_read + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_IOQueue_queue_read fun_data_asyncio_core_IOQueue_queue_read[0]
#endif

// child of asyncio_core_IOQueue
// frozen bytecode for file asyncio/core.py, scope asyncio_core_IOQueue_queue_write
static const byte fun_data_asyncio_core_IOQueue_queue_write[15] = {
    0x2a,0x06, // prelude
    0x3a,0x63,0x64, // names: queue_write, self, s
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x32, // LOAD_METHOD '_enqueue'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_IOQueue_queue_write = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_IOQueue_queue_write,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 15,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 58,
        .line_info = fun_data_asyncio_core_IOQueue_queue_write + 5,
        .line_info_top = fun_data_asyncio_core_IOQueue_queue_write + 5,
        .opcodes = fun_data_asyncio_core_IOQueue_queue_write + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_IOQueue_queue_write fun_data_asyncio_core_IOQueue_queue_write[0]
#endif

// child of asyncio_core_IOQueue
// frozen bytecode for file asyncio/core.py, scope asyncio_core_IOQueue_remove
static const byte fun_data_asyncio_core_IOQueue_remove[65] = {
    0x6a,0x06, // prelude
    0x3b,0x63,0x0b, // names: remove, self, task
     // code info
    0x51, // LOAD_CONST_NONE
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x13,0x1e, // LOAD_ATTR 'map'
    0x5f, // GET_ITER_STACK
    0x4b,0x1f, // FOR_ITER 31
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x1e, // LOAD_ATTR 'map'
    0xb3, // LOAD_FAST 3
    0x55, // LOAD_SUBSCR
    0x30,0x03, // UNPACK_SEQUENCE 3
    0xc4, // STORE_FAST 4
    0xc5, // STORE_FAST 5
    0xc6, // STORE_FAST 6
    0xb4, // LOAD_FAST 4
    0xb1, // LOAD_FAST 1
    0xde, // BINARY_OP 7 <is>
    0x43,0x45, // POP_JUMP_IF_TRUE 5
    0xb5, // LOAD_FAST 5
    0xb1, // LOAD_FAST 1
    0xde, // BINARY_OP 7 <is>
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0xb6, // LOAD_FAST 6
    0xc2, // STORE_FAST 2
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x42,0x42, // JUMP 2
    0x42,0x1f, // JUMP -33
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0xb0, // LOAD_FAST 0
    0x14,0x37, // LOAD_METHOD '_dequeue'
    0xb6, // LOAD_FAST 6
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x42, // JUMP 2
    0x42,0x42, // JUMP 2
    0x42,0x06, // JUMP -58
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_IOQueue_remove = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_IOQueue_remove,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 65,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 59,
        .line_info = fun_data_asyncio_core_IOQueue_remove + 5,
        .line_info_top = fun_data_asyncio_core_IOQueue_remove + 5,
        .opcodes = fun_data_asyncio_core_IOQueue_remove + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_IOQueue_remove fun_data_asyncio_core_IOQueue_remove[0]
#endif

// child of asyncio_core_IOQueue
// frozen bytecode for file asyncio/core.py, scope asyncio_core_IOQueue_wait_io_event
static const byte fun_data_asyncio_core_IOQueue_wait_io_event[156] = {
    0x62,0x06, // prelude
    0x1f,0x63,0x67, // names: wait_io_event, self, dt
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x31, // LOAD_ATTR 'poller'
    0x14,0x3c, // LOAD_METHOD 'ipoll'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x5f, // GET_ITER_STACK
    0x4b,0x89,0x01, // FOR_ITER 137
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc2, // STORE_FAST 2
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x1e, // LOAD_ATTR 'map'
    0x12,0x66, // LOAD_GLOBAL 'id'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0x55, // LOAD_SUBSCR
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x13,0x35, // LOAD_ATTR 'POLLOUT'
    0xd2, // UNARY_OP 2 __invert__
    0xef, // BINARY_OP 24 __and__
    0x44,0x56, // POP_JUMP_IF_FALSE 22
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x12,0x5b, // LOAD_GLOBAL '_task_queue'
    0x14,0x1a, // LOAD_METHOD 'push'
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x56, // STORE_SUBSCR
    0xb3, // LOAD_FAST 3
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x13,0x34, // LOAD_ATTR 'POLLIN'
    0xd2, // UNARY_OP 2 __invert__
    0xef, // BINARY_OP 24 __and__
    0x44,0x56, // POP_JUMP_IF_FALSE 22
    0xb4, // LOAD_FAST 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x12,0x5b, // LOAD_GLOBAL '_task_queue'
    0x14,0x1a, // LOAD_METHOD 'push'
    0xb4, // LOAD_FAST 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0xb4, // LOAD_FAST 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0x56, // STORE_SUBSCR
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0xb4, // LOAD_FAST 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0xb0, // LOAD_FAST 0
    0x14,0x37, // LOAD_METHOD '_dequeue'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x63, // JUMP 35
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0xb0, // LOAD_FAST 0
    0x13,0x31, // LOAD_ATTR 'poller'
    0x14,0x36, // LOAD_METHOD 'modify'
    0xb2, // LOAD_FAST 2
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x13,0x35, // LOAD_ATTR 'POLLOUT'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x42,0x4d, // JUMP 13
    0xb0, // LOAD_FAST 0
    0x13,0x31, // LOAD_ATTR 'poller'
    0x14,0x36, // LOAD_METHOD 'modify'
    0xb2, // LOAD_FAST 2
    0x12,0x07, // LOAD_GLOBAL 'select'
    0x13,0x34, // LOAD_ATTR 'POLLIN'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x42,0xf4,0x7e, // JUMP -140
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_IOQueue_wait_io_event = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_IOQueue_wait_io_event,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 156,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 31,
        .line_info = fun_data_asyncio_core_IOQueue_wait_io_event + 5,
        .line_info_top = fun_data_asyncio_core_IOQueue_wait_io_event + 5,
        .opcodes = fun_data_asyncio_core_IOQueue_wait_io_event + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_IOQueue_wait_io_event fun_data_asyncio_core_IOQueue_wait_io_event[0]
#endif

static const mp_raw_code_t *const children_asyncio_core_IOQueue[] = {
    (const mp_raw_code_t *)&proto_fun_asyncio_core_IOQueue___init__,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_IOQueue__enqueue,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_IOQueue__dequeue,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_IOQueue_queue_read,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_IOQueue_queue_write,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_IOQueue_remove,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_IOQueue_wait_io_event,
};

static const mp_raw_code_truncated_t proto_fun_asyncio_core_IOQueue = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_IOQueue,
    .children = (void *)&children_asyncio_core_IOQueue,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 41,
    .n_children = 7,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 18,
        .line_info = fun_data_asyncio_core_IOQueue + 3,
        .line_info_top = fun_data_asyncio_core_IOQueue + 3,
        .opcodes = fun_data_asyncio_core_IOQueue + 3,
    },
    #endif
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core__promote_to_task
static const byte fun_data_asyncio_core__promote_to_task[21] = {
    0x19,0x04, // prelude
    0x17,0x56, // names: _promote_to_task, aw
     // code info
    0x12,0x57, // LOAD_GLOBAL 'isinstance'
    0xb0, // LOAD_FAST 0
    0x12,0x09, // LOAD_GLOBAL 'Task'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
    0x12,0x18, // LOAD_GLOBAL 'create_task'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core__promote_to_task = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core__promote_to_task,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 21,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 23,
        .line_info = fun_data_asyncio_core__promote_to_task + 4,
        .line_info_top = fun_data_asyncio_core__promote_to_task + 4,
        .opcodes = fun_data_asyncio_core__promote_to_task + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core__promote_to_task fun_data_asyncio_core__promote_to_task[0]
#endif

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_create_task
static const byte fun_data_asyncio_core_create_task[40] = {
    0x21,0x04, // prelude
    0x18,0x22, // names: create_task, coro
     // code info
    0x12,0x58, // LOAD_GLOBAL 'hasattr'
    0xb0, // LOAD_FAST 0
    0x10,0x19, // LOAD_CONST_STRING 'send'
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0x12,0x59, // LOAD_GLOBAL 'TypeError'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x12,0x09, // LOAD_GLOBAL 'Task'
    0xb0, // LOAD_FAST 0
    0x12,0x5a, // LOAD_GLOBAL 'globals'
    0x34,0x00, // CALL_FUNCTION 0
    0x34,0x02, // CALL_FUNCTION 2
    0xc1, // STORE_FAST 1
    0x12,0x5b, // LOAD_GLOBAL '_task_queue'
    0x14,0x1a, // LOAD_METHOD 'push'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_create_task = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_create_task,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 40,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 24,
        .line_info = fun_data_asyncio_core_create_task + 4,
        .line_info_top = fun_data_asyncio_core_create_task + 4,
        .opcodes = fun_data_asyncio_core_create_task + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_create_task fun_data_asyncio_core_create_task[0]
#endif

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_run_until_complete
static const byte fun_data_asyncio_core_run_until_complete[346] = {
    0xf1,0x03,0x04, // prelude
    0x1b,0x5c, // names: run_until_complete, main_task
     // code info
    0x12,0x0c, // LOAD_GLOBAL 'CancelledError'
    0x12,0x4b, // LOAD_GLOBAL 'Exception'
    0x2a,0x02, // BUILD_TUPLE 2
    0xc1, // STORE_FAST 1
    0x12,0x0c, // LOAD_GLOBAL 'CancelledError'
    0x12,0x5d, // LOAD_GLOBAL 'StopIteration'
    0x2a,0x02, // BUILD_TUPLE 2
    0xc2, // STORE_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0xc3, // STORE_FAST 3
    0x42,0x7e, // JUMP 62
    0x7f, // LOAD_CONST_SMALL_INT -1
    0xc3, // STORE_FAST 3
    0x12,0x5b, // LOAD_GLOBAL '_task_queue'
    0x14,0x1c, // LOAD_METHOD 'peek'
    0x36,0x00, // CALL_METHOD 0
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x44,0x53, // POP_JUMP_IF_FALSE 19
    0x12,0x54, // LOAD_GLOBAL 'max'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x12,0x03, // LOAD_GLOBAL 'ticks_diff'
    0xb4, // LOAD_FAST 4
    0x13,0x1d, // LOAD_ATTR 'ph_key'
    0x12,0x49, // LOAD_GLOBAL 'ticks'
    0x34,0x00, // CALL_FUNCTION 0
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x02, // CALL_FUNCTION 2
    0xc3, // STORE_FAST 3
    0x42,0x57, // JUMP 23
    0x12,0x5e, // LOAD_GLOBAL '_io_queue'
    0x13,0x1e, // LOAD_ATTR 'map'
    0x43,0x51, // POP_JUMP_IF_TRUE 17
    0x51, // LOAD_CONST_NONE
    0x17,0x4d, // STORE_GLOBAL 'cur_task'
    0xb0, // LOAD_FAST 0
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'state'
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x83, // LOAD_CONST_SMALL_INT 3
    0xc3, // STORE_FAST 3
    0x42,0x40, // JUMP 0
    0x12,0x5e, // LOAD_GLOBAL '_io_queue'
    0x14,0x1f, // LOAD_METHOD 'wait_io_event'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd8, // BINARY_OP 1 __gt__
    0x43,0xbc,0x7f, // POP_JUMP_IF_TRUE -68
    0x12,0x5b, // LOAD_GLOBAL '_task_queue'
    0x14,0x20, // LOAD_METHOD 'pop'
    0x36,0x00, // CALL_METHOD 0
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x17,0x4d, // STORE_GLOBAL 'cur_task'
    0x48,0x22, // SETUP_EXCEPT 34
    0xb4, // LOAD_FAST 4
    0x13,0x21, // LOAD_ATTR 'data'
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x43,0x4b, // POP_JUMP_IF_TRUE 11
    0xb4, // LOAD_FAST 4
    0x13,0x22, // LOAD_ATTR 'coro'
    0x14,0x19, // LOAD_METHOD 'send'
    0x51, // LOAD_CONST_NONE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x4d, // JUMP 13
    0x51, // LOAD_CONST_NONE
    0xb4, // LOAD_FAST 4
    0x18,0x21, // STORE_ATTR 'data'
    0xb4, // LOAD_FAST 4
    0x13,0x22, // LOAD_ATTR 'coro'
    0x14,0x23, // LOAD_METHOD 'throw'
    0xb5, // LOAD_FAST 5
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0xcc,0x01, // POP_EXCEPT_JUMP 204
    0x57, // DUP_TOP
    0xb1, // LOAD_FAST 1
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0xc5,0x81, // POP_JUMP_IF_FALSE 197
    0xc6, // STORE_FAST 6
    0x49,0xba,0x01, // SETUP_FINALLY 186
    0xb4, // LOAD_FAST 4
    0xb0, // LOAD_FAST 0
    0xde, // BINARY_OP 7 <is>
    0xc7, // STORE_FAST 7
    0xb7, // LOAD_FAST 7
    0x44,0x5d, // POP_JUMP_IF_FALSE 29
    0x51, // LOAD_CONST_NONE
    0x17,0x4d, // STORE_GLOBAL 'cur_task'
    0x12,0x57, // LOAD_GLOBAL 'isinstance'
    0xb6, // LOAD_FAST 6
    0x12,0x5d, // LOAD_GLOBAL 'StopIteration'
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x46, // POP_JUMP_IF_TRUE 6
    0x50, // LOAD_CONST_FALSE
    0xb4, // LOAD_FAST 4
    0x18,0x15, // STORE_ATTR 'state'
    0xb6, // LOAD_FAST 6
    0x65, // RAISE_OBJ
    0xb4, // LOAD_FAST 4
    0x13,0x15, // LOAD_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x50, // LOAD_CONST_FALSE
    0xb4, // LOAD_FAST 4
    0x18,0x15, // STORE_ATTR 'state'
    0xb4, // LOAD_FAST 4
    0x13,0x15, // LOAD_ATTR 'state'
    0x44,0xe4,0x80, // POP_JUMP_IF_FALSE 100
    0xb4, // LOAD_FAST 4
    0x13,0x15, // LOAD_ATTR 'state'
    0x52, // LOAD_CONST_TRUE
    0xde, // BINARY_OP 7 <is>
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0xb7, // LOAD_FAST 7
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x50, // LOAD_CONST_FALSE
    0x42,0x41, // JUMP 1
    0x51, // LOAD_CONST_NONE
    0xb4, // LOAD_FAST 4
    0x18,0x15, // STORE_ATTR 'state'
    0x42,0x78, // JUMP 56
    0x12,0x5f, // LOAD_GLOBAL 'callable'
    0xb4, // LOAD_FAST 4
    0x13,0x15, // LOAD_ATTR 'state'
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0xb4, // LOAD_FAST 4
    0x14,0x15, // LOAD_METHOD 'state'
    0xb4, // LOAD_FAST 4
    0xb6, // LOAD_FAST 6
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0xb4, // LOAD_FAST 4
    0x18,0x15, // STORE_ATTR 'state'
    0x52, // LOAD_CONST_TRUE
    0xc7, // STORE_FAST 7
    0x42,0x5f, // JUMP 31
    0x42,0x50, // JUMP 16
    0x12,0x5b, // LOAD_GLOBAL '_task_queue'
    0x14,0x1a, // LOAD_METHOD 'push'
    0xb4, // LOAD_FAST 4
    0x13,0x15, // LOAD_ATTR 'state'
    0x14,0x20, // LOAD_METHOD 'pop'
    0x36,0x00, // CALL_METHOD 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x52, // LOAD_CONST_TRUE
    0xc7, // STORE_FAST 7
    0xb4, // LOAD_FAST 4
    0x13,0x15, // LOAD_ATTR 'state'
    0x14,0x1c, // LOAD_METHOD 'peek'
    0x36,0x00, // CALL_METHOD 0
    0x43,0x27, // POP_JUMP_IF_TRUE -25
    0x50, // LOAD_CONST_FALSE
    0xb4, // LOAD_FAST 4
    0x18,0x15, // STORE_ATTR 'state'
    0xb7, // LOAD_FAST 7
    0x43,0x50, // POP_JUMP_IF_TRUE 16
    0x12,0x57, // LOAD_GLOBAL 'isinstance'
    0xb6, // LOAD_FAST 6
    0xb2, // LOAD_FAST 2
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x48, // POP_JUMP_IF_TRUE 8
    0x12,0x5b, // LOAD_GLOBAL '_task_queue'
    0x14,0x1a, // LOAD_METHOD 'push'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb6, // LOAD_FAST 6
    0xb4, // LOAD_FAST 4
    0x18,0x21, // STORE_ATTR 'data'
    0x42,0x62, // JUMP 34
    0xb4, // LOAD_FAST 4
    0x13,0x15, // LOAD_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x5b, // POP_JUMP_IF_FALSE 27
    0xb5, // LOAD_FAST 5
    0xb4, // LOAD_FAST 4
    0x18,0x21, // STORE_ATTR 'data'
    0xb5, // LOAD_FAST 5
    0x12,0x4c, // LOAD_GLOBAL '_exc_context'
    0x10,0x0f, // LOAD_CONST_STRING 'exception'
    0x56, // STORE_SUBSCR
    0xb4, // LOAD_FAST 4
    0x12,0x4c, // LOAD_GLOBAL '_exc_context'
    0x10,0x10, // LOAD_CONST_STRING 'future'
    0x56, // STORE_SUBSCR
    0x12,0x13, // LOAD_GLOBAL 'Loop'
    0x14,0x24, // LOAD_METHOD 'call_exception_handler'
    0x12,0x4c, // LOAD_GLOBAL '_exc_context'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x40, // JUMP 0
    0xb4, // LOAD_FAST 4
    0xb0, // LOAD_FAST 0
    0xde, // BINARY_OP 7 <is>
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0xb6, // LOAD_FAST 6
    0x13,0x25, // LOAD_ATTR 'value'
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc6, // STORE_FAST 6
    0x28,0x06, // DELETE_FAST 6
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x42,0xbb,0x7d, // JUMP -325
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_run_until_complete = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_run_until_complete,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 346,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 15,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 27,
        .line_info = fun_data_asyncio_core_run_until_complete + 5,
        .line_info_top = fun_data_asyncio_core_run_until_complete + 5,
        .opcodes = fun_data_asyncio_core_run_until_complete + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_run_until_complete fun_data_asyncio_core_run_until_complete[0]
#endif

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_run
static const byte fun_data_asyncio_core_run[14] = {
    0x19,0x04, // prelude
    0x26,0x22, // names: run, coro
     // code info
    0x12,0x1b, // LOAD_GLOBAL 'run_until_complete'
    0x12,0x18, // LOAD_GLOBAL 'create_task'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_run = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_run,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 14,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 38,
        .line_info = fun_data_asyncio_core_run + 4,
        .line_info_top = fun_data_asyncio_core_run + 4,
        .opcodes = fun_data_asyncio_core_run + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_run fun_data_asyncio_core_run[0]
#endif

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core__stopper
static const byte fun_data_asyncio_core__stopper[6] = {
    0x80,0x40,0x02, // prelude
    0x27, // names: _stopper
     // code info
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core__stopper = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_core__stopper,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 6,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 39,
        .line_info = fun_data_asyncio_core__stopper + 4,
        .line_info_top = fun_data_asyncio_core__stopper + 4,
        .opcodes = fun_data_asyncio_core__stopper + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core__stopper fun_data_asyncio_core__stopper[0]
#endif

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop
static const byte fun_data_asyncio_core_Loop[52] = {
    0x00,0x02, // prelude
    0x13, // names: Loop
     // code info
    0x11,0x4f, // LOAD_NAME '__name__'
    0x16,0x50, // STORE_NAME '__module__'
    0x10,0x13, // LOAD_CONST_STRING 'Loop'
    0x16,0x51, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x16,0x41, // STORE_NAME '_exc_handler'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x18, // STORE_NAME 'create_task'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x3d, // STORE_NAME 'run_forever'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x1b, // STORE_NAME 'run_until_complete'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x3e, // STORE_NAME 'stop'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x3f, // STORE_NAME 'close'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x40, // STORE_NAME 'set_exception_handler'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x42, // STORE_NAME 'get_exception_handler'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x43, // STORE_NAME 'default_exception_handler'
    0x32,0x08, // MAKE_FUNCTION 8
    0x16,0x24, // STORE_NAME 'call_exception_handler'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_create_task
static const byte fun_data_asyncio_core_Loop_create_task[10] = {
    0x11,0x04, // prelude
    0x18,0x22, // names: create_task, coro
     // code info
    0x12,0x18, // LOAD_GLOBAL 'create_task'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_Loop_create_task = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_Loop_create_task,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 10,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 24,
        .line_info = fun_data_asyncio_core_Loop_create_task + 4,
        .line_info_top = fun_data_asyncio_core_Loop_create_task + 4,
        .opcodes = fun_data_asyncio_core_Loop_create_task + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_Loop_create_task fun_data_asyncio_core_Loop_create_task[0]
#endif

// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_run_forever
static const byte fun_data_asyncio_core_Loop_run_forever[26] = {
    0x10,0x02, // prelude
    0x3d, // names: run_forever
     // code info
    0x12,0x09, // LOAD_GLOBAL 'Task'
    0x12,0x27, // LOAD_GLOBAL '_stopper'
    0x34,0x00, // CALL_FUNCTION 0
    0x12,0x5a, // LOAD_GLOBAL 'globals'
    0x34,0x00, // CALL_FUNCTION 0
    0x34,0x02, // CALL_FUNCTION 2
    0x17,0x4e, // STORE_GLOBAL '_stop_task'
    0x12,0x1b, // LOAD_GLOBAL 'run_until_complete'
    0x12,0x4e, // LOAD_GLOBAL '_stop_task'
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_Loop_run_forever = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_Loop_run_forever,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 26,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 61,
        .line_info = fun_data_asyncio_core_Loop_run_forever + 3,
        .line_info_top = fun_data_asyncio_core_Loop_run_forever + 3,
        .opcodes = fun_data_asyncio_core_Loop_run_forever + 3,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_Loop_run_forever fun_data_asyncio_core_Loop_run_forever[0]
#endif

// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_run_until_complete
static const byte fun_data_asyncio_core_Loop_run_until_complete[14] = {
    0x19,0x04, // prelude
    0x1b,0x56, // names: run_until_complete, aw
     // code info
    0x12,0x1b, // LOAD_GLOBAL 'run_until_complete'
    0x12,0x17, // LOAD_GLOBAL '_promote_to_task'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_Loop_run_until_complete = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_Loop_run_until_complete,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 14,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 27,
        .line_info = fun_data_asyncio_core_Loop_run_until_complete + 4,
        .line_info_top = fun_data_asyncio_core_Loop_run_until_complete + 4,
        .opcodes = fun_data_asyncio_core_Loop_run_until_complete + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_Loop_run_until_complete fun_data_asyncio_core_Loop_run_until_complete[0]
#endif

// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_stop
static const byte fun_data_asyncio_core_Loop_stop[24] = {
    0x10,0x02, // prelude
    0x3e, // names: stop
     // code info
    0x12,0x4e, // LOAD_GLOBAL '_stop_task'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0x12,0x5b, // LOAD_GLOBAL '_task_queue'
    0x14,0x1a, // LOAD_METHOD 'push'
    0x12,0x4e, // LOAD_GLOBAL '_stop_task'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x17,0x4e, // STORE_GLOBAL '_stop_task'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_Loop_stop = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_Loop_stop,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 24,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 62,
        .line_info = fun_data_asyncio_core_Loop_stop + 3,
        .line_info_top = fun_data_asyncio_core_Loop_stop + 3,
        .opcodes = fun_data_asyncio_core_Loop_stop + 3,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_Loop_stop fun_data_asyncio_core_Loop_stop[0]
#endif

// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_close
static const byte fun_data_asyncio_core_Loop_close[5] = {
    0x00,0x02, // prelude
    0x3f, // names: close
     // code info
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_Loop_close = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_Loop_close,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 5,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 63,
        .line_info = fun_data_asyncio_core_Loop_close + 3,
        .line_info_top = fun_data_asyncio_core_Loop_close + 3,
        .opcodes = fun_data_asyncio_core_Loop_close + 3,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_Loop_close fun_data_asyncio_core_Loop_close[0]
#endif

// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_set_exception_handler
static const byte fun_data_asyncio_core_Loop_set_exception_handler[11] = {
    0x11,0x04, // prelude
    0x40,0x68, // names: set_exception_handler, handler
     // code info
    0xb0, // LOAD_FAST 0
    0x12,0x13, // LOAD_GLOBAL 'Loop'
    0x18,0x41, // STORE_ATTR '_exc_handler'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_Loop_set_exception_handler = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_Loop_set_exception_handler,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 11,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 64,
        .line_info = fun_data_asyncio_core_Loop_set_exception_handler + 4,
        .line_info_top = fun_data_asyncio_core_Loop_set_exception_handler + 4,
        .opcodes = fun_data_asyncio_core_Loop_set_exception_handler + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_Loop_set_exception_handler fun_data_asyncio_core_Loop_set_exception_handler[0]
#endif

// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_get_exception_handler
static const byte fun_data_asyncio_core_Loop_get_exception_handler[8] = {
    0x00,0x02, // prelude
    0x42, // names: get_exception_handler
     // code info
    0x12,0x13, // LOAD_GLOBAL 'Loop'
    0x13,0x41, // LOAD_ATTR '_exc_handler'
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_Loop_get_exception_handler = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_Loop_get_exception_handler,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 8,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 66,
        .line_info = fun_data_asyncio_core_Loop_get_exception_handler + 3,
        .line_info_top = fun_data_asyncio_core_Loop_get_exception_handler + 3,
        .opcodes = fun_data_asyncio_core_Loop_get_exception_handler + 3,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_Loop_get_exception_handler fun_data_asyncio_core_Loop_get_exception_handler[0]
#endif

// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_default_exception_handler
static const byte fun_data_asyncio_core_Loop_default_exception_handler[64] = {
    0x42,0x06, // prelude
    0x43,0x69,0x6a, // names: default_exception_handler, loop, context
     // code info
    0x12,0x6b, // LOAD_GLOBAL 'print'
    0xb1, // LOAD_FAST 1
    0x10,0x0e, // LOAD_CONST_STRING 'message'
    0x55, // LOAD_SUBSCR
    0x10,0x44, // LOAD_CONST_STRING 'file'
    0x12,0x06, // LOAD_GLOBAL 'sys'
    0x13,0x45, // LOAD_ATTR 'stderr'
    0x34,0x82,0x01, // CALL_FUNCTION 257
    0x59, // POP_TOP
    0x12,0x6b, // LOAD_GLOBAL 'print'
    0x10,0x46, // LOAD_CONST_STRING 'future:'
    0xb1, // LOAD_FAST 1
    0x10,0x10, // LOAD_CONST_STRING 'future'
    0x55, // LOAD_SUBSCR
    0x10,0x47, // LOAD_CONST_STRING 'coro='
    0xb1, // LOAD_FAST 1
    0x10,0x10, // LOAD_CONST_STRING 'future'
    0x55, // LOAD_SUBSCR
    0x13,0x22, // LOAD_ATTR 'coro'
    0x10,0x44, // LOAD_CONST_STRING 'file'
    0x12,0x06, // LOAD_GLOBAL 'sys'
    0x13,0x45, // LOAD_ATTR 'stderr'
    0x34,0x82,0x04, // CALL_FUNCTION 260
    0x59, // POP_TOP
    0x12,0x06, // LOAD_GLOBAL 'sys'
    0x14,0x48, // LOAD_METHOD 'print_exception'
    0xb1, // LOAD_FAST 1
    0x10,0x0f, // LOAD_CONST_STRING 'exception'
    0x55, // LOAD_SUBSCR
    0x12,0x06, // LOAD_GLOBAL 'sys'
    0x13,0x45, // LOAD_ATTR 'stderr'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_Loop_default_exception_handler = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_Loop_default_exception_handler,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 64,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 67,
        .line_info = fun_data_asyncio_core_Loop_default_exception_handler + 5,
        .line_info_top = fun_data_asyncio_core_Loop_default_exception_handler + 5,
        .opcodes = fun_data_asyncio_core_Loop_default_exception_handler + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_Loop_default_exception_handler fun_data_asyncio_core_Loop_default_exception_handler[0]
#endif

// child of asyncio_core_Loop
// frozen bytecode for file asyncio/core.py, scope asyncio_core_Loop_call_exception_handler
static const byte fun_data_asyncio_core_Loop_call_exception_handler[22] = {
    0x19,0x04, // prelude
    0x24,0x6a, // names: call_exception_handler, context
     // code info
    0x12,0x13, // LOAD_GLOBAL 'Loop'
    0x13,0x41, // LOAD_ATTR '_exc_handler'
    0x45,0x04, // JUMP_IF_TRUE_OR_POP 4
    0x12,0x13, // LOAD_GLOBAL 'Loop'
    0x13,0x43, // LOAD_ATTR 'default_exception_handler'
    0x12,0x13, // LOAD_GLOBAL 'Loop'
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_Loop_call_exception_handler = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_Loop_call_exception_handler,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 22,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 36,
        .line_info = fun_data_asyncio_core_Loop_call_exception_handler + 4,
        .line_info_top = fun_data_asyncio_core_Loop_call_exception_handler + 4,
        .opcodes = fun_data_asyncio_core_Loop_call_exception_handler + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_Loop_call_exception_handler fun_data_asyncio_core_Loop_call_exception_handler[0]
#endif

static const mp_raw_code_t *const children_asyncio_core_Loop[] = {
    (const mp_raw_code_t *)&proto_fun_asyncio_core_Loop_create_task,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_Loop_run_forever,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_Loop_run_until_complete,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_Loop_stop,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_Loop_close,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_Loop_set_exception_handler,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_Loop_get_exception_handler,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_Loop_default_exception_handler,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_Loop_call_exception_handler,
};

static const mp_raw_code_truncated_t proto_fun_asyncio_core_Loop = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_Loop,
    .children = (void *)&children_asyncio_core_Loop,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 52,
    .n_children = 9,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 19,
        .line_info = fun_data_asyncio_core_Loop + 3,
        .line_info_top = fun_data_asyncio_core_Loop + 3,
        .opcodes = fun_data_asyncio_core_Loop + 3,
    },
    #endif
    #endif
};

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_get_event_loop
static const byte fun_data_asyncio_core_get_event_loop[10] = {
    0x92,0x80,0x01,0x06, // prelude
    0x28,0x60,0x61, // names: get_event_loop, runq_len, waitq_len
     // code info
    0x12,0x13, // LOAD_GLOBAL 'Loop'
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_get_event_loop = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_get_event_loop,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 10,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 40,
        .line_info = fun_data_asyncio_core_get_event_loop + 7,
        .line_info_top = fun_data_asyncio_core_get_event_loop + 7,
        .opcodes = fun_data_asyncio_core_get_event_loop + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_get_event_loop fun_data_asyncio_core_get_event_loop[0]
#endif

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_current_task
static const byte fun_data_asyncio_core_current_task[19] = {
    0x08,0x02, // prelude
    0x29, // names: current_task
     // code info
    0x12,0x4d, // LOAD_GLOBAL 'cur_task'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x62, // LOAD_GLOBAL 'RuntimeError'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x12,0x4d, // LOAD_GLOBAL 'cur_task'
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_current_task = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_current_task,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 19,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 41,
        .line_info = fun_data_asyncio_core_current_task + 3,
        .line_info_top = fun_data_asyncio_core_current_task + 3,
        .opcodes = fun_data_asyncio_core_current_task + 3,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_current_task fun_data_asyncio_core_current_task[0]
#endif

// child of asyncio_core__lt_module_gt_
// frozen bytecode for file asyncio/core.py, scope asyncio_core_new_event_loop
static const byte fun_data_asyncio_core_new_event_loop[18] = {
    0x00,0x02, // prelude
    0x2a, // names: new_event_loop
     // code info
    0x12,0x08, // LOAD_GLOBAL 'TaskQueue'
    0x34,0x00, // CALL_FUNCTION 0
    0x17,0x5b, // STORE_GLOBAL '_task_queue'
    0x12,0x12, // LOAD_GLOBAL 'IOQueue'
    0x34,0x00, // CALL_FUNCTION 0
    0x17,0x5e, // STORE_GLOBAL '_io_queue'
    0x12,0x13, // LOAD_GLOBAL 'Loop'
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_core_new_event_loop = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core_new_event_loop,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 18,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 42,
        .line_info = fun_data_asyncio_core_new_event_loop + 3,
        .line_info_top = fun_data_asyncio_core_new_event_loop + 3,
        .opcodes = fun_data_asyncio_core_new_event_loop + 3,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_core_new_event_loop fun_data_asyncio_core_new_event_loop[0]
#endif

static const mp_raw_code_t *const children_asyncio_core__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_asyncio_core_CancelledError,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_TimeoutError,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_SingletonGenerator,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_sleep_ms,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_sleep,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_IOQueue,
    (const mp_raw_code_t *)&proto_fun_asyncio_core__promote_to_task,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_create_task,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_run_until_complete,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_run,
    (const mp_raw_code_t *)&proto_fun_asyncio_core__stopper,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_Loop,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_get_event_loop,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_current_task,
    (const mp_raw_code_t *)&proto_fun_asyncio_core_new_event_loop,
};

static const mp_raw_code_truncated_t proto_fun_asyncio_core__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_core__lt_module_gt_,
    .children = (void *)&children_asyncio_core__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 218,
    .n_children = 15,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_asyncio_core__lt_module_gt_ + 3,
        .line_info_top = fun_data_asyncio_core__lt_module_gt_ + 3,
        .opcodes = fun_data_asyncio_core__lt_module_gt_ + 3,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_asyncio_core[108] = {
    MP_QSTR_asyncio_slash_core_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_ticks_ms,
    MP_QSTR_ticks_diff,
    MP_QSTR_ticks_add,
    MP_QSTR_time,
    MP_QSTR_sys,
    MP_QSTR_select,
    MP_QSTR_TaskQueue,
    MP_QSTR_Task,
    MP_QSTR__asyncio,
    MP_QSTR_task,
    MP_QSTR_CancelledError,
    MP_QSTR_TimeoutError,
    MP_QSTR_message,
    MP_QSTR_exception,
    MP_QSTR_future,
    MP_QSTR_SingletonGenerator,
    MP_QSTR_IOQueue,
    MP_QSTR_Loop,
    MP_QSTR_sleep_ms,
    MP_QSTR_state,
    MP_QSTR_sleep,
    MP_QSTR__promote_to_task,
    MP_QSTR_create_task,
    MP_QSTR_send,
    MP_QSTR_push,
    MP_QSTR_run_until_complete,
    MP_QSTR_peek,
    MP_QSTR_ph_key,
    MP_QSTR_map,
    MP_QSTR_wait_io_event,
    MP_QSTR_pop,
    MP_QSTR_data,
    MP_QSTR_coro,
    MP_QSTR_throw,
    MP_QSTR_call_exception_handler,
    MP_QSTR_value,
    MP_QSTR_run,
    MP_QSTR__stopper,
    MP_QSTR_get_event_loop,
    MP_QSTR_current_task,
    MP_QSTR_new_event_loop,
    MP_QSTR___init__,
    MP_QSTR_exc,
    MP_QSTR___iter__,
    MP_QSTR___next__,
    MP_QSTR___traceback__,
    MP_QSTR_poll,
    MP_QSTR_poller,
    MP_QSTR__enqueue,
    MP_QSTR_register,
    MP_QSTR_POLLIN,
    MP_QSTR_POLLOUT,
    MP_QSTR_modify,
    MP_QSTR__dequeue,
    MP_QSTR_unregister,
    MP_QSTR_queue_read,
    MP_QSTR_queue_write,
    MP_QSTR_remove,
    MP_QSTR_ipoll,
    MP_QSTR_run_forever,
    MP_QSTR_stop,
    MP_QSTR_close,
    MP_QSTR_set_exception_handler,
    MP_QSTR__exc_handler,
    MP_QSTR_get_exception_handler,
    MP_QSTR_default_exception_handler,
    MP_QSTR_file,
    MP_QSTR_stderr,
    MP_QSTR_future_colon_,
    MP_QSTR_coro_equals_,
    MP_QSTR_print_exception,
    MP_QSTR_ticks,
    MP_QSTR_BaseException,
    MP_QSTR_Exception,
    MP_QSTR__exc_context,
    MP_QSTR_cur_task,
    MP_QSTR__stop_task,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_t,
    MP_QSTR_sgen,
    MP_QSTR_max,
    MP_QSTR_int,
    MP_QSTR_aw,
    MP_QSTR_isinstance,
    MP_QSTR_hasattr,
    MP_QSTR_TypeError,
    MP_QSTR_globals,
    MP_QSTR__task_queue,
    MP_QSTR_main_task,
    MP_QSTR_StopIteration,
    MP_QSTR__io_queue,
    MP_QSTR_callable,
    MP_QSTR_runq_len,
    MP_QSTR_waitq_len,
    MP_QSTR_RuntimeError,
    MP_QSTR_self,
    MP_QSTR_s,
    MP_QSTR_idx,
    MP_QSTR_id,
    MP_QSTR_dt,
    MP_QSTR_handler,
    MP_QSTR_loop,
    MP_QSTR_context,
    MP_QSTR_print,
};

// constants
static const mp_obj_str_t const_obj_asyncio_core_0 = {{&mp_type_str}, 64973, 31, (const byte*)"\x54\x61\x73\x6b\x20\x65\x78\x63\x65\x70\x74\x69\x6f\x6e\x20\x77\x61\x73\x6e\x27\x74\x20\x72\x65\x74\x72\x69\x65\x76\x65\x64"};

// constant table
static const mp_rom_obj_t const_obj_table_data_asyncio_core[3] = {
    MP_ROM_PTR(&const_obj_asyncio_core_0),
    MP_ROM_QSTR(MP_QSTR_coroutine_space_expected),
    MP_ROM_QSTR(MP_QSTR_no_space_running_space_event_space_loop),
};

static const mp_frozen_module_t frozen_module_asyncio_core = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_asyncio_core,
        .obj_table = (mp_obj_t *)&const_obj_table_data_asyncio_core,
    },
    .proto_fun = &proto_fun_asyncio_core__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module asyncio_event
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/asyncio/event.mpy
// - frozen file name: asyncio/event.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file asyncio/event.py, scope asyncio_event__lt_module_gt_
static const byte fun_data_asyncio_event__lt_module_gt_[59] = {
    0x2c,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x02, // LOAD_CONST_STRING 'core'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME ''
    0x1c,0x02, // IMPORT_FROM 'core'
    0x16,0x02, // STORE_NAME 'core'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'Event'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x04, // STORE_NAME 'Event'
    0x48,0x15, // SETUP_EXCEPT 21
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x05, // IMPORT_NAME 'io'
    0x16,0x05, // STORE_NAME 'io'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x06, // LOAD_CONST_STRING 'ThreadSafeFlag'
    0x11,0x05, // LOAD_NAME 'io'
    0x13,0x07, // LOAD_ATTR 'IOBase'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x06, // STORE_NAME 'ThreadSafeFlag'
    0x4a,0x0a, // POP_EXCEPT_JUMP 10
    0x57, // DUP_TOP
    0x11,0x19, // LOAD_NAME 'ImportError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_event__lt_module_gt_
// frozen bytecode for file asyncio/event.py, scope asyncio_event_Event
static const byte fun_data_asyncio_event_Event[33] = {
    0x00,0x02, // prelude
    0x04, // names: Event
     // code info
    0x11,0x1a, // LOAD_NAME '__name__'
    0x16,0x1b, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'Event'
    0x16,0x1c, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x08, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x0c, // STORE_NAME 'is_set'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0d, // STORE_NAME 'set'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x12, // STORE_NAME 'clear'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x13, // STORE_NAME 'wait'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_event_Event
// frozen bytecode for file asyncio/event.py, scope asyncio_event_Event___init__
static const byte fun_data_asyncio_event_Event___init__[19] = {
    0x11,0x04, // prelude
    0x08,0x1d, // names: __init__, self
     // code info
    0x50, // LOAD_CONST_FALSE
    0xb0, // LOAD_FAST 0
    0x18,0x09, // STORE_ATTR 'state'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x0a, // LOAD_METHOD 'TaskQueue'
    0x36,0x00, // CALL_METHOD 0
    0xb0, // LOAD_FAST 0
    0x18,0x0b, // STORE_ATTR 'waiting'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_event_Event___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_event_Event___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 19,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 8,
        .line_info = fun_data_asyncio_event_Event___init__ + 4,
        .line_info_top = fun_data_asyncio_event_Event___init__ + 4,
        .opcodes = fun_data_asyncio_event_Event___init__ + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_event_Event___init__ fun_data_asyncio_event_Event___init__[0]
#endif

// child of asyncio_event_Event
// frozen bytecode for file asyncio/event.py, scope asyncio_event_Event_is_set
static const byte fun_data_asyncio_event_Event_is_set[8] = {
    0x09,0x04, // prelude
    0x0c,0x1d, // names: is_set, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'state'
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_event_Event_is_set = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_event_Event_is_set,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 8,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_asyncio_event_Event_is_set + 4,
        .line_info_top = fun_data_asyncio_event_Event_is_set + 4,
        .opcodes = fun_data_asyncio_event_Event_is_set + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_event_Event_is_set fun_data_asyncio_event_Event_is_set[0]
#endif

// child of asyncio_event_Event
// frozen bytecode for file asyncio/event.py, scope asyncio_event_Event_set
static const byte fun_data_asyncio_event_Event_set[37] = {
    0x21,0x04, // prelude
    0x0d,0x1d, // names: set, self
     // code info
    0x42,0x50, // JUMP 16
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x0e, // LOAD_ATTR '_task_queue'
    0x14,0x0f, // LOAD_METHOD 'push'
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'waiting'
    0x14,0x10, // LOAD_METHOD 'pop'
    0x36,0x00, // CALL_METHOD 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'waiting'
    0x14,0x11, // LOAD_METHOD 'peek'
    0x36,0x00, // CALL_METHOD 0
    0x43,0x27, // POP_JUMP_IF_TRUE -25
    0x52, // LOAD_CONST_TRUE
    0xb0, // LOAD_FAST 0
    0x18,0x09, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_event_Event_set = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_event_Event_set,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 37,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_asyncio_event_Event_set + 4,
        .line_info_top = fun_data_asyncio_event_Event_set + 4,
        .opcodes = fun_data_asyncio_event_Event_set + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_event_Event_set fun_data_asyncio_event_Event_set[0]
#endif

// child of asyncio_event_Event
// frozen bytecode for file asyncio/event.py, scope asyncio_event_Event_clear
static const byte fun_data_asyncio_event_Event_clear[10] = {
    0x11,0x04, // prelude
    0x12,0x1d, // names: clear, self
     // code info
    0x50, // LOAD_CONST_FALSE
    0xb0, // LOAD_FAST 0
    0x18,0x09, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_event_Event_clear = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_event_Event_clear,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 10,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 18,
        .line_info = fun_data_asyncio_event_Event_clear + 4,
        .line_info_top = fun_data_asyncio_event_Event_clear + 4,
        .opcodes = fun_data_asyncio_event_Event_clear + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_event_Event_clear fun_data_asyncio_event_Event_clear[0]
#endif

// child of asyncio_event_Event
// frozen bytecode for file asyncio/event.py, scope asyncio_event_Event_wait
static const byte fun_data_asyncio_event_Event_wait[36] = {
    0x99,0x40,0x04, // prelude
    0x13,0x1d, // names: wait, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'state'
    0x43,0x58, // POP_JUMP_IF_TRUE 24
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'waiting'
    0x14,0x0f, // LOAD_METHOD 'push'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x14, // LOAD_ATTR 'cur_task'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'waiting'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x14, // LOAD_ATTR 'cur_task'
    0x18,0x15, // STORE_ATTR 'data'
    0x51, // LOAD_CONST_NONE
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_event_Event_wait = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_event_Event_wait,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 36,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 19,
        .line_info = fun_data_asyncio_event_Event_wait + 5,
        .line_info_top = fun_data_asyncio_event_Event_wait + 5,
        .opcodes = fun_data_asyncio_event_Event_wait + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_event_Event_wait fun_data_asyncio_event_Event_wait[0]
#endif

static const mp_raw_code_t *const children_asyncio_event_Event[] = {
    (const mp_raw_code_t *)&proto_fun_asyncio_event_Event___init__,
    (const mp_raw_code_t *)&proto_fun_asyncio_event_Event_is_set,
    (const mp_raw_code_t *)&proto_fun_asyncio_event_Event_set,
    (const mp_raw_code_t *)&proto_fun_asyncio_event_Event_clear,
    (const mp_raw_code_t *)&proto_fun_asyncio_event_Event_wait,
};

static const mp_raw_code_truncated_t proto_fun_asyncio_event_Event = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_event_Event,
    .children = (void *)&children_asyncio_event_Event,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 33,
    .n_children = 5,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_asyncio_event_Event + 3,
        .line_info_top = fun_data_asyncio_event_Event + 3,
        .opcodes = fun_data_asyncio_event_Event + 3,
    },
    #endif
    #endif
};

// child of asyncio_event__lt_module_gt_
// frozen bytecode for file asyncio/event.py, scope asyncio_event_ThreadSafeFlag
static const byte fun_data_asyncio_event_ThreadSafeFlag[33] = {
    0x00,0x02, // prelude
    0x06, // names: ThreadSafeFlag
     // code info
    0x11,0x1a, // LOAD_NAME '__name__'
    0x16,0x1b, // STORE_NAME '__module__'
    0x10,0x06, // LOAD_CONST_STRING 'ThreadSafeFlag'
    0x16,0x1c, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x08, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x16, // STORE_NAME 'ioctl'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0d, // STORE_NAME 'set'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x12, // STORE_NAME 'clear'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x13, // STORE_NAME 'wait'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_event_ThreadSafeFlag
// frozen bytecode for file asyncio/event.py, scope asyncio_event_ThreadSafeFlag___init__
static const byte fun_data_asyncio_event_ThreadSafeFlag___init__[10] = {
    0x11,0x04, // prelude
    0x08,0x1d, // names: __init__, self
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x09, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_event_ThreadSafeFlag___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_event_ThreadSafeFlag___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 10,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 8,
        .line_info = fun_data_asyncio_event_ThreadSafeFlag___init__ + 4,
        .line_info_top = fun_data_asyncio_event_ThreadSafeFlag___init__ + 4,
        .opcodes = fun_data_asyncio_event_ThreadSafeFlag___init__ + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_event_ThreadSafeFlag___init__ fun_data_asyncio_event_ThreadSafeFlag___init__[0]
#endif

// child of asyncio_event_ThreadSafeFlag
// frozen bytecode for file asyncio/event.py, scope asyncio_event_ThreadSafeFlag_ioctl
static const byte fun_data_asyncio_event_ThreadSafeFlag_ioctl[19] = {
    0x23,0x08, // prelude
    0x16,0x1d,0x1e,0x1f, // names: ioctl, self, req, flags
     // code info
    0xb1, // LOAD_FAST 1
    0x83, // LOAD_CONST_SMALL_INT 3
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'state'
    0xb2, // LOAD_FAST 2
    0xf4, // BINARY_OP 29 __mul__
    0x63, // RETURN_VALUE
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_event_ThreadSafeFlag_ioctl = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_event_ThreadSafeFlag_ioctl,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 19,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 22,
        .line_info = fun_data_asyncio_event_ThreadSafeFlag_ioctl + 6,
        .line_info_top = fun_data_asyncio_event_ThreadSafeFlag_ioctl + 6,
        .opcodes = fun_data_asyncio_event_ThreadSafeFlag_ioctl + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_event_ThreadSafeFlag_ioctl fun_data_asyncio_event_ThreadSafeFlag_ioctl[0]
#endif

// child of asyncio_event_ThreadSafeFlag
// frozen bytecode for file asyncio/event.py, scope asyncio_event_ThreadSafeFlag_set
static const byte fun_data_asyncio_event_ThreadSafeFlag_set[10] = {
    0x11,0x04, // prelude
    0x0d,0x1d, // names: set, self
     // code info
    0x81, // LOAD_CONST_SMALL_INT 1
    0xb0, // LOAD_FAST 0
    0x18,0x09, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_event_ThreadSafeFlag_set = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_event_ThreadSafeFlag_set,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 10,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_asyncio_event_ThreadSafeFlag_set + 4,
        .line_info_top = fun_data_asyncio_event_ThreadSafeFlag_set + 4,
        .opcodes = fun_data_asyncio_event_ThreadSafeFlag_set + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_event_ThreadSafeFlag_set fun_data_asyncio_event_ThreadSafeFlag_set[0]
#endif

// child of asyncio_event_ThreadSafeFlag
// frozen bytecode for file asyncio/event.py, scope asyncio_event_ThreadSafeFlag_clear
static const byte fun_data_asyncio_event_ThreadSafeFlag_clear[10] = {
    0x11,0x04, // prelude
    0x12,0x1d, // names: clear, self
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x09, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_event_ThreadSafeFlag_clear = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_event_ThreadSafeFlag_clear,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 10,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 18,
        .line_info = fun_data_asyncio_event_ThreadSafeFlag_clear + 4,
        .line_info_top = fun_data_asyncio_event_ThreadSafeFlag_clear + 4,
        .opcodes = fun_data_asyncio_event_ThreadSafeFlag_clear + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_event_ThreadSafeFlag_clear fun_data_asyncio_event_ThreadSafeFlag_clear[0]
#endif

// child of asyncio_event_ThreadSafeFlag
// frozen bytecode for file asyncio/event.py, scope asyncio_event_ThreadSafeFlag_wait
static const byte fun_data_asyncio_event_ThreadSafeFlag_wait[27] = {
    0x99,0x40,0x04, // prelude
    0x13,0x1d, // names: wait, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'state'
    0x43,0x4b, // POP_JUMP_IF_TRUE 11
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x17, // LOAD_ATTR '_io_queue'
    0x14,0x18, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x09, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_event_ThreadSafeFlag_wait = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_event_ThreadSafeFlag_wait,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 27,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 19,
        .line_info = fun_data_asyncio_event_ThreadSafeFlag_wait + 5,
        .line_info_top = fun_data_asyncio_event_ThreadSafeFlag_wait + 5,
        .opcodes = fun_data_asyncio_event_ThreadSafeFlag_wait + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_event_ThreadSafeFlag_wait fun_data_asyncio_event_ThreadSafeFlag_wait[0]
#endif

static const mp_raw_code_t *const children_asyncio_event_ThreadSafeFlag[] = {
    (const mp_raw_code_t *)&proto_fun_asyncio_event_ThreadSafeFlag___init__,
    (const mp_raw_code_t *)&proto_fun_asyncio_event_ThreadSafeFlag_ioctl,
    (const mp_raw_code_t *)&proto_fun_asyncio_event_ThreadSafeFlag_set,
    (const mp_raw_code_t *)&proto_fun_asyncio_event_ThreadSafeFlag_clear,
    (const mp_raw_code_t *)&proto_fun_asyncio_event_ThreadSafeFlag_wait,
};

static const mp_raw_code_truncated_t proto_fun_asyncio_event_ThreadSafeFlag = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_event_ThreadSafeFlag,
    .children = (void *)&children_asyncio_event_ThreadSafeFlag,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 33,
    .n_children = 5,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_asyncio_event_ThreadSafeFlag + 3,
        .line_info_top = fun_data_asyncio_event_ThreadSafeFlag + 3,
        .opcodes = fun_data_asyncio_event_ThreadSafeFlag + 3,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_asyncio_event__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_asyncio_event_Event,
    (const mp_raw_code_t *)&proto_fun_asyncio_event_ThreadSafeFlag,
};

static const mp_raw_code_truncated_t proto_fun_asyncio_event__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_event__lt_module_gt_,
    .children = (void *)&children_asyncio_event__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 59,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_asyncio_event__lt_module_gt_ + 3,
        .line_info_top = fun_data_asyncio_event__lt_module_gt_ + 3,
        .opcodes = fun_data_asyncio_event__lt_module_gt_ + 3,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_asyncio_event[32] = {
    MP_QSTR_asyncio_slash_event_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_core,
    MP_QSTR_,
    MP_QSTR_Event,
    MP_QSTR_io,
    MP_QSTR_ThreadSafeFlag,
    MP_QSTR_IOBase,
    MP_QSTR___init__,
    MP_QSTR_state,
    MP_QSTR_TaskQueue,
    MP_QSTR_waiting,
    MP_QSTR_is_set,
    MP_QSTR_set,
    MP_QSTR__task_queue,
    MP_QSTR_push,
    MP_QSTR_pop,
    MP_QSTR_peek,
    MP_QSTR_clear,
    MP_QSTR_wait,
    MP_QSTR_cur_task,
    MP_QSTR_data,
    MP_QSTR_ioctl,
    MP_QSTR__io_queue,
    MP_QSTR_queue_read,
    MP_QSTR_ImportError,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_self,
    MP_QSTR_req,
    MP_QSTR_flags,
};

static const mp_frozen_module_t frozen_module_asyncio_event = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_asyncio_event,
        .obj_table = NULL,
    },
    .proto_fun = &proto_fun_asyncio_event__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module asyncio_funcs
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/asyncio/funcs.mpy
// - frozen file name: asyncio/funcs.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs__lt_module_gt_
static const byte fun_data_asyncio_funcs__lt_module_gt_[56] = {
    0x18,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x02, // LOAD_CONST_STRING 'core'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME ''
    0x1c,0x02, // IMPORT_FROM 'core'
    0x16,0x02, // STORE_NAME 'core'
    0x59, // POP_TOP
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x07, // STORE_NAME '_run'
    0x11,0x02, // LOAD_NAME 'core'
    0x13,0x04, // LOAD_ATTR 'sleep'
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x0b, // STORE_NAME 'wait_for'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x12, // STORE_NAME 'wait_for_ms'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x03, // MAKE_FUNCTION 3
    0x10,0x05, // LOAD_CONST_STRING '_Remove'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x05, // STORE_NAME '_Remove'
    0x53, // LOAD_NULL
    0x2c,0x00, // BUILD_MAP 0
    0x50, // LOAD_CONST_FALSE
    0x10,0x06, // LOAD_CONST_STRING 'return_exceptions'
    0x62, // STORE_MAP
    0x33,0x04, // MAKE_FUNCTION_DEFARGS 4
    0x16,0x14, // STORE_NAME 'gather'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_funcs__lt_module_gt_
// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs__run
static const byte fun_data_asyncio_funcs__run[66] = {
    0xd2,0x42,0x06, // prelude
    0x07,0x1b,0x1c, // names: _run, waiter, aw
     // code info
    0x48,0x09, // SETUP_EXCEPT 9
    0xb1, // LOAD_FAST 1
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0xc2, // STORE_FAST 2
    0x52, // LOAD_CONST_TRUE
    0xc3, // STORE_FAST 3
    0x4a,0x16, // POP_EXCEPT_JUMP 22
    0x57, // DUP_TOP
    0x12,0x1d, // LOAD_GLOBAL 'BaseException'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0xc4, // STORE_FAST 4
    0x49,0x05, // SETUP_FINALLY 5
    0x51, // LOAD_CONST_NONE
    0xc2, // STORE_FAST 2
    0xb4, // LOAD_FAST 4
    0xc3, // STORE_FAST 3
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc4, // STORE_FAST 4
    0x28,0x04, // DELETE_FAST 4
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'data'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0xb0, // LOAD_FAST 0
    0x14,0x09, // LOAD_METHOD 'cancel'
    0x36,0x00, // CALL_METHOD 0
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x0a, // LOAD_METHOD 'CancelledError'
    0xb3, // LOAD_FAST 3
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0xb0, // LOAD_FAST 0
    0x18,0x08, // STORE_ATTR 'data'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_funcs__run = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_funcs__run,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 66,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 2,
        .scope_flags = 1,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_asyncio_funcs__run + 6,
        .line_info_top = fun_data_asyncio_funcs__run + 6,
        .opcodes = fun_data_asyncio_funcs__run + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_funcs__run fun_data_asyncio_funcs__run[0]
#endif

// child of asyncio_funcs__lt_module_gt_
// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs_wait_for
static const byte fun_data_asyncio_funcs_wait_for[119] = {
    0xdb,0x43,0x08, // prelude
    0x0b,0x1c,0x1e,0x04, // names: wait_for, aw, timeout, sleep
     // code info
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x0c, // LOAD_METHOD '_promote_to_task'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0xc0, // STORE_FAST 0
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb0, // LOAD_FAST 0
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x63, // RETURN_VALUE
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x0d, // LOAD_METHOD 'create_task'
    0x12,0x07, // LOAD_GLOBAL '_run'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x0e, // LOAD_ATTR 'cur_task'
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0x48,0x0a, // SETUP_EXCEPT 10
    0xb2, // LOAD_FAST 2
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x4a,0x32, // POP_EXCEPT_JUMP 50
    0x57, // DUP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x0a, // LOAD_ATTR 'CancelledError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x69, // POP_JUMP_IF_FALSE 41
    0xc4, // STORE_FAST 4
    0x49,0x1f, // SETUP_FINALLY 31
    0xb4, // LOAD_FAST 4
    0x13,0x0f, // LOAD_ATTR 'value'
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0xb3, // LOAD_FAST 3
    0x14,0x09, // LOAD_METHOD 'cancel'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb4, // LOAD_FAST 4
    0x65, // RAISE_OBJ
    0xb5, // LOAD_FAST 5
    0x52, // LOAD_CONST_TRUE
    0xde, // BINARY_OP 7 <is>
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0xb4, // LOAD_FAST 4
    0x13,0x10, // LOAD_ATTR 'args'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
    0xb5, // LOAD_FAST 5
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc4, // STORE_FAST 4
    0x28,0x04, // DELETE_FAST 4
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xb3, // LOAD_FAST 3
    0x14,0x09, // LOAD_METHOD 'cancel'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x11, // LOAD_ATTR 'TimeoutError'
    0x65, // RAISE_OBJ
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_funcs_wait_for = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_funcs_wait_for,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 119,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 2,
        .scope_flags = 1,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_asyncio_funcs_wait_for + 7,
        .line_info_top = fun_data_asyncio_funcs_wait_for + 7,
        .opcodes = fun_data_asyncio_funcs_wait_for + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_funcs_wait_for fun_data_asyncio_funcs_wait_for[0]
#endif

// child of asyncio_funcs__lt_module_gt_
// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs_wait_for_ms
static const byte fun_data_asyncio_funcs_wait_for_ms[16] = {
    0x2a,0x06, // prelude
    0x12,0x1c,0x1e, // names: wait_for_ms, aw, timeout
     // code info
    0x12,0x0b, // LOAD_GLOBAL 'wait_for'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x13, // LOAD_ATTR 'sleep_ms'
    0x34,0x03, // CALL_FUNCTION 3
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_funcs_wait_for_ms = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_funcs_wait_for_ms,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 16,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 18,
        .line_info = fun_data_asyncio_funcs_wait_for_ms + 5,
        .line_info_top = fun_data_asyncio_funcs_wait_for_ms + 5,
        .opcodes = fun_data_asyncio_funcs_wait_for_ms + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_funcs_wait_for_ms fun_data_asyncio_funcs_wait_for_ms[0]
#endif

// child of asyncio_funcs__lt_module_gt_
// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs__Remove
static const byte fun_data_asyncio_funcs__Remove[21] = {
    0x08,0x02, // prelude
    0x05, // names: _Remove
     // code info
    0x11,0x1f, // LOAD_NAME '__name__'
    0x16,0x20, // STORE_NAME '__module__'
    0x10,0x05, // LOAD_CONST_STRING '_Remove'
    0x16,0x21, // STORE_NAME '__qualname__'
    0x11,0x22, // LOAD_NAME 'staticmethod'
    0x32,0x00, // MAKE_FUNCTION 0
    0x34,0x01, // CALL_FUNCTION 1
    0x16,0x16, // STORE_NAME 'remove'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_funcs__Remove
// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs__Remove_remove
static const byte fun_data_asyncio_funcs__Remove_remove[6] = {
    0x09,0x04, // prelude
    0x16,0x28, // names: remove, t
     // code info
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_funcs__Remove_remove = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_funcs__Remove_remove,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 6,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 22,
        .line_info = fun_data_asyncio_funcs__Remove_remove + 4,
        .line_info_top = fun_data_asyncio_funcs__Remove_remove + 4,
        .opcodes = fun_data_asyncio_funcs__Remove_remove + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_funcs__Remove_remove fun_data_asyncio_funcs__Remove_remove[0]
#endif

static const mp_raw_code_t *const children_asyncio_funcs__Remove[] = {
    (const mp_raw_code_t *)&proto_fun_asyncio_funcs__Remove_remove,
};

static const mp_raw_code_truncated_t proto_fun_asyncio_funcs__Remove = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_funcs__Remove,
    .children = (void *)&children_asyncio_funcs__Remove,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 21,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 5,
        .line_info = fun_data_asyncio_funcs__Remove + 3,
        .line_info_top = fun_data_asyncio_funcs__Remove + 3,
        .opcodes = fun_data_asyncio_funcs__Remove + 3,
    },
    #endif
    #endif
};

// child of asyncio_funcs__lt_module_gt_
// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs_gather
static const byte fun_data_asyncio_funcs_gather[281] = {
    0xf0,0xca,0x80,0xc0,0x40,0x85,0x01, // prelude
    0x14,0x06, // names: gather, return_exceptions
    0x00,0x07,0x08, // code info
    0xb0, // LOAD_FAST 0
    0xb7, // LOAD_FAST 7
    0xb8, // LOAD_FAST 8
    0x20,0x00,0x03, // MAKE_CLOSURE 0
    0xc2, // STORE_FAST 2
    0x32,0x01, // MAKE_FUNCTION 1
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x27,0x07, // STORE_DEREF 7
    0x12,0x23, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0xc4,0x80, // JUMP 68
    0x57, // DUP_TOP
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x15, // LOAD_ATTR 'state'
    0x52, // LOAD_CONST_TRUE
    0xde, // BINARY_OP 7 <is>
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x18,0x15, // STORE_ATTR 'state'
    0x25,0x07, // LOAD_DEREF 7
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x27,0x07, // STORE_DEREF 7
    0x42,0x69, // JUMP 41
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x15, // LOAD_ATTR 'state'
    0x43,0x5b, // POP_JUMP_IF_TRUE 27
    0x12,0x24, // LOAD_GLOBAL 'isinstance'
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x08, // LOAD_ATTR 'data'
    0x12,0x25, // LOAD_GLOBAL 'StopIteration'
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x4c, // POP_JUMP_IF_TRUE 12
    0x25,0x00, // LOAD_DEREF 0
    0x43,0x48, // POP_JUMP_IF_TRUE 8
    0x12,0x23, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0xd1, // UNARY_OP 1 __neg__
    0x27,0x07, // STORE_DEREF 7
    0x42,0x47, // JUMP 7
    0x12,0x26, // LOAD_GLOBAL 'RuntimeError'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0xb6,0x7f, // POP_JUMP_IF_TRUE -74
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x0e, // LOAD_ATTR 'cur_task'
    0x27,0x08, // STORE_DEREF 8
    0x50, // LOAD_CONST_FALSE
    0xc5, // STORE_FAST 5
    0x25,0x07, // LOAD_DEREF 7
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x66, // POP_JUMP_IF_FALSE 38
    0x12,0x05, // LOAD_GLOBAL '_Remove'
    0x25,0x08, // LOAD_DEREF 8
    0x18,0x08, // STORE_ATTR 'data'
    0x48,0x05, // SETUP_EXCEPT 5
    0x51, // LOAD_CONST_NONE
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x4a,0x19, // POP_EXCEPT_JUMP 25
    0x57, // DUP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x0a, // LOAD_ATTR 'CancelledError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0xc6, // STORE_FAST 6
    0x49,0x06, // SETUP_FINALLY 6
    0x52, // LOAD_CONST_TRUE
    0xc5, // STORE_FAST 5
    0xb6, // LOAD_FAST 6
    0x27,0x07, // STORE_DEREF 7
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc6, // STORE_FAST 6
    0x28,0x06, // DELETE_FAST 6
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x12,0x23, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0xda,0x80, // JUMP 90
    0x57, // DUP_TOP
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x15, // LOAD_ATTR 'state'
    0xb2, // LOAD_FAST 2
    0xde, // BINARY_OP 7 <is>
    0x44,0x53, // POP_JUMP_IF_FALSE 19
    0x52, // LOAD_CONST_TRUE
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x18,0x15, // STORE_ATTR 'state'
    0xb5, // LOAD_FAST 5
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x14,0x09, // LOAD_METHOD 'cancel'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x42,0x7a, // JUMP 58
    0x12,0x24, // LOAD_GLOBAL 'isinstance'
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x08, // LOAD_ATTR 'data'
    0x12,0x25, // LOAD_GLOBAL 'StopIteration'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x08, // LOAD_ATTR 'data'
    0x13,0x0f, // LOAD_ATTR 'value'
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x56, // STORE_SUBSCR
    0x42,0x61, // JUMP 33
    0x25,0x00, // LOAD_DEREF 0
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x08, // LOAD_ATTR 'data'
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x56, // STORE_SUBSCR
    0x42,0x53, // JUMP 19
    0x12,0x24, // LOAD_GLOBAL 'isinstance'
    0x25,0x07, // LOAD_DEREF 7
    0x12,0x27, // LOAD_GLOBAL 'int'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0x13,0x08, // LOAD_ATTR 'data'
    0x27,0x07, // STORE_DEREF 7
    0x42,0x40, // JUMP 0
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0xa0,0x7f, // POP_JUMP_IF_TRUE -96
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x25,0x07, // LOAD_DEREF 7
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x25,0x07, // LOAD_DEREF 7
    0x65, // RAISE_OBJ
    0xb3, // LOAD_FAST 3
    0x63, // RETURN_VALUE
};
// child of asyncio_funcs_gather
// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs_gather_done
static const byte fun_data_asyncio_funcs_gather_done[64] = {
    0xb9,0x04,0x0c, // prelude
    0x17,0x29,0x29,0x29,0x28,0x2a, // names: done, *, *, *, t, er
     // code info
    0x25,0x02, // LOAD_DEREF 2
    0x13,0x08, // LOAD_ATTR 'data'
    0x12,0x05, // LOAD_GLOBAL '_Remove'
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x25,0x00, // LOAD_DEREF 0
    0x43,0x4e, // POP_JUMP_IF_TRUE 14
    0x12,0x24, // LOAD_GLOBAL 'isinstance'
    0xb4, // LOAD_FAST 4
    0x12,0x25, // LOAD_GLOBAL 'StopIteration'
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x45, // POP_JUMP_IF_TRUE 5
    0xb4, // LOAD_FAST 4
    0x27,0x01, // STORE_DEREF 1
    0x42,0x4c, // JUMP 12
    0x25,0x01, // LOAD_DEREF 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe6, // BINARY_OP 15 __isub__
    0x27,0x01, // STORE_DEREF 1
    0x25,0x01, // LOAD_DEREF 1
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x18, // LOAD_ATTR '_task_queue'
    0x14,0x19, // LOAD_METHOD 'push'
    0x25,0x02, // LOAD_DEREF 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_funcs_gather_done = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_funcs_gather_done,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 64,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 5,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 23,
        .line_info = fun_data_asyncio_funcs_gather_done + 9,
        .line_info_top = fun_data_asyncio_funcs_gather_done + 9,
        .opcodes = fun_data_asyncio_funcs_gather_done + 9,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_funcs_gather_done fun_data_asyncio_funcs_gather_done[0]
#endif

// child of asyncio_funcs_gather
// frozen bytecode for file asyncio/funcs.py, scope asyncio_funcs_gather__lt_listcomp_gt_
static const byte fun_data_asyncio_funcs_gather__lt_listcomp_gt_[23] = {
    0x49,0x04, // prelude
    0x1a,0x29, // names: <listcomp>, *
     // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x0c, // FOR_ITER 12
    0xc1, // STORE_FAST 1
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x0c, // LOAD_METHOD '_promote_to_task'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x2f,0x14, // STORE_COMP 20
    0x42,0x32, // JUMP -14
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_funcs_gather__lt_listcomp_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_funcs_gather__lt_listcomp_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 23,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 26,
        .line_info = fun_data_asyncio_funcs_gather__lt_listcomp_gt_ + 4,
        .line_info_top = fun_data_asyncio_funcs_gather__lt_listcomp_gt_ + 4,
        .opcodes = fun_data_asyncio_funcs_gather__lt_listcomp_gt_ + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_funcs_gather__lt_listcomp_gt_ fun_data_asyncio_funcs_gather__lt_listcomp_gt_[0]
#endif

static const mp_raw_code_t *const children_asyncio_funcs_gather[] = {
    (const mp_raw_code_t *)&proto_fun_asyncio_funcs_gather_done,
    (const mp_raw_code_t *)&proto_fun_asyncio_funcs_gather__lt_listcomp_gt_,
};

static const mp_raw_code_truncated_t proto_fun_asyncio_funcs_gather = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_funcs_gather,
    .children = (void *)&children_asyncio_funcs_gather,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 281,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 15,
        .n_exc_stack = 2,
        .scope_flags = 13,
        .n_pos_args = 0,
        .n_kwonly_args = 1,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 20,
        .line_info = fun_data_asyncio_funcs_gather + 9,
        .line_info_top = fun_data_asyncio_funcs_gather + 9,
        .opcodes = fun_data_asyncio_funcs_gather + 12,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_asyncio_funcs__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_asyncio_funcs__run,
    (const mp_raw_code_t *)&proto_fun_asyncio_funcs_wait_for,
    (const mp_raw_code_t *)&proto_fun_asyncio_funcs_wait_for_ms,
    (const mp_raw_code_t *)&proto_fun_asyncio_funcs__Remove,
    (const mp_raw_code_t *)&proto_fun_asyncio_funcs_gather,
};

static const mp_raw_code_truncated_t proto_fun_asyncio_funcs__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_funcs__lt_module_gt_,
    .children = (void *)&children_asyncio_funcs__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 56,
    .n_children = 5,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_asyncio_funcs__lt_module_gt_ + 3,
        .line_info_top = fun_data_asyncio_funcs__lt_module_gt_ + 3,
        .opcodes = fun_data_asyncio_funcs__lt_module_gt_ + 3,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_asyncio_funcs[43] = {
    MP_QSTR_asyncio_slash_funcs_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_core,
    MP_QSTR_,
    MP_QSTR_sleep,
    MP_QSTR__Remove,
    MP_QSTR_return_exceptions,
    MP_QSTR__run,
    MP_QSTR_data,
    MP_QSTR_cancel,
    MP_QSTR_CancelledError,
    MP_QSTR_wait_for,
    MP_QSTR__promote_to_task,
    MP_QSTR_create_task,
    MP_QSTR_cur_task,
    MP_QSTR_value,
    MP_QSTR_args,
    MP_QSTR_TimeoutError,
    MP_QSTR_wait_for_ms,
    MP_QSTR_sleep_ms,
    MP_QSTR_gather,
    MP_QSTR_state,
    MP_QSTR_remove,
    MP_QSTR_done,
    MP_QSTR__task_queue,
    MP_QSTR_push,
    MP_QSTR__lt_listcomp_gt_,
    MP_QSTR_waiter,
    MP_QSTR_aw,
    MP_QSTR_BaseException,
    MP_QSTR_timeout,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_staticmethod,
    MP_QSTR_len,
    MP_QSTR_isinstance,
    MP_QSTR_StopIteration,
    MP_QSTR_RuntimeError,
    MP_QSTR_int,
    MP_QSTR_t,
    MP_QSTR__star_,
    MP_QSTR_er,
};

// constants

// constant table
static const mp_rom_obj_t const_obj_table_data_asyncio_funcs[1] = {
    MP_ROM_QSTR(MP_QSTR_can_squot_t_space_gather),
};

static const mp_frozen_module_t frozen_module_asyncio_funcs = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_asyncio_funcs,
        .obj_table = (mp_obj_t *)&const_obj_table_data_asyncio_funcs,
    },
    .proto_fun = &proto_fun_asyncio_funcs__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module asyncio_lock
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/asyncio/lock.mpy
// - frozen file name: asyncio/lock.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file asyncio/lock.py, scope asyncio_lock__lt_module_gt_
static const byte fun_data_asyncio_lock__lt_module_gt_[26] = {
    0x10,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x02, // LOAD_CONST_STRING 'core'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME ''
    0x1c,0x02, // IMPORT_FROM 'core'
    0x16,0x02, // STORE_NAME 'core'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'Lock'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x04, // STORE_NAME 'Lock'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_lock__lt_module_gt_
// frozen bytecode for file asyncio/lock.py, scope asyncio_lock_Lock
static const byte fun_data_asyncio_lock_Lock[37] = {
    0x00,0x02, // prelude
    0x04, // names: Lock
     // code info
    0x11,0x15, // LOAD_NAME '__name__'
    0x16,0x16, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'Lock'
    0x16,0x17, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x05, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x09, // STORE_NAME 'locked'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0a, // STORE_NAME 'release'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x0f, // STORE_NAME 'acquire'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x13, // STORE_NAME '__aenter__'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x14, // STORE_NAME '__aexit__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_lock_Lock
// frozen bytecode for file asyncio/lock.py, scope asyncio_lock_Lock___init__
static const byte fun_data_asyncio_lock_Lock___init__[19] = {
    0x11,0x04, // prelude
    0x05,0x18, // names: __init__, self
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x06, // STORE_ATTR 'state'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x07, // LOAD_METHOD 'TaskQueue'
    0x36,0x00, // CALL_METHOD 0
    0xb0, // LOAD_FAST 0
    0x18,0x08, // STORE_ATTR 'waiting'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_lock_Lock___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_lock_Lock___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 19,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 5,
        .line_info = fun_data_asyncio_lock_Lock___init__ + 4,
        .line_info_top = fun_data_asyncio_lock_Lock___init__ + 4,
        .opcodes = fun_data_asyncio_lock_Lock___init__ + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_lock_Lock___init__ fun_data_asyncio_lock_Lock___init__[0]
#endif

// child of asyncio_lock_Lock
// frozen bytecode for file asyncio/lock.py, scope asyncio_lock_Lock_locked
static const byte fun_data_asyncio_lock_Lock_locked[10] = {
    0x11,0x04, // prelude
    0x09,0x18, // names: locked, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x06, // LOAD_ATTR 'state'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xd9, // BINARY_OP 2 __eq__
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_lock_Lock_locked = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_lock_Lock_locked,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 10,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_asyncio_lock_Lock_locked + 4,
        .line_info_top = fun_data_asyncio_lock_Lock_locked + 4,
        .opcodes = fun_data_asyncio_lock_Lock_locked + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_lock_Lock_locked fun_data_asyncio_lock_Lock_locked[0]
#endif

// child of asyncio_lock_Lock
// frozen bytecode for file asyncio/lock.py, scope asyncio_lock_Lock_release
static const byte fun_data_asyncio_lock_Lock_release[57] = {
    0x19,0x04, // prelude
    0x0a,0x18, // names: release, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x06, // LOAD_ATTR 'state'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x19, // LOAD_GLOBAL 'RuntimeError'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'waiting'
    0x14,0x0b, // LOAD_METHOD 'peek'
    0x36,0x00, // CALL_METHOD 0
    0x44,0x58, // POP_JUMP_IF_FALSE 24
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'waiting'
    0x14,0x0c, // LOAD_METHOD 'pop'
    0x36,0x00, // CALL_METHOD 0
    0xb0, // LOAD_FAST 0
    0x18,0x06, // STORE_ATTR 'state'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x0d, // LOAD_ATTR '_task_queue'
    0x14,0x0e, // LOAD_METHOD 'push'
    0xb0, // LOAD_FAST 0
    0x13,0x06, // LOAD_ATTR 'state'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x44, // JUMP 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x06, // STORE_ATTR 'state'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_lock_Lock_release = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_lock_Lock_release,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 57,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_asyncio_lock_Lock_release + 4,
        .line_info_top = fun_data_asyncio_lock_Lock_release + 4,
        .opcodes = fun_data_asyncio_lock_Lock_release + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_lock_Lock_release fun_data_asyncio_lock_Lock_release[0]
#endif

// child of asyncio_lock_Lock
// frozen bytecode for file asyncio/lock.py, scope asyncio_lock_Lock_acquire
static const byte fun_data_asyncio_lock_Lock_acquire[88] = {
    0xb9,0x42,0x04, // prelude
    0x0f,0x18, // names: acquire, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x06, // LOAD_ATTR 'state'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdc, // BINARY_OP 5 __ne__
    0x44,0xc5,0x80, // POP_JUMP_IF_FALSE 69
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'waiting'
    0x14,0x0e, // LOAD_METHOD 'push'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x10, // LOAD_ATTR 'cur_task'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'waiting'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x10, // LOAD_ATTR 'cur_task'
    0x18,0x11, // STORE_ATTR 'data'
    0x48,0x05, // SETUP_EXCEPT 5
    0x51, // LOAD_CONST_NONE
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x4a,0x29, // POP_EXCEPT_JUMP 41
    0x57, // DUP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x12, // LOAD_ATTR 'CancelledError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x60, // POP_JUMP_IF_FALSE 32
    0xc1, // STORE_FAST 1
    0x49,0x16, // SETUP_FINALLY 22
    0xb0, // LOAD_FAST 0
    0x13,0x06, // LOAD_ATTR 'state'
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x10, // LOAD_ATTR 'cur_task'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x81, // LOAD_CONST_SMALL_INT 1
    0xb0, // LOAD_FAST 0
    0x18,0x06, // STORE_ATTR 'state'
    0xb0, // LOAD_FAST 0
    0x14,0x0a, // LOAD_METHOD 'release'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0xc1, // STORE_FAST 1
    0x28,0x01, // DELETE_FAST 1
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x81, // LOAD_CONST_SMALL_INT 1
    0xb0, // LOAD_FAST 0
    0x18,0x06, // STORE_ATTR 'state'
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_lock_Lock_acquire = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_lock_Lock_acquire,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 88,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 2,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 15,
        .line_info = fun_data_asyncio_lock_Lock_acquire + 5,
        .line_info_top = fun_data_asyncio_lock_Lock_acquire + 5,
        .opcodes = fun_data_asyncio_lock_Lock_acquire + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_lock_Lock_acquire fun_data_asyncio_lock_Lock_acquire[0]
#endif

// child of asyncio_lock_Lock
// frozen bytecode for file asyncio/lock.py, scope asyncio_lock_Lock___aenter__
static const byte fun_data_asyncio_lock_Lock___aenter__[14] = {
    0x91,0x40,0x04, // prelude
    0x13,0x18, // names: __aenter__, self
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x0f, // LOAD_METHOD 'acquire'
    0x36,0x00, // CALL_METHOD 0
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_lock_Lock___aenter__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_lock_Lock___aenter__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 14,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 19,
        .line_info = fun_data_asyncio_lock_Lock___aenter__ + 5,
        .line_info_top = fun_data_asyncio_lock_Lock___aenter__ + 5,
        .opcodes = fun_data_asyncio_lock_Lock___aenter__ + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_lock_Lock___aenter__ fun_data_asyncio_lock_Lock___aenter__[0]
#endif

// child of asyncio_lock_Lock
// frozen bytecode for file asyncio/lock.py, scope asyncio_lock_Lock___aexit__
static const byte fun_data_asyncio_lock_Lock___aexit__[14] = {
    0xa8,0x44,0x0a, // prelude
    0x14,0x18,0x1a,0x1b,0x1c, // names: __aexit__, self, exc_type, exc, tb
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x0a, // LOAD_METHOD 'release'
    0x36,0x00, // CALL_METHOD 0
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_lock_Lock___aexit__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_lock_Lock___aexit__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 14,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 20,
        .line_info = fun_data_asyncio_lock_Lock___aexit__ + 8,
        .line_info_top = fun_data_asyncio_lock_Lock___aexit__ + 8,
        .opcodes = fun_data_asyncio_lock_Lock___aexit__ + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_lock_Lock___aexit__ fun_data_asyncio_lock_Lock___aexit__[0]
#endif

static const mp_raw_code_t *const children_asyncio_lock_Lock[] = {
    (const mp_raw_code_t *)&proto_fun_asyncio_lock_Lock___init__,
    (const mp_raw_code_t *)&proto_fun_asyncio_lock_Lock_locked,
    (const mp_raw_code_t *)&proto_fun_asyncio_lock_Lock_release,
    (const mp_raw_code_t *)&proto_fun_asyncio_lock_Lock_acquire,
    (const mp_raw_code_t *)&proto_fun_asyncio_lock_Lock___aenter__,
    (const mp_raw_code_t *)&proto_fun_asyncio_lock_Lock___aexit__,
};

static const mp_raw_code_truncated_t proto_fun_asyncio_lock_Lock = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_lock_Lock,
    .children = (void *)&children_asyncio_lock_Lock,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 37,
    .n_children = 6,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_asyncio_lock_Lock + 3,
        .line_info_top = fun_data_asyncio_lock_Lock + 3,
        .opcodes = fun_data_asyncio_lock_Lock + 3,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_asyncio_lock__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_asyncio_lock_Lock,
};

static const mp_raw_code_truncated_t proto_fun_asyncio_lock__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_lock__lt_module_gt_,
    .children = (void *)&children_asyncio_lock__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 26,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_asyncio_lock__lt_module_gt_ + 3,
        .line_info_top = fun_data_asyncio_lock__lt_module_gt_ + 3,
        .opcodes = fun_data_asyncio_lock__lt_module_gt_ + 3,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_asyncio_lock[29] = {
    MP_QSTR_asyncio_slash_lock_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_core,
    MP_QSTR_,
    MP_QSTR_Lock,
    MP_QSTR___init__,
    MP_QSTR_state,
    MP_QSTR_TaskQueue,
    MP_QSTR_waiting,
    MP_QSTR_locked,
    MP_QSTR_release,
    MP_QSTR_peek,
    MP_QSTR_pop,
    MP_QSTR__task_queue,
    MP_QSTR_push,
    MP_QSTR_acquire,
    MP_QSTR_cur_task,
    MP_QSTR_data,
    MP_QSTR_CancelledError,
    MP_QSTR___aenter__,
    MP_QSTR___aexit__,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_self,
    MP_QSTR_RuntimeError,
    MP_QSTR_exc_type,
    MP_QSTR_exc,
    MP_QSTR_tb,
};

// constants

// constant table
static const mp_rom_obj_t const_obj_table_data_asyncio_lock[1] = {
    MP_ROM_QSTR(MP_QSTR_Lock_space_not_space_acquired),
};

static const mp_frozen_module_t frozen_module_asyncio_lock = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_asyncio_lock,
        .obj_table = (mp_obj_t *)&const_obj_table_data_asyncio_lock,
    },
    .proto_fun = &proto_fun_asyncio_lock__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module asyncio_stream
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/asyncio/stream.mpy
// - frozen file name: asyncio/stream.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file asyncio/stream.py, scope asyncio_stream__lt_module_gt_
static const byte fun_data_asyncio_stream__lt_module_gt_[90] = {
    0x10,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x02, // LOAD_CONST_STRING 'core'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME ''
    0x1c,0x02, // IMPORT_FROM 'core'
    0x16,0x02, // STORE_NAME 'core'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'Stream'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x04, // STORE_NAME 'Stream'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x16,0x3c, // STORE_NAME 'StreamReader'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x16,0x3d, // STORE_NAME 'StreamWriter'
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x0a, // STORE_NAME 'open_connection'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x05, // LOAD_CONST_STRING 'Server'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x05, // STORE_NAME 'Server'
    0x85, // LOAD_CONST_SMALL_INT 5
    0x51, // LOAD_CONST_NONE
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x03, // MAKE_FUNCTION_DEFARGS 3
    0x16,0x1a, // STORE_NAME 'start_server'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x04, // MAKE_FUNCTION_DEFARGS 4
    0x16,0x26, // STORE_NAME 'stream_awrite'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x13,0x06, // LOAD_ATTR 'wait_closed'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x18,0x07, // STORE_ATTR 'aclose'
    0x11,0x26, // LOAD_NAME 'stream_awrite'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x18,0x08, // STORE_ATTR 'awrite'
    0x11,0x26, // LOAD_NAME 'stream_awrite'
    0x11,0x04, // LOAD_NAME 'Stream'
    0x18,0x09, // STORE_ATTR 'awritestr'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_stream__lt_module_gt_
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream
static const byte fun_data_asyncio_stream_Stream[62] = {
    0x08,0x02, // prelude
    0x04, // names: Stream
     // code info
    0x11,0x3e, // LOAD_NAME '__name__'
    0x16,0x3f, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'Stream'
    0x16,0x40, // STORE_NAME '__qualname__'
    0x2c,0x00, // BUILD_MAP 0
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x29, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x2d, // STORE_NAME 'get_extra_info'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x2e, // STORE_NAME 'close'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x06, // STORE_NAME 'wait_closed'
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x04, // MAKE_FUNCTION_DEFARGS 4
    0x16,0x2f, // STORE_NAME 'read'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x31, // STORE_NAME 'readinto'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x32, // STORE_NAME 'readexactly'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x33, // STORE_NAME 'readline'
    0x32,0x08, // MAKE_FUNCTION 8
    0x16,0x27, // STORE_NAME 'write'
    0x32,0x09, // MAKE_FUNCTION 9
    0x16,0x28, // STORE_NAME 'drain'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream___init__
static const byte fun_data_asyncio_stream_Stream___init__[22] = {
    0xa3,0x01,0x08, // prelude
    0x29,0x46,0x2a,0x2b, // names: __init__, self, s, e
     // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x2a, // STORE_ATTR 's'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x2b, // STORE_ATTR 'e'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xb0, // LOAD_FAST 0
    0x18,0x2c, // STORE_ATTR 'out_buf'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Stream___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_stream_Stream___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 22,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 41,
        .line_info = fun_data_asyncio_stream_Stream___init__ + 7,
        .line_info_top = fun_data_asyncio_stream_Stream___init__ + 7,
        .opcodes = fun_data_asyncio_stream_Stream___init__ + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_Stream___init__ fun_data_asyncio_stream_Stream___init__[0]
#endif

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_get_extra_info
static const byte fun_data_asyncio_stream_Stream_get_extra_info[11] = {
    0x1a,0x06, // prelude
    0x2d,0x46,0x4c, // names: get_extra_info, self, v
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x2b, // LOAD_ATTR 'e'
    0xb1, // LOAD_FAST 1
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Stream_get_extra_info = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_stream_Stream_get_extra_info,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 11,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 45,
        .line_info = fun_data_asyncio_stream_Stream_get_extra_info + 5,
        .line_info_top = fun_data_asyncio_stream_Stream_get_extra_info + 5,
        .opcodes = fun_data_asyncio_stream_Stream_get_extra_info + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_Stream_get_extra_info fun_data_asyncio_stream_Stream_get_extra_info[0]
#endif

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_close
static const byte fun_data_asyncio_stream_Stream_close[6] = {
    0x09,0x04, // prelude
    0x2e,0x46, // names: close, self
     // code info
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Stream_close = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_stream_Stream_close,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 6,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 46,
        .line_info = fun_data_asyncio_stream_Stream_close + 4,
        .line_info_top = fun_data_asyncio_stream_Stream_close + 4,
        .opcodes = fun_data_asyncio_stream_Stream_close + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_Stream_close fun_data_asyncio_stream_Stream_close[0]
#endif

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_wait_closed
static const byte fun_data_asyncio_stream_Stream_wait_closed[15] = {
    0x91,0x40,0x04, // prelude
    0x06,0x46, // names: wait_closed, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x2a, // LOAD_ATTR 's'
    0x14,0x2e, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Stream_wait_closed = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_stream_Stream_wait_closed,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 15,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_asyncio_stream_Stream_wait_closed + 5,
        .line_info_top = fun_data_asyncio_stream_Stream_wait_closed + 5,
        .opcodes = fun_data_asyncio_stream_Stream_wait_closed + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_Stream_wait_closed fun_data_asyncio_stream_Stream_wait_closed[0]
#endif

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_read
static const byte fun_data_asyncio_stream_Stream_read[61] = {
    0xb2,0x41,0x06, // prelude
    0x2f,0x46,0x4d, // names: read, self, n
     // code info
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xc2, // STORE_FAST 2
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x18, // LOAD_ATTR '_io_queue'
    0x14,0x30, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x13,0x2a, // LOAD_ATTR 's'
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x2a, // LOAD_ATTR 's'
    0x14,0x2f, // LOAD_METHOD 'read'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x54, // POP_JUMP_IF_FALSE 20
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdb, // BINARY_OP 4 __ge__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb3, // LOAD_FAST 3
    0x63, // RETURN_VALUE
    0x12,0x4b, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xe5, // BINARY_OP 14 __iadd__
    0xc2, // STORE_FAST 2
    0x42,0x0e, // JUMP -50
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Stream_read = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_stream_Stream_read,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 61,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 47,
        .line_info = fun_data_asyncio_stream_Stream_read + 6,
        .line_info_top = fun_data_asyncio_stream_Stream_read + 6,
        .opcodes = fun_data_asyncio_stream_Stream_read + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_Stream_read fun_data_asyncio_stream_Stream_read[0]
#endif

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_readinto
static const byte fun_data_asyncio_stream_Stream_readinto[28] = {
    0xa2,0x40,0x06, // prelude
    0x31,0x46,0x47, // names: readinto, self, buf
     // code info
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x18, // LOAD_ATTR '_io_queue'
    0x14,0x30, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x13,0x2a, // LOAD_ATTR 's'
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x2a, // LOAD_ATTR 's'
    0x14,0x31, // LOAD_METHOD 'readinto'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Stream_readinto = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_stream_Stream_readinto,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 28,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 49,
        .line_info = fun_data_asyncio_stream_Stream_readinto + 6,
        .line_info_top = fun_data_asyncio_stream_Stream_readinto + 6,
        .opcodes = fun_data_asyncio_stream_Stream_readinto + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_Stream_readinto fun_data_asyncio_stream_Stream_readinto[0]
#endif

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_readexactly
static const byte fun_data_asyncio_stream_Stream_readexactly[66] = {
    0xb2,0x40,0x06, // prelude
    0x32,0x46,0x4d, // names: readexactly, self, n
     // code info
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xc2, // STORE_FAST 2
    0x42,0x72, // JUMP 50
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x18, // LOAD_ATTR '_io_queue'
    0x14,0x30, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x13,0x2a, // LOAD_ATTR 's'
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x2a, // LOAD_ATTR 's'
    0x14,0x2f, // LOAD_METHOD 'read'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x56, // POP_JUMP_IF_FALSE 22
    0x12,0x4b, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x4e, // LOAD_GLOBAL 'EOFError'
    0x65, // RAISE_OBJ
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xe5, // BINARY_OP 14 __iadd__
    0xc2, // STORE_FAST 2
    0xb1, // LOAD_FAST 1
    0x12,0x4b, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0xe6, // BINARY_OP 15 __isub__
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x43,0x0b, // POP_JUMP_IF_TRUE -53
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Stream_readexactly = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_stream_Stream_readexactly,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 66,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 50,
        .line_info = fun_data_asyncio_stream_Stream_readexactly + 6,
        .line_info_top = fun_data_asyncio_stream_Stream_readexactly + 6,
        .opcodes = fun_data_asyncio_stream_Stream_readexactly + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_Stream_readexactly fun_data_asyncio_stream_Stream_readexactly[0]
#endif

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_readline
static const byte fun_data_asyncio_stream_Stream_readline[56] = {
    0xa9,0x40,0x04, // prelude
    0x33,0x46, // names: readline, self
     // code info
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xc1, // STORE_FAST 1
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x18, // LOAD_ATTR '_io_queue'
    0x14,0x30, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x13,0x2a, // LOAD_ATTR 's'
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x2a, // LOAD_ATTR 's'
    0x14,0x33, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x42,0x50, // JUMP 16
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0xe5, // BINARY_OP 14 __iadd__
    0xc1, // STORE_FAST 1
    0xb2, // LOAD_FAST 2
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb1, // LOAD_FAST 1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0x8a, // LOAD_CONST_SMALL_INT 10
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
    0x42,0x12, // JUMP -46
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Stream_readline = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_stream_Stream_readline,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 56,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 51,
        .line_info = fun_data_asyncio_stream_Stream_readline + 5,
        .line_info_top = fun_data_asyncio_stream_Stream_readline + 5,
        .opcodes = fun_data_asyncio_stream_Stream_readline + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_Stream_readline fun_data_asyncio_stream_Stream_readline[0]
#endif

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_write
static const byte fun_data_asyncio_stream_Stream_write[54] = {
    0x2a,0x06, // prelude
    0x27,0x46,0x47, // names: write, self, buf
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x2c, // LOAD_ATTR 'out_buf'
    0x43,0x61, // POP_JUMP_IF_TRUE 33
    0xb0, // LOAD_FAST 0
    0x13,0x2a, // LOAD_ATTR 's'
    0x14,0x27, // LOAD_METHOD 'write'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x12,0x4b, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x2c, // LOAD_ATTR 'out_buf'
    0xb1, // LOAD_FAST 1
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x2c, // STORE_ATTR 'out_buf'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Stream_write = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_stream_Stream_write,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 54,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 39,
        .line_info = fun_data_asyncio_stream_Stream_write + 5,
        .line_info_top = fun_data_asyncio_stream_Stream_write + 5,
        .opcodes = fun_data_asyncio_stream_Stream_write + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_Stream_write fun_data_asyncio_stream_Stream_write[0]
#endif

// child of asyncio_stream_Stream
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Stream_drain
static const byte fun_data_asyncio_stream_Stream_drain[86] = {
    0xc1,0x40,0x04, // prelude
    0x28,0x46, // names: drain, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x2c, // LOAD_ATTR 'out_buf'
    0x43,0x4b, // POP_JUMP_IF_TRUE 11
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x23, // LOAD_METHOD 'sleep_ms'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x01, // CALL_METHOD 1
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x63, // RETURN_VALUE
    0x12,0x4a, // LOAD_GLOBAL 'memoryview'
    0xb0, // LOAD_FAST 0
    0x13,0x2c, // LOAD_ATTR 'out_buf'
    0x34,0x01, // CALL_FUNCTION 1
    0xc1, // STORE_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc2, // STORE_FAST 2
    0x42,0x65, // JUMP 37
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x18, // LOAD_ATTR '_io_queue'
    0x14,0x19, // LOAD_METHOD 'queue_write'
    0xb0, // LOAD_FAST 0
    0x13,0x2a, // LOAD_ATTR 's'
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x2a, // LOAD_ATTR 's'
    0x14,0x27, // LOAD_METHOD 'write'
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xe5, // BINARY_OP 14 __iadd__
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x12,0x4b, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x12, // POP_JUMP_IF_TRUE -46
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xb0, // LOAD_FAST 0
    0x18,0x2c, // STORE_ATTR 'out_buf'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Stream_drain = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_stream_Stream_drain,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 86,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 40,
        .line_info = fun_data_asyncio_stream_Stream_drain + 5,
        .line_info_top = fun_data_asyncio_stream_Stream_drain + 5,
        .opcodes = fun_data_asyncio_stream_Stream_drain + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_Stream_drain fun_data_asyncio_stream_Stream_drain[0]
#endif

static const mp_raw_code_t *const children_asyncio_stream_Stream[] = {
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Stream___init__,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Stream_get_extra_info,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Stream_close,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Stream_wait_closed,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Stream_read,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Stream_readinto,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Stream_readexactly,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Stream_readline,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Stream_write,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Stream_drain,
};

static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Stream = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_stream_Stream,
    .children = (void *)&children_asyncio_stream_Stream,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 62,
    .n_children = 10,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_asyncio_stream_Stream + 3,
        .line_info_top = fun_data_asyncio_stream_Stream + 3,
        .opcodes = fun_data_asyncio_stream_Stream + 3,
    },
    #endif
    #endif
};

// child of asyncio_stream__lt_module_gt_
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_open_connection
static const byte fun_data_asyncio_stream_open_connection[171] = {
    0x88,0xd6,0x01,0x0a, // prelude
    0x0a,0x41,0x42,0x12,0x16, // names: open_connection, host, port, ssl, server_hostname
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x0b, // LOAD_CONST_STRING 'EINPROGRESS'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x0c, // IMPORT_NAME 'errno'
    0x1c,0x0b, // IMPORT_FROM 'EINPROGRESS'
    0xc4, // STORE_FAST 4
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x0d, // IMPORT_NAME 'socket'
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x14,0x0e, // LOAD_METHOD 'getaddrinfo'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb5, // LOAD_FAST 5
    0x13,0x0f, // LOAD_ATTR 'SOCK_STREAM'
    0x36,0x04, // CALL_METHOD 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc6, // STORE_FAST 6
    0xb5, // LOAD_FAST 5
    0x14,0x0d, // LOAD_METHOD 'socket'
    0xb6, // LOAD_FAST 6
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb6, // LOAD_FAST 6
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb6, // LOAD_FAST 6
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x36,0x03, // CALL_METHOD 3
    0xc7, // STORE_FAST 7
    0xb7, // LOAD_FAST 7
    0x14,0x10, // LOAD_METHOD 'setblocking'
    0x50, // LOAD_CONST_FALSE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x48,0x0b, // SETUP_EXCEPT 11
    0xb7, // LOAD_FAST 7
    0x14,0x11, // LOAD_METHOD 'connect'
    0xb6, // LOAD_FAST 6
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0x1b, // POP_EXCEPT_JUMP 27
    0x57, // DUP_TOP
    0x12,0x43, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x54, // POP_JUMP_IF_FALSE 20
    0xc8, // STORE_FAST 8
    0x49,0x0a, // SETUP_FINALLY 10
    0xb8, // LOAD_FAST 8
    0x13,0x0c, // LOAD_ATTR 'errno'
    0xb4, // LOAD_FAST 4
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb8, // LOAD_FAST 8
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc8, // STORE_FAST 8
    0x28,0x08, // DELETE_FAST 8
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xb2, // LOAD_FAST 2
    0x44,0x6d, // POP_JUMP_IF_FALSE 45
    0xb2, // LOAD_FAST 2
    0x52, // LOAD_CONST_TRUE
    0xde, // BINARY_OP 7 <is>
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x12, // IMPORT_NAME 'ssl'
    0xc9, // STORE_FAST 9
    0xb9, // LOAD_FAST 9
    0x14,0x13, // LOAD_METHOD 'SSLContext'
    0xb9, // LOAD_FAST 9
    0x13,0x14, // LOAD_ATTR 'PROTOCOL_TLS_CLIENT'
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0xb3, // LOAD_FAST 3
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0xb0, // LOAD_FAST 0
    0xc3, // STORE_FAST 3
    0xb2, // LOAD_FAST 2
    0x14,0x15, // LOAD_METHOD 'wrap_socket'
    0xb7, // LOAD_FAST 7
    0x10,0x16, // LOAD_CONST_STRING 'server_hostname'
    0xb3, // LOAD_FAST 3
    0x10,0x17, // LOAD_CONST_STRING 'do_handshake_on_connect'
    0x50, // LOAD_CONST_FALSE
    0x36,0x84,0x01, // CALL_METHOD 513
    0xc7, // STORE_FAST 7
    0xb7, // LOAD_FAST 7
    0x14,0x10, // LOAD_METHOD 'setblocking'
    0x50, // LOAD_CONST_FALSE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x04, // LOAD_GLOBAL 'Stream'
    0xb7, // LOAD_FAST 7
    0x34,0x01, // CALL_FUNCTION 1
    0xca, // STORE_FAST 10
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x18, // LOAD_ATTR '_io_queue'
    0x14,0x19, // LOAD_METHOD 'queue_write'
    0xb7, // LOAD_FAST 7
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xba, // LOAD_FAST 10
    0xba, // LOAD_FAST 10
    0x2a,0x02, // BUILD_TUPLE 2
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_open_connection = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_stream_open_connection,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 171,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 18,
        .n_exc_stack = 2,
        .scope_flags = 1,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_asyncio_stream_open_connection + 9,
        .line_info_top = fun_data_asyncio_stream_open_connection + 9,
        .opcodes = fun_data_asyncio_stream_open_connection + 9,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_open_connection fun_data_asyncio_stream_open_connection[0]
#endif

// child of asyncio_stream__lt_module_gt_
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Server
static const byte fun_data_asyncio_stream_Server[33] = {
    0x00,0x02, // prelude
    0x05, // names: Server
     // code info
    0x11,0x3e, // LOAD_NAME '__name__'
    0x16,0x3f, // STORE_NAME '__module__'
    0x10,0x05, // LOAD_CONST_STRING 'Server'
    0x16,0x40, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x34, // STORE_NAME '__aenter__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x35, // STORE_NAME '__aexit__'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x2e, // STORE_NAME 'close'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x06, // STORE_NAME 'wait_closed'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x21, // STORE_NAME '_serve'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of asyncio_stream_Server
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Server___aenter__
static const byte fun_data_asyncio_stream_Server___aenter__[7] = {
    0x89,0x40,0x04, // prelude
    0x34,0x46, // names: __aenter__, self
     // code info
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Server___aenter__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_stream_Server___aenter__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 7,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 52,
        .line_info = fun_data_asyncio_stream_Server___aenter__ + 5,
        .line_info_top = fun_data_asyncio_stream_Server___aenter__ + 5,
        .opcodes = fun_data_asyncio_stream_Server___aenter__ + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_Server___aenter__ fun_data_asyncio_stream_Server___aenter__[0]
#endif

// child of asyncio_stream_Server
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Server___aexit__
static const byte fun_data_asyncio_stream_Server___aexit__[25] = {
    0xa8,0x44,0x0a, // prelude
    0x35,0x46,0x4f,0x50,0x51, // names: __aexit__, self, exc_type, exc, tb
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x2e, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x06, // LOAD_METHOD 'wait_closed'
    0x36,0x00, // CALL_METHOD 0
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Server___aexit__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_stream_Server___aexit__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 25,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 53,
        .line_info = fun_data_asyncio_stream_Server___aexit__ + 8,
        .line_info_top = fun_data_asyncio_stream_Server___aexit__ + 8,
        .opcodes = fun_data_asyncio_stream_Server___aexit__ + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_Server___aexit__ fun_data_asyncio_stream_Server___aexit__[0]
#endif

// child of asyncio_stream_Server
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Server_close
static const byte fun_data_asyncio_stream_Server_close[18] = {
    0x11,0x04, // prelude
    0x2e,0x46, // names: close, self
     // code info
    0x52, // LOAD_CONST_TRUE
    0xb0, // LOAD_FAST 0
    0x18,0x36, // STORE_ATTR 'state'
    0xb0, // LOAD_FAST 0
    0x13,0x22, // LOAD_ATTR 'task'
    0x14,0x25, // LOAD_METHOD 'cancel'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Server_close = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_stream_Server_close,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 18,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 46,
        .line_info = fun_data_asyncio_stream_Server_close + 4,
        .line_info_top = fun_data_asyncio_stream_Server_close + 4,
        .opcodes = fun_data_asyncio_stream_Server_close + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_Server_close fun_data_asyncio_stream_Server_close[0]
#endif

// child of asyncio_stream_Server
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Server_wait_closed
static const byte fun_data_asyncio_stream_Server_wait_closed[14] = {
    0x91,0x40,0x04, // prelude
    0x06,0x46, // names: wait_closed, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x22, // LOAD_ATTR 'task'
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Server_wait_closed = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_stream_Server_wait_closed,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 14,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_asyncio_stream_Server_wait_closed + 5,
        .line_info_top = fun_data_asyncio_stream_Server_wait_closed + 5,
        .opcodes = fun_data_asyncio_stream_Server_wait_closed + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_Server_wait_closed fun_data_asyncio_stream_Server_wait_closed[0]
#endif

// child of asyncio_stream_Server
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_Server__serve
static const byte fun_data_asyncio_stream_Server__serve[174] = {
    0xf8,0x46,0x0a, // prelude
    0x21,0x46,0x2a,0x44,0x12, // names: _serve, self, s, cb, ssl
     // code info
    0x50, // LOAD_CONST_FALSE
    0xb0, // LOAD_FAST 0
    0x18,0x36, // STORE_ATTR 'state'
    0x48,0x0d, // SETUP_EXCEPT 13
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x18, // LOAD_ATTR '_io_queue'
    0x14,0x30, // LOAD_METHOD 'queue_read'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x4a,0x23, // POP_EXCEPT_JUMP 35
    0x57, // DUP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x24, // LOAD_ATTR 'CancelledError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x5a, // POP_JUMP_IF_FALSE 26
    0xc4, // STORE_FAST 4
    0x49,0x10, // SETUP_FINALLY 16
    0xb1, // LOAD_FAST 1
    0x14,0x2e, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x36, // LOAD_ATTR 'state'
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb4, // LOAD_FAST 4
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc4, // STORE_FAST 4
    0x28,0x04, // DELETE_FAST 4
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x48,0x0b, // SETUP_EXCEPT 11
    0xb1, // LOAD_FAST 1
    0x14,0x37, // LOAD_METHOD 'accept'
    0x36,0x00, // CALL_METHOD 0
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc5, // STORE_FAST 5
    0xc6, // STORE_FAST 6
    0x4a,0x06, // POP_EXCEPT_JUMP 6
    0x59, // POP_TOP
    0x40,0xda,0x80,0x01, // UNWIND_JUMP 90
    0x5d, // END_FINALLY
    0xb3, // LOAD_FAST 3
    0x44,0x76, // POP_JUMP_IF_FALSE 54
    0x48,0x10, // SETUP_EXCEPT 16
    0xb3, // LOAD_FAST 3
    0x14,0x15, // LOAD_METHOD 'wrap_socket'
    0xb5, // LOAD_FAST 5
    0x10,0x38, // LOAD_CONST_STRING 'server_side'
    0x52, // LOAD_CONST_TRUE
    0x10,0x17, // LOAD_CONST_STRING 'do_handshake_on_connect'
    0x50, // LOAD_CONST_FALSE
    0x36,0x84,0x01, // CALL_METHOD 513
    0xc5, // STORE_FAST 5
    0x4a,0x24, // POP_EXCEPT_JUMP 36
    0x57, // DUP_TOP
    0x12,0x43, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x5d, // POP_JUMP_IF_FALSE 29
    0xc7, // STORE_FAST 7
    0x49,0x13, // SETUP_FINALLY 19
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x39, // LOAD_ATTR 'sys'
    0x14,0x3a, // LOAD_METHOD 'print_exception'
    0xb7, // LOAD_FAST 7
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb5, // LOAD_FAST 5
    0x14,0x2e, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x40,0x68,0x02, // UNWIND_JUMP 40
    0x51, // LOAD_CONST_NONE
    0xc7, // STORE_FAST 7
    0x28,0x07, // DELETE_FAST 7
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xb5, // LOAD_FAST 5
    0x14,0x10, // LOAD_METHOD 'setblocking'
    0x50, // LOAD_CONST_FALSE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x04, // LOAD_GLOBAL 'Stream'
    0xb5, // LOAD_FAST 5
    0x2c,0x01, // BUILD_MAP 1
    0xb6, // LOAD_FAST 6
    0x10,0x3b, // LOAD_CONST_STRING 'peername'
    0x62, // STORE_MAP
    0x34,0x02, // CALL_FUNCTION 2
    0xc8, // STORE_FAST 8
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x20, // LOAD_METHOD 'create_task'
    0xb2, // LOAD_FAST 2
    0xb8, // LOAD_FAST 8
    0xb8, // LOAD_FAST 8
    0x34,0x02, // CALL_FUNCTION 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0xe0,0x7e, // JUMP -160
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Server__serve = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_stream_Server__serve,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 174,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 16,
        .n_exc_stack = 2,
        .scope_flags = 1,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 33,
        .line_info = fun_data_asyncio_stream_Server__serve + 8,
        .line_info_top = fun_data_asyncio_stream_Server__serve + 8,
        .opcodes = fun_data_asyncio_stream_Server__serve + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_Server__serve fun_data_asyncio_stream_Server__serve[0]
#endif

static const mp_raw_code_t *const children_asyncio_stream_Server[] = {
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Server___aenter__,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Server___aexit__,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Server_close,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Server_wait_closed,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Server__serve,
};

static const mp_raw_code_truncated_t proto_fun_asyncio_stream_Server = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_stream_Server,
    .children = (void *)&children_asyncio_stream_Server,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 33,
    .n_children = 5,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 5,
        .line_info = fun_data_asyncio_stream_Server + 3,
        .line_info_top = fun_data_asyncio_stream_Server + 3,
        .opcodes = fun_data_asyncio_stream_Server + 3,
    },
    #endif
    #endif
};

// child of asyncio_stream__lt_module_gt_
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_start_server
static const byte fun_data_asyncio_stream_start_server[138] = {
    0x81,0xd6,0x01,0x0c, // prelude
    0x1a,0x44,0x41,0x42,0x45,0x12, // names: start_server, cb, host, port, backlog, ssl
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x0d, // IMPORT_NAME 'socket'
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x14,0x0e, // LOAD_METHOD 'getaddrinfo'
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc6, // STORE_FAST 6
    0xb5, // LOAD_FAST 5
    0x14,0x0d, // LOAD_METHOD 'socket'
    0xb6, // LOAD_FAST 6
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xc7, // STORE_FAST 7
    0xb7, // LOAD_FAST 7
    0x14,0x10, // LOAD_METHOD 'setblocking'
    0x50, // LOAD_CONST_FALSE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb7, // LOAD_FAST 7
    0x14,0x1b, // LOAD_METHOD 'setsockopt'
    0xb5, // LOAD_FAST 5
    0x13,0x1c, // LOAD_ATTR 'SOL_SOCKET'
    0xb5, // LOAD_FAST 5
    0x13,0x1d, // LOAD_ATTR 'SO_REUSEADDR'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x03, // CALL_METHOD 3
    0x59, // POP_TOP
    0xb7, // LOAD_FAST 7
    0x14,0x1e, // LOAD_METHOD 'bind'
    0xb6, // LOAD_FAST 6
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb7, // LOAD_FAST 7
    0x14,0x1f, // LOAD_METHOD 'listen'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x05, // LOAD_GLOBAL 'Server'
    0x34,0x00, // CALL_FUNCTION 0
    0xc8, // STORE_FAST 8
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x20, // LOAD_METHOD 'create_task'
    0xb8, // LOAD_FAST 8
    0x14,0x21, // LOAD_METHOD '_serve'
    0xb7, // LOAD_FAST 7
    0xb0, // LOAD_FAST 0
    0xb4, // LOAD_FAST 4
    0x36,0x03, // CALL_METHOD 3
    0x36,0x01, // CALL_METHOD 1
    0xb8, // LOAD_FAST 8
    0x18,0x22, // STORE_ATTR 'task'
    0x48,0x0d, // SETUP_EXCEPT 13
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x14,0x23, // LOAD_METHOD 'sleep_ms'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x01, // CALL_METHOD 1
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x4a,0x1d, // POP_EXCEPT_JUMP 29
    0x57, // DUP_TOP
    0x12,0x02, // LOAD_GLOBAL 'core'
    0x13,0x24, // LOAD_ATTR 'CancelledError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x54, // POP_JUMP_IF_FALSE 20
    0xc9, // STORE_FAST 9
    0x49,0x0a, // SETUP_FINALLY 10
    0xb8, // LOAD_FAST 8
    0x13,0x22, // LOAD_ATTR 'task'
    0x14,0x25, // LOAD_METHOD 'cancel'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb9, // LOAD_FAST 9
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0xc9, // STORE_FAST 9
    0x28,0x09, // DELETE_FAST 9
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xb8, // LOAD_FAST 8
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_start_server = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_stream_start_server,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 138,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 17,
        .n_exc_stack = 2,
        .scope_flags = 1,
        .n_pos_args = 5,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 26,
        .line_info = fun_data_asyncio_stream_start_server + 10,
        .line_info_top = fun_data_asyncio_stream_start_server + 10,
        .opcodes = fun_data_asyncio_stream_start_server + 10,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_start_server fun_data_asyncio_stream_start_server[0]
#endif

// child of asyncio_stream__lt_module_gt_
// frozen bytecode for file asyncio/stream.py, scope asyncio_stream_stream_awrite
static const byte fun_data_asyncio_stream_stream_awrite[63] = {
    0xb8,0xc4,0x01,0x0a, // prelude
    0x26,0x46,0x47,0x48,0x49, // names: stream_awrite, self, buf, off, sz
     // code info
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdc, // BINARY_OP 5 __ne__
    0x43,0x45, // POP_JUMP_IF_TRUE 5
    0xb3, // LOAD_FAST 3
    0x7f, // LOAD_CONST_SMALL_INT -1
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x5a, // POP_JUMP_IF_FALSE 26
    0x12,0x4a, // LOAD_GLOBAL 'memoryview'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc1, // STORE_FAST 1
    0xb3, // LOAD_FAST 3
    0x7f, // LOAD_CONST_SMALL_INT -1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x12,0x4b, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xf2, // BINARY_OP 27 __add__
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x14,0x27, // LOAD_METHOD 'write'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x28, // LOAD_METHOD 'drain'
    0x36,0x00, // CALL_METHOD 0
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_asyncio_stream_stream_awrite = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_asyncio_stream_stream_awrite,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 63,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 38,
        .line_info = fun_data_asyncio_stream_stream_awrite + 9,
        .line_info_top = fun_data_asyncio_stream_stream_awrite + 9,
        .opcodes = fun_data_asyncio_stream_stream_awrite + 9,
    },
    #endif
    #endif
};
#else
#define proto_fun_asyncio_stream_stream_awrite fun_data_asyncio_stream_stream_awrite[0]
#endif

static const mp_raw_code_t *const children_asyncio_stream__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Stream,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_open_connection,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_Server,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_start_server,
    (const mp_raw_code_t *)&proto_fun_asyncio_stream_stream_awrite,
};

static const mp_raw_code_truncated_t proto_fun_asyncio_stream__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_asyncio_stream__lt_module_gt_,
    .children = (void *)&children_asyncio_stream__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 90,
    .n_children = 5,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_asyncio_stream__lt_module_gt_ + 3,
        .line_info_top = fun_data_asyncio_stream__lt_module_gt_ + 3,
        .opcodes = fun_data_asyncio_stream__lt_module_gt_ + 3,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_asyncio_stream[82] = {
    MP_QSTR_asyncio_slash_stream_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_core,
    MP_QSTR_,
    MP_QSTR_Stream,
    MP_QSTR_Server,
    MP_QSTR_wait_closed,
    MP_QSTR_aclose,
    MP_QSTR_awrite,
    MP_QSTR_awritestr,
    MP_QSTR_open_connection,
    MP_QSTR_EINPROGRESS,
    MP_QSTR_errno,
    MP_QSTR_socket,
    MP_QSTR_getaddrinfo,
    MP_QSTR_SOCK_STREAM,
    MP_QSTR_setblocking,
    MP_QSTR_connect,
    MP_QSTR_ssl,
    MP_QSTR_SSLContext,
    MP_QSTR_PROTOCOL_TLS_CLIENT,
    MP_QSTR_wrap_socket,
    MP_QSTR_server_hostname,
    MP_QSTR_do_handshake_on_connect,
    MP_QSTR__io_queue,
    MP_QSTR_queue_write,
    MP_QSTR_start_server,
    MP_QSTR_setsockopt,
    MP_QSTR_SOL_SOCKET,
    MP_QSTR_SO_REUSEADDR,
    MP_QSTR_bind,
    MP_QSTR_listen,
    MP_QSTR_create_task,
    MP_QSTR__serve,
    MP_QSTR_task,
    MP_QSTR_sleep_ms,
    MP_QSTR_CancelledError,
    MP_QSTR_cancel,
    MP_QSTR_stream_awrite,
    MP_QSTR_write,
    MP_QSTR_drain,
    MP_QSTR___init__,
    MP_QSTR_s,
    MP_QSTR_e,
    MP_QSTR_out_buf,
    MP_QSTR_get_extra_info,
    MP_QSTR_close,
    MP_QSTR_read,
    MP_QSTR_queue_read,
    MP_QSTR_readinto,
    MP_QSTR_readexactly,
    MP_QSTR_readline,
    MP_QSTR___aenter__,
    MP_QSTR___aexit__,
    MP_QSTR_state,
    MP_QSTR_accept,
    MP_QSTR_server_side,
    MP_QSTR_sys,
    MP_QSTR_print_exception,
    MP_QSTR_peername,
    MP_QSTR_StreamReader,
    MP_QSTR_StreamWriter,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_host,
    MP_QSTR_port,
    MP_QSTR_OSError,
    MP_QSTR_cb,
    MP_QSTR_backlog,
    MP_QSTR_self,
    MP_QSTR_buf,
    MP_QSTR_off,
    MP_QSTR_sz,
    MP_QSTR_memoryview,
    MP_QSTR_len,
    MP_QSTR_v,
    MP_QSTR_n,
    MP_QSTR_EOFError,
    MP_QSTR_exc_type,
    MP_QSTR_exc,
    MP_QSTR_tb,
};

// constants

// constant table
static const mp_rom_obj_t const_obj_table_data_asyncio_stream[1] = {
    MP_ROM_PTR(&mp_const_empty_bytes_obj),
};

static const mp_frozen_module_t frozen_module_asyncio_stream = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_asyncio_stream,
        .obj_table = (mp_obj_t *)&const_obj_table_data_asyncio_stream,
    },
    .proto_fun = &proto_fun_asyncio_stream__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module uasyncio
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/uasyncio.mpy
// - frozen file name: uasyncio.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file uasyncio.py, scope uasyncio__lt_module_gt_
static const byte fun_data_uasyncio__lt_module_gt_[9] = {
    0x00,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x02, // STORE_NAME '__getattr__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of uasyncio__lt_module_gt_
// frozen bytecode for file uasyncio.py, scope uasyncio___getattr__
static const byte fun_data_uasyncio___getattr__[16] = {
    0x21,0x04, // prelude
    0x02,0x04, // names: __getattr__, attr
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'asyncio'
    0xc1, // STORE_FAST 1
    0x12,0x05, // LOAD_GLOBAL 'getattr'
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_uasyncio___getattr__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_uasyncio___getattr__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 16,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 2,
        .line_info = fun_data_uasyncio___getattr__ + 4,
        .line_info_top = fun_data_uasyncio___getattr__ + 4,
        .opcodes = fun_data_uasyncio___getattr__ + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_uasyncio___getattr__ fun_data_uasyncio___getattr__[0]
#endif

static const mp_raw_code_t *const children_uasyncio__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_uasyncio___getattr__,
};

static const mp_raw_code_truncated_t proto_fun_uasyncio__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_uasyncio__lt_module_gt_,
    .children = (void *)&children_uasyncio__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 9,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_uasyncio__lt_module_gt_ + 3,
        .line_info_top = fun_data_uasyncio__lt_module_gt_ + 3,
        .opcodes = fun_data_uasyncio__lt_module_gt_ + 3,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_uasyncio[6] = {
    MP_QSTR_uasyncio_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR___getattr__,
    MP_QSTR_asyncio,
    MP_QSTR_attr,
    MP_QSTR_getattr,
};

static const mp_frozen_module_t frozen_module_uasyncio = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_uasyncio,
        .obj_table = NULL,
    },
    .proto_fun = &proto_fun_uasyncio__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module requests___init__
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/requests/__init__.mpy
// - frozen file name: requests/__init__.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file requests/__init__.py, scope requests___init____lt_module_gt_
static const byte fun_data_requests___init____lt_module_gt_[79] = {
    0x30,0x24, // prelude
    0x01, // names: <module>
    0x66,0x89,0x20,0x8e,0xa4,0x64,0x20,0x64,0x20,0x64,0x20,0x64,0x20,0x64,0x20,0x64,0x20, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'socket'
    0x16,0x02, // STORE_NAME 'socket'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x03, // LOAD_CONST_STRING 'Response'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x03, // STORE_NAME 'Response'
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x52, // LOAD_CONST_TRUE
    0x2a,0x07, // BUILD_TUPLE 7
    0x53, // LOAD_NULL
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x05, // STORE_NAME 'request'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x33, // STORE_NAME 'head'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x27, // STORE_NAME 'get'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x35, // STORE_NAME 'post'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x37, // STORE_NAME 'put'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x39, // STORE_NAME 'patch'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x3b, // STORE_NAME 'delete'
    0x10,0x04, // LOAD_CONST_STRING '0.10.2'
    0x16,0x45, // STORE_NAME '__version__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of requests___init____lt_module_gt_
// frozen bytecode for file requests/__init__.py, scope requests___init___Response
static const byte fun_data_requests___init___Response[51] = {
    0x08,0x16, // prelude
    0x03, // names: Response
    0x68,0x20,0x64,0x40,0x64,0x60,0x88,0x0a,0x68,0x20, // code info
    0x11,0x46, // LOAD_NAME '__name__'
    0x16,0x47, // STORE_NAME '__module__'
    0x10,0x03, // LOAD_CONST_STRING 'Response'
    0x16,0x48, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x3d, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x26, // STORE_NAME 'close'
    0x11,0x49, // LOAD_NAME 'property'
    0x32,0x02, // MAKE_FUNCTION 2
    0x34,0x01, // CALL_FUNCTION 1
    0x16,0x41, // STORE_NAME 'content'
    0x11,0x49, // LOAD_NAME 'property'
    0x32,0x03, // MAKE_FUNCTION 3
    0x34,0x01, // CALL_FUNCTION 1
    0x16,0x43, // STORE_NAME 'text'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x23, // STORE_NAME 'json'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of requests___init___Response
// frozen bytecode for file requests/__init__.py, scope requests___init___Response___init__
static const byte fun_data_requests___init___Response___init__[24] = {
    0x1a,0x0e, // prelude
    0x3d,0x58,0x59, // names: __init__, self, f
    0x60,0x40,0x24,0x25, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x3e, // STORE_ATTR 'raw'
    0x10,0x2c, // LOAD_CONST_STRING 'utf-8'
    0xb0, // LOAD_FAST 0
    0x18,0x3f, // STORE_ATTR 'encoding'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x40, // STORE_ATTR '_cached'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_requests___init___Response___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_requests___init___Response___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 24,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 61,
        .line_info = fun_data_requests___init___Response___init__ + 5,
        .line_info_top = fun_data_requests___init___Response___init__ + 9,
        .opcodes = fun_data_requests___init___Response___init__ + 9,
    },
    #endif
    #endif
};
#else
#define proto_fun_requests___init___Response___init__ fun_data_requests___init___Response___init__[0]
#endif

// child of requests___init___Response
// frozen bytecode for file requests/__init__.py, scope requests___init___Response_close
static const byte fun_data_requests___init___Response_close[32] = {
    0x11,0x0e, // prelude
    0x26,0x58, // names: close, self
    0x80,0x0a,0x25,0x28,0x24, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x3e, // LOAD_ATTR 'raw'
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0xb0, // LOAD_FAST 0
    0x13,0x3e, // LOAD_ATTR 'raw'
    0x14,0x26, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x3e, // STORE_ATTR 'raw'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x40, // STORE_ATTR '_cached'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_requests___init___Response_close = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_requests___init___Response_close,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 32,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 38,
        .line_info = fun_data_requests___init___Response_close + 4,
        .line_info_top = fun_data_requests___init___Response_close + 9,
        .opcodes = fun_data_requests___init___Response_close + 9,
    },
    #endif
    #endif
};
#else
#define proto_fun_requests___init___Response_close fun_data_requests___init___Response_close[0]
#endif

// child of requests___init___Response
// frozen bytecode for file requests/__init__.py, scope requests___init___Response_content
static const byte fun_data_requests___init___Response_content[48] = {
    0x2d,0x12, // prelude
    0x41,0x58, // names: content, self
    0x80,0x11,0x27,0x22,0x4b,0x28,0x25, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x40, // LOAD_ATTR '_cached'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x5a, // POP_JUMP_IF_FALSE 26
    0x49,0x0b, // SETUP_FINALLY 11
    0xb0, // LOAD_FAST 0
    0x13,0x3e, // LOAD_ATTR 'raw'
    0x14,0x42, // LOAD_METHOD 'read'
    0x36,0x00, // CALL_METHOD 0
    0xb0, // LOAD_FAST 0
    0x18,0x40, // STORE_ATTR '_cached'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x13,0x3e, // LOAD_ATTR 'raw'
    0x14,0x26, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x3e, // STORE_ATTR 'raw'
    0x5d, // END_FINALLY
    0xb0, // LOAD_FAST 0
    0x13,0x40, // LOAD_ATTR '_cached'
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_requests___init___Response_content = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_requests___init___Response_content,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 48,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 65,
        .line_info = fun_data_requests___init___Response_content + 4,
        .line_info_top = fun_data_requests___init___Response_content + 11,
        .opcodes = fun_data_requests___init___Response_content + 11,
    },
    #endif
    #endif
};
#else
#define proto_fun_requests___init___Response_content fun_data_requests___init___Response_content[0]
#endif

// child of requests___init___Response
// frozen bytecode for file requests/__init__.py, scope requests___init___Response_text
static const byte fun_data_requests___init___Response_text[17] = {
    0x19,0x08, // prelude
    0x43,0x58, // names: text, self
    0x80,0x1b, // code info
    0x12,0x52, // LOAD_GLOBAL 'str'
    0xb0, // LOAD_FAST 0
    0x13,0x41, // LOAD_ATTR 'content'
    0xb0, // LOAD_FAST 0
    0x13,0x3f, // LOAD_ATTR 'encoding'
    0x34,0x02, // CALL_FUNCTION 2
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_requests___init___Response_text = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_requests___init___Response_text,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 17,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 67,
        .line_info = fun_data_requests___init___Response_text + 4,
        .line_info_top = fun_data_requests___init___Response_text + 6,
        .opcodes = fun_data_requests___init___Response_text + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_requests___init___Response_text fun_data_requests___init___Response_text[0]
#endif

// child of requests___init___Response
// frozen bytecode for file requests/__init__.py, scope requests___init___Response_json
static const byte fun_data_requests___init___Response_json[21] = {
    0x21,0x0a, // prelude
    0x23,0x58, // names: json, self
    0x80,0x1e,0x45, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x23, // IMPORT_NAME 'json'
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x44, // LOAD_METHOD 'loads'
    0xb0, // LOAD_FAST 0
    0x13,0x41, // LOAD_ATTR 'content'
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_requests___init___Response_json = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_requests___init___Response_json,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 21,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 35,
        .line_info = fun_data_requests___init___Response_json + 4,
        .line_info_top = fun_data_requests___init___Response_json + 7,
        .opcodes = fun_data_requests___init___Response_json + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_requests___init___Response_json fun_data_requests___init___Response_json[0]
#endif

static const mp_raw_code_t *const children_requests___init___Response[] = {
    (const mp_raw_code_t *)&proto_fun_requests___init___Response___init__,
    (const mp_raw_code_t *)&proto_fun_requests___init___Response_close,
    (const mp_raw_code_t *)&proto_fun_requests___init___Response_content,
    (const mp_raw_code_t *)&proto_fun_requests___init___Response_text,
    (const mp_raw_code_t *)&proto_fun_requests___init___Response_json,
};

static const mp_raw_code_truncated_t proto_fun_requests___init___Response = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_requests___init___Response,
    .children = (void *)&children_requests___init___Response,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 51,
    .n_children = 5,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 3,
        .line_info = fun_data_requests___init___Response + 3,
        .line_info_top = fun_data_requests___init___Response + 13,
        .opcodes = fun_data_requests___init___Response + 13,
    },
    #endif
    #endif
};

// child of requests___init____lt_module_gt_
// frozen bytecode for file requests/__init__.py, scope requests___init___request
static const byte fun_data_requests___init___request[1150] = {
    0xcd,0xa1,0x85,0x01,0xf6,0x02, // prelude
    0x05,0x4a,0x4b,0x4c,0x23,0x32,0x4d,0x4e,0x4f,0x50, // names: request, method, url, data, json, headers, stream, auth, timeout, parse_headers
    0x80,0x2e,0x25,0x45,0x46,0x22,0x57,0x26,0x45,0x25,0x29,0x32,0x4b,0x22,0x5a,0x2f,0x27,0x26,0x27,0x26,0x46,0x47,0x49,0x27,0x2f,0x48,0x31,0x46,0x23,0x26,0x44,0x54,0x66,0x48,0x23,0x2b,0x26,0x2c,0x28,0x2f,0x4f,0x27,0x46,0x26,0x28,0x4c,0x46,0x27,0x46,0x23,0x23,0x2e,0x28,0x27,0x4f,0x27,0x66,0x26,0x29,0x29,0x2b,0x4b,0x49,0x24,0x24,0x2d,0x26,0x30,0x29,0x2b,0x4b,0x26,0x4d,0x48,0x48,0x2a,0x4a,0x2a,0x2a,0x24,0x2a,0x2a,0x20,0x28,0x2b,0x43,0x2a,0x27,0x33,0x3f,0x36,0x50,0x2c,0x25,0x22,0x25,0x2a,0x2f,0x4d,0x54,0x27,0x42,0x23,0x47,0x29,0x30,0x4c,0x4d,0x28,0x26,0x26,0x27,0x26, // code info
    0xb4, // LOAD_FAST 4
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0x2c,0x00, // BUILD_MAP 0
    0xc4, // STORE_FAST 4
    0x42,0x46, // JUMP 6
    0xb4, // LOAD_FAST 4
    0x14,0x06, // LOAD_METHOD 'copy'
    0x36,0x00, // CALL_METHOD 0
    0xc4, // STORE_FAST 4
    0x51, // LOAD_CONST_NONE
    0xc9, // STORE_FAST 9
    0xb2, // LOAD_FAST 2
    0x46,0x13, // JUMP_IF_FALSE_OR_POP 19
    0x12,0x51, // LOAD_GLOBAL 'getattr'
    0xb2, // LOAD_FAST 2
    0x10,0x07, // LOAD_CONST_STRING '__next__'
    0x51, // LOAD_CONST_NONE
    0x34,0x03, // CALL_FUNCTION 3
    0x46,0x09, // JUMP_IF_FALSE_OR_POP 9
    0x12,0x51, // LOAD_GLOBAL 'getattr'
    0xb2, // LOAD_FAST 2
    0x10,0x08, // LOAD_CONST_STRING '__len__'
    0x51, // LOAD_CONST_NONE
    0x34,0x03, // CALL_FUNCTION 3
    0xd3, // UNARY_OP 3 <not>
    0xca, // STORE_FAST 10
    0xb6, // LOAD_FAST 6
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x70, // POP_JUMP_IF_FALSE 48
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x09, // IMPORT_NAME 'binascii'
    0xcb, // STORE_FAST 11
    0xb6, // LOAD_FAST 6
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xcc, // STORE_FAST 12
    0xcd, // STORE_FAST 13
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x14,0x0a, // LOAD_METHOD 'format'
    0xbc, // LOAD_FAST 12
    0xbd, // LOAD_FAST 13
    0x36,0x02, // CALL_METHOD 2
    0xce, // STORE_FAST 14
    0x12,0x52, // LOAD_GLOBAL 'str'
    0xbb, // LOAD_FAST 11
    0x14,0x0b, // LOAD_METHOD 'b2a_base64'
    0xbe, // LOAD_FAST 14
    0x36,0x01, // CALL_METHOD 1
    0x51, // LOAD_CONST_NONE
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x10,0x0c, // LOAD_CONST_STRING 'ascii'
    0x34,0x02, // CALL_FUNCTION 2
    0xce, // STORE_FAST 14
    0x10,0x0d, // LOAD_CONST_STRING 'Basic {}'
    0x14,0x0a, // LOAD_METHOD 'format'
    0xbe, // LOAD_FAST 14
    0x36,0x01, // CALL_METHOD 1
    0xb4, // LOAD_FAST 4
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x56, // STORE_SUBSCR
    0x48,0x13, // SETUP_EXCEPT 19
    0xb1, // LOAD_FAST 1
    0x14,0x0e, // LOAD_METHOD 'split'
    0x10,0x0f, // LOAD_CONST_STRING '/'
    0x83, // LOAD_CONST_SMALL_INT 3
    0x36,0x02, // CALL_METHOD 2
    0x30,0x04, // UNPACK_SEQUENCE 4
    0xcf, // STORE_FAST 15
    0x26,0x10, // STORE_FAST_N 16
    0x26,0x11, // STORE_FAST_N 17
    0x26,0x12, // STORE_FAST_N 18
    0x4a,0x1d, // POP_EXCEPT_JUMP 29
    0x57, // DUP_TOP
    0x12,0x53, // LOAD_GLOBAL 'ValueError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x56, // POP_JUMP_IF_FALSE 22
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x14,0x0e, // LOAD_METHOD 'split'
    0x10,0x0f, // LOAD_CONST_STRING '/'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x36,0x02, // CALL_METHOD 2
    0x30,0x03, // UNPACK_SEQUENCE 3
    0xcf, // STORE_FAST 15
    0x26,0x10, // STORE_FAST_N 16
    0x26,0x11, // STORE_FAST_N 17
    0x10,0x10, // LOAD_CONST_STRING ''
    0x26,0x12, // STORE_FAST_N 18
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xbf, // LOAD_FAST 15
    0x10,0x11, // LOAD_CONST_STRING 'http:'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x22,0x80,0x50, // LOAD_CONST_SMALL_INT 80
    0x26,0x13, // STORE_FAST_N 19
    0x42,0x5c, // JUMP 28
    0xbf, // LOAD_FAST 15
    0x10,0x12, // LOAD_CONST_STRING 'https:'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x13, // IMPORT_NAME 'tls'
    0x26,0x14, // STORE_FAST_N 20
    0x22,0x83,0x3b, // LOAD_CONST_SMALL_INT 443
    0x26,0x13, // STORE_FAST_N 19
    0x42,0x49, // JUMP 9
    0x12,0x53, // LOAD_GLOBAL 'ValueError'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xbf, // LOAD_FAST 15
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x10,0x14, // LOAD_CONST_STRING ':'
    0x24,0x11, // LOAD_FAST_N 17
    0xdd, // BINARY_OP 6 <in>
    0x44,0x57, // POP_JUMP_IF_FALSE 23
    0x24,0x11, // LOAD_FAST_N 17
    0x14,0x0e, // LOAD_METHOD 'split'
    0x10,0x14, // LOAD_CONST_STRING ':'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x02, // CALL_METHOD 2
    0x30,0x02, // UNPACK_SEQUENCE 2
    0x26,0x11, // STORE_FAST_N 17
    0x26,0x13, // STORE_FAST_N 19
    0x12,0x54, // LOAD_GLOBAL 'int'
    0x24,0x13, // LOAD_FAST_N 19
    0x34,0x01, // CALL_FUNCTION 1
    0x26,0x13, // STORE_FAST_N 19
    0x12,0x02, // LOAD_GLOBAL 'socket'
    0x14,0x15, // LOAD_METHOD 'getaddrinfo'
    0x24,0x11, // LOAD_FAST_N 17
    0x24,0x13, // LOAD_FAST_N 19
    0x80, // LOAD_CONST_SMALL_INT 0
    0x12,0x02, // LOAD_GLOBAL 'socket'
    0x13,0x16, // LOAD_ATTR 'SOCK_STREAM'
    0x36,0x04, // CALL_METHOD 4
    0x26,0x15, // STORE_FAST_N 21
    0x24,0x15, // LOAD_FAST_N 21
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x26,0x15, // STORE_FAST_N 21
    0x51, // LOAD_CONST_NONE
    0x26,0x16, // STORE_FAST_N 22
    0xb8, // LOAD_FAST 8
    0x50, // LOAD_CONST_FALSE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x2c,0x00, // BUILD_MAP 0
    0x26,0x16, // STORE_FAST_N 22
    0x12,0x02, // LOAD_GLOBAL 'socket'
    0x14,0x02, // LOAD_METHOD 'socket'
    0x24,0x15, // LOAD_FAST_N 21
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x12,0x02, // LOAD_GLOBAL 'socket'
    0x13,0x16, // LOAD_ATTR 'SOCK_STREAM'
    0x24,0x15, // LOAD_FAST_N 21
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x36,0x03, // CALL_METHOD 3
    0x26,0x17, // STORE_FAST_N 23
    0xb7, // LOAD_FAST 7
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x17, // LOAD_METHOD 'settimeout'
    0xb7, // LOAD_FAST 7
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x48,0xea,0x04, // SETUP_EXCEPT 618
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x18, // LOAD_METHOD 'connect'
    0x24,0x15, // LOAD_FAST_N 21
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xbf, // LOAD_FAST 15
    0x10,0x12, // LOAD_CONST_STRING 'https:'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x63, // POP_JUMP_IF_FALSE 35
    0x24,0x14, // LOAD_FAST_N 20
    0x14,0x19, // LOAD_METHOD 'SSLContext'
    0x24,0x14, // LOAD_FAST_N 20
    0x13,0x1a, // LOAD_ATTR 'PROTOCOL_TLS_CLIENT'
    0x36,0x01, // CALL_METHOD 1
    0x26,0x18, // STORE_FAST_N 24
    0x24,0x14, // LOAD_FAST_N 20
    0x13,0x1b, // LOAD_ATTR 'CERT_NONE'
    0x24,0x18, // LOAD_FAST_N 24
    0x18,0x1c, // STORE_ATTR 'verify_mode'
    0x24,0x18, // LOAD_FAST_N 24
    0x14,0x1d, // LOAD_METHOD 'wrap_socket'
    0x24,0x17, // LOAD_FAST_N 23
    0x10,0x1e, // LOAD_CONST_STRING 'server_hostname'
    0x24,0x11, // LOAD_FAST_N 17
    0x36,0x82,0x01, // CALL_METHOD 257
    0x26,0x17, // STORE_FAST_N 23
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1f, // LOAD_METHOD 'write'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xb0, // LOAD_FAST 0
    0x24,0x12, // LOAD_FAST_N 18
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x10,0x20, // LOAD_CONST_STRING 'Host'
    0xb4, // LOAD_FAST 4
    0xdd, // BINARY_OP 6 <in>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x24,0x11, // LOAD_FAST_N 17
    0xb4, // LOAD_FAST 4
    0x10,0x20, // LOAD_CONST_STRING 'Host'
    0x56, // STORE_SUBSCR
    0xb3, // LOAD_FAST 3
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x67, // POP_JUMP_IF_FALSE 39
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x21, // LOAD_GLOBAL 'AssertionError'
    0x65, // RAISE_OBJ
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x22, // LOAD_CONST_STRING 'dumps'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x23, // IMPORT_NAME 'json'
    0x1c,0x22, // IMPORT_FROM 'dumps'
    0x26,0x19, // STORE_FAST_N 25
    0x59, // POP_TOP
    0x24,0x19, // LOAD_FAST_N 25
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0xc2, // STORE_FAST 2
    0x23,0x04, // LOAD_CONST_OBJ 4
    0xb4, // LOAD_FAST 4
    0xdd, // BINARY_OP 6 <in>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x23,0x05, // LOAD_CONST_OBJ 5
    0xb4, // LOAD_FAST 4
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x56, // STORE_SUBSCR
    0xb2, // LOAD_FAST 2
    0x44,0x6f, // POP_JUMP_IF_FALSE 47
    0xba, // LOAD_FAST 10
    0x44,0x56, // POP_JUMP_IF_FALSE 22
    0x23,0x06, // LOAD_CONST_OBJ 6
    0xb4, // LOAD_FAST 4
    0xdd, // BINARY_OP 6 <in>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x23,0x07, // LOAD_CONST_OBJ 7
    0xb4, // LOAD_FAST 4
    0xdd, // BINARY_OP 6 <in>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x10,0x24, // LOAD_CONST_STRING 'chunked'
    0xb4, // LOAD_FAST 4
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x56, // STORE_SUBSCR
    0x42,0x56, // JUMP 22
    0x23,0x07, // LOAD_CONST_OBJ 7
    0xb4, // LOAD_FAST 4
    0xdd, // BINARY_OP 6 <in>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x12,0x52, // LOAD_GLOBAL 'str'
    0x12,0x55, // LOAD_GLOBAL 'len'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0xb4, // LOAD_FAST 4
    0x23,0x07, // LOAD_CONST_OBJ 7
    0x56, // STORE_SUBSCR
    0x42,0x40, // JUMP 0
    0x10,0x25, // LOAD_CONST_STRING 'Connection'
    0xb4, // LOAD_FAST 4
    0xdd, // BINARY_OP 6 <in>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x10,0x26, // LOAD_CONST_STRING 'close'
    0xb4, // LOAD_FAST 4
    0x10,0x25, // LOAD_CONST_STRING 'Connection'
    0x56, // STORE_SUBSCR
    0xb4, // LOAD_FAST 4
    0x5f, // GET_ITER_STACK
    0x4b,0x2a, // FOR_ITER 42
    0x26,0x1a, // STORE_FAST_N 26
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1f, // LOAD_METHOD 'write'
    0x24,0x1a, // LOAD_FAST_N 26
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1f, // LOAD_METHOD 'write'
    0x23,0x08, // LOAD_CONST_OBJ 8
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1f, // LOAD_METHOD 'write'
    0xb4, // LOAD_FAST 4
    0x24,0x1a, // LOAD_FAST_N 26
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1f, // LOAD_METHOD 'write'
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x14, // JUMP -44
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1f, // LOAD_METHOD 'write'
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x44,0xe1,0x80, // POP_JUMP_IF_FALSE 97
    0xba, // LOAD_FAST 10
    0x44,0xd5,0x80, // POP_JUMP_IF_FALSE 85
    0xb4, // LOAD_FAST 4
    0x14,0x27, // LOAD_METHOD 'get'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x51, // LOAD_CONST_NONE
    0x36,0x02, // CALL_METHOD 2
    0x10,0x24, // LOAD_CONST_STRING 'chunked'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x75, // POP_JUMP_IF_FALSE 53
    0xb2, // LOAD_FAST 2
    0x5f, // GET_ITER_STACK
    0x4b,0x26, // FOR_ITER 38
    0x26,0x1b, // STORE_FAST_N 27
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1f, // LOAD_METHOD 'write'
    0x23,0x0a, // LOAD_CONST_OBJ 10
    0x12,0x55, // LOAD_GLOBAL 'len'
    0x24,0x1b, // LOAD_FAST_N 27
    0x34,0x01, // CALL_FUNCTION 1
    0xf8, // BINARY_OP 33 __mod__
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1f, // LOAD_METHOD 'write'
    0x24,0x1b, // LOAD_FAST_N 27
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1f, // LOAD_METHOD 'write'
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x18, // JUMP -40
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1f, // LOAD_METHOD 'write'
    0x10,0x28, // LOAD_CONST_STRING '0\r\n\r\n'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x51, // JUMP 17
    0xb2, // LOAD_FAST 2
    0x5f, // GET_ITER_STACK
    0x4b,0x0d, // FOR_ITER 13
    0x26,0x1b, // STORE_FAST_N 27
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1f, // LOAD_METHOD 'write'
    0x24,0x1b, // LOAD_FAST_N 27
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x31, // JUMP -15
    0x42,0x48, // JUMP 8
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x1f, // LOAD_METHOD 'write'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x29, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0x26,0x1c, // STORE_FAST_N 28
    0x24,0x1c, // LOAD_FAST_N 28
    0x14,0x0e, // LOAD_METHOD 'split'
    0x51, // LOAD_CONST_NONE
    0x82, // LOAD_CONST_SMALL_INT 2
    0x36,0x02, // CALL_METHOD 2
    0x26,0x1c, // STORE_FAST_N 28
    0x12,0x55, // LOAD_GLOBAL 'len'
    0x24,0x1c, // LOAD_FAST_N 28
    0x34,0x01, // CALL_FUNCTION 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x12,0x53, // LOAD_GLOBAL 'ValueError'
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0x24,0x1c, // LOAD_FAST_N 28
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x12,0x54, // LOAD_GLOBAL 'int'
    0x24,0x1c, // LOAD_FAST_N 28
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x26,0x1d, // STORE_FAST_N 29
    0x10,0x10, // LOAD_CONST_STRING ''
    0x26,0x1e, // STORE_FAST_N 30
    0x12,0x55, // LOAD_GLOBAL 'len'
    0x24,0x1c, // LOAD_FAST_N 28
    0x34,0x01, // CALL_FUNCTION 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x24,0x1c, // LOAD_FAST_N 28
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x14,0x2a, // LOAD_METHOD 'rstrip'
    0x36,0x00, // CALL_METHOD 0
    0x26,0x1e, // STORE_FAST_N 30
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x29, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0x26,0x1c, // STORE_FAST_N 28
    0x24,0x1c, // LOAD_FAST_N 28
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x24,0x1c, // LOAD_FAST_N 28
    0x23,0x09, // LOAD_CONST_OBJ 9
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x42,0xb2,0x81, // JUMP 178
    0x24,0x1c, // LOAD_FAST_N 28
    0x14,0x2b, // LOAD_METHOD 'startswith'
    0x23,0x0c, // LOAD_CONST_OBJ 12
    0x36,0x01, // CALL_METHOD 1
    0x44,0x5a, // POP_JUMP_IF_FALSE 26
    0x23,0x0d, // LOAD_CONST_OBJ 13
    0x24,0x1c, // LOAD_FAST_N 28
    0xdd, // BINARY_OP 6 <in>
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0x12,0x53, // LOAD_GLOBAL 'ValueError'
    0x23,0x0e, // LOAD_CONST_OBJ 14
    0x12,0x52, // LOAD_GLOBAL 'str'
    0x24,0x1c, // LOAD_FAST_N 28
    0x10,0x2c, // LOAD_CONST_STRING 'utf-8'
    0x34,0x02, // CALL_FUNCTION 2
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x42,0xd1,0x80, // JUMP 81
    0x24,0x1c, // LOAD_FAST_N 28
    0x14,0x2b, // LOAD_METHOD 'startswith'
    0x23,0x0f, // LOAD_CONST_OBJ 15
    0x36,0x01, // CALL_METHOD 1
    0x44,0xc6,0x80, // POP_JUMP_IF_FALSE 70
    0x22,0x81,0x48, // LOAD_CONST_SMALL_INT 200
    0x24,0x1d, // LOAD_FAST_N 29
    0x57, // DUP_TOP
    0x5b, // ROT_THREE
    0xda, // BINARY_OP 3 __le__
    0x46,0x06, // JUMP_IF_FALSE_OR_POP 6
    0x22,0x82,0x2b, // LOAD_CONST_SMALL_INT 299
    0xda, // BINARY_OP 3 __le__
    0x42,0x42, // JUMP 2
    0x5a, // ROT_TWO
    0x59, // POP_TOP
    0x43,0x72, // POP_JUMP_IF_TRUE 50
    0x24,0x1d, // LOAD_FAST_N 29
    0x22,0x82,0x2d, // LOAD_CONST_SMALL_INT 301
    0x22,0x82,0x2e, // LOAD_CONST_SMALL_INT 302
    0x22,0x82,0x2f, // LOAD_CONST_SMALL_INT 303
    0x22,0x82,0x33, // LOAD_CONST_SMALL_INT 307
    0x22,0x82,0x34, // LOAD_CONST_SMALL_INT 308
    0x2b,0x05, // BUILD_LIST 5
    0xdd, // BINARY_OP 6 <in>
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0x12,0x52, // LOAD_GLOBAL 'str'
    0x24,0x1c, // LOAD_FAST_N 28
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x7e, // LOAD_CONST_SMALL_INT -2
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x10,0x2c, // LOAD_CONST_STRING 'utf-8'
    0x34,0x02, // CALL_FUNCTION 2
    0xc9, // STORE_FAST 9
    0x42,0x4a, // JUMP 10
    0x12,0x56, // LOAD_GLOBAL 'NotImplementedError'
    0x23,0x10, // LOAD_CONST_OBJ 16
    0x24,0x1d, // LOAD_FAST_N 29
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x42,0x40, // JUMP 0
    0xb8, // LOAD_FAST 8
    0x50, // LOAD_CONST_FALSE
    0xde, // BINARY_OP 7 <is>
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x42,0x73, // JUMP 51
    0xb8, // LOAD_FAST 8
    0x52, // LOAD_CONST_TRUE
    0xde, // BINARY_OP 7 <is>
    0x44,0x66, // POP_JUMP_IF_FALSE 38
    0x12,0x52, // LOAD_GLOBAL 'str'
    0x24,0x1c, // LOAD_FAST_N 28
    0x10,0x2c, // LOAD_CONST_STRING 'utf-8'
    0x34,0x02, // CALL_FUNCTION 2
    0x26,0x1c, // STORE_FAST_N 28
    0x24,0x1c, // LOAD_FAST_N 28
    0x14,0x0e, // LOAD_METHOD 'split'
    0x10,0x14, // LOAD_CONST_STRING ':'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x02, // CALL_METHOD 2
    0x30,0x02, // UNPACK_SEQUENCE 2
    0x26,0x1a, // STORE_FAST_N 26
    0x26,0x1f, // STORE_FAST_N 31
    0x24,0x1f, // LOAD_FAST_N 31
    0x14,0x2d, // LOAD_METHOD 'strip'
    0x36,0x00, // CALL_METHOD 0
    0x24,0x16, // LOAD_FAST_N 22
    0x24,0x1a, // LOAD_FAST_N 26
    0x56, // STORE_SUBSCR
    0x42,0x48, // JUMP 8
    0xb8, // LOAD_FAST 8
    0x24,0x1c, // LOAD_FAST_N 28
    0x24,0x16, // LOAD_FAST_N 22
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x42,0xb8,0x7e, // JUMP -200
    0x4a,0x10, // POP_EXCEPT_JUMP 16
    0x57, // DUP_TOP
    0x12,0x57, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0x59, // POP_TOP
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x26, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x64, // RAISE_LAST
    0x5d, // END_FINALLY
    0xb9, // LOAD_FAST 9
    0x44,0x79, // POP_JUMP_IF_FALSE 57
    0x24,0x17, // LOAD_FAST_N 23
    0x14,0x26, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb4, // LOAD_FAST 4
    0x14,0x2e, // LOAD_METHOD 'pop'
    0x10,0x20, // LOAD_CONST_STRING 'Host'
    0x51, // LOAD_CONST_NONE
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x24,0x1d, // LOAD_FAST_N 29
    0x22,0x82,0x2d, // LOAD_CONST_SMALL_INT 301
    0x22,0x82,0x2e, // LOAD_CONST_SMALL_INT 302
    0x22,0x82,0x2f, // LOAD_CONST_SMALL_INT 303
    0x2b,0x03, // BUILD_LIST 3
    0xdd, // BINARY_OP 6 <in>
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0x12,0x05, // LOAD_GLOBAL 'request'
    0x10,0x2f, // LOAD_CONST_STRING 'GET'
    0xb9, // LOAD_FAST 9
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xb4, // LOAD_FAST 4
    0xb5, // LOAD_FAST 5
    0x34,0x06, // CALL_FUNCTION 6
    0x63, // RETURN_VALUE
    0x12,0x05, // LOAD_GLOBAL 'request'
    0xb0, // LOAD_FAST 0
    0xb9, // LOAD_FAST 9
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0xb5, // LOAD_FAST 5
    0x34,0x06, // CALL_FUNCTION 6
    0x63, // RETURN_VALUE
    0x42,0x64, // JUMP 36
    0x12,0x03, // LOAD_GLOBAL 'Response'
    0x24,0x17, // LOAD_FAST_N 23
    0x34,0x01, // CALL_FUNCTION 1
    0x26,0x20, // STORE_FAST_N 32
    0x24,0x1d, // LOAD_FAST_N 29
    0x24,0x20, // LOAD_FAST_N 32
    0x18,0x30, // STORE_ATTR 'status_code'
    0x24,0x1e, // LOAD_FAST_N 30
    0x24,0x20, // LOAD_FAST_N 32
    0x18,0x31, // STORE_ATTR 'reason'
    0x24,0x16, // LOAD_FAST_N 22
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x24,0x16, // LOAD_FAST_N 22
    0x24,0x20, // LOAD_FAST_N 32
    0x18,0x32, // STORE_ATTR 'headers'
    0x24,0x20, // LOAD_FAST_N 32
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_requests___init___request = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_requests___init___request,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 1150,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 42,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 9,
        .n_kwonly_args = 0,
        .n_def_pos_args = 7,
        .qstr_block_name_idx = 5,
        .line_info = fun_data_requests___init___request + 16,
        .line_info_top = fun_data_requests___init___request + 129,
        .opcodes = fun_data_requests___init___request + 129,
    },
    #endif
    #endif
};
#else
#define proto_fun_requests___init___request fun_data_requests___init___request[0]
#endif

// child of requests___init____lt_module_gt_
// frozen bytecode for file requests/__init__.py, scope requests___init___head
static const byte fun_data_requests___init___head[20] = {
    0xb9,0x80,0x40,0x08, // prelude
    0x33,0x4b, // names: head, url
    0x80,0xc8, // code info
    0x12,0x05, // LOAD_GLOBAL 'request'
    0x10,0x34, // LOAD_CONST_STRING 'HEAD'
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x82,0x02, // CALL_FUNCTION_VAR_KW 258
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_requests___init___head = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_requests___init___head,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 20,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 51,
        .line_info = fun_data_requests___init___head + 6,
        .line_info_top = fun_data_requests___init___head + 8,
        .opcodes = fun_data_requests___init___head + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_requests___init___head fun_data_requests___init___head[0]
#endif

// child of requests___init____lt_module_gt_
// frozen bytecode for file requests/__init__.py, scope requests___init___get
static const byte fun_data_requests___init___get[20] = {
    0xb9,0x80,0x40,0x08, // prelude
    0x27,0x4b, // names: get, url
    0x80,0xcc, // code info
    0x12,0x05, // LOAD_GLOBAL 'request'
    0x10,0x2f, // LOAD_CONST_STRING 'GET'
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x82,0x02, // CALL_FUNCTION_VAR_KW 258
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_requests___init___get = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_requests___init___get,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 20,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 39,
        .line_info = fun_data_requests___init___get + 6,
        .line_info_top = fun_data_requests___init___get + 8,
        .opcodes = fun_data_requests___init___get + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_requests___init___get fun_data_requests___init___get[0]
#endif

// child of requests___init____lt_module_gt_
// frozen bytecode for file requests/__init__.py, scope requests___init___post
static const byte fun_data_requests___init___post[20] = {
    0xb9,0x80,0x40,0x08, // prelude
    0x35,0x4b, // names: post, url
    0x80,0xd0, // code info
    0x12,0x05, // LOAD_GLOBAL 'request'
    0x10,0x36, // LOAD_CONST_STRING 'POST'
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x82,0x02, // CALL_FUNCTION_VAR_KW 258
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_requests___init___post = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_requests___init___post,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 20,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 53,
        .line_info = fun_data_requests___init___post + 6,
        .line_info_top = fun_data_requests___init___post + 8,
        .opcodes = fun_data_requests___init___post + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_requests___init___post fun_data_requests___init___post[0]
#endif

// child of requests___init____lt_module_gt_
// frozen bytecode for file requests/__init__.py, scope requests___init___put
static const byte fun_data_requests___init___put[20] = {
    0xb9,0x80,0x40,0x08, // prelude
    0x37,0x4b, // names: put, url
    0x80,0xd4, // code info
    0x12,0x05, // LOAD_GLOBAL 'request'
    0x10,0x38, // LOAD_CONST_STRING 'PUT'
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x82,0x02, // CALL_FUNCTION_VAR_KW 258
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_requests___init___put = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_requests___init___put,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 20,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 55,
        .line_info = fun_data_requests___init___put + 6,
        .line_info_top = fun_data_requests___init___put + 8,
        .opcodes = fun_data_requests___init___put + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_requests___init___put fun_data_requests___init___put[0]
#endif

// child of requests___init____lt_module_gt_
// frozen bytecode for file requests/__init__.py, scope requests___init___patch
static const byte fun_data_requests___init___patch[20] = {
    0xb9,0x80,0x40,0x08, // prelude
    0x39,0x4b, // names: patch, url
    0x80,0xd8, // code info
    0x12,0x05, // LOAD_GLOBAL 'request'
    0x10,0x3a, // LOAD_CONST_STRING 'PATCH'
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x82,0x02, // CALL_FUNCTION_VAR_KW 258
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_requests___init___patch = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_requests___init___patch,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 20,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 57,
        .line_info = fun_data_requests___init___patch + 6,
        .line_info_top = fun_data_requests___init___patch + 8,
        .opcodes = fun_data_requests___init___patch + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_requests___init___patch fun_data_requests___init___patch[0]
#endif

// child of requests___init____lt_module_gt_
// frozen bytecode for file requests/__init__.py, scope requests___init___delete
static const byte fun_data_requests___init___delete[20] = {
    0xb9,0x80,0x40,0x08, // prelude
    0x3b,0x4b, // names: delete, url
    0x80,0xdc, // code info
    0x12,0x05, // LOAD_GLOBAL 'request'
    0x10,0x3c, // LOAD_CONST_STRING 'DELETE'
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x35,0x82,0x02, // CALL_FUNCTION_VAR_KW 258
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_requests___init___delete = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_requests___init___delete,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 20,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 59,
        .line_info = fun_data_requests___init___delete + 6,
        .line_info_top = fun_data_requests___init___delete + 8,
        .opcodes = fun_data_requests___init___delete + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_requests___init___delete fun_data_requests___init___delete[0]
#endif

static const mp_raw_code_t *const children_requests___init____lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_requests___init___Response,
    (const mp_raw_code_t *)&proto_fun_requests___init___request,
    (const mp_raw_code_t *)&proto_fun_requests___init___head,
    (const mp_raw_code_t *)&proto_fun_requests___init___get,
    (const mp_raw_code_t *)&proto_fun_requests___init___post,
    (const mp_raw_code_t *)&proto_fun_requests___init___put,
    (const mp_raw_code_t *)&proto_fun_requests___init___patch,
    (const mp_raw_code_t *)&proto_fun_requests___init___delete,
};

static const mp_raw_code_truncated_t proto_fun_requests___init____lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_requests___init____lt_module_gt_,
    .children = (void *)&children_requests___init____lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 79,
    .n_children = 8,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_requests___init____lt_module_gt_ + 3,
        .line_info_top = fun_data_requests___init____lt_module_gt_ + 20,
        .opcodes = fun_data_requests___init____lt_module_gt_ + 20,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_requests___init__[90] = {
    MP_QSTR_requests_slash___init___dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_socket,
    MP_QSTR_Response,
    MP_QSTR_0_dot_10_dot_2,
    MP_QSTR_request,
    MP_QSTR_copy,
    MP_QSTR___next__,
    MP_QSTR___len__,
    MP_QSTR_binascii,
    MP_QSTR_format,
    MP_QSTR_b2a_base64,
    MP_QSTR_ascii,
    MP_QSTR_Basic_space__brace_open__brace_close_,
    MP_QSTR_split,
    MP_QSTR__slash_,
    MP_QSTR_,
    MP_QSTR_http_colon_,
    MP_QSTR_https_colon_,
    MP_QSTR_tls,
    MP_QSTR__colon_,
    MP_QSTR_getaddrinfo,
    MP_QSTR_SOCK_STREAM,
    MP_QSTR_settimeout,
    MP_QSTR_connect,
    MP_QSTR_SSLContext,
    MP_QSTR_PROTOCOL_TLS_CLIENT,
    MP_QSTR_CERT_NONE,
    MP_QSTR_verify_mode,
    MP_QSTR_wrap_socket,
    MP_QSTR_server_hostname,
    MP_QSTR_write,
    MP_QSTR_Host,
    MP_QSTR_AssertionError,
    MP_QSTR_dumps,
    MP_QSTR_json,
    MP_QSTR_chunked,
    MP_QSTR_Connection,
    MP_QSTR_close,
    MP_QSTR_get,
    MP_QSTR_0_0x0d__0x0a__0x0d__0x0a_,
    MP_QSTR_readline,
    MP_QSTR_rstrip,
    MP_QSTR_startswith,
    MP_QSTR_utf_hyphen_8,
    MP_QSTR_strip,
    MP_QSTR_pop,
    MP_QSTR_GET,
    MP_QSTR_status_code,
    MP_QSTR_reason,
    MP_QSTR_headers,
    MP_QSTR_head,
    MP_QSTR_HEAD,
    MP_QSTR_post,
    MP_QSTR_POST,
    MP_QSTR_put,
    MP_QSTR_PUT,
    MP_QSTR_patch,
    MP_QSTR_PATCH,
    MP_QSTR_delete,
    MP_QSTR_DELETE,
    MP_QSTR___init__,
    MP_QSTR_raw,
    MP_QSTR_encoding,
    MP_QSTR__cached,
    MP_QSTR_content,
    MP_QSTR_read,
    MP_QSTR_text,
    MP_QSTR_loads,
    MP_QSTR___version__,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_property,
    MP_QSTR_method,
    MP_QSTR_url,
    MP_QSTR_data,
    MP_QSTR_stream,
    MP_QSTR_auth,
    MP_QSTR_timeout,
    MP_QSTR_parse_headers,
    MP_QSTR_getattr,
    MP_QSTR_str,
    MP_QSTR_ValueError,
    MP_QSTR_int,
    MP_QSTR_len,
    MP_QSTR_NotImplementedError,
    MP_QSTR_OSError,
    MP_QSTR_self,
    MP_QSTR_f,
};

// constants
static const mp_obj_str_t const_obj_requests___init___0 = {{&mp_type_bytes}, 14943, 5, (const byte*)"\x7b\x7d\x3a\x7b\x7d"};
static const mp_obj_str_t const_obj_requests___init___3 = {{&mp_type_bytes}, 47477, 17, (const byte*)"\x25\x73\x20\x2f\x25\x73\x20\x48\x54\x54\x50\x2f\x31\x2e\x30\x0d\x0a"};
static const mp_obj_str_t const_obj_requests___init___8 = {{&mp_type_bytes}, 26975, 2, (const byte*)"\x3a\x20"};
static const mp_obj_str_t const_obj_requests___init___9 = {{&mp_type_bytes}, 27298, 2, (const byte*)"\x0d\x0a"};
static const mp_obj_str_t const_obj_requests___init___10 = {{&mp_type_bytes}, 50079, 4, (const byte*)"\x25\x78\x0d\x0a"};
static const mp_obj_str_t const_obj_requests___init___11 = {{&mp_type_str}, 62148, 29, (const byte*)"\x48\x54\x54\x50\x20\x65\x72\x72\x6f\x72\x3a\x20\x42\x61\x64\x53\x74\x61\x74\x75\x73\x4c\x69\x6e\x65\x3a\x0a\x25\x73"};
static const mp_obj_str_t const_obj_requests___init___12 = {{&mp_type_bytes}, 59642, 18, (const byte*)"\x54\x72\x61\x6e\x73\x66\x65\x72\x2d\x45\x6e\x63\x6f\x64\x69\x6e\x67\x3a"};
static const mp_obj_str_t const_obj_requests___init___13 = {{&mp_type_bytes}, 3583, 7, (const byte*)"\x63\x68\x75\x6e\x6b\x65\x64"};
static const mp_obj_str_t const_obj_requests___init___15 = {{&mp_type_bytes}, 1090, 9, (const byte*)"\x4c\x6f\x63\x61\x74\x69\x6f\x6e\x3a"};
static const mp_obj_str_t const_obj_requests___init___16 = {{&mp_type_str}, 18157, 29, (const byte*)"\x52\x65\x64\x69\x72\x65\x63\x74\x20\x25\x64\x20\x6e\x6f\x74\x20\x79\x65\x74\x20\x73\x75\x70\x70\x6f\x72\x74\x65\x64"};

// constant table
static const mp_rom_obj_t const_obj_table_data_requests___init__[17] = {
    MP_ROM_PTR(&const_obj_requests___init___0),
    MP_ROM_QSTR(MP_QSTR_Authorization),
    MP_ROM_QSTR(MP_QSTR_Unsupported_space_protocol_colon__space_),
    MP_ROM_PTR(&const_obj_requests___init___3),
    MP_ROM_QSTR(MP_QSTR_Content_hyphen_Type),
    MP_ROM_QSTR(MP_QSTR_application_slash_json),
    MP_ROM_QSTR(MP_QSTR_Transfer_hyphen_Encoding),
    MP_ROM_QSTR(MP_QSTR_Content_hyphen_Length),
    MP_ROM_PTR(&const_obj_requests___init___8),
    MP_ROM_PTR(&const_obj_requests___init___9),
    MP_ROM_PTR(&const_obj_requests___init___10),
    MP_ROM_PTR(&const_obj_requests___init___11),
    MP_ROM_PTR(&const_obj_requests___init___12),
    MP_ROM_PTR(&const_obj_requests___init___13),
    MP_ROM_QSTR(MP_QSTR_Unsupported_space_),
    MP_ROM_PTR(&const_obj_requests___init___15),
    MP_ROM_PTR(&const_obj_requests___init___16),
};

static const mp_frozen_module_t frozen_module_requests___init__ = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_requests___init__,
        .obj_table = (mp_obj_t *)&const_obj_table_data_requests___init__,
    },
    .proto_fun = &proto_fun_requests___init____lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module mip___init__
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/mip/__init__.mpy
// - frozen file name: mip/__init__.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file mip/__init__.py, scope mip___init____lt_module_gt_
static const byte fun_data_mip___init____lt_module_gt_[80] = {
    0x18,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'sys'
    0x16,0x02, // STORE_NAME 'sys'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'requests'
    0x16,0x03, // STORE_NAME 'requests'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x04, // LOAD_CONST_STRING 'const'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x05, // IMPORT_NAME 'micropython'
    0x1c,0x04, // IMPORT_FROM 'const'
    0x16,0x04, // STORE_NAME 'const'
    0x59, // POP_TOP
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x16,0x45, // STORE_NAME 'allowed_mip_url_prefixes'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x07, // STORE_NAME '_ensure_path_exists'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x0f, // STORE_NAME '_chunk'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x11, // STORE_NAME '_check_exists'
    0x51, // LOAD_CONST_NONE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x03, // MAKE_FUNCTION_DEFARGS 3
    0x16,0x1a, // STORE_NAME '_rewrite_url'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x21, // STORE_NAME '_download_file'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x2b, // STORE_NAME '_install_json'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x34, // STORE_NAME '_install_package'
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x52, // LOAD_CONST_TRUE
    0x2a,0x04, // BUILD_TUPLE 4
    0x53, // LOAD_NULL
    0x33,0x07, // MAKE_FUNCTION_DEFARGS 7
    0x16,0x3e, // STORE_NAME 'install'
    0x10,0x06, // LOAD_CONST_STRING '0.4.2'
    0x16,0x46, // STORE_NAME '__version__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of mip___init____lt_module_gt_
// frozen bytecode for file mip/__init__.py, scope mip___init____ensure_path_exists
static const byte fun_data_mip___init____ensure_path_exists[97] = {
    0x65,0x04, // prelude
    0x07,0x3f, // names: _ensure_path_exists, path
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x08, // IMPORT_NAME 'os'
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x14,0x09, // LOAD_METHOD 'split'
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x43,0x50, // POP_JUMP_IF_TRUE 16
    0xb2, // LOAD_FAST 2
    0x14,0x0b, // LOAD_METHOD 'pop'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x56, // STORE_SUBSCR
    0x10,0x0c, // LOAD_CONST_STRING ''
    0xc3, // STORE_FAST 3
    0x12,0x47, // LOAD_GLOBAL 'len'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf3, // BINARY_OP 28 __sub__
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x65, // JUMP 37
    0x57, // DUP_TOP
    0xc4, // STORE_FAST 4
    0xb3, // LOAD_FAST 3
    0xb2, // LOAD_FAST 2
    0xb4, // LOAD_FAST 4
    0x55, // LOAD_SUBSCR
    0xe5, // BINARY_OP 14 __iadd__
    0xc3, // STORE_FAST 3
    0x48,0x09, // SETUP_EXCEPT 9
    0xb1, // LOAD_FAST 1
    0x14,0x0d, // LOAD_METHOD 'stat'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0x0b, // POP_EXCEPT_JUMP 11
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x14,0x0e, // LOAD_METHOD 'mkdir'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xb3, // LOAD_FAST 3
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xe5, // BINARY_OP 14 __iadd__
    0xc3, // STORE_FAST 3
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x16, // POP_JUMP_IF_TRUE -42
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_mip___init____ensure_path_exists = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_mip___init____ensure_path_exists,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 97,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_mip___init____ensure_path_exists + 4,
        .line_info_top = fun_data_mip___init____ensure_path_exists + 4,
        .opcodes = fun_data_mip___init____ensure_path_exists + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_mip___init____ensure_path_exists fun_data_mip___init____ensure_path_exists[0]
#endif

// child of mip___init____lt_module_gt_
// frozen bytecode for file mip/__init__.py, scope mip___init____chunk
static const byte fun_data_mip___init____chunk[55] = {
    0x3a,0x06, // prelude
    0x0f,0x48,0x49, // names: _chunk, src, dest
     // code info
    0x12,0x4a, // LOAD_GLOBAL 'memoryview'
    0x12,0x4b, // LOAD_GLOBAL 'bytearray'
    0x22,0x81,0x00, // LOAD_CONST_SMALL_INT 128
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x14,0x10, // LOAD_METHOD 'readinto'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x42,0x56, // JUMP 22
    0xb1, // LOAD_FAST 1
    0xb3, // LOAD_FAST 3
    0x22,0x81,0x00, // LOAD_CONST_SMALL_INT 128
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0xb2, // LOAD_FAST 2
    0x42,0x46, // JUMP 6
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0xb3, // LOAD_FAST 3
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x1c, // JUMP -36
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_mip___init____chunk = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_mip___init____chunk,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 55,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 15,
        .line_info = fun_data_mip___init____chunk + 5,
        .line_info_top = fun_data_mip___init____chunk + 5,
        .opcodes = fun_data_mip___init____chunk + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_mip___init____chunk fun_data_mip___init____chunk[0]
#endif

// child of mip___init____lt_module_gt_
// frozen bytecode for file mip/__init__.py, scope mip___init____check_exists
static const byte fun_data_mip___init____check_exists[88] = {
    0xf2,0x02,0x06, // prelude
    0x11,0x3f,0x4c, // names: _check_exists, path, short_hash
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x08, // IMPORT_NAME 'os'
    0xc2, // STORE_FAST 2
    0x48,0x45, // SETUP_EXCEPT 69
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x12, // IMPORT_NAME 'binascii'
    0xc3, // STORE_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x13, // IMPORT_NAME 'hashlib'
    0xc4, // STORE_FAST 4
    0x12,0x4d, // LOAD_GLOBAL 'open'
    0xb0, // LOAD_FAST 0
    0x10,0x14, // LOAD_CONST_STRING 'rb'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x2e, // SETUP_WITH 46
    0xc5, // STORE_FAST 5
    0xb4, // LOAD_FAST 4
    0x14,0x15, // LOAD_METHOD 'sha256'
    0x36,0x00, // CALL_METHOD 0
    0xc6, // STORE_FAST 6
    0x12,0x0f, // LOAD_GLOBAL '_chunk'
    0xb5, // LOAD_FAST 5
    0xb6, // LOAD_FAST 6
    0x13,0x16, // LOAD_ATTR 'update'
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x12,0x4e, // LOAD_GLOBAL 'str'
    0xb3, // LOAD_FAST 3
    0x14,0x17, // LOAD_METHOD 'hexlify'
    0xb6, // LOAD_FAST 6
    0x14,0x18, // LOAD_METHOD 'digest'
    0x36,0x00, // CALL_METHOD 0
    0x36,0x01, // CALL_METHOD 1
    0x51, // LOAD_CONST_NONE
    0x12,0x47, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x10,0x19, // LOAD_CONST_STRING 'utf-8'
    0x34,0x02, // CALL_FUNCTION 2
    0xc7, // STORE_FAST 7
    0xb7, // LOAD_FAST 7
    0xb1, // LOAD_FAST 1
    0xd9, // BINARY_OP 2 __eq__
    0x63, // RETURN_VALUE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x4a,0x04, // POP_EXCEPT_JUMP 4
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_mip___init____check_exists = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_mip___init____check_exists,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 88,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 15,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 17,
        .line_info = fun_data_mip___init____check_exists + 6,
        .line_info_top = fun_data_mip___init____check_exists + 6,
        .opcodes = fun_data_mip___init____check_exists + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_mip___init____check_exists fun_data_mip___init____check_exists[0]
#endif

// child of mip___init____lt_module_gt_
// frozen bytecode for file mip/__init__.py, scope mip___init____rewrite_url
static const byte fun_data_mip___init____rewrite_url[132] = {
    0xba,0x01,0x06, // prelude
    0x1a,0x4f,0x50, // names: _rewrite_url, url, branch
     // code info
    0xb1, // LOAD_FAST 1
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x10,0x1b, // LOAD_CONST_STRING 'HEAD'
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x14,0x1c, // LOAD_METHOD 'startswith'
    0x10,0x1d, // LOAD_CONST_STRING 'github:'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x72, // POP_JUMP_IF_FALSE 50
    0xb0, // LOAD_FAST 0
    0x87, // LOAD_CONST_SMALL_INT 7
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x14,0x09, // LOAD_METHOD 'split'
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0xc0, // STORE_FAST 0
    0x23,0x01, // LOAD_CONST_OBJ 1
    0xb0, // LOAD_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xf2, // BINARY_OP 27 __add__
    0xb0, // LOAD_FAST 0
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xf2, // BINARY_OP 27 __add__
    0xb1, // LOAD_FAST 1
    0xf2, // BINARY_OP 27 __add__
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xf2, // BINARY_OP 27 __add__
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x14,0x1e, // LOAD_METHOD 'join'
    0xb0, // LOAD_FAST 0
    0x82, // LOAD_CONST_SMALL_INT 2
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xf2, // BINARY_OP 27 __add__
    0xc0, // STORE_FAST 0
    0x42,0x7b, // JUMP 59
    0xb0, // LOAD_FAST 0
    0x14,0x1c, // LOAD_METHOD 'startswith'
    0x10,0x1f, // LOAD_CONST_STRING 'gitlab:'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x72, // POP_JUMP_IF_FALSE 50
    0xb0, // LOAD_FAST 0
    0x87, // LOAD_CONST_SMALL_INT 7
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x14,0x09, // LOAD_METHOD 'split'
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0xc0, // STORE_FAST 0
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xb0, // LOAD_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xf2, // BINARY_OP 27 __add__
    0xb0, // LOAD_FAST 0
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0x10,0x20, // LOAD_CONST_STRING '/-/raw/'
    0xf2, // BINARY_OP 27 __add__
    0xb1, // LOAD_FAST 1
    0xf2, // BINARY_OP 27 __add__
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xf2, // BINARY_OP 27 __add__
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x14,0x1e, // LOAD_METHOD 'join'
    0xb0, // LOAD_FAST 0
    0x82, // LOAD_CONST_SMALL_INT 2
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xf2, // BINARY_OP 27 __add__
    0xc0, // STORE_FAST 0
    0x42,0x40, // JUMP 0
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_mip___init____rewrite_url = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_mip___init____rewrite_url,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 132,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 26,
        .line_info = fun_data_mip___init____rewrite_url + 6,
        .line_info_top = fun_data_mip___init____rewrite_url + 6,
        .opcodes = fun_data_mip___init____rewrite_url + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_mip___init____rewrite_url fun_data_mip___init____rewrite_url[0]
#endif

// child of mip___init____lt_module_gt_
// frozen bytecode for file mip/__init__.py, scope mip___init____download_file
static const byte fun_data_mip___init____download_file[89] = {
    0xda,0x02,0x06, // prelude
    0x21,0x4f,0x49, // names: _download_file, url, dest
     // code info
    0x12,0x03, // LOAD_GLOBAL 'requests'
    0x14,0x22, // LOAD_METHOD 'get'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0x49,0x40, // SETUP_FINALLY 64
    0xb2, // LOAD_FAST 2
    0x13,0x23, // LOAD_ATTR 'status_code'
    0x22,0x81,0x48, // LOAD_CONST_SMALL_INT 200
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0x12,0x51, // LOAD_GLOBAL 'print'
    0x10,0x24, // LOAD_CONST_STRING 'Error'
    0xb2, // LOAD_FAST 2
    0x13,0x23, // LOAD_ATTR 'status_code'
    0x10,0x25, // LOAD_CONST_STRING 'requesting'
    0xb0, // LOAD_FAST 0
    0x34,0x04, // CALL_FUNCTION 4
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
    0x12,0x51, // LOAD_GLOBAL 'print'
    0x10,0x26, // LOAD_CONST_STRING 'Copying:'
    0xb1, // LOAD_FAST 1
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x12,0x07, // LOAD_GLOBAL '_ensure_path_exists'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x4d, // LOAD_GLOBAL 'open'
    0xb1, // LOAD_FAST 1
    0x10,0x27, // LOAD_CONST_STRING 'wb'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x0d, // SETUP_WITH 13
    0xc3, // STORE_FAST 3
    0x12,0x0f, // LOAD_GLOBAL '_chunk'
    0xb2, // LOAD_FAST 2
    0x13,0x28, // LOAD_ATTR 'raw'
    0xb3, // LOAD_FAST 3
    0x13,0x29, // LOAD_ATTR 'write'
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
    0xb2, // LOAD_FAST 2
    0x14,0x2a, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_mip___init____download_file = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_mip___init____download_file,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 89,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 33,
        .line_info = fun_data_mip___init____download_file + 6,
        .line_info_top = fun_data_mip___init____download_file + 6,
        .opcodes = fun_data_mip___init____download_file + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_mip___init____download_file fun_data_mip___init____download_file[0]
#endif

// child of mip___init____lt_module_gt_
// frozen bytecode for file mip/__init__.py, scope mip___init____install_json
static const byte fun_data_mip___init____install_json[271] = {
    0xcd,0x14,0x0d, // prelude
    0x2b,0x52,0x53,0x54,0x55,0x56, // names: _install_json, package_json_url, index, target, version, mpy
    0x0f, // code info
    0x12,0x03, // LOAD_GLOBAL 'requests'
    0x14,0x22, // LOAD_METHOD 'get'
    0x12,0x1a, // LOAD_GLOBAL '_rewrite_url'
    0xb0, // LOAD_FAST 0
    0xb3, // LOAD_FAST 3
    0x34,0x02, // CALL_FUNCTION 2
    0x36,0x01, // CALL_METHOD 1
    0xc5, // STORE_FAST 5
    0x49,0x1a, // SETUP_FINALLY 26
    0xb5, // LOAD_FAST 5
    0x13,0x23, // LOAD_ATTR 'status_code'
    0x22,0x81,0x48, // LOAD_CONST_SMALL_INT 200
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x12,0x51, // LOAD_GLOBAL 'print'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
    0xb5, // LOAD_FAST 5
    0x14,0x2c, // LOAD_METHOD 'json'
    0x36,0x00, // CALL_METHOD 0
    0xc6, // STORE_FAST 6
    0x51, // LOAD_CONST_NONE
    0xb5, // LOAD_FAST 5
    0x14,0x2a, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x5d, // END_FINALLY
    0xb6, // LOAD_FAST 6
    0x14,0x22, // LOAD_METHOD 'get'
    0x10,0x2d, // LOAD_CONST_STRING 'hashes'
    0x2a,0x00, // BUILD_TUPLE 0
    0x36,0x02, // CALL_METHOD 2
    0x5f, // GET_ITER_STACK
    0x4b,0x46, // FOR_ITER 70
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc7, // STORE_FAST 7
    0xc8, // STORE_FAST 8
    0xb2, // LOAD_FAST 2
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xf2, // BINARY_OP 27 __add__
    0xb7, // LOAD_FAST 7
    0xf2, // BINARY_OP 27 __add__
    0xc9, // STORE_FAST 9
    0x12,0x11, // LOAD_GLOBAL '_check_exists'
    0xb9, // LOAD_FAST 9
    0xb8, // LOAD_FAST 8
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x12,0x51, // LOAD_GLOBAL 'print'
    0x10,0x2e, // LOAD_CONST_STRING 'Exists:'
    0xb9, // LOAD_FAST 9
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x42,0x66, // JUMP 38
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x14,0x2f, // LOAD_METHOD 'format'
    0xb1, // LOAD_FAST 1
    0xb8, // LOAD_FAST 8
    0x51, // LOAD_CONST_NONE
    0x82, // LOAD_CONST_SMALL_INT 2
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xb8, // LOAD_FAST 8
    0x36,0x03, // CALL_METHOD 3
    0xca, // STORE_FAST 10
    0x12,0x21, // LOAD_GLOBAL '_download_file'
    0xba, // LOAD_FAST 10
    0xb9, // LOAD_FAST 9
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x4f, // POP_JUMP_IF_TRUE 15
    0x12,0x51, // LOAD_GLOBAL 'print'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x14,0x2f, // LOAD_METHOD 'format'
    0xb7, // LOAD_FAST 7
    0xb8, // LOAD_FAST 8
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
    0x42,0xb8,0x7f, // JUMP -72
    0xb0, // LOAD_FAST 0
    0x14,0x30, // LOAD_METHOD 'rpartition'
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xcb, // STORE_FAST 11
    0xb6, // LOAD_FAST 6
    0x14,0x22, // LOAD_METHOD 'get'
    0x10,0x31, // LOAD_CONST_STRING 'urls'
    0x2a,0x00, // BUILD_TUPLE 0
    0x36,0x02, // CALL_METHOD 2
    0x5f, // GET_ITER_STACK
    0x4b,0x4c, // FOR_ITER 76
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc7, // STORE_FAST 7
    0x27,0x0f, // STORE_DEREF 15
    0xb2, // LOAD_FAST 2
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xf2, // BINARY_OP 27 __add__
    0xb7, // LOAD_FAST 7
    0xf2, // BINARY_OP 27 __add__
    0xc9, // STORE_FAST 9
    0x12,0x57, // LOAD_GLOBAL 'any'
    0xbf, // LOAD_FAST 15
    0x20,0x00,0x01, // MAKE_CLOSURE 0
    0x12,0x45, // LOAD_GLOBAL 'allowed_mip_url_prefixes'
    0x5e, // GET_ITER
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0xcc, // STORE_FAST 12
    0xbb, // LOAD_FAST 11
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0xbc, // LOAD_FAST 12
    0x43,0x4b, // POP_JUMP_IF_TRUE 11
    0x10,0x32, // LOAD_CONST_STRING '{}/{}'
    0x14,0x2f, // LOAD_METHOD 'format'
    0xbb, // LOAD_FAST 11
    0x25,0x0f, // LOAD_DEREF 15
    0x36,0x02, // CALL_METHOD 2
    0x27,0x0f, // STORE_DEREF 15
    0x12,0x21, // LOAD_GLOBAL '_download_file'
    0x12,0x1a, // LOAD_GLOBAL '_rewrite_url'
    0x25,0x0f, // LOAD_DEREF 15
    0xb3, // LOAD_FAST 3
    0x34,0x02, // CALL_FUNCTION 2
    0xb9, // LOAD_FAST 9
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x50, // POP_JUMP_IF_TRUE 16
    0x12,0x51, // LOAD_GLOBAL 'print'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x14,0x2f, // LOAD_METHOD 'format'
    0xb7, // LOAD_FAST 7
    0x25,0x0f, // LOAD_DEREF 15
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
    0x42,0xb2,0x7f, // JUMP -78
    0xb6, // LOAD_FAST 6
    0x14,0x22, // LOAD_METHOD 'get'
    0x10,0x33, // LOAD_CONST_STRING 'deps'
    0x2a,0x00, // BUILD_TUPLE 0
    0x36,0x02, // CALL_METHOD 2
    0x5f, // GET_ITER_STACK
    0x4b,0x13, // FOR_ITER 19
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xcd, // STORE_FAST 13
    0xce, // STORE_FAST 14
    0x12,0x34, // LOAD_GLOBAL '_install_package'
    0xbd, // LOAD_FAST 13
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0xbe, // LOAD_FAST 14
    0xb4, // LOAD_FAST 4
    0x34,0x05, // CALL_FUNCTION 5
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
    0x42,0x2b, // JUMP -21
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
};
// child of mip___init____install_json
// frozen bytecode for file mip/__init__.py, scope mip___init____install_json__lt_genexpr_gt_
static const byte fun_data_mip___init____install_json__lt_genexpr_gt_[26] = {
    0xca,0x40,0x06, // prelude
    0x44,0x5a,0x5a, // names: <genexpr>, *, *
     // code info
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x53, // LOAD_NULL
    0x53, // LOAD_NULL
    0x4b,0x0c, // FOR_ITER 12
    0xc2, // STORE_FAST 2
    0x25,0x00, // LOAD_DEREF 0
    0x14,0x1c, // LOAD_METHOD 'startswith'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x42,0x32, // JUMP -14
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_mip___init____install_json__lt_genexpr_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_mip___init____install_json__lt_genexpr_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 26,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 68,
        .line_info = fun_data_mip___init____install_json__lt_genexpr_gt_ + 6,
        .line_info_top = fun_data_mip___init____install_json__lt_genexpr_gt_ + 6,
        .opcodes = fun_data_mip___init____install_json__lt_genexpr_gt_ + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_mip___init____install_json__lt_genexpr_gt_ fun_data_mip___init____install_json__lt_genexpr_gt_[0]
#endif

static const mp_raw_code_t *const children_mip___init____install_json[] = {
    (const mp_raw_code_t *)&proto_fun_mip___init____install_json__lt_genexpr_gt_,
};

static const mp_raw_code_truncated_t proto_fun_mip___init____install_json = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_mip___init____install_json,
    .children = (void *)&children_mip___init____install_json,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 271,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 26,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 5,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 43,
        .line_info = fun_data_mip___init____install_json + 9,
        .line_info_top = fun_data_mip___init____install_json + 9,
        .opcodes = fun_data_mip___init____install_json + 10,
    },
    #endif
    #endif
};

// child of mip___init____lt_module_gt_
// frozen bytecode for file mip/__init__.py, scope mip___init____install_package
static const byte fun_data_mip___init____install_package[214] = {
    0xe1,0x04,0x0d, // prelude
    0x34,0x58,0x53,0x54,0x55,0x56, // names: _install_package, package, index, target, version, mpy
    0x00, // code info
    0x12,0x57, // LOAD_GLOBAL 'any'
    0xb0, // LOAD_FAST 0
    0x20,0x00,0x01, // MAKE_CLOSURE 0
    0x12,0x45, // LOAD_GLOBAL 'allowed_mip_url_prefixes'
    0x5e, // GET_ITER
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0xf0,0x80, // POP_JUMP_IF_FALSE 112
    0x25,0x00, // LOAD_DEREF 0
    0x14,0x35, // LOAD_METHOD 'endswith'
    0x10,0x36, // LOAD_CONST_STRING '.py'
    0x36,0x01, // CALL_METHOD 1
    0x43,0x4a, // POP_JUMP_IF_TRUE 10
    0x25,0x00, // LOAD_DEREF 0
    0x14,0x35, // LOAD_METHOD 'endswith'
    0x10,0x37, // LOAD_CONST_STRING '.mpy'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x69, // POP_JUMP_IF_FALSE 41
    0x12,0x51, // LOAD_GLOBAL 'print'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x14,0x2f, // LOAD_METHOD 'format'
    0x25,0x00, // LOAD_DEREF 0
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x21, // LOAD_GLOBAL '_download_file'
    0x12,0x1a, // LOAD_GLOBAL '_rewrite_url'
    0x25,0x00, // LOAD_DEREF 0
    0xb3, // LOAD_FAST 3
    0x34,0x02, // CALL_FUNCTION 2
    0xb2, // LOAD_FAST 2
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xf2, // BINARY_OP 27 __add__
    0x25,0x00, // LOAD_DEREF 0
    0x14,0x38, // LOAD_METHOD 'rsplit'
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0x34,0x02, // CALL_FUNCTION 2
    0x63, // RETURN_VALUE
    0x25,0x00, // LOAD_DEREF 0
    0x14,0x35, // LOAD_METHOD 'endswith'
    0x10,0x39, // LOAD_CONST_STRING '.json'
    0x36,0x01, // CALL_METHOD 1
    0x43,0x58, // POP_JUMP_IF_TRUE 24
    0x25,0x00, // LOAD_DEREF 0
    0x14,0x35, // LOAD_METHOD 'endswith'
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0x25,0x00, // LOAD_DEREF 0
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0xe5, // BINARY_OP 14 __iadd__
    0x27,0x00, // STORE_DEREF 0
    0x25,0x00, // LOAD_DEREF 0
    0x23,0x07, // LOAD_CONST_OBJ 7
    0xe5, // BINARY_OP 14 __iadd__
    0x27,0x00, // STORE_DEREF 0
    0x12,0x51, // LOAD_GLOBAL 'print'
    0x23,0x08, // LOAD_CONST_OBJ 8
    0x14,0x2f, // LOAD_METHOD 'format'
    0x25,0x00, // LOAD_DEREF 0
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0xc1,0x80, // JUMP 65
    0xb3, // LOAD_FAST 3
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x10,0x3a, // LOAD_CONST_STRING 'latest'
    0xc3, // STORE_FAST 3
    0x12,0x51, // LOAD_GLOBAL 'print'
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x14,0x2f, // LOAD_METHOD 'format'
    0x25,0x00, // LOAD_DEREF 0
    0xb3, // LOAD_FAST 3
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x36,0x04, // CALL_METHOD 4
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb4, // LOAD_FAST 4
    0x44,0x58, // POP_JUMP_IF_FALSE 24
    0x12,0x59, // LOAD_GLOBAL 'hasattr'
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x3b, // LOAD_ATTR 'implementation'
    0x10,0x3c, // LOAD_CONST_STRING '_mpy'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x3b, // LOAD_ATTR 'implementation'
    0x13,0x3c, // LOAD_ATTR '_mpy'
    0x22,0x81,0x7f, // LOAD_CONST_SMALL_INT 255
    0xef, // BINARY_OP 24 __and__
    0x42,0x42, // JUMP 2
    0x10,0x3d, // LOAD_CONST_STRING 'py'
    0xc5, // STORE_FAST 5
    0x23,0x0a, // LOAD_CONST_OBJ 10
    0x14,0x2f, // LOAD_METHOD 'format'
    0xb1, // LOAD_FAST 1
    0xb5, // LOAD_FAST 5
    0x25,0x00, // LOAD_DEREF 0
    0xb3, // LOAD_FAST 3
    0x36,0x04, // CALL_METHOD 4
    0x27,0x00, // STORE_DEREF 0
    0x12,0x2b, // LOAD_GLOBAL '_install_json'
    0x25,0x00, // LOAD_DEREF 0
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x34,0x05, // CALL_FUNCTION 5
    0x63, // RETURN_VALUE
};
// child of mip___init____install_package
// frozen bytecode for file mip/__init__.py, scope mip___init____install_package__lt_genexpr_gt_
static const byte fun_data_mip___init____install_package__lt_genexpr_gt_[26] = {
    0xca,0x40,0x06, // prelude
    0x44,0x5a,0x5a, // names: <genexpr>, *, *
     // code info
    0x53, // LOAD_NULL
    0xb1, // LOAD_FAST 1
    0x53, // LOAD_NULL
    0x53, // LOAD_NULL
    0x4b,0x0c, // FOR_ITER 12
    0xc2, // STORE_FAST 2
    0x25,0x00, // LOAD_DEREF 0
    0x14,0x1c, // LOAD_METHOD 'startswith'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x42,0x32, // JUMP -14
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_mip___init____install_package__lt_genexpr_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_mip___init____install_package__lt_genexpr_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 26,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 68,
        .line_info = fun_data_mip___init____install_package__lt_genexpr_gt_ + 6,
        .line_info_top = fun_data_mip___init____install_package__lt_genexpr_gt_ + 6,
        .opcodes = fun_data_mip___init____install_package__lt_genexpr_gt_ + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_mip___init____install_package__lt_genexpr_gt_ fun_data_mip___init____install_package__lt_genexpr_gt_[0]
#endif

static const mp_raw_code_t *const children_mip___init____install_package[] = {
    (const mp_raw_code_t *)&proto_fun_mip___init____install_package__lt_genexpr_gt_,
};

static const mp_raw_code_truncated_t proto_fun_mip___init____install_package = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_mip___init____install_package,
    .children = (void *)&children_mip___init____install_package,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 214,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 5,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 52,
        .line_info = fun_data_mip___init____install_package + 9,
        .line_info_top = fun_data_mip___init____install_package + 9,
        .opcodes = fun_data_mip___init____install_package + 10,
    },
    #endif
    #endif
};

// child of mip___init____lt_module_gt_
// frozen bytecode for file mip/__init__.py, scope mip___init___install
static const byte fun_data_mip___init___install[100] = {
    0xe1,0x84,0x80,0x01,0x0c, // prelude
    0x3e,0x58,0x53,0x54,0x55,0x56, // names: install, package, index, target, version, mpy
     // code info
    0xb2, // LOAD_FAST 2
    0x43,0x6d, // POP_JUMP_IF_TRUE 45
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x3f, // LOAD_ATTR 'path'
    0x5f, // GET_ITER_STACK
    0x4b,0x1d, // FOR_ITER 29
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x14,0x1c, // LOAD_METHOD 'startswith'
    0x10,0x40, // LOAD_CONST_STRING '/rom'
    0x36,0x01, // CALL_METHOD 1
    0x43,0x51, // POP_JUMP_IF_TRUE 17
    0xb5, // LOAD_FAST 5
    0x14,0x35, // LOAD_METHOD 'endswith'
    0x10,0x41, // LOAD_CONST_STRING '/lib'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0xb5, // LOAD_FAST 5
    0xc2, // STORE_FAST 2
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x42,0x4b, // JUMP 11
    0x42,0x21, // JUMP -31
    0x12,0x51, // LOAD_GLOBAL 'print'
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb1, // LOAD_FAST 1
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x23,0x0c, // LOAD_CONST_OBJ 12
    0xc1, // STORE_FAST 1
    0x12,0x34, // LOAD_GLOBAL '_install_package'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0x14,0x42, // LOAD_METHOD 'rstrip'
    0x10,0x0a, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x34,0x05, // CALL_FUNCTION 5
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0x12,0x51, // LOAD_GLOBAL 'print'
    0x10,0x43, // LOAD_CONST_STRING 'Done'
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x47, // JUMP 7
    0x12,0x51, // LOAD_GLOBAL 'print'
    0x23,0x0d, // LOAD_CONST_OBJ 13
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_mip___init___install = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_mip___init___install,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 100,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 5,
        .n_kwonly_args = 0,
        .n_def_pos_args = 4,
        .qstr_block_name_idx = 62,
        .line_info = fun_data_mip___init___install + 11,
        .line_info_top = fun_data_mip___init___install + 11,
        .opcodes = fun_data_mip___init___install + 11,
    },
    #endif
    #endif
};
#else
#define proto_fun_mip___init___install fun_data_mip___init___install[0]
#endif

static const mp_raw_code_t *const children_mip___init____lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_mip___init____ensure_path_exists,
    (const mp_raw_code_t *)&proto_fun_mip___init____chunk,
    (const mp_raw_code_t *)&proto_fun_mip___init____check_exists,
    (const mp_raw_code_t *)&proto_fun_mip___init____rewrite_url,
    (const mp_raw_code_t *)&proto_fun_mip___init____download_file,
    (const mp_raw_code_t *)&proto_fun_mip___init____install_json,
    (const mp_raw_code_t *)&proto_fun_mip___init____install_package,
    (const mp_raw_code_t *)&proto_fun_mip___init___install,
};

static const mp_raw_code_truncated_t proto_fun_mip___init____lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_mip___init____lt_module_gt_,
    .children = (void *)&children_mip___init____lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 80,
    .n_children = 8,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_mip___init____lt_module_gt_ + 3,
        .line_info_top = fun_data_mip___init____lt_module_gt_ + 3,
        .opcodes = fun_data_mip___init____lt_module_gt_ + 3,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_mip___init__[91] = {
    MP_QSTR_mip_slash___init___dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_sys,
    MP_QSTR_requests,
    MP_QSTR_const,
    MP_QSTR_micropython,
    MP_QSTR_0_dot_4_dot_2,
    MP_QSTR__ensure_path_exists,
    MP_QSTR_os,
    MP_QSTR_split,
    MP_QSTR__slash_,
    MP_QSTR_pop,
    MP_QSTR_,
    MP_QSTR_stat,
    MP_QSTR_mkdir,
    MP_QSTR__chunk,
    MP_QSTR_readinto,
    MP_QSTR__check_exists,
    MP_QSTR_binascii,
    MP_QSTR_hashlib,
    MP_QSTR_rb,
    MP_QSTR_sha256,
    MP_QSTR_update,
    MP_QSTR_hexlify,
    MP_QSTR_digest,
    MP_QSTR_utf_hyphen_8,
    MP_QSTR__rewrite_url,
    MP_QSTR_HEAD,
    MP_QSTR_startswith,
    MP_QSTR_github_colon_,
    MP_QSTR_join,
    MP_QSTR_gitlab_colon_,
    MP_QSTR__slash__hyphen__slash_raw_slash_,
    MP_QSTR__download_file,
    MP_QSTR_get,
    MP_QSTR_status_code,
    MP_QSTR_Error,
    MP_QSTR_requesting,
    MP_QSTR_Copying_colon_,
    MP_QSTR_wb,
    MP_QSTR_raw,
    MP_QSTR_write,
    MP_QSTR_close,
    MP_QSTR__install_json,
    MP_QSTR_json,
    MP_QSTR_hashes,
    MP_QSTR_Exists_colon_,
    MP_QSTR_format,
    MP_QSTR_rpartition,
    MP_QSTR_urls,
    MP_QSTR__brace_open__brace_close__slash__brace_open__brace_close_,
    MP_QSTR_deps,
    MP_QSTR__install_package,
    MP_QSTR_endswith,
    MP_QSTR__dot_py,
    MP_QSTR__dot_mpy,
    MP_QSTR_rsplit,
    MP_QSTR__dot_json,
    MP_QSTR_latest,
    MP_QSTR_implementation,
    MP_QSTR__mpy,
    MP_QSTR_py,
    MP_QSTR_install,
    MP_QSTR_path,
    MP_QSTR__slash_rom,
    MP_QSTR__slash_lib,
    MP_QSTR_rstrip,
    MP_QSTR_Done,
    MP_QSTR__lt_genexpr_gt_,
    MP_QSTR_allowed_mip_url_prefixes,
    MP_QSTR___version__,
    MP_QSTR_len,
    MP_QSTR_src,
    MP_QSTR_dest,
    MP_QSTR_memoryview,
    MP_QSTR_bytearray,
    MP_QSTR_short_hash,
    MP_QSTR_open,
    MP_QSTR_str,
    MP_QSTR_url,
    MP_QSTR_branch,
    MP_QSTR_print,
    MP_QSTR_package_json_url,
    MP_QSTR_index,
    MP_QSTR_target,
    MP_QSTR_version,
    MP_QSTR_mpy,
    MP_QSTR_any,
    MP_QSTR_package,
    MP_QSTR_hasattr,
    MP_QSTR__star_,
};

// constants
static const mp_rom_obj_tuple_t const_obj_mip___init___0 = {{&mp_type_tuple}, 4, {
    MP_ROM_QSTR(MP_QSTR_http_colon__slash__slash_),
    MP_ROM_QSTR(MP_QSTR_https_colon__slash__slash_),
    MP_ROM_QSTR(MP_QSTR_github_colon_),
    MP_ROM_QSTR(MP_QSTR_gitlab_colon_),
}};
static const mp_obj_str_t const_obj_mip___init___1 = {{&mp_type_str}, 18723, 34, (const byte*)"\x68\x74\x74\x70\x73\x3a\x2f\x2f\x72\x61\x77\x2e\x67\x69\x74\x68\x75\x62\x75\x73\x65\x72\x63\x6f\x6e\x74\x65\x6e\x74\x2e\x63\x6f\x6d\x2f"};
static const mp_obj_str_t const_obj_mip___init___9 = {{&mp_type_str}, 17480, 32, (const byte*)"\x49\x6e\x73\x74\x61\x6c\x6c\x69\x6e\x67\x20\x7b\x7d\x20\x28\x7b\x7d\x29\x20\x66\x72\x6f\x6d\x20\x7b\x7d\x20\x74\x6f\x20\x7b\x7d"};
static const mp_obj_str_t const_obj_mip___init___11 = {{&mp_type_str}, 43215, 34, (const byte*)"\x55\x6e\x61\x62\x6c\x65\x20\x74\x6f\x20\x66\x69\x6e\x64\x20\x6c\x69\x62\x20\x64\x69\x72\x20\x69\x6e\x20\x73\x79\x73\x2e\x70\x61\x74\x68"};
static const mp_obj_str_t const_obj_mip___init___12 = {{&mp_type_str}, 61939, 29, (const byte*)"\x68\x74\x74\x70\x73\x3a\x2f\x2f\x6d\x69\x63\x72\x6f\x70\x79\x74\x68\x6f\x6e\x2e\x6f\x72\x67\x2f\x70\x69\x2f\x76\x32"};
static const mp_obj_str_t const_obj_mip___init___13 = {{&mp_type_str}, 57771, 34, (const byte*)"\x50\x61\x63\x6b\x61\x67\x65\x20\x6d\x61\x79\x20\x62\x65\x20\x70\x61\x72\x74\x69\x61\x6c\x6c\x79\x20\x69\x6e\x73\x74\x61\x6c\x6c\x65\x64"};

// constant table
static const mp_rom_obj_t const_obj_table_data_mip___init__[14] = {
    MP_ROM_PTR(&const_obj_mip___init___0),
    MP_ROM_PTR(&const_obj_mip___init___1),
    MP_ROM_QSTR(MP_QSTR_https_colon__slash__slash_gitlab_dot_com_slash_),
    MP_ROM_QSTR(MP_QSTR_Package_space_not_space_found_colon_),
    MP_ROM_QSTR(MP_QSTR__brace_open__brace_close__slash_file_slash__brace_open__brace_close__slash__brace_open__brace_close_),
    MP_ROM_QSTR(MP_QSTR_File_space_not_space_found_colon__space__brace_open__brace_close__space__brace_open__brace_close_),
    MP_ROM_QSTR(MP_QSTR_Downloading_space__brace_open__brace_close__space_to_space__brace_open__brace_close_),
    MP_ROM_QSTR(MP_QSTR_package_dot_json),
    MP_ROM_QSTR(MP_QSTR_Installing_space__brace_open__brace_close__space_to_space__brace_open__brace_close_),
    MP_ROM_PTR(&const_obj_mip___init___9),
    MP_ROM_QSTR(MP_QSTR__brace_open__brace_close__slash_package_slash__brace_open__brace_close__slash__brace_open__brace_close__slash__brace_open__brace_close__dot_json),
    MP_ROM_PTR(&const_obj_mip___init___11),
    MP_ROM_PTR(&const_obj_mip___init___12),
    MP_ROM_PTR(&const_obj_mip___init___13),
};

static const mp_frozen_module_t frozen_module_mip___init__ = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_mip___init__,
        .obj_table = (mp_obj_t *)&const_obj_table_data_mip___init__,
    },
    .proto_fun = &proto_fun_mip___init____lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module ntptime
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/ntptime.mpy
// - frozen file name: ntptime.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file ntptime.py, scope ntptime__lt_module_gt_
static const byte fun_data_ntptime__lt_module_gt_[48] = {
    0x08,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING 'gmtime'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'time'
    0x1c,0x02, // IMPORT_FROM 'gmtime'
    0x16,0x02, // STORE_NAME 'gmtime'
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x04, // IMPORT_NAME 'socket'
    0x16,0x04, // STORE_NAME 'socket'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x05, // IMPORT_NAME 'struct'
    0x16,0x05, // STORE_NAME 'struct'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x16,0x15, // STORE_NAME 'host'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x16,0x16, // STORE_NAME 'timeout'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x03, // STORE_NAME 'time'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x11, // STORE_NAME 'settime'
    0x10,0x06, // LOAD_CONST_STRING '0.1.1'
    0x16,0x17, // STORE_NAME '__version__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ntptime__lt_module_gt_
// frozen bytecode for file ntptime.py, scope ntptime_time
static const byte fun_data_ntptime_time[157] = {
    0x74,0x02, // prelude
    0x03, // names: time
     // code info
    0x12,0x18, // LOAD_GLOBAL 'bytearray'
    0x22,0x30, // LOAD_CONST_SMALL_INT 48
    0x34,0x01, // CALL_FUNCTION 1
    0xc0, // STORE_FAST 0
    0x9b, // LOAD_CONST_SMALL_INT 27
    0xb0, // LOAD_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x56, // STORE_SUBSCR
    0x12,0x04, // LOAD_GLOBAL 'socket'
    0x14,0x07, // LOAD_METHOD 'getaddrinfo'
    0x12,0x15, // LOAD_GLOBAL 'host'
    0x22,0x80,0x7b, // LOAD_CONST_SMALL_INT 123
    0x36,0x02, // CALL_METHOD 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0xc1, // STORE_FAST 1
    0x12,0x04, // LOAD_GLOBAL 'socket'
    0x14,0x04, // LOAD_METHOD 'socket'
    0x12,0x04, // LOAD_GLOBAL 'socket'
    0x13,0x08, // LOAD_ATTR 'AF_INET'
    0x12,0x04, // LOAD_GLOBAL 'socket'
    0x13,0x09, // LOAD_ATTR 'SOCK_DGRAM'
    0x36,0x02, // CALL_METHOD 2
    0xc2, // STORE_FAST 2
    0x49,0x19, // SETUP_FINALLY 25
    0xb2, // LOAD_FAST 2
    0x14,0x0a, // LOAD_METHOD 'settimeout'
    0x12,0x16, // LOAD_GLOBAL 'timeout'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x14,0x0b, // LOAD_METHOD 'sendto'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x14,0x0c, // LOAD_METHOD 'recv'
    0x22,0x30, // LOAD_CONST_SMALL_INT 48
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0x51, // LOAD_CONST_NONE
    0xb2, // LOAD_FAST 2
    0x14,0x0d, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x5d, // END_FINALLY
    0x12,0x05, // LOAD_GLOBAL 'struct'
    0x14,0x0e, // LOAD_METHOD 'unpack'
    0x10,0x0f, // LOAD_CONST_STRING '!I'
    0xb3, // LOAD_FAST 3
    0xa8, // LOAD_CONST_SMALL_INT 40
    0xac, // LOAD_CONST_SMALL_INT 44
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x36,0x02, // CALL_METHOD 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc4, // STORE_FAST 4
    0x23,0x01, // LOAD_CONST_OBJ 1
    0xc5, // STORE_FAST 5
    0xb4, // LOAD_FAST 4
    0xb5, // LOAD_FAST 5
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb4, // LOAD_FAST 4
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xe5, // BINARY_OP 14 __iadd__
    0xc4, // STORE_FAST 4
    0x12,0x02, // LOAD_GLOBAL 'gmtime'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x34,0x01, // CALL_FUNCTION 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc6, // STORE_FAST 6
    0xb6, // LOAD_FAST 6
    0x22,0x8f,0x50, // LOAD_CONST_SMALL_INT 2000
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xc7, // STORE_FAST 7
    0x42,0x58, // JUMP 24
    0xb6, // LOAD_FAST 6
    0x22,0x8f,0x32, // LOAD_CONST_SMALL_INT 1970
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0x23,0x04, // LOAD_CONST_OBJ 4
    0xc7, // STORE_FAST 7
    0x42,0x4c, // JUMP 12
    0x12,0x19, // LOAD_GLOBAL 'Exception'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x14,0x10, // LOAD_METHOD 'format'
    0xb6, // LOAD_FAST 6
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb4, // LOAD_FAST 4
    0xb7, // LOAD_FAST 7
    0xf3, // BINARY_OP 28 __sub__
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ntptime_time = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ntptime_time,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 157,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 15,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 3,
        .line_info = fun_data_ntptime_time + 3,
        .line_info_top = fun_data_ntptime_time + 3,
        .opcodes = fun_data_ntptime_time + 3,
    },
    #endif
    #endif
};
#else
#define proto_fun_ntptime_time fun_data_ntptime_time[0]
#endif

// child of ntptime__lt_module_gt_
// frozen bytecode for file ntptime.py, scope ntptime_settime
static const byte fun_data_ntptime_settime[57] = {
    0x60,0x02, // prelude
    0x11, // names: settime
     // code info
    0x12,0x03, // LOAD_GLOBAL 'time'
    0x34,0x00, // CALL_FUNCTION 0
    0xc0, // STORE_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x12, // IMPORT_NAME 'machine'
    0xc1, // STORE_FAST 1
    0x12,0x02, // LOAD_GLOBAL 'gmtime'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0xc2, // STORE_FAST 2
    0xb1, // LOAD_FAST 1
    0x14,0x13, // LOAD_METHOD 'RTC'
    0x36,0x00, // CALL_METHOD 0
    0x14,0x14, // LOAD_METHOD 'datetime'
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb2, // LOAD_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb2, // LOAD_FAST 2
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0xb2, // LOAD_FAST 2
    0x86, // LOAD_CONST_SMALL_INT 6
    0x55, // LOAD_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0xb2, // LOAD_FAST 2
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xb2, // LOAD_FAST 2
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0xb2, // LOAD_FAST 2
    0x85, // LOAD_CONST_SMALL_INT 5
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x08, // BUILD_TUPLE 8
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ntptime_settime = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ntptime_settime,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 57,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 17,
        .line_info = fun_data_ntptime_settime + 3,
        .line_info_top = fun_data_ntptime_settime + 3,
        .opcodes = fun_data_ntptime_settime + 3,
    },
    #endif
    #endif
};
#else
#define proto_fun_ntptime_settime fun_data_ntptime_settime[0]
#endif

static const mp_raw_code_t *const children_ntptime__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_ntptime_time,
    (const mp_raw_code_t *)&proto_fun_ntptime_settime,
};

static const mp_raw_code_truncated_t proto_fun_ntptime__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ntptime__lt_module_gt_,
    .children = (void *)&children_ntptime__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 48,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_ntptime__lt_module_gt_ + 3,
        .line_info_top = fun_data_ntptime__lt_module_gt_ + 3,
        .opcodes = fun_data_ntptime__lt_module_gt_ + 3,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_ntptime[26] = {
    MP_QSTR_ntptime_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_gmtime,
    MP_QSTR_time,
    MP_QSTR_socket,
    MP_QSTR_struct,
    MP_QSTR_0_dot_1_dot_1,
    MP_QSTR_getaddrinfo,
    MP_QSTR_AF_INET,
    MP_QSTR_SOCK_DGRAM,
    MP_QSTR_settimeout,
    MP_QSTR_sendto,
    MP_QSTR_recv,
    MP_QSTR_close,
    MP_QSTR_unpack,
    MP_QSTR__bang_I,
    MP_QSTR_format,
    MP_QSTR_settime,
    MP_QSTR_machine,
    MP_QSTR_RTC,
    MP_QSTR_datetime,
    MP_QSTR_host,
    MP_QSTR_timeout,
    MP_QSTR___version__,
    MP_QSTR_bytearray,
    MP_QSTR_Exception,
};

// constants
static const mp_obj_int_t const_obj_ntptime_1 = {{&mp_type_int}, {.neg=0, .fixed_dig=1, .alloc=2, .len=2, .dig=(uint16_t*)(const uint16_t[]){0x7f00,0xe93c}}};
static const mp_obj_int_t const_obj_ntptime_2 = {{&mp_type_int}, {.neg=0, .fixed_dig=1, .alloc=3, .len=3, .dig=(uint16_t*)(const uint16_t[]){0x0,0x0,0x1}}};
static const mp_obj_int_t const_obj_ntptime_3 = {{&mp_type_int}, {.neg=0, .fixed_dig=1, .alloc=2, .len=2, .dig=(uint16_t*)(const uint16_t[]){0xc200,0xbc17}}};
static const mp_obj_int_t const_obj_ntptime_4 = {{&mp_type_int}, {.neg=0, .fixed_dig=1, .alloc=2, .len=2, .dig=(uint16_t*)(const uint16_t[]){0x7e80,0x83aa}}};

// constant table
static const mp_rom_obj_t const_obj_table_data_ntptime[6] = {
    MP_ROM_QSTR(MP_QSTR_pool_dot_ntp_dot_org),
    MP_ROM_PTR(&const_obj_ntptime_1),
    MP_ROM_PTR(&const_obj_ntptime_2),
    MP_ROM_PTR(&const_obj_ntptime_3),
    MP_ROM_PTR(&const_obj_ntptime_4),
    MP_ROM_QSTR(MP_QSTR_Unsupported_space_epoch_colon__space__brace_open__brace_close_),
};

static const mp_frozen_module_t frozen_module_ntptime = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_ntptime,
        .obj_table = (mp_obj_t *)&const_obj_table_data_ntptime,
    },
    .proto_fun = &proto_fun_ntptime__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module ssl
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/ssl.mpy
// - frozen file name: ssl.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file ssl.py, scope ssl__lt_module_gt_
static const byte fun_data_ssl__lt_module_gt_[47] = {
    0x30,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'tls'
    0x16,0x02, // STORE_NAME 'tls'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x03, // LOAD_CONST_STRING '*'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x02, // IMPORT_NAME 'tls'
    0x69, // IMPORT_STAR
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'SSLContext'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x04, // STORE_NAME 'SSLContext'
    0x50, // LOAD_CONST_FALSE
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x11,0x13, // LOAD_NAME 'CERT_NONE'
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x52, // LOAD_CONST_TRUE
    0x2a,0x07, // BUILD_TUPLE 7
    0x53, // LOAD_NULL
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x07, // STORE_NAME 'wrap_socket'
    0x10,0x05, // LOAD_CONST_STRING '0.2.1'
    0x16,0x14, // STORE_NAME '__version__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ssl__lt_module_gt_
// frozen bytecode for file ssl.py, scope ssl_SSLContext
static const byte fun_data_ssl_SSLContext[58] = {
    0x10,0x02, // prelude
    0x04, // names: SSLContext
     // code info
    0x11,0x15, // LOAD_NAME '__name__'
    0x16,0x16, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'SSLContext'
    0x16,0x17, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x0f, // STORE_NAME '__init__'
    0x11,0x18, // LOAD_NAME 'property'
    0x32,0x01, // MAKE_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x16,0x0b, // STORE_NAME 'verify_mode'
    0x11,0x0b, // LOAD_NAME 'verify_mode'
    0x13,0x06, // LOAD_ATTR 'setter'
    0x32,0x02, // MAKE_FUNCTION 2
    0x34,0x01, // CALL_FUNCTION 1
    0x16,0x0b, // STORE_NAME 'verify_mode'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x08, // STORE_NAME 'load_cert_chain'
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x04, // MAKE_FUNCTION_DEFARGS 4
    0x16,0x09, // STORE_NAME 'load_verify_locations'
    0x50, // LOAD_CONST_FALSE
    0x52, // LOAD_CONST_TRUE
    0x51, // LOAD_CONST_NONE
    0x2a,0x03, // BUILD_TUPLE 3
    0x53, // LOAD_NULL
    0x33,0x05, // MAKE_FUNCTION_DEFARGS 5
    0x16,0x07, // STORE_NAME 'wrap_socket'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ssl_SSLContext
// frozen bytecode for file ssl.py, scope ssl_SSLContext___init__
static const byte fun_data_ssl_SSLContext___init__[27] = {
    0xa9,0x80,0x80,0x40,0x04, // prelude
    0x0f,0x20, // names: __init__, self
     // code info
    0x12,0x02, // LOAD_GLOBAL 'tls'
    0x14,0x04, // LOAD_METHOD 'SSLContext'
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x37,0x01, // CALL_METHOD_VAR_KW 1
    0xb0, // LOAD_FAST 0
    0x18,0x10, // STORE_ATTR '_context'
    0x12,0x13, // LOAD_GLOBAL 'CERT_NONE'
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR '_context'
    0x18,0x0b, // STORE_ATTR 'verify_mode'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ssl_SSLContext___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ssl_SSLContext___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 27,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 4,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 15,
        .line_info = fun_data_ssl_SSLContext___init__ + 7,
        .line_info_top = fun_data_ssl_SSLContext___init__ + 7,
        .opcodes = fun_data_ssl_SSLContext___init__ + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_ssl_SSLContext___init__ fun_data_ssl_SSLContext___init__[0]
#endif

// child of ssl_SSLContext
// frozen bytecode for file ssl.py, scope ssl_SSLContext_verify_mode
static const byte fun_data_ssl_SSLContext_verify_mode[10] = {
    0x09,0x04, // prelude
    0x0b,0x20, // names: verify_mode, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR '_context'
    0x13,0x0b, // LOAD_ATTR 'verify_mode'
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ssl_SSLContext_verify_mode = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ssl_SSLContext_verify_mode,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 10,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_ssl_SSLContext_verify_mode + 4,
        .line_info_top = fun_data_ssl_SSLContext_verify_mode + 4,
        .opcodes = fun_data_ssl_SSLContext_verify_mode + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_ssl_SSLContext_verify_mode fun_data_ssl_SSLContext_verify_mode[0]
#endif

// child of ssl_SSLContext
// frozen bytecode for file ssl.py, scope ssl_SSLContext_verify_mode2
static const byte fun_data_ssl_SSLContext_verify_mode2[13] = {
    0x1a,0x06, // prelude
    0x0b,0x20,0x21, // names: verify_mode, self, val
     // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR '_context'
    0x18,0x0b, // STORE_ATTR 'verify_mode'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ssl_SSLContext_verify_mode2 = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ssl_SSLContext_verify_mode2,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 13,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_ssl_SSLContext_verify_mode2 + 5,
        .line_info_top = fun_data_ssl_SSLContext_verify_mode2 + 5,
        .opcodes = fun_data_ssl_SSLContext_verify_mode2 + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_ssl_SSLContext_verify_mode2 fun_data_ssl_SSLContext_verify_mode2[0]
#endif

// child of ssl_SSLContext
// frozen bytecode for file ssl.py, scope ssl_SSLContext_load_cert_chain
static const byte fun_data_ssl_SSLContext_load_cert_chain[74] = {
    0x47,0x08, // prelude
    0x08,0x20,0x22,0x23, // names: load_cert_chain, self, certfile, keyfile
     // code info
    0x12,0x24, // LOAD_GLOBAL 'isinstance'
    0xb1, // LOAD_FAST 1
    0x12,0x25, // LOAD_GLOBAL 'str'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x53, // POP_JUMP_IF_FALSE 19
    0x12,0x26, // LOAD_GLOBAL 'open'
    0xb1, // LOAD_FAST 1
    0x10,0x11, // LOAD_CONST_STRING 'rb'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x08, // SETUP_WITH 8
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x14,0x12, // LOAD_METHOD 'read'
    0x36,0x00, // CALL_METHOD 0
    0xc1, // STORE_FAST 1
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x12,0x24, // LOAD_GLOBAL 'isinstance'
    0xb2, // LOAD_FAST 2
    0x12,0x25, // LOAD_GLOBAL 'str'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x53, // POP_JUMP_IF_FALSE 19
    0x12,0x26, // LOAD_GLOBAL 'open'
    0xb2, // LOAD_FAST 2
    0x10,0x11, // LOAD_CONST_STRING 'rb'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x08, // SETUP_WITH 8
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x14,0x12, // LOAD_METHOD 'read'
    0x36,0x00, // CALL_METHOD 0
    0xc2, // STORE_FAST 2
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR '_context'
    0x14,0x08, // LOAD_METHOD 'load_cert_chain'
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ssl_SSLContext_load_cert_chain = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ssl_SSLContext_load_cert_chain,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 74,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 8,
        .line_info = fun_data_ssl_SSLContext_load_cert_chain + 6,
        .line_info_top = fun_data_ssl_SSLContext_load_cert_chain + 6,
        .opcodes = fun_data_ssl_SSLContext_load_cert_chain + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_ssl_SSLContext_load_cert_chain fun_data_ssl_SSLContext_load_cert_chain[0]
#endif

// child of ssl_SSLContext
// frozen bytecode for file ssl.py, scope ssl_SSLContext_load_verify_locations
static const byte fun_data_ssl_SSLContext_load_verify_locations[41] = {
    0xc7,0x80,0x01,0x08, // prelude
    0x09,0x20,0x27,0x0a, // names: load_verify_locations, self, cafile, cadata
     // code info
    0xb1, // LOAD_FAST 1
    0x44,0x53, // POP_JUMP_IF_FALSE 19
    0x12,0x26, // LOAD_GLOBAL 'open'
    0xb1, // LOAD_FAST 1
    0x10,0x11, // LOAD_CONST_STRING 'rb'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x08, // SETUP_WITH 8
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x14,0x12, // LOAD_METHOD 'read'
    0x36,0x00, // CALL_METHOD 0
    0xc2, // STORE_FAST 2
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR '_context'
    0x14,0x09, // LOAD_METHOD 'load_verify_locations'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ssl_SSLContext_load_verify_locations = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ssl_SSLContext_load_verify_locations,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 41,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_ssl_SSLContext_load_verify_locations + 8,
        .line_info_top = fun_data_ssl_SSLContext_load_verify_locations + 8,
        .opcodes = fun_data_ssl_SSLContext_load_verify_locations + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_ssl_SSLContext_load_verify_locations fun_data_ssl_SSLContext_load_verify_locations[0]
#endif

// child of ssl_SSLContext
// frozen bytecode for file ssl.py, scope ssl_SSLContext_wrap_socket
static const byte fun_data_ssl_SSLContext_wrap_socket[29] = {
    0xe9,0x85,0x01,0x0c, // prelude
    0x07,0x20,0x19,0x0c,0x0d,0x0e, // names: wrap_socket, self, sock, server_side, do_handshake_on_connect, server_hostname
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR '_context'
    0x14,0x07, // LOAD_METHOD 'wrap_socket'
    0xb1, // LOAD_FAST 1
    0x10,0x0c, // LOAD_CONST_STRING 'server_side'
    0xb2, // LOAD_FAST 2
    0x10,0x0d, // LOAD_CONST_STRING 'do_handshake_on_connect'
    0xb3, // LOAD_FAST 3
    0x10,0x0e, // LOAD_CONST_STRING 'server_hostname'
    0xb4, // LOAD_FAST 4
    0x36,0x86,0x01, // CALL_METHOD 769
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ssl_SSLContext_wrap_socket = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ssl_SSLContext_wrap_socket,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 29,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 5,
        .n_kwonly_args = 0,
        .n_def_pos_args = 3,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_ssl_SSLContext_wrap_socket + 10,
        .line_info_top = fun_data_ssl_SSLContext_wrap_socket + 10,
        .opcodes = fun_data_ssl_SSLContext_wrap_socket + 10,
    },
    #endif
    #endif
};
#else
#define proto_fun_ssl_SSLContext_wrap_socket fun_data_ssl_SSLContext_wrap_socket[0]
#endif

static const mp_raw_code_t *const children_ssl_SSLContext[] = {
    (const mp_raw_code_t *)&proto_fun_ssl_SSLContext___init__,
    (const mp_raw_code_t *)&proto_fun_ssl_SSLContext_verify_mode,
    (const mp_raw_code_t *)&proto_fun_ssl_SSLContext_verify_mode2,
    (const mp_raw_code_t *)&proto_fun_ssl_SSLContext_load_cert_chain,
    (const mp_raw_code_t *)&proto_fun_ssl_SSLContext_load_verify_locations,
    (const mp_raw_code_t *)&proto_fun_ssl_SSLContext_wrap_socket,
};

static const mp_raw_code_truncated_t proto_fun_ssl_SSLContext = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ssl_SSLContext,
    .children = (void *)&children_ssl_SSLContext,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 58,
    .n_children = 6,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_ssl_SSLContext + 3,
        .line_info_top = fun_data_ssl_SSLContext + 3,
        .opcodes = fun_data_ssl_SSLContext + 3,
    },
    #endif
    #endif
};

// child of ssl__lt_module_gt_
// frozen bytecode for file ssl.py, scope ssl_wrap_socket
static const byte fun_data_ssl_wrap_socket[76] = {
    0x88,0x91,0x85,0x01,0x12, // prelude
    0x07,0x19,0x0c,0x1a,0x1b,0x1c,0x0a,0x0e,0x1d, // names: wrap_socket, sock, server_side, key, cert, cert_reqs, cadata, server_hostname, do_handshake
     // code info
    0x12,0x04, // LOAD_GLOBAL 'SSLContext'
    0xb1, // LOAD_FAST 1
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x12,0x1e, // LOAD_GLOBAL 'PROTOCOL_TLS_SERVER'
    0x42,0x42, // JUMP 2
    0x12,0x1f, // LOAD_GLOBAL 'PROTOCOL_TLS_CLIENT'
    0x34,0x01, // CALL_FUNCTION 1
    0xc8, // STORE_FAST 8
    0xb3, // LOAD_FAST 3
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0xb2, // LOAD_FAST 2
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0xb8, // LOAD_FAST 8
    0x14,0x08, // LOAD_METHOD 'load_cert_chain'
    0xb3, // LOAD_FAST 3
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0xb5, // LOAD_FAST 5
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0xb8, // LOAD_FAST 8
    0x14,0x09, // LOAD_METHOD 'load_verify_locations'
    0x10,0x0a, // LOAD_CONST_STRING 'cadata'
    0xb5, // LOAD_FAST 5
    0x36,0x82,0x00, // CALL_METHOD 256
    0x59, // POP_TOP
    0xb4, // LOAD_FAST 4
    0xb8, // LOAD_FAST 8
    0x18,0x0b, // STORE_ATTR 'verify_mode'
    0xb8, // LOAD_FAST 8
    0x14,0x07, // LOAD_METHOD 'wrap_socket'
    0xb0, // LOAD_FAST 0
    0x10,0x0c, // LOAD_CONST_STRING 'server_side'
    0xb1, // LOAD_FAST 1
    0x10,0x0d, // LOAD_CONST_STRING 'do_handshake_on_connect'
    0xb7, // LOAD_FAST 7
    0x10,0x0e, // LOAD_CONST_STRING 'server_hostname'
    0xb6, // LOAD_FAST 6
    0x36,0x86,0x01, // CALL_METHOD 769
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ssl_wrap_socket = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ssl_wrap_socket,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 76,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 18,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 8,
        .n_kwonly_args = 0,
        .n_def_pos_args = 7,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_ssl_wrap_socket + 14,
        .line_info_top = fun_data_ssl_wrap_socket + 14,
        .opcodes = fun_data_ssl_wrap_socket + 14,
    },
    #endif
    #endif
};
#else
#define proto_fun_ssl_wrap_socket fun_data_ssl_wrap_socket[0]
#endif

static const mp_raw_code_t *const children_ssl__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_ssl_SSLContext,
    (const mp_raw_code_t *)&proto_fun_ssl_wrap_socket,
};

static const mp_raw_code_truncated_t proto_fun_ssl__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ssl__lt_module_gt_,
    .children = (void *)&children_ssl__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 47,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_ssl__lt_module_gt_ + 3,
        .line_info_top = fun_data_ssl__lt_module_gt_ + 3,
        .opcodes = fun_data_ssl__lt_module_gt_ + 3,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_ssl[40] = {
    MP_QSTR_ssl_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_tls,
    MP_QSTR__star_,
    MP_QSTR_SSLContext,
    MP_QSTR_0_dot_2_dot_1,
    MP_QSTR_setter,
    MP_QSTR_wrap_socket,
    MP_QSTR_load_cert_chain,
    MP_QSTR_load_verify_locations,
    MP_QSTR_cadata,
    MP_QSTR_verify_mode,
    MP_QSTR_server_side,
    MP_QSTR_do_handshake_on_connect,
    MP_QSTR_server_hostname,
    MP_QSTR___init__,
    MP_QSTR__context,
    MP_QSTR_rb,
    MP_QSTR_read,
    MP_QSTR_CERT_NONE,
    MP_QSTR___version__,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_property,
    MP_QSTR_sock,
    MP_QSTR_key,
    MP_QSTR_cert,
    MP_QSTR_cert_reqs,
    MP_QSTR_do_handshake,
    MP_QSTR_PROTOCOL_TLS_SERVER,
    MP_QSTR_PROTOCOL_TLS_CLIENT,
    MP_QSTR_self,
    MP_QSTR_val,
    MP_QSTR_certfile,
    MP_QSTR_keyfile,
    MP_QSTR_isinstance,
    MP_QSTR_str,
    MP_QSTR_open,
    MP_QSTR_cafile,
};

static const mp_frozen_module_t frozen_module_ssl = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_ssl,
        .obj_table = NULL,
    },
    .proto_fun = &proto_fun_ssl__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module webrepl
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/webrepl.mpy
// - frozen file name: webrepl.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file webrepl.py, scope webrepl__lt_module_gt_
static const byte fun_data_webrepl__lt_module_gt_[128] = {
    0x10,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'binascii'
    0x16,0x02, // STORE_NAME 'binascii'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'hashlib'
    0x16,0x03, // STORE_NAME 'hashlib'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x04, // LOAD_CONST_STRING 'const'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x05, // IMPORT_NAME 'micropython'
    0x1c,0x04, // IMPORT_FROM 'const'
    0x16,0x04, // STORE_NAME 'const'
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x06, // IMPORT_NAME 'network'
    0x16,0x06, // STORE_NAME 'network'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x07, // IMPORT_NAME 'os'
    0x16,0x07, // STORE_NAME 'os'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x08, // IMPORT_NAME 'socket'
    0x16,0x08, // STORE_NAME 'socket'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x09, // IMPORT_NAME 'sys'
    0x16,0x09, // STORE_NAME 'sys'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x0a, // IMPORT_NAME 'websocket'
    0x16,0x0a, // STORE_NAME 'websocket'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x0b, // IMPORT_NAME '_webrepl'
    0x16,0x0b, // STORE_NAME '_webrepl'
    0x51, // LOAD_CONST_NONE
    0x17,0x39, // STORE_GLOBAL 'listen_s'
    0x51, // LOAD_CONST_NONE
    0x17,0x3a, // STORE_GLOBAL 'client_s'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x16,0x3b, // STORE_NAME 'DEBUG'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x17,0x3c, // STORE_GLOBAL 'static_host'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x0d, // STORE_NAME 'server_handshake'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x1b, // STORE_NAME 'send_html'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x1d, // STORE_NAME 'setup_conn'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x2a, // STORE_NAME 'accept_conn'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x30, // STORE_NAME 'stop'
    0x22,0x80,0xc0,0x4a, // LOAD_CONST_SMALL_INT 8266
    0x51, // LOAD_CONST_NONE
    0x11,0x2a, // LOAD_NAME 'accept_conn'
    0x2a,0x03, // BUILD_TUPLE 3
    0x53, // LOAD_NULL
    0x33,0x05, // MAKE_FUNCTION_DEFARGS 5
    0x16,0x31, // STORE_NAME 'start'
    0x22,0x80,0xc0,0x4a, // LOAD_CONST_SMALL_INT 8266
    0x51, // LOAD_CONST_NONE
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x06, // MAKE_FUNCTION_DEFARGS 6
    0x16,0x36, // STORE_NAME 'start_foreground'
    0x10,0x0c, // LOAD_CONST_STRING '0.1.0'
    0x16,0x3d, // STORE_NAME '__version__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of webrepl__lt_module_gt_
// frozen bytecode for file webrepl.py, scope webrepl_server_handshake
static const byte fun_data_webrepl_server_handshake[251] = {
    0x71,0x04, // prelude
    0x0d,0x3e, // names: server_handshake, cl
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x0e, // LOAD_METHOD 'makefile'
    0x10,0x0f, // LOAD_CONST_STRING 'rwb'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x02, // CALL_METHOD 2
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x10, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0xc2, // STORE_FAST 2
    0x12,0x3b, // LOAD_GLOBAL 'DEBUG'
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x12,0x09, // LOAD_GLOBAL 'sys'
    0x13,0x11, // LOAD_ATTR 'stdout'
    0x14,0x12, // LOAD_METHOD 'write'
    0x12,0x3f, // LOAD_GLOBAL 'repr'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0xc3, // STORE_FAST 3
    0x50, // LOAD_CONST_FALSE
    0xc4, // STORE_FAST 4
    0x50, // LOAD_CONST_FALSE
    0xc5, // STORE_FAST 5
    0xb1, // LOAD_FAST 1
    0x14,0x10, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
    0xb2, // LOAD_FAST 2
    0x23,0x01, // LOAD_CONST_OBJ 1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x42,0xd8,0x80, // JUMP 88
    0x12,0x3b, // LOAD_GLOBAL 'DEBUG'
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x12,0x09, // LOAD_GLOBAL 'sys'
    0x13,0x11, // LOAD_ATTR 'stdout'
    0x14,0x12, // LOAD_METHOD 'write'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x32,0x00, // MAKE_FUNCTION 0
    0xb2, // LOAD_FAST 2
    0x14,0x13, // LOAD_METHOD 'split'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x02, // CALL_METHOD 2
    0x34,0x01, // CALL_FUNCTION 1
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc6, // STORE_FAST 6
    0xc7, // STORE_FAST 7
    0x12,0x3b, // LOAD_GLOBAL 'DEBUG'
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0x12,0x40, // LOAD_GLOBAL 'print'
    0xb6, // LOAD_FAST 6
    0xb7, // LOAD_FAST 7
    0x2a,0x02, // BUILD_TUPLE 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0xb6, // LOAD_FAST 6
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0xb7, // LOAD_FAST 7
    0xc3, // STORE_FAST 3
    0x42,0x60, // JUMP 32
    0xb6, // LOAD_FAST 6
    0x23,0x04, // LOAD_CONST_OBJ 4
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0x23,0x05, // LOAD_CONST_OBJ 5
    0xb7, // LOAD_FAST 7
    0xdd, // BINARY_OP 6 <in>
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x52, // LOAD_CONST_TRUE
    0xc4, // STORE_FAST 4
    0x42,0x50, // JUMP 16
    0xb6, // LOAD_FAST 6
    0x23,0x05, // LOAD_CONST_OBJ 5
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0xb7, // LOAD_FAST 7
    0x23,0x06, // LOAD_CONST_OBJ 6
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x52, // LOAD_CONST_TRUE
    0xc5, // STORE_FAST 5
    0x42,0x40, // JUMP 0
    0x42,0x94,0x7f, // JUMP -108
    0xb4, // LOAD_FAST 4
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0xb5, // LOAD_FAST 5
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0xb3, // LOAD_FAST 3
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
    0x12,0x3b, // LOAD_GLOBAL 'DEBUG'
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x12,0x40, // LOAD_GLOBAL 'print'
    0x23,0x07, // LOAD_CONST_OBJ 7
    0xb3, // LOAD_FAST 3
    0x12,0x41, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0x12,0x03, // LOAD_GLOBAL 'hashlib'
    0x14,0x14, // LOAD_METHOD 'sha1'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0xc8, // STORE_FAST 8
    0xb8, // LOAD_FAST 8
    0x14,0x15, // LOAD_METHOD 'update'
    0x23,0x08, // LOAD_CONST_OBJ 8
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb8, // LOAD_FAST 8
    0x14,0x16, // LOAD_METHOD 'digest'
    0x36,0x00, // CALL_METHOD 0
    0xc9, // STORE_FAST 9
    0x12,0x02, // LOAD_GLOBAL 'binascii'
    0x14,0x17, // LOAD_METHOD 'b2a_base64'
    0xb9, // LOAD_FAST 9
    0x36,0x01, // CALL_METHOD 1
    0x51, // LOAD_CONST_NONE
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xc9, // STORE_FAST 9
    0x12,0x3b, // LOAD_GLOBAL 'DEBUG'
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x12,0x40, // LOAD_GLOBAL 'print'
    0x10,0x18, // LOAD_CONST_STRING 'respkey:'
    0xb9, // LOAD_FAST 9
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD 'send'
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD 'send'
    0xb9, // LOAD_FAST 9
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD 'send'
    0x10,0x1a, // LOAD_CONST_STRING '\r\n\r\n'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
};
// child of webrepl_server_handshake
// frozen bytecode for file webrepl.py, scope webrepl_server_handshake__lt_listcomp_gt_
static const byte fun_data_webrepl_server_handshake__lt_listcomp_gt_[21] = {
    0x41,0x04, // prelude
    0x37,0x46, // names: <listcomp>, *
     // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x0a, // FOR_ITER 10
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x38, // LOAD_METHOD 'strip'
    0x36,0x00, // CALL_METHOD 0
    0x2f,0x14, // STORE_COMP 20
    0x42,0x34, // JUMP -12
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_webrepl_server_handshake__lt_listcomp_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl_server_handshake__lt_listcomp_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 21,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 55,
        .line_info = fun_data_webrepl_server_handshake__lt_listcomp_gt_ + 4,
        .line_info_top = fun_data_webrepl_server_handshake__lt_listcomp_gt_ + 4,
        .opcodes = fun_data_webrepl_server_handshake__lt_listcomp_gt_ + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_webrepl_server_handshake__lt_listcomp_gt_ fun_data_webrepl_server_handshake__lt_listcomp_gt_[0]
#endif

static const mp_raw_code_t *const children_webrepl_server_handshake[] = {
    (const mp_raw_code_t *)&proto_fun_webrepl_server_handshake__lt_listcomp_gt_,
};

static const mp_raw_code_truncated_t proto_fun_webrepl_server_handshake = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl_server_handshake,
    .children = (void *)&children_webrepl_server_handshake,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 251,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 15,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_webrepl_server_handshake + 4,
        .line_info_top = fun_data_webrepl_server_handshake + 4,
        .opcodes = fun_data_webrepl_server_handshake + 4,
    },
    #endif
    #endif
};

// child of webrepl__lt_module_gt_
// frozen bytecode for file webrepl.py, scope webrepl_send_html
static const byte fun_data_webrepl_send_html[36] = {
    0x19,0x04, // prelude
    0x1b,0x3e, // names: send_html, cl
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD 'send'
    0x23,0x0a, // LOAD_CONST_OBJ 10
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD 'send'
    0x12,0x3c, // LOAD_GLOBAL 'static_host'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD 'send'
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x1c, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_webrepl_send_html = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl_send_html,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 36,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 27,
        .line_info = fun_data_webrepl_send_html + 4,
        .line_info_top = fun_data_webrepl_send_html + 4,
        .opcodes = fun_data_webrepl_send_html + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_webrepl_send_html fun_data_webrepl_send_html[0]
#endif

// child of webrepl__lt_module_gt_
// frozen bytecode for file webrepl.py, scope webrepl_setup_conn
static const byte fun_data_webrepl_setup_conn[133] = {
    0x6a,0x06, // prelude
    0x1d,0x42,0x43, // names: setup_conn, port, accept_handler
     // code info
    0x12,0x08, // LOAD_GLOBAL 'socket'
    0x14,0x08, // LOAD_METHOD 'socket'
    0x36,0x00, // CALL_METHOD 0
    0x17,0x39, // STORE_GLOBAL 'listen_s'
    0x12,0x39, // LOAD_GLOBAL 'listen_s'
    0x14,0x1e, // LOAD_METHOD 'setsockopt'
    0x12,0x08, // LOAD_GLOBAL 'socket'
    0x13,0x1f, // LOAD_ATTR 'SOL_SOCKET'
    0x12,0x08, // LOAD_GLOBAL 'socket'
    0x13,0x20, // LOAD_ATTR 'SO_REUSEADDR'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x03, // CALL_METHOD 3
    0x59, // POP_TOP
    0x12,0x08, // LOAD_GLOBAL 'socket'
    0x14,0x21, // LOAD_METHOD 'getaddrinfo'
    0x10,0x22, // LOAD_CONST_STRING '0.0.0.0'
    0xb0, // LOAD_FAST 0
    0x36,0x02, // CALL_METHOD 2
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0xc3, // STORE_FAST 3
    0x12,0x39, // LOAD_GLOBAL 'listen_s'
    0x14,0x23, // LOAD_METHOD 'bind'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x39, // LOAD_GLOBAL 'listen_s'
    0x14,0x24, // LOAD_METHOD 'listen'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0x12,0x39, // LOAD_GLOBAL 'listen_s'
    0x14,0x1e, // LOAD_METHOD 'setsockopt'
    0x12,0x08, // LOAD_GLOBAL 'socket'
    0x13,0x1f, // LOAD_ATTR 'SOL_SOCKET'
    0x94, // LOAD_CONST_SMALL_INT 20
    0xb1, // LOAD_FAST 1
    0x36,0x03, // CALL_METHOD 3
    0x59, // POP_TOP
    0x12,0x06, // LOAD_GLOBAL 'network'
    0x13,0x25, // LOAD_ATTR 'WLAN'
    0x13,0x26, // LOAD_ATTR 'IF_AP'
    0x12,0x06, // LOAD_GLOBAL 'network'
    0x13,0x25, // LOAD_ATTR 'WLAN'
    0x13,0x27, // LOAD_ATTR 'IF_STA'
    0x2a,0x02, // BUILD_TUPLE 2
    0x5f, // GET_ITER_STACK
    0x4b,0x24, // FOR_ITER 36
    0xc4, // STORE_FAST 4
    0x12,0x06, // LOAD_GLOBAL 'network'
    0x14,0x25, // LOAD_METHOD 'WLAN'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x14,0x28, // LOAD_METHOD 'active'
    0x36,0x00, // CALL_METHOD 0
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0x12,0x40, // LOAD_GLOBAL 'print'
    0x23,0x0c, // LOAD_CONST_OBJ 12
    0xb5, // LOAD_FAST 5
    0x14,0x29, // LOAD_METHOD 'ifconfig'
    0x36,0x00, // CALL_METHOD 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x1a, // JUMP -38
    0x12,0x39, // LOAD_GLOBAL 'listen_s'
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_webrepl_setup_conn = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl_setup_conn,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 133,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 29,
        .line_info = fun_data_webrepl_setup_conn + 5,
        .line_info_top = fun_data_webrepl_setup_conn + 5,
        .opcodes = fun_data_webrepl_setup_conn + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_webrepl_setup_conn fun_data_webrepl_setup_conn[0]
#endif

// child of webrepl__lt_module_gt_
// frozen bytecode for file webrepl.py, scope webrepl_accept_conn
static const byte fun_data_webrepl_accept_conn[135] = {
    0x49,0x04, // prelude
    0x2a,0x44, // names: accept_conn, listen_sock
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x2b, // LOAD_METHOD 'accept'
    0x36,0x00, // CALL_METHOD 0
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc1, // STORE_FAST 1
    0xc2, // STORE_FAST 2
    0x12,0x0d, // LOAD_GLOBAL 'server_handshake'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x48, // POP_JUMP_IF_TRUE 8
    0x12,0x1b, // LOAD_GLOBAL 'send_html'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
    0x12,0x07, // LOAD_GLOBAL 'os'
    0x14,0x2c, // LOAD_METHOD 'dupterm'
    0x51, // LOAD_CONST_NONE
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0x12,0x07, // LOAD_GLOBAL 'os'
    0x14,0x2c, // LOAD_METHOD 'dupterm'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0x12,0x40, // LOAD_GLOBAL 'print'
    0x23,0x0d, // LOAD_CONST_OBJ 13
    0xb2, // LOAD_FAST 2
    0x10,0x2d, // LOAD_CONST_STRING 'rejected'
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x14,0x1c, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
    0x12,0x40, // LOAD_GLOBAL 'print'
    0x23,0x0e, // LOAD_CONST_OBJ 14
    0xb2, // LOAD_FAST 2
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x17,0x3a, // STORE_GLOBAL 'client_s'
    0x12,0x0a, // LOAD_GLOBAL 'websocket'
    0x14,0x0a, // LOAD_METHOD 'websocket'
    0xb1, // LOAD_FAST 1
    0x52, // LOAD_CONST_TRUE
    0x36,0x02, // CALL_METHOD 2
    0xc4, // STORE_FAST 4
    0x12,0x0b, // LOAD_GLOBAL '_webrepl'
    0x14,0x0b, // LOAD_METHOD '_webrepl'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0xc4, // STORE_FAST 4
    0xb1, // LOAD_FAST 1
    0x14,0x2e, // LOAD_METHOD 'setblocking'
    0x50, // LOAD_CONST_FALSE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x45, // LOAD_GLOBAL 'hasattr'
    0x12,0x07, // LOAD_GLOBAL 'os'
    0x23,0x0f, // LOAD_CONST_OBJ 15
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x4f, // POP_JUMP_IF_FALSE 15
    0xb1, // LOAD_FAST 1
    0x14,0x1e, // LOAD_METHOD 'setsockopt'
    0x12,0x08, // LOAD_GLOBAL 'socket'
    0x13,0x1f, // LOAD_ATTR 'SOL_SOCKET'
    0x94, // LOAD_CONST_SMALL_INT 20
    0x12,0x07, // LOAD_GLOBAL 'os'
    0x13,0x2f, // LOAD_ATTR 'dupterm_notify'
    0x36,0x03, // CALL_METHOD 3
    0x59, // POP_TOP
    0x12,0x07, // LOAD_GLOBAL 'os'
    0x14,0x2c, // LOAD_METHOD 'dupterm'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_webrepl_accept_conn = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl_accept_conn,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 135,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 42,
        .line_info = fun_data_webrepl_accept_conn + 4,
        .line_info_top = fun_data_webrepl_accept_conn + 4,
        .opcodes = fun_data_webrepl_accept_conn + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_webrepl_accept_conn fun_data_webrepl_accept_conn[0]
#endif

// child of webrepl__lt_module_gt_
// frozen bytecode for file webrepl.py, scope webrepl_stop
static const byte fun_data_webrepl_stop[35] = {
    0x10,0x02, // prelude
    0x30, // names: stop
     // code info
    0x12,0x07, // LOAD_GLOBAL 'os'
    0x14,0x2c, // LOAD_METHOD 'dupterm'
    0x51, // LOAD_CONST_NONE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x3a, // LOAD_GLOBAL 'client_s'
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x3a, // LOAD_GLOBAL 'client_s'
    0x14,0x1c, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x12,0x39, // LOAD_GLOBAL 'listen_s'
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x39, // LOAD_GLOBAL 'listen_s'
    0x14,0x1c, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_webrepl_stop = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl_stop,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 35,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 48,
        .line_info = fun_data_webrepl_stop + 3,
        .line_info_top = fun_data_webrepl_stop + 3,
        .opcodes = fun_data_webrepl_stop + 3,
    },
    #endif
    #endif
};
#else
#define proto_fun_webrepl_stop fun_data_webrepl_stop[0]
#endif

// child of webrepl__lt_module_gt_
// frozen bytecode for file webrepl.py, scope webrepl_start
static const byte fun_data_webrepl_start[119] = {
    0xd7,0x81,0x01,0x08, // prelude
    0x31,0x42,0x35,0x43, // names: start, port, password, accept_handler
     // code info
    0x12,0x30, // LOAD_GLOBAL 'stop'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x66, // POP_JUMP_IF_FALSE 38
    0x48,0x19, // SETUP_EXCEPT 25
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x32, // IMPORT_NAME 'webrepl_cfg'
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x13,0x33, // LOAD_ATTR 'PASS'
    0xc3, // STORE_FAST 3
    0x12,0x45, // LOAD_GLOBAL 'hasattr'
    0xb4, // LOAD_FAST 4
    0x10,0x34, // LOAD_CONST_STRING 'BASE'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb4, // LOAD_FAST 4
    0x13,0x34, // LOAD_ATTR 'BASE'
    0x17,0x3c, // STORE_GLOBAL 'static_host'
    0x4a,0x0b, // POP_EXCEPT_JUMP 11
    0x59, // POP_TOP
    0x12,0x40, // LOAD_GLOBAL 'print'
    0x23,0x10, // LOAD_CONST_OBJ 16
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x12,0x0b, // LOAD_GLOBAL '_webrepl'
    0x14,0x35, // LOAD_METHOD 'password'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x1d, // LOAD_GLOBAL 'setup_conn'
    0xb0, // LOAD_FAST 0
    0xb2, // LOAD_FAST 2
    0x34,0x02, // CALL_FUNCTION 2
    0xc5, // STORE_FAST 5
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0x12,0x40, // LOAD_GLOBAL 'print'
    0x23,0x11, // LOAD_CONST_OBJ 17
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x40, // JUMP 0
    0x12,0x2a, // LOAD_GLOBAL 'accept_conn'
    0xb5, // LOAD_FAST 5
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x39, // POP_JUMP_IF_FALSE -7
    0x42,0x55, // JUMP 21
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0x12,0x40, // LOAD_GLOBAL 'print'
    0x23,0x12, // LOAD_CONST_OBJ 18
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x47, // JUMP 7
    0x12,0x40, // LOAD_GLOBAL 'print'
    0x23,0x13, // LOAD_CONST_OBJ 19
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_webrepl_start = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl_start,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 119,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 3,
        .qstr_block_name_idx = 49,
        .line_info = fun_data_webrepl_start + 8,
        .line_info_top = fun_data_webrepl_start + 8,
        .opcodes = fun_data_webrepl_start + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_webrepl_start fun_data_webrepl_start[0]
#endif

// child of webrepl__lt_module_gt_
// frozen bytecode for file webrepl.py, scope webrepl_start_foreground
static const byte fun_data_webrepl_start_foreground[17] = {
    0xaa,0x80,0x01,0x06, // prelude
    0x36,0x42,0x35, // names: start_foreground, port, password
     // code info
    0x12,0x31, // LOAD_GLOBAL 'start'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0x34,0x03, // CALL_FUNCTION 3
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_webrepl_start_foreground = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl_start_foreground,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 17,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 54,
        .line_info = fun_data_webrepl_start_foreground + 7,
        .line_info_top = fun_data_webrepl_start_foreground + 7,
        .opcodes = fun_data_webrepl_start_foreground + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_webrepl_start_foreground fun_data_webrepl_start_foreground[0]
#endif

static const mp_raw_code_t *const children_webrepl__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_webrepl_server_handshake,
    (const mp_raw_code_t *)&proto_fun_webrepl_send_html,
    (const mp_raw_code_t *)&proto_fun_webrepl_setup_conn,
    (const mp_raw_code_t *)&proto_fun_webrepl_accept_conn,
    (const mp_raw_code_t *)&proto_fun_webrepl_stop,
    (const mp_raw_code_t *)&proto_fun_webrepl_start,
    (const mp_raw_code_t *)&proto_fun_webrepl_start_foreground,
};

static const mp_raw_code_truncated_t proto_fun_webrepl__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl__lt_module_gt_,
    .children = (void *)&children_webrepl__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 128,
    .n_children = 7,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_webrepl__lt_module_gt_ + 3,
        .line_info_top = fun_data_webrepl__lt_module_gt_ + 3,
        .opcodes = fun_data_webrepl__lt_module_gt_ + 3,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_webrepl[71] = {
    MP_QSTR_webrepl_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_binascii,
    MP_QSTR_hashlib,
    MP_QSTR_const,
    MP_QSTR_micropython,
    MP_QSTR_network,
    MP_QSTR_os,
    MP_QSTR_socket,
    MP_QSTR_sys,
    MP_QSTR_websocket,
    MP_QSTR__webrepl,
    MP_QSTR_0_dot_1_dot_0,
    MP_QSTR_server_handshake,
    MP_QSTR_makefile,
    MP_QSTR_rwb,
    MP_QSTR_readline,
    MP_QSTR_stdout,
    MP_QSTR_write,
    MP_QSTR_split,
    MP_QSTR_sha1,
    MP_QSTR_update,
    MP_QSTR_digest,
    MP_QSTR_b2a_base64,
    MP_QSTR_respkey_colon_,
    MP_QSTR_send,
    MP_QSTR__0x0d__0x0a__0x0d__0x0a_,
    MP_QSTR_send_html,
    MP_QSTR_close,
    MP_QSTR_setup_conn,
    MP_QSTR_setsockopt,
    MP_QSTR_SOL_SOCKET,
    MP_QSTR_SO_REUSEADDR,
    MP_QSTR_getaddrinfo,
    MP_QSTR_0_dot_0_dot_0_dot_0,
    MP_QSTR_bind,
    MP_QSTR_listen,
    MP_QSTR_WLAN,
    MP_QSTR_IF_AP,
    MP_QSTR_IF_STA,
    MP_QSTR_active,
    MP_QSTR_ifconfig,
    MP_QSTR_accept_conn,
    MP_QSTR_accept,
    MP_QSTR_dupterm,
    MP_QSTR_rejected,
    MP_QSTR_setblocking,
    MP_QSTR_dupterm_notify,
    MP_QSTR_stop,
    MP_QSTR_start,
    MP_QSTR_webrepl_cfg,
    MP_QSTR_PASS,
    MP_QSTR_BASE,
    MP_QSTR_password,
    MP_QSTR_start_foreground,
    MP_QSTR__lt_listcomp_gt_,
    MP_QSTR_strip,
    MP_QSTR_listen_s,
    MP_QSTR_client_s,
    MP_QSTR_DEBUG,
    MP_QSTR_static_host,
    MP_QSTR___version__,
    MP_QSTR_cl,
    MP_QSTR_repr,
    MP_QSTR_print,
    MP_QSTR_len,
    MP_QSTR_port,
    MP_QSTR_accept_handler,
    MP_QSTR_listen_sock,
    MP_QSTR_hasattr,
    MP_QSTR__star_,
};

// constants
static const mp_obj_str_t const_obj_webrepl_0 = {{&mp_type_str}, 33749, 32, (const byte*)"\x68\x74\x74\x70\x73\x3a\x2f\x2f\x6d\x69\x63\x72\x6f\x70\x79\x74\x68\x6f\x6e\x2e\x6f\x72\x67\x2f\x77\x65\x62\x72\x65\x70\x6c\x2f"};
static const mp_obj_str_t const_obj_webrepl_1 = {{&mp_type_bytes}, 27298, 2, (const byte*)"\x0d\x0a"};
static const mp_obj_str_t const_obj_webrepl_2 = {{&mp_type_bytes}, 46495, 1, (const byte*)"\x3a"};
static const mp_obj_str_t const_obj_webrepl_3 = {{&mp_type_bytes}, 24914, 17, (const byte*)"\x53\x65\x63\x2d\x57\x65\x62\x53\x6f\x63\x6b\x65\x74\x2d\x4b\x65\x79"};
static const mp_obj_str_t const_obj_webrepl_4 = {{&mp_type_bytes}, 42451, 10, (const byte*)"\x43\x6f\x6e\x6e\x65\x63\x74\x69\x6f\x6e"};
static const mp_obj_str_t const_obj_webrepl_5 = {{&mp_type_bytes}, 6069, 7, (const byte*)"\x55\x70\x67\x72\x61\x64\x65"};
static const mp_obj_str_t const_obj_webrepl_6 = {{&mp_type_bytes}, 36240, 9, (const byte*)"\x77\x65\x62\x73\x6f\x63\x6b\x65\x74"};
static const mp_obj_str_t const_obj_webrepl_8 = {{&mp_type_bytes}, 43888, 36, (const byte*)"\x32\x35\x38\x45\x41\x46\x41\x35\x2d\x45\x39\x31\x34\x2d\x34\x37\x44\x41\x2d\x39\x35\x43\x41\x2d\x43\x35\x41\x42\x30\x44\x43\x38\x35\x42\x31\x31"};
static const mp_obj_str_t const_obj_webrepl_9 = {{&mp_type_bytes}, 65413, 97, (const byte*)"\x48\x54\x54\x50\x2f\x31\x2e\x31\x20\x31\x30\x31\x20\x53\x77\x69\x74\x63\x68\x69\x6e\x67\x20\x50\x72\x6f\x74\x6f\x63\x6f\x6c\x73\x0d\x0a\x55\x70\x67\x72\x61\x64\x65\x3a\x20\x77\x65\x62\x73\x6f\x63\x6b\x65\x74\x0d\x0a\x43\x6f\x6e\x6e\x65\x63\x74\x69\x6f\x6e\x3a\x20\x55\x70\x67\x72\x61\x64\x65\x0d\x0a\x53\x65\x63\x2d\x57\x65\x62\x53\x6f\x63\x6b\x65\x74\x2d\x41\x63\x63\x65\x70\x74\x3a\x20"};
static const mp_obj_str_t const_obj_webrepl_10 = {{&mp_type_bytes}, 46084, 31, (const byte*)"\x48\x54\x54\x50\x2f\x31\x2e\x30\x20\x32\x30\x30\x20\x4f\x4b\x0d\x0a\x0d\x0a\x3c\x62\x61\x73\x65\x20\x68\x72\x65\x66\x3d\x22"};
static const mp_obj_str_t const_obj_webrepl_11 = {{&mp_type_bytes}, 1387, 55, (const byte*)"\x22\x3e\x3c\x2f\x62\x61\x73\x65\x3e\x0d\x0a\x3c\x73\x63\x72\x69\x70\x74\x20\x73\x72\x63\x3d\x22\x77\x65\x62\x72\x65\x70\x6c\x5f\x63\x6f\x6e\x74\x65\x6e\x74\x2e\x6a\x73\x22\x3e\x3c\x2f\x73\x63\x72\x69\x70\x74\x3e\x0d\x0a"};
static const mp_obj_str_t const_obj_webrepl_12 = {{&mp_type_str}, 9563, 39, (const byte*)"\x57\x65\x62\x52\x45\x50\x4c\x20\x73\x65\x72\x76\x65\x72\x20\x73\x74\x61\x72\x74\x65\x64\x20\x6f\x6e\x20\x68\x74\x74\x70\x3a\x2f\x2f\x25\x73\x3a\x25\x64\x2f"};
static const mp_obj_str_t const_obj_webrepl_13 = {{&mp_type_str}, 16607, 35, (const byte*)"\x0a\x43\x6f\x6e\x63\x75\x72\x72\x65\x6e\x74\x20\x57\x65\x62\x52\x45\x50\x4c\x20\x63\x6f\x6e\x6e\x65\x63\x74\x69\x6f\x6e\x20\x66\x72\x6f\x6d"};
static const mp_obj_str_t const_obj_webrepl_14 = {{&mp_type_str}, 13454, 25, (const byte*)"\x0a\x57\x65\x62\x52\x45\x50\x4c\x20\x63\x6f\x6e\x6e\x65\x63\x74\x69\x6f\x6e\x20\x66\x72\x6f\x6d\x3a"};
static const mp_obj_str_t const_obj_webrepl_16 = {{&mp_type_str}, 39750, 53, (const byte*)"\x57\x65\x62\x52\x45\x50\x4c\x20\x69\x73\x20\x6e\x6f\x74\x20\x63\x6f\x6e\x66\x69\x67\x75\x72\x65\x64\x2c\x20\x72\x75\x6e\x20\x27\x69\x6d\x70\x6f\x72\x74\x20\x77\x65\x62\x72\x65\x70\x6c\x5f\x73\x65\x74\x75\x70\x27"};
static const mp_obj_str_t const_obj_webrepl_17 = {{&mp_type_str}, 51073, 35, (const byte*)"\x53\x74\x61\x72\x74\x69\x6e\x67\x20\x77\x65\x62\x72\x65\x70\x6c\x20\x69\x6e\x20\x66\x6f\x72\x65\x67\x72\x6f\x75\x6e\x64\x20\x6d\x6f\x64\x65"};
static const mp_obj_str_t const_obj_webrepl_18 = {{&mp_type_str}, 16616, 30, (const byte*)"\x53\x74\x61\x72\x74\x65\x64\x20\x77\x65\x62\x72\x65\x70\x6c\x20\x69\x6e\x20\x6e\x6f\x72\x6d\x61\x6c\x20\x6d\x6f\x64\x65"};
static const mp_obj_str_t const_obj_webrepl_19 = {{&mp_type_str}, 57813, 39, (const byte*)"\x53\x74\x61\x72\x74\x65\x64\x20\x77\x65\x62\x72\x65\x70\x6c\x20\x69\x6e\x20\x6d\x61\x6e\x75\x61\x6c\x20\x6f\x76\x65\x72\x72\x69\x64\x65\x20\x6d\x6f\x64\x65"};

// constant table
static const mp_rom_obj_t const_obj_table_data_webrepl[20] = {
    MP_ROM_PTR(&const_obj_webrepl_0),
    MP_ROM_PTR(&const_obj_webrepl_1),
    MP_ROM_PTR(&const_obj_webrepl_2),
    MP_ROM_PTR(&const_obj_webrepl_3),
    MP_ROM_PTR(&const_obj_webrepl_4),
    MP_ROM_PTR(&const_obj_webrepl_5),
    MP_ROM_PTR(&const_obj_webrepl_6),
    MP_ROM_QSTR(MP_QSTR_Sec_hyphen_WebSocket_hyphen_Key_colon_),
    MP_ROM_PTR(&const_obj_webrepl_8),
    MP_ROM_PTR(&const_obj_webrepl_9),
    MP_ROM_PTR(&const_obj_webrepl_10),
    MP_ROM_PTR(&const_obj_webrepl_11),
    MP_ROM_PTR(&const_obj_webrepl_12),
    MP_ROM_PTR(&const_obj_webrepl_13),
    MP_ROM_PTR(&const_obj_webrepl_14),
    MP_ROM_QSTR(MP_QSTR_dupterm_notify),
    MP_ROM_PTR(&const_obj_webrepl_16),
    MP_ROM_PTR(&const_obj_webrepl_17),
    MP_ROM_PTR(&const_obj_webrepl_18),
    MP_ROM_PTR(&const_obj_webrepl_19),
};

static const mp_frozen_module_t frozen_module_webrepl = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_webrepl,
        .obj_table = (mp_obj_t *)&const_obj_table_data_webrepl,
    },
    .proto_fun = &proto_fun_webrepl__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module webrepl_setup
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/webrepl_setup.mpy
// - frozen file name: webrepl_setup.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file webrepl_setup.py, scope webrepl_setup__lt_module_gt_
static const byte fun_data_webrepl_setup__lt_module_gt_[68] = {
    0x08,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'sys'
    0x16,0x02, // STORE_NAME 'sys'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'os'
    0x16,0x03, // STORE_NAME 'os'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x04, // IMPORT_NAME 'machine'
    0x16,0x04, // STORE_NAME 'machine'
    0x10,0x05, // LOAD_CONST_STRING './boot.py'
    0x16,0x20, // STORE_NAME 'RC'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x16,0x21, // STORE_NAME 'CONFIG'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x07, // STORE_NAME 'input_choice'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x08, // STORE_NAME 'getpass'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x09, // STORE_NAME 'input_pass'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x0a, // STORE_NAME 'exists'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x0b, // STORE_NAME 'get_daemon_status'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x0f, // STORE_NAME 'change_daemon'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x15, // STORE_NAME 'main'
    0x11,0x15, // LOAD_NAME 'main'
    0x34,0x00, // CALL_FUNCTION 0
    0x59, // POP_TOP
    0x10,0x06, // LOAD_CONST_STRING '0.1.0'
    0x16,0x22, // STORE_NAME '__version__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of webrepl_setup__lt_module_gt_
// frozen bytecode for file webrepl_setup.py, scope webrepl_setup_input_choice
static const byte fun_data_webrepl_setup_input_choice[22] = {
    0x22,0x06, // prelude
    0x07,0x23,0x24, // names: input_choice, prompt, choices
     // code info
    0x12,0x25, // LOAD_GLOBAL 'input'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0xb1, // LOAD_FAST 1
    0xdd, // BINARY_OP 6 <in>
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
    0x42,0x31, // JUMP -15
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_webrepl_setup_input_choice = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl_setup_input_choice,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 22,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_webrepl_setup_input_choice + 5,
        .line_info_top = fun_data_webrepl_setup_input_choice + 5,
        .opcodes = fun_data_webrepl_setup_input_choice + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_webrepl_setup_input_choice fun_data_webrepl_setup_input_choice[0]
#endif

// child of webrepl_setup__lt_module_gt_
// frozen bytecode for file webrepl_setup.py, scope webrepl_setup_getpass
static const byte fun_data_webrepl_setup_getpass[10] = {
    0x11,0x04, // prelude
    0x08,0x23, // names: getpass, prompt
     // code info
    0x12,0x25, // LOAD_GLOBAL 'input'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_webrepl_setup_getpass = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl_setup_getpass,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 10,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 8,
        .line_info = fun_data_webrepl_setup_getpass + 4,
        .line_info_top = fun_data_webrepl_setup_getpass + 4,
        .opcodes = fun_data_webrepl_setup_getpass + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_webrepl_setup_getpass fun_data_webrepl_setup_getpass[0]
#endif

// child of webrepl_setup__lt_module_gt_
// frozen bytecode for file webrepl_setup.py, scope webrepl_setup_input_pass
static const byte fun_data_webrepl_setup_input_pass[62] = {
    0x18,0x02, // prelude
    0x09, // names: input_pass
     // code info
    0x12,0x08, // LOAD_GLOBAL 'getpass'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc0, // STORE_FAST 0
    0x12,0x26, // LOAD_GLOBAL 'len'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x84, // LOAD_CONST_SMALL_INT 4
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x49, // POP_JUMP_IF_TRUE 9
    0x12,0x26, // LOAD_GLOBAL 'len'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x89, // LOAD_CONST_SMALL_INT 9
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0x12,0x27, // LOAD_GLOBAL 'print'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x55, // JUMP 21
    0x12,0x08, // LOAD_GLOBAL 'getpass'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x34,0x01, // CALL_FUNCTION 1
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
    0x12,0x27, // LOAD_GLOBAL 'print'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x07, // JUMP -57
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_webrepl_setup_input_pass = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl_setup_input_pass,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 62,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_webrepl_setup_input_pass + 3,
        .line_info_top = fun_data_webrepl_setup_input_pass + 3,
        .opcodes = fun_data_webrepl_setup_input_pass + 3,
    },
    #endif
    #endif
};
#else
#define proto_fun_webrepl_setup_input_pass fun_data_webrepl_setup_input_pass[0]
#endif

// child of webrepl_setup__lt_module_gt_
// frozen bytecode for file webrepl_setup.py, scope webrepl_setup_exists
static const byte fun_data_webrepl_setup_exists[32] = {
    0xb1,0x02,0x04, // prelude
    0x0a,0x28, // names: exists, fname
     // code info
    0x48,0x0d, // SETUP_EXCEPT 13
    0x12,0x29, // LOAD_GLOBAL 'open'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x47,0x02, // SETUP_WITH 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
    0x57, // DUP_TOP
    0x12,0x2a, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_webrepl_setup_exists = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl_setup_exists,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 32,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_webrepl_setup_exists + 5,
        .line_info_top = fun_data_webrepl_setup_exists + 5,
        .opcodes = fun_data_webrepl_setup_exists + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_webrepl_setup_exists fun_data_webrepl_setup_exists[0]
#endif

// child of webrepl_setup__lt_module_gt_
// frozen bytecode for file webrepl_setup.py, scope webrepl_setup_get_daemon_status
static const byte fun_data_webrepl_setup_get_daemon_status[44] = {
    0x54,0x02, // prelude
    0x0b, // names: get_daemon_status
     // code info
    0x12,0x29, // LOAD_GLOBAL 'open'
    0x12,0x20, // LOAD_GLOBAL 'RC'
    0x34,0x01, // CALL_FUNCTION 1
    0x47,0x1d, // SETUP_WITH 29
    0xc0, // STORE_FAST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x16, // FOR_ITER 22
    0xc1, // STORE_FAST 1
    0x10,0x0c, // LOAD_CONST_STRING 'webrepl'
    0xb1, // LOAD_FAST 1
    0xdd, // BINARY_OP 6 <in>
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0xb1, // LOAD_FAST 1
    0x14,0x0d, // LOAD_METHOD 'startswith'
    0x10,0x0e, // LOAD_CONST_STRING '#'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x50, // LOAD_CONST_FALSE
    0x63, // RETURN_VALUE
    0x52, // LOAD_CONST_TRUE
    0x63, // RETURN_VALUE
    0x42,0x28, // JUMP -24
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_webrepl_setup_get_daemon_status = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl_setup_get_daemon_status,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 44,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_webrepl_setup_get_daemon_status + 3,
        .line_info_top = fun_data_webrepl_setup_get_daemon_status + 3,
        .opcodes = fun_data_webrepl_setup_get_daemon_status + 3,
    },
    #endif
    #endif
};
#else
#define proto_fun_webrepl_setup_get_daemon_status fun_data_webrepl_setup_get_daemon_status[0]
#endif

// child of webrepl_setup__lt_module_gt_
// frozen bytecode for file webrepl_setup.py, scope webrepl_setup_change_daemon
static const byte fun_data_webrepl_setup_change_daemon[144] = {
    0xa9,0x12,0x04, // prelude
    0x0f,0x2b, // names: change_daemon, action
     // code info
    0x23,0x05, // LOAD_CONST_OBJ 5
    0xc1, // STORE_FAST 1
    0x12,0x29, // LOAD_GLOBAL 'open'
    0x12,0x20, // LOAD_GLOBAL 'RC'
    0x34,0x01, // CALL_FUNCTION 1
    0x47,0x65, // SETUP_WITH 101
    0xc2, // STORE_FAST 2
    0x12,0x29, // LOAD_GLOBAL 'open'
    0x12,0x20, // LOAD_GLOBAL 'RC'
    0x10,0x10, // LOAD_CONST_STRING '.tmp'
    0xf2, // BINARY_OP 27 __add__
    0x10,0x11, // LOAD_CONST_STRING 'w'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x54, // SETUP_WITH 84
    0xc3, // STORE_FAST 3
    0x50, // LOAD_CONST_FALSE
    0xc4, // STORE_FAST 4
    0xb2, // LOAD_FAST 2
    0x5f, // GET_ITER_STACK
    0x4b,0x41, // FOR_ITER 65
    0xc5, // STORE_FAST 5
    0xb1, // LOAD_FAST 1
    0x5f, // GET_ITER_STACK
    0x4b,0x32, // FOR_ITER 50
    0xc6, // STORE_FAST 6
    0xb6, // LOAD_FAST 6
    0xb5, // LOAD_FAST 5
    0xdd, // BINARY_OP 6 <in>
    0x44,0x6a, // POP_JUMP_IF_FALSE 42
    0x52, // LOAD_CONST_TRUE
    0xc4, // STORE_FAST 4
    0xb0, // LOAD_FAST 0
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0xb5, // LOAD_FAST 5
    0x14,0x0d, // LOAD_METHOD 'startswith'
    0x10,0x0e, // LOAD_CONST_STRING '#'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0xb5, // LOAD_FAST 5
    0x81, // LOAD_CONST_SMALL_INT 1
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xc5, // STORE_FAST 5
    0x42,0x53, // JUMP 19
    0xb0, // LOAD_FAST 0
    0x43,0x50, // POP_JUMP_IF_TRUE 16
    0xb5, // LOAD_FAST 5
    0x14,0x0d, // LOAD_METHOD 'startswith'
    0x10,0x0e, // LOAD_CONST_STRING '#'
    0x36,0x01, // CALL_METHOD 1
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0x10,0x0e, // LOAD_CONST_STRING '#'
    0xb5, // LOAD_FAST 5
    0xf2, // BINARY_OP 27 __add__
    0xc5, // STORE_FAST 5
    0x42,0x40, // JUMP 0
    0x42,0x0c, // JUMP -52
    0xb3, // LOAD_FAST 3
    0x14,0x12, // LOAD_METHOD 'write'
    0xb5, // LOAD_FAST 5
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0xbd,0x7f, // JUMP -67
    0xb4, // LOAD_FAST 4
    0x43,0x48, // POP_JUMP_IF_TRUE 8
    0xb3, // LOAD_FAST 3
    0x14,0x12, // LOAD_METHOD 'write'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x13, // LOAD_METHOD 'remove'
    0x12,0x20, // LOAD_GLOBAL 'RC'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x14, // LOAD_METHOD 'rename'
    0x12,0x20, // LOAD_GLOBAL 'RC'
    0x10,0x10, // LOAD_CONST_STRING '.tmp'
    0xf2, // BINARY_OP 27 __add__
    0x12,0x20, // LOAD_GLOBAL 'RC'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_webrepl_setup_change_daemon = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl_setup_change_daemon,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 144,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 22,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 15,
        .line_info = fun_data_webrepl_setup_change_daemon + 5,
        .line_info_top = fun_data_webrepl_setup_change_daemon + 5,
        .opcodes = fun_data_webrepl_setup_change_daemon + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_webrepl_setup_change_daemon fun_data_webrepl_setup_change_daemon[0]
#endif

// child of webrepl_setup__lt_module_gt_
// frozen bytecode for file webrepl_setup.py, scope webrepl_setup_main
static const byte fun_data_webrepl_setup_main[199] = {
    0x54,0x02, // prelude
    0x15, // names: main
     // code info
    0x12,0x0b, // LOAD_GLOBAL 'get_daemon_status'
    0x34,0x00, // CALL_FUNCTION 0
    0xc0, // STORE_FAST 0
    0x12,0x27, // LOAD_GLOBAL 'print'
    0x23,0x07, // LOAD_CONST_OBJ 7
    0xb0, // LOAD_FAST 0
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x10,0x16, // LOAD_CONST_STRING 'enabled'
    0x42,0x42, // JUMP 2
    0x10,0x17, // LOAD_CONST_STRING 'disabled'
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x12,0x27, // LOAD_GLOBAL 'print'
    0x23,0x08, // LOAD_CONST_OBJ 8
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x27, // LOAD_GLOBAL 'print'
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x25, // LOAD_GLOBAL 'input'
    0x10,0x18, // LOAD_CONST_STRING '> '
    0x34,0x01, // CALL_FUNCTION 1
    0x14,0x19, // LOAD_METHOD 'upper'
    0x36,0x00, // CALL_METHOD 0
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x10,0x1a, // LOAD_CONST_STRING 'E'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0xc0,0x80, // POP_JUMP_IF_FALSE 64
    0x12,0x0a, // LOAD_GLOBAL 'exists'
    0x12,0x21, // LOAD_GLOBAL 'CONFIG'
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0x12,0x07, // LOAD_GLOBAL 'input_choice'
    0x23,0x0a, // LOAD_CONST_OBJ 10
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0x34,0x02, // CALL_FUNCTION 2
    0xc2, // STORE_FAST 2
    0x42,0x4a, // JUMP 10
    0x12,0x27, // LOAD_GLOBAL 'print'
    0x23,0x0c, // LOAD_CONST_OBJ 12
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x10,0x1b, // LOAD_CONST_STRING 'y'
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x10,0x1b, // LOAD_CONST_STRING 'y'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x5d, // POP_JUMP_IF_FALSE 29
    0x12,0x09, // LOAD_GLOBAL 'input_pass'
    0x34,0x00, // CALL_FUNCTION 0
    0xc3, // STORE_FAST 3
    0x12,0x29, // LOAD_GLOBAL 'open'
    0x12,0x21, // LOAD_GLOBAL 'CONFIG'
    0x10,0x11, // LOAD_CONST_STRING 'w'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x0c, // SETUP_WITH 12
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x14,0x12, // LOAD_METHOD 'write'
    0x10,0x1c, // LOAD_CONST_STRING 'PASS = %r\n'
    0xb3, // LOAD_FAST 3
    0xf8, // BINARY_OP 33 __mod__
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0xb1, // LOAD_FAST 1
    0x23,0x0d, // LOAD_CONST_OBJ 13
    0xdd, // BINARY_OP 6 <in>
    0xd3, // UNARY_OP 3 <not>
    0x43,0x52, // POP_JUMP_IF_TRUE 18
    0xb1, // LOAD_FAST 1
    0x10,0x1d, // LOAD_CONST_STRING 'D'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0xb0, // LOAD_FAST 0
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0xb1, // LOAD_FAST 1
    0x10,0x1a, // LOAD_CONST_STRING 'E'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x51, // POP_JUMP_IF_FALSE 17
    0xb0, // LOAD_FAST 0
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x12,0x27, // LOAD_GLOBAL 'print'
    0x23,0x0e, // LOAD_CONST_OBJ 14
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x14,0x1e, // LOAD_METHOD 'exit'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x12,0x0f, // LOAD_GLOBAL 'change_daemon'
    0xb1, // LOAD_FAST 1
    0x10,0x1a, // LOAD_CONST_STRING 'E'
    0xd9, // BINARY_OP 2 __eq__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x27, // LOAD_GLOBAL 'print'
    0x23,0x0f, // LOAD_CONST_OBJ 15
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x07, // LOAD_GLOBAL 'input_choice'
    0x23,0x10, // LOAD_CONST_OBJ 16
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0x34,0x02, // CALL_FUNCTION 2
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x10,0x1b, // LOAD_CONST_STRING 'y'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x04, // LOAD_GLOBAL 'machine'
    0x14,0x1f, // LOAD_METHOD 'reset'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_webrepl_setup_main = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl_setup_main,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 199,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 21,
        .line_info = fun_data_webrepl_setup_main + 3,
        .line_info_top = fun_data_webrepl_setup_main + 3,
        .opcodes = fun_data_webrepl_setup_main + 3,
    },
    #endif
    #endif
};
#else
#define proto_fun_webrepl_setup_main fun_data_webrepl_setup_main[0]
#endif

static const mp_raw_code_t *const children_webrepl_setup__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_webrepl_setup_input_choice,
    (const mp_raw_code_t *)&proto_fun_webrepl_setup_getpass,
    (const mp_raw_code_t *)&proto_fun_webrepl_setup_input_pass,
    (const mp_raw_code_t *)&proto_fun_webrepl_setup_exists,
    (const mp_raw_code_t *)&proto_fun_webrepl_setup_get_daemon_status,
    (const mp_raw_code_t *)&proto_fun_webrepl_setup_change_daemon,
    (const mp_raw_code_t *)&proto_fun_webrepl_setup_main,
};

static const mp_raw_code_truncated_t proto_fun_webrepl_setup__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_webrepl_setup__lt_module_gt_,
    .children = (void *)&children_webrepl_setup__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 68,
    .n_children = 7,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_webrepl_setup__lt_module_gt_ + 3,
        .line_info_top = fun_data_webrepl_setup__lt_module_gt_ + 3,
        .opcodes = fun_data_webrepl_setup__lt_module_gt_ + 3,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_webrepl_setup[44] = {
    MP_QSTR_webrepl_setup_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_sys,
    MP_QSTR_os,
    MP_QSTR_machine,
    MP_QSTR__dot__slash_boot_dot_py,
    MP_QSTR_0_dot_1_dot_0,
    MP_QSTR_input_choice,
    MP_QSTR_getpass,
    MP_QSTR_input_pass,
    MP_QSTR_exists,
    MP_QSTR_get_daemon_status,
    MP_QSTR_webrepl,
    MP_QSTR_startswith,
    MP_QSTR__hash_,
    MP_QSTR_change_daemon,
    MP_QSTR__dot_tmp,
    MP_QSTR_w,
    MP_QSTR_write,
    MP_QSTR_remove,
    MP_QSTR_rename,
    MP_QSTR_main,
    MP_QSTR_enabled,
    MP_QSTR_disabled,
    MP_QSTR__gt__space_,
    MP_QSTR_upper,
    MP_QSTR_E,
    MP_QSTR_y,
    MP_QSTR_PASS_space__equals__space__percent_r_0x0a_,
    MP_QSTR_D,
    MP_QSTR_exit,
    MP_QSTR_reset,
    MP_QSTR_RC,
    MP_QSTR_CONFIG,
    MP_QSTR___version__,
    MP_QSTR_prompt,
    MP_QSTR_choices,
    MP_QSTR_input,
    MP_QSTR_len,
    MP_QSTR_print,
    MP_QSTR_fname,
    MP_QSTR_open,
    MP_QSTR_OSError,
    MP_QSTR_action,
};

// constants
static const mp_obj_str_t const_obj_webrepl_setup_1 = {{&mp_type_str}, 12822, 26, (const byte*)"\x4e\x65\x77\x20\x70\x61\x73\x73\x77\x6f\x72\x64\x20\x28\x34\x2d\x39\x20\x63\x68\x61\x72\x73\x29\x3a\x20"};
static const mp_rom_obj_tuple_t const_obj_webrepl_setup_5 = {{&mp_type_tuple}, 2, {
    MP_ROM_QSTR(MP_QSTR_import_space_webrepl),
    MP_ROM_QSTR(MP_QSTR_webrepl_dot_start_paren_open__paren_close_),
}};
static const mp_obj_str_t const_obj_webrepl_setup_6 = {{&mp_type_str}, 34423, 31, (const byte*)"\x69\x6d\x70\x6f\x72\x74\x20\x77\x65\x62\x72\x65\x70\x6c\x0a\x77\x65\x62\x72\x65\x70\x6c\x2e\x73\x74\x61\x72\x74\x28\x29\x0a"};
static const mp_obj_str_t const_obj_webrepl_setup_7 = {{&mp_type_str}, 24126, 33, (const byte*)"\x57\x65\x62\x52\x45\x50\x4c\x20\x64\x61\x65\x6d\x6f\x6e\x20\x61\x75\x74\x6f\x2d\x73\x74\x61\x72\x74\x20\x73\x74\x61\x74\x75\x73\x3a"};
static const mp_obj_str_t const_obj_webrepl_setup_8 = {{&mp_type_str}, 9123, 60, (const byte*)"\x0a\x57\x6f\x75\x6c\x64\x20\x79\x6f\x75\x20\x6c\x69\x6b\x65\x20\x74\x6f\x20\x28\x45\x29\x6e\x61\x62\x6c\x65\x20\x6f\x72\x20\x28\x44\x29\x69\x73\x61\x62\x6c\x65\x20\x69\x74\x20\x72\x75\x6e\x6e\x69\x6e\x67\x20\x6f\x6e\x20\x62\x6f\x6f\x74\x3f"};
static const mp_obj_str_t const_obj_webrepl_setup_10 = {{&mp_type_str}, 49111, 49, (const byte*)"\x57\x6f\x75\x6c\x64\x20\x79\x6f\x75\x20\x6c\x69\x6b\x65\x20\x74\x6f\x20\x63\x68\x61\x6e\x67\x65\x20\x57\x65\x62\x52\x45\x50\x4c\x20\x70\x61\x73\x73\x77\x6f\x72\x64\x3f\x20\x28\x79\x2f\x6e\x29\x20"};
static const mp_rom_obj_tuple_t const_obj_webrepl_setup_11 = {{&mp_type_tuple}, 3, {
    MP_ROM_QSTR(MP_QSTR_y),
    MP_ROM_QSTR(MP_QSTR_n),
    MP_ROM_QSTR(MP_QSTR_),
}};
static const mp_obj_str_t const_obj_webrepl_setup_12 = {{&mp_type_str}, 7023, 47, (const byte*)"\x54\x6f\x20\x65\x6e\x61\x62\x6c\x65\x20\x57\x65\x62\x52\x45\x50\x4c\x2c\x20\x79\x6f\x75\x20\x6d\x75\x73\x74\x20\x73\x65\x74\x20\x70\x61\x73\x73\x77\x6f\x72\x64\x20\x66\x6f\x72\x20\x69\x74"};
static const mp_rom_obj_tuple_t const_obj_webrepl_setup_13 = {{&mp_type_tuple}, 2, {
    MP_ROM_QSTR(MP_QSTR_D),
    MP_ROM_QSTR(MP_QSTR_E),
}};
static const mp_obj_str_t const_obj_webrepl_setup_14 = {{&mp_type_str}, 24025, 26, (const byte*)"\x4e\x6f\x20\x66\x75\x72\x74\x68\x65\x72\x20\x61\x63\x74\x69\x6f\x6e\x20\x72\x65\x71\x75\x69\x72\x65\x64"};
static const mp_obj_str_t const_obj_webrepl_setup_15 = {{&mp_type_str}, 15057, 38, (const byte*)"\x43\x68\x61\x6e\x67\x65\x73\x20\x77\x69\x6c\x6c\x20\x62\x65\x20\x61\x63\x74\x69\x76\x61\x74\x65\x64\x20\x61\x66\x74\x65\x72\x20\x72\x65\x62\x6f\x6f\x74"};
static const mp_obj_str_t const_obj_webrepl_setup_16 = {{&mp_type_str}, 7554, 36, (const byte*)"\x57\x6f\x75\x6c\x64\x20\x79\x6f\x75\x20\x6c\x69\x6b\x65\x20\x74\x6f\x20\x72\x65\x62\x6f\x6f\x74\x20\x6e\x6f\x77\x3f\x20\x28\x79\x2f\x6e\x29\x20"};

// constant table
static const mp_rom_obj_t const_obj_table_data_webrepl_setup[17] = {
    MP_ROM_QSTR(MP_QSTR__dot__slash_webrepl_cfg_dot_py),
    MP_ROM_PTR(&const_obj_webrepl_setup_1),
    MP_ROM_QSTR(MP_QSTR_Invalid_space_password_space_length),
    MP_ROM_QSTR(MP_QSTR_Confirm_space_password_colon__space_),
    MP_ROM_QSTR(MP_QSTR_Passwords_space_do_space_not_space_match),
    MP_ROM_PTR(&const_obj_webrepl_setup_5),
    MP_ROM_PTR(&const_obj_webrepl_setup_6),
    MP_ROM_PTR(&const_obj_webrepl_setup_7),
    MP_ROM_PTR(&const_obj_webrepl_setup_8),
    MP_ROM_QSTR(MP_QSTR__paren_open_Empty_space_line_space_to_space_quit_paren_close_),
    MP_ROM_PTR(&const_obj_webrepl_setup_10),
    MP_ROM_PTR(&const_obj_webrepl_setup_11),
    MP_ROM_PTR(&const_obj_webrepl_setup_12),
    MP_ROM_PTR(&const_obj_webrepl_setup_13),
    MP_ROM_PTR(&const_obj_webrepl_setup_14),
    MP_ROM_PTR(&const_obj_webrepl_setup_15),
    MP_ROM_PTR(&const_obj_webrepl_setup_16),
};

static const mp_frozen_module_t frozen_module_webrepl_setup = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_webrepl_setup,
        .obj_table = (mp_obj_t *)&const_obj_table_data_webrepl_setup,
    },
    .proto_fun = &proto_fun_webrepl_setup__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module urequests
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/urequests.mpy
// - frozen file name: urequests.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file urequests.py, scope urequests__lt_module_gt_
static const byte fun_data_urequests__lt_module_gt_[17] = {
    0x00,0x0a, // prelude
    0x01, // names: <module>
    0x60,0x20,0x64,0x60, // code info
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x03, // STORE_NAME '__getattr__'
    0x10,0x02, // LOAD_CONST_STRING '0.8.0'
    0x16,0x05, // STORE_NAME '__version__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of urequests__lt_module_gt_
// frozen bytecode for file urequests.py, scope urequests___getattr__
static const byte fun_data_urequests___getattr__[19] = {
    0x21,0x0a, // prelude
    0x03,0x06, // names: __getattr__, attr
    0x60,0x40,0x45, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x04, // IMPORT_NAME 'requests'
    0xc1, // STORE_FAST 1
    0x12,0x07, // LOAD_GLOBAL 'getattr'
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x34,0x02, // CALL_FUNCTION 2
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_urequests___getattr__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_urequests___getattr__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 19,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 3,
        .line_info = fun_data_urequests___getattr__ + 4,
        .line_info_top = fun_data_urequests___getattr__ + 7,
        .opcodes = fun_data_urequests___getattr__ + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_urequests___getattr__ fun_data_urequests___getattr__[0]
#endif

static const mp_raw_code_t *const children_urequests__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_urequests___getattr__,
};

static const mp_raw_code_truncated_t proto_fun_urequests__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_urequests__lt_module_gt_,
    .children = (void *)&children_urequests__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 17,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_urequests__lt_module_gt_ + 3,
        .line_info_top = fun_data_urequests__lt_module_gt_ + 7,
        .opcodes = fun_data_urequests__lt_module_gt_ + 7,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_urequests[8] = {
    MP_QSTR_urequests_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_0_dot_8_dot_0,
    MP_QSTR___getattr__,
    MP_QSTR_requests,
    MP_QSTR___version__,
    MP_QSTR_attr,
    MP_QSTR_getattr,
};

static const mp_frozen_module_t frozen_module_urequests = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_urequests,
        .obj_table = NULL,
    },
    .proto_fun = &proto_fun_urequests__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module aioespnow
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/aioespnow.mpy
// - frozen file name: aioespnow.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file aioespnow.py, scope aioespnow__lt_module_gt_
static const byte fun_data_aioespnow__lt_module_gt_[40] = {
    0x18,0x0e, // prelude
    0x01, // names: <module>
    0x60,0x26,0x66,0x40,0x8d,0x18, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'asyncio'
    0x16,0x02, // STORE_NAME 'asyncio'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'espnow'
    0x16,0x03, // STORE_NAME 'espnow'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'AIOESPNow'
    0x11,0x03, // LOAD_NAME 'espnow'
    0x13,0x05, // LOAD_ATTR 'ESPNow'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x04, // STORE_NAME 'AIOESPNow'
    0x10,0x06, // LOAD_CONST_STRING '0.1.0'
    0x16,0x13, // STORE_NAME '__version__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of aioespnow__lt_module_gt_
// frozen bytecode for file aioespnow.py, scope aioespnow_AIOESPNow
static const byte fun_data_aioespnow_AIOESPNow[47] = {
    0x08,0x14, // prelude
    0x04, // names: AIOESPNow
    0x88,0x0b,0x64,0x20,0x64,0x20,0x89,0x07,0x64, // code info
    0x11,0x14, // LOAD_NAME '__name__'
    0x16,0x15, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'AIOESPNow'
    0x16,0x16, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x07, // STORE_NAME 'arecv'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x0c, // STORE_NAME 'airecv'
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x02, // MAKE_FUNCTION_DEFARGS 2
    0x16,0x0e, // STORE_NAME 'asend'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x11, // STORE_NAME '__aiter__'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x12, // STORE_NAME '__anext__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of aioespnow_AIOESPNow
// frozen bytecode for file aioespnow.py, scope aioespnow_AIOESPNow_arecv
static const byte fun_data_aioespnow_AIOESPNow_arecv[28] = {
    0x99,0x40,0x0a, // prelude
    0x07,0x17, // names: arecv, self
    0x80,0x0c,0x2d, // code info
    0x12,0x02, // LOAD_GLOBAL 'asyncio'
    0x13,0x08, // LOAD_ATTR 'core'
    0x13,0x09, // LOAD_ATTR '_io_queue'
    0x14,0x0a, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x0b, // LOAD_METHOD 'recv'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_aioespnow_AIOESPNow_arecv = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_aioespnow_AIOESPNow_arecv,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 28,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_aioespnow_AIOESPNow_arecv + 5,
        .line_info_top = fun_data_aioespnow_AIOESPNow_arecv + 8,
        .opcodes = fun_data_aioespnow_AIOESPNow_arecv + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_aioespnow_AIOESPNow_arecv fun_data_aioespnow_AIOESPNow_arecv[0]
#endif

// child of aioespnow_AIOESPNow
// frozen bytecode for file aioespnow.py, scope aioespnow_AIOESPNow_airecv
static const byte fun_data_aioespnow_AIOESPNow_airecv[28] = {
    0x99,0x40,0x0a, // prelude
    0x0c,0x17, // names: airecv, self
    0x80,0x10,0x2d, // code info
    0x12,0x02, // LOAD_GLOBAL 'asyncio'
    0x13,0x08, // LOAD_ATTR 'core'
    0x13,0x09, // LOAD_ATTR '_io_queue'
    0x14,0x0a, // LOAD_METHOD 'queue_read'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x0d, // LOAD_METHOD 'irecv'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_aioespnow_AIOESPNow_airecv = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_aioespnow_AIOESPNow_airecv,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 28,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_aioespnow_AIOESPNow_airecv + 5,
        .line_info_top = fun_data_aioespnow_AIOESPNow_airecv + 8,
        .opcodes = fun_data_aioespnow_AIOESPNow_airecv + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_aioespnow_AIOESPNow_airecv fun_data_aioespnow_AIOESPNow_airecv[0]
#endif

// child of aioespnow_AIOESPNow
// frozen bytecode for file aioespnow.py, scope aioespnow_AIOESPNow_asend
static const byte fun_data_aioespnow_AIOESPNow_asend[46] = {
    0xc0,0xc4,0x01,0x14, // prelude
    0x0e,0x17,0x18,0x19,0x1a, // names: asend, self, mac, msg, sync
    0x80,0x14,0x25,0x25,0x2d, // code info
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0x5a, // ROT_TWO
    0xc2, // STORE_FAST 2
    0xc1, // STORE_FAST 1
    0x12,0x02, // LOAD_GLOBAL 'asyncio'
    0x13,0x08, // LOAD_ATTR 'core'
    0x13,0x09, // LOAD_ATTR '_io_queue'
    0x14,0x0f, // LOAD_METHOD 'queue_write'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x10, // LOAD_METHOD 'send'
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0x36,0x03, // CALL_METHOD 3
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_aioespnow_AIOESPNow_asend = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_aioespnow_AIOESPNow_asend,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 46,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_aioespnow_AIOESPNow_asend + 9,
        .line_info_top = fun_data_aioespnow_AIOESPNow_asend + 14,
        .opcodes = fun_data_aioespnow_AIOESPNow_asend + 14,
    },
    #endif
    #endif
};
#else
#define proto_fun_aioespnow_AIOESPNow_asend fun_data_aioespnow_AIOESPNow_asend[0]
#endif

// child of aioespnow_AIOESPNow
// frozen bytecode for file aioespnow.py, scope aioespnow_AIOESPNow___aiter__
static const byte fun_data_aioespnow_AIOESPNow___aiter__[8] = {
    0x09,0x08, // prelude
    0x11,0x17, // names: __aiter__, self
    0x80,0x1b, // code info
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_aioespnow_AIOESPNow___aiter__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_aioespnow_AIOESPNow___aiter__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 8,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 17,
        .line_info = fun_data_aioespnow_AIOESPNow___aiter__ + 4,
        .line_info_top = fun_data_aioespnow_AIOESPNow___aiter__ + 6,
        .opcodes = fun_data_aioespnow_AIOESPNow___aiter__ + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_aioespnow_AIOESPNow___aiter__ fun_data_aioespnow_AIOESPNow___aiter__[0]
#endif

// child of aioespnow_AIOESPNow
// frozen bytecode for file aioespnow.py, scope aioespnow_AIOESPNow___anext__
static const byte fun_data_aioespnow_AIOESPNow___anext__[16] = {
    0x91,0x40,0x08, // prelude
    0x12,0x17, // names: __anext__, self
    0x80,0x1e, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x0c, // LOAD_METHOD 'airecv'
    0x36,0x00, // CALL_METHOD 0
    0x5e, // GET_ITER
    0x51, // LOAD_CONST_NONE
    0x68, // YIELD_FROM
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_aioespnow_AIOESPNow___anext__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_aioespnow_AIOESPNow___anext__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 16,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 18,
        .line_info = fun_data_aioespnow_AIOESPNow___anext__ + 5,
        .line_info_top = fun_data_aioespnow_AIOESPNow___anext__ + 7,
        .opcodes = fun_data_aioespnow_AIOESPNow___anext__ + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_aioespnow_AIOESPNow___anext__ fun_data_aioespnow_AIOESPNow___anext__[0]
#endif

static const mp_raw_code_t *const children_aioespnow_AIOESPNow[] = {
    (const mp_raw_code_t *)&proto_fun_aioespnow_AIOESPNow_arecv,
    (const mp_raw_code_t *)&proto_fun_aioespnow_AIOESPNow_airecv,
    (const mp_raw_code_t *)&proto_fun_aioespnow_AIOESPNow_asend,
    (const mp_raw_code_t *)&proto_fun_aioespnow_AIOESPNow___aiter__,
    (const mp_raw_code_t *)&proto_fun_aioespnow_AIOESPNow___anext__,
};

static const mp_raw_code_truncated_t proto_fun_aioespnow_AIOESPNow = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_aioespnow_AIOESPNow,
    .children = (void *)&children_aioespnow_AIOESPNow,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 47,
    .n_children = 5,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_aioespnow_AIOESPNow + 3,
        .line_info_top = fun_data_aioespnow_AIOESPNow + 12,
        .opcodes = fun_data_aioespnow_AIOESPNow + 12,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_aioespnow__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_aioespnow_AIOESPNow,
};

static const mp_raw_code_truncated_t proto_fun_aioespnow__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_aioespnow__lt_module_gt_,
    .children = (void *)&children_aioespnow__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 40,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_aioespnow__lt_module_gt_ + 3,
        .line_info_top = fun_data_aioespnow__lt_module_gt_ + 9,
        .opcodes = fun_data_aioespnow__lt_module_gt_ + 9,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_aioespnow[27] = {
    MP_QSTR_aioespnow_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_asyncio,
    MP_QSTR_espnow,
    MP_QSTR_AIOESPNow,
    MP_QSTR_ESPNow,
    MP_QSTR_0_dot_1_dot_0,
    MP_QSTR_arecv,
    MP_QSTR_core,
    MP_QSTR__io_queue,
    MP_QSTR_queue_read,
    MP_QSTR_recv,
    MP_QSTR_airecv,
    MP_QSTR_irecv,
    MP_QSTR_asend,
    MP_QSTR_queue_write,
    MP_QSTR_send,
    MP_QSTR___aiter__,
    MP_QSTR___anext__,
    MP_QSTR___version__,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_self,
    MP_QSTR_mac,
    MP_QSTR_msg,
    MP_QSTR_sync,
};

static const mp_frozen_module_t frozen_module_aioespnow = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_aioespnow,
        .obj_table = NULL,
    },
    .proto_fun = &proto_fun_aioespnow__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module dht
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/dht.mpy
// - frozen file name: dht.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file dht.py, scope dht__lt_module_gt_
static const byte fun_data_dht__lt_module_gt_[139] = {
    0x18,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'sys'
    0x16,0x02, // STORE_NAME 'sys'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'machine'
    0x16,0x03, // STORE_NAME 'machine'
    0x11,0x14, // LOAD_NAME 'hasattr'
    0x11,0x03, // LOAD_NAME 'machine'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x04, // LOAD_CONST_STRING 'dht_readinto'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'machine'
    0x1c,0x04, // IMPORT_FROM 'dht_readinto'
    0x16,0x04, // STORE_NAME 'dht_readinto'
    0x59, // POP_TOP
    0x42,0x7d, // JUMP 61
    0x11,0x02, // LOAD_NAME 'sys'
    0x13,0x05, // LOAD_ATTR 'platform'
    0x14,0x06, // LOAD_METHOD 'startswith'
    0x10,0x07, // LOAD_CONST_STRING 'esp'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x04, // LOAD_CONST_STRING 'dht_readinto'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x07, // IMPORT_NAME 'esp'
    0x1c,0x04, // IMPORT_FROM 'dht_readinto'
    0x16,0x04, // STORE_NAME 'dht_readinto'
    0x59, // POP_TOP
    0x42,0x63, // JUMP 35
    0x11,0x02, // LOAD_NAME 'sys'
    0x13,0x05, // LOAD_ATTR 'platform'
    0x10,0x08, // LOAD_CONST_STRING 'pyboard'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4e, // POP_JUMP_IF_FALSE 14
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x04, // LOAD_CONST_STRING 'dht_readinto'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x09, // IMPORT_NAME 'pyb'
    0x1c,0x04, // IMPORT_FROM 'dht_readinto'
    0x16,0x04, // STORE_NAME 'dht_readinto'
    0x59, // POP_TOP
    0x42,0x4c, // JUMP 12
    0x11,0x15, // LOAD_NAME '__import__'
    0x11,0x02, // LOAD_NAME 'sys'
    0x13,0x05, // LOAD_ATTR 'platform'
    0x34,0x01, // CALL_FUNCTION 1
    0x13,0x04, // LOAD_ATTR 'dht_readinto'
    0x16,0x04, // STORE_NAME 'dht_readinto'
    0x19,0x03, // DELETE_NAME 'machine'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x0a, // LOAD_CONST_STRING 'DHTBase'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x0a, // STORE_NAME 'DHTBase'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x0b, // LOAD_CONST_STRING 'DHT11'
    0x11,0x0a, // LOAD_NAME 'DHTBase'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x0b, // STORE_NAME 'DHT11'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x0c, // LOAD_CONST_STRING 'DHT22'
    0x11,0x0a, // LOAD_NAME 'DHTBase'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x0c, // STORE_NAME 'DHT22'
    0x10,0x0d, // LOAD_CONST_STRING '0.1.0'
    0x16,0x16, // STORE_NAME '__version__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of dht__lt_module_gt_
// frozen bytecode for file dht.py, scope dht_DHTBase
static const byte fun_data_dht_DHTBase[21] = {
    0x00,0x02, // prelude
    0x0a, // names: DHTBase
     // code info
    0x11,0x17, // LOAD_NAME '__name__'
    0x16,0x18, // STORE_NAME '__module__'
    0x10,0x0a, // LOAD_CONST_STRING 'DHTBase'
    0x16,0x19, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x0e, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x11, // STORE_NAME 'measure'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of dht_DHTBase
// frozen bytecode for file dht.py, scope dht_DHTBase___init__
static const byte fun_data_dht_DHTBase___init__[19] = {
    0x1a,0x06, // prelude
    0x0e,0x1a,0x0f, // names: __init__, self, pin
     // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x0f, // STORE_ATTR 'pin'
    0x12,0x1b, // LOAD_GLOBAL 'bytearray'
    0x85, // LOAD_CONST_SMALL_INT 5
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x18,0x10, // STORE_ATTR 'buf'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_dht_DHTBase___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_dht_DHTBase___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 19,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_dht_DHTBase___init__ + 5,
        .line_info_top = fun_data_dht_DHTBase___init__ + 5,
        .opcodes = fun_data_dht_DHTBase___init__ + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_dht_DHTBase___init__ fun_data_dht_DHTBase___init__[0]
#endif

// child of dht_DHTBase
// frozen bytecode for file dht.py, scope dht_DHTBase_measure
static const byte fun_data_dht_DHTBase_measure[51] = {
    0x21,0x04, // prelude
    0x11,0x1a, // names: measure, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'buf'
    0xc1, // STORE_FAST 1
    0x12,0x04, // LOAD_GLOBAL 'dht_readinto'
    0xb0, // LOAD_FAST 0
    0x13,0x0f, // LOAD_ATTR 'pin'
    0xb1, // LOAD_FAST 1
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0xb1, // LOAD_FAST 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0xb1, // LOAD_FAST 1
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0x22,0x81,0x7f, // LOAD_CONST_SMALL_INT 255
    0xef, // BINARY_OP 24 __and__
    0xb1, // LOAD_FAST 1
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x1c, // LOAD_GLOBAL 'Exception'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_dht_DHTBase_measure = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_dht_DHTBase_measure,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 51,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 17,
        .line_info = fun_data_dht_DHTBase_measure + 4,
        .line_info_top = fun_data_dht_DHTBase_measure + 4,
        .opcodes = fun_data_dht_DHTBase_measure + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_dht_DHTBase_measure fun_data_dht_DHTBase_measure[0]
#endif

static const mp_raw_code_t *const children_dht_DHTBase[] = {
    (const mp_raw_code_t *)&proto_fun_dht_DHTBase___init__,
    (const mp_raw_code_t *)&proto_fun_dht_DHTBase_measure,
};

static const mp_raw_code_truncated_t proto_fun_dht_DHTBase = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_dht_DHTBase,
    .children = (void *)&children_dht_DHTBase,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 21,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_dht_DHTBase + 3,
        .line_info_top = fun_data_dht_DHTBase + 3,
        .opcodes = fun_data_dht_DHTBase + 3,
    },
    #endif
    #endif
};

// child of dht__lt_module_gt_
// frozen bytecode for file dht.py, scope dht_DHT11
static const byte fun_data_dht_DHT11[21] = {
    0x00,0x02, // prelude
    0x0b, // names: DHT11
     // code info
    0x11,0x17, // LOAD_NAME '__name__'
    0x16,0x18, // STORE_NAME '__module__'
    0x10,0x0b, // LOAD_CONST_STRING 'DHT11'
    0x16,0x19, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x12, // STORE_NAME 'humidity'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x13, // STORE_NAME 'temperature'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of dht_DHT11
// frozen bytecode for file dht.py, scope dht_DHT11_humidity
static const byte fun_data_dht_DHT11_humidity[10] = {
    0x11,0x04, // prelude
    0x12,0x1a, // names: humidity, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'buf'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_dht_DHT11_humidity = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_dht_DHT11_humidity,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 10,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 18,
        .line_info = fun_data_dht_DHT11_humidity + 4,
        .line_info_top = fun_data_dht_DHT11_humidity + 4,
        .opcodes = fun_data_dht_DHT11_humidity + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_dht_DHT11_humidity fun_data_dht_DHT11_humidity[0]
#endif

// child of dht_DHT11
// frozen bytecode for file dht.py, scope dht_DHT11_temperature
static const byte fun_data_dht_DHT11_temperature[10] = {
    0x11,0x04, // prelude
    0x13,0x1a, // names: temperature, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'buf'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_dht_DHT11_temperature = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_dht_DHT11_temperature,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 10,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 19,
        .line_info = fun_data_dht_DHT11_temperature + 4,
        .line_info_top = fun_data_dht_DHT11_temperature + 4,
        .opcodes = fun_data_dht_DHT11_temperature + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_dht_DHT11_temperature fun_data_dht_DHT11_temperature[0]
#endif

static const mp_raw_code_t *const children_dht_DHT11[] = {
    (const mp_raw_code_t *)&proto_fun_dht_DHT11_humidity,
    (const mp_raw_code_t *)&proto_fun_dht_DHT11_temperature,
};

static const mp_raw_code_truncated_t proto_fun_dht_DHT11 = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_dht_DHT11,
    .children = (void *)&children_dht_DHT11,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 21,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_dht_DHT11 + 3,
        .line_info_top = fun_data_dht_DHT11 + 3,
        .opcodes = fun_data_dht_DHT11 + 3,
    },
    #endif
    #endif
};

// child of dht__lt_module_gt_
// frozen bytecode for file dht.py, scope dht_DHT22
static const byte fun_data_dht_DHT22[21] = {
    0x00,0x02, // prelude
    0x0c, // names: DHT22
     // code info
    0x11,0x17, // LOAD_NAME '__name__'
    0x16,0x18, // STORE_NAME '__module__'
    0x10,0x0c, // LOAD_CONST_STRING 'DHT22'
    0x16,0x19, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x12, // STORE_NAME 'humidity'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x13, // STORE_NAME 'temperature'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of dht_DHT22
// frozen bytecode for file dht.py, scope dht_DHT22_humidity
static const byte fun_data_dht_DHT22_humidity[21] = {
    0x19,0x04, // prelude
    0x12,0x1a, // names: humidity, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'buf'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x88, // LOAD_CONST_SMALL_INT 8
    0xf0, // BINARY_OP 25 __lshift__
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'buf'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xed, // BINARY_OP 22 __or__
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xf4, // BINARY_OP 29 __mul__
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_dht_DHT22_humidity = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_dht_DHT22_humidity,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 21,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 18,
        .line_info = fun_data_dht_DHT22_humidity + 4,
        .line_info_top = fun_data_dht_DHT22_humidity + 4,
        .opcodes = fun_data_dht_DHT22_humidity + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_dht_DHT22_humidity fun_data_dht_DHT22_humidity[0]
#endif

// child of dht_DHT22
// frozen bytecode for file dht.py, scope dht_DHT22_temperature
static const byte fun_data_dht_DHT22_temperature[41] = {
    0x21,0x04, // prelude
    0x13,0x1a, // names: temperature, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'buf'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x22,0x80,0x7f, // LOAD_CONST_SMALL_INT 127
    0xef, // BINARY_OP 24 __and__
    0x88, // LOAD_CONST_SMALL_INT 8
    0xf0, // BINARY_OP 25 __lshift__
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'buf'
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xed, // BINARY_OP 22 __or__
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xf4, // BINARY_OP 29 __mul__
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'buf'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x22,0x81,0x00, // LOAD_CONST_SMALL_INT 128
    0xef, // BINARY_OP 24 __and__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0xb1, // LOAD_FAST 1
    0xd1, // UNARY_OP 1 __neg__
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_dht_DHT22_temperature = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_dht_DHT22_temperature,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 41,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 19,
        .line_info = fun_data_dht_DHT22_temperature + 4,
        .line_info_top = fun_data_dht_DHT22_temperature + 4,
        .opcodes = fun_data_dht_DHT22_temperature + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_dht_DHT22_temperature fun_data_dht_DHT22_temperature[0]
#endif

static const mp_raw_code_t *const children_dht_DHT22[] = {
    (const mp_raw_code_t *)&proto_fun_dht_DHT22_humidity,
    (const mp_raw_code_t *)&proto_fun_dht_DHT22_temperature,
};

static const mp_raw_code_truncated_t proto_fun_dht_DHT22 = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_dht_DHT22,
    .children = (void *)&children_dht_DHT22,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 21,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_dht_DHT22 + 3,
        .line_info_top = fun_data_dht_DHT22 + 3,
        .opcodes = fun_data_dht_DHT22 + 3,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_dht__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_dht_DHTBase,
    (const mp_raw_code_t *)&proto_fun_dht_DHT11,
    (const mp_raw_code_t *)&proto_fun_dht_DHT22,
};

static const mp_raw_code_truncated_t proto_fun_dht__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_dht__lt_module_gt_,
    .children = (void *)&children_dht__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 139,
    .n_children = 3,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_dht__lt_module_gt_ + 3,
        .line_info_top = fun_data_dht__lt_module_gt_ + 3,
        .opcodes = fun_data_dht__lt_module_gt_ + 3,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_dht[29] = {
    MP_QSTR_dht_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_sys,
    MP_QSTR_machine,
    MP_QSTR_dht_readinto,
    MP_QSTR_platform,
    MP_QSTR_startswith,
    MP_QSTR_esp,
    MP_QSTR_pyboard,
    MP_QSTR_pyb,
    MP_QSTR_DHTBase,
    MP_QSTR_DHT11,
    MP_QSTR_DHT22,
    MP_QSTR_0_dot_1_dot_0,
    MP_QSTR___init__,
    MP_QSTR_pin,
    MP_QSTR_buf,
    MP_QSTR_measure,
    MP_QSTR_humidity,
    MP_QSTR_temperature,
    MP_QSTR_hasattr,
    MP_QSTR___import__,
    MP_QSTR___version__,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_self,
    MP_QSTR_bytearray,
    MP_QSTR_Exception,
};

// constants
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_dht_2 = {{&mp_type_float}, (mp_float_t)0.1};
#define const_obj_dht_2_macro MP_ROM_PTR(&const_obj_dht_2)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_dht_2_macro ((mp_rom_obj_t)(0xbe4cccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_dht_2_macro ((mp_rom_obj_t)(0xbfbd99999999999a))
#endif

// constant table
static const mp_rom_obj_t const_obj_table_data_dht[3] = {
    MP_ROM_QSTR(MP_QSTR_dht_readinto),
    MP_ROM_QSTR(MP_QSTR_checksum_space_error),
    const_obj_dht_2_macro,
};

static const mp_frozen_module_t frozen_module_dht = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_dht,
        .obj_table = (mp_obj_t *)&const_obj_table_data_dht,
    },
    .proto_fun = &proto_fun_dht__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module onewire
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/onewire.mpy
// - frozen file name: onewire.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file onewire.py, scope onewire__lt_module_gt_
static const byte fun_data_onewire__lt_module_gt_[35] = {
    0x18,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME '_onewire'
    0x16,0x18, // STORE_NAME '_ow'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x03, // LOAD_CONST_STRING 'OneWireError'
    0x11,0x19, // LOAD_NAME 'Exception'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x03, // STORE_NAME 'OneWireError'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x04, // LOAD_CONST_STRING 'OneWire'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x04, // STORE_NAME 'OneWire'
    0x10,0x05, // LOAD_CONST_STRING '0.1.0'
    0x16,0x1a, // STORE_NAME '__version__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of onewire__lt_module_gt_
// frozen bytecode for file onewire.py, scope onewire_OneWireError
static const byte fun_data_onewire_OneWireError[13] = {
    0x00,0x02, // prelude
    0x03, // names: OneWireError
     // code info
    0x11,0x1b, // LOAD_NAME '__name__'
    0x16,0x1c, // STORE_NAME '__module__'
    0x10,0x03, // LOAD_CONST_STRING 'OneWireError'
    0x16,0x1d, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_onewire_OneWireError = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_onewire_OneWireError,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 13,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 3,
        .line_info = fun_data_onewire_OneWireError + 3,
        .line_info_top = fun_data_onewire_OneWireError + 3,
        .opcodes = fun_data_onewire_OneWireError + 3,
    },
    #endif
    #endif
};
#else
#define proto_fun_onewire_OneWireError fun_data_onewire_OneWireError[0]
#endif

// child of onewire__lt_module_gt_
// frozen bytecode for file onewire.py, scope onewire_OneWire
static const byte fun_data_onewire_OneWire[80] = {
    0x08,0x02, // prelude
    0x04, // names: OneWire
     // code info
    0x11,0x1b, // LOAD_NAME '__name__'
    0x16,0x1c, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'OneWire'
    0x16,0x1d, // STORE_NAME '__qualname__'
    0x22,0x81,0x70, // LOAD_CONST_SMALL_INT 240
    0x16,0x16, // STORE_NAME 'SEARCH_ROM'
    0x22,0x80,0x55, // LOAD_CONST_SMALL_INT 85
    0x16,0x13, // STORE_NAME 'MATCH_ROM'
    0x22,0x81,0x4c, // LOAD_CONST_SMALL_INT 204
    0x16,0x1e, // STORE_NAME 'SKIP_ROM'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x06, // STORE_NAME '__init__'
    0x50, // LOAD_CONST_FALSE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x0b, // STORE_NAME 'reset'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0c, // STORE_NAME 'readbit'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x0d, // STORE_NAME 'readbyte'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x0e, // STORE_NAME 'readinto'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x0f, // STORE_NAME 'writebit'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x10, // STORE_NAME 'writebyte'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x11, // STORE_NAME 'write'
    0x32,0x08, // MAKE_FUNCTION 8
    0x16,0x12, // STORE_NAME 'select_rom'
    0x32,0x09, // MAKE_FUNCTION 9
    0x16,0x14, // STORE_NAME 'scan'
    0x32,0x0a, // MAKE_FUNCTION 10
    0x16,0x15, // STORE_NAME '_search_rom'
    0x32,0x0b, // MAKE_FUNCTION 11
    0x16,0x17, // STORE_NAME 'crc8'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_OneWire___init__
static const byte fun_data_onewire_OneWire___init__[25] = {
    0x2a,0x06, // prelude
    0x06,0x1f,0x07, // names: __init__, self, pin
     // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x07, // STORE_ATTR 'pin'
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'pin'
    0x14,0x08, // LOAD_METHOD 'init'
    0xb1, // LOAD_FAST 1
    0x13,0x09, // LOAD_ATTR 'OPEN_DRAIN'
    0xb1, // LOAD_FAST 1
    0x13,0x0a, // LOAD_ATTR 'PULL_UP'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_onewire_OneWire___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_onewire_OneWire___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 25,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_onewire_OneWire___init__ + 5,
        .line_info_top = fun_data_onewire_OneWire___init__ + 5,
        .opcodes = fun_data_onewire_OneWire___init__ + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_onewire_OneWire___init__ fun_data_onewire_OneWire___init__[0]
#endif

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_OneWire_reset
static const byte fun_data_onewire_OneWire_reset[27] = {
    0xaa,0x01,0x06, // prelude
    0x0b,0x1f,0x20, // names: reset, self, required
     // code info
    0x12,0x18, // LOAD_GLOBAL '_ow'
    0x14,0x0b, // LOAD_METHOD 'reset'
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'pin'
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0xb1, // LOAD_FAST 1
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0xb2, // LOAD_FAST 2
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x03, // LOAD_GLOBAL 'OneWireError'
    0x65, // RAISE_OBJ
    0xb2, // LOAD_FAST 2
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_onewire_OneWire_reset = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_onewire_OneWire_reset,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 27,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_onewire_OneWire_reset + 6,
        .line_info_top = fun_data_onewire_OneWire_reset + 6,
        .opcodes = fun_data_onewire_OneWire_reset + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_onewire_OneWire_reset fun_data_onewire_OneWire_reset[0]
#endif

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_OneWire_readbit
static const byte fun_data_onewire_OneWire_readbit[14] = {
    0x19,0x04, // prelude
    0x0c,0x1f, // names: readbit, self
     // code info
    0x12,0x18, // LOAD_GLOBAL '_ow'
    0x14,0x0c, // LOAD_METHOD 'readbit'
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'pin'
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_onewire_OneWire_readbit = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_onewire_OneWire_readbit,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 14,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_onewire_OneWire_readbit + 4,
        .line_info_top = fun_data_onewire_OneWire_readbit + 4,
        .opcodes = fun_data_onewire_OneWire_readbit + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_onewire_OneWire_readbit fun_data_onewire_OneWire_readbit[0]
#endif

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_OneWire_readbyte
static const byte fun_data_onewire_OneWire_readbyte[14] = {
    0x19,0x04, // prelude
    0x0d,0x1f, // names: readbyte, self
     // code info
    0x12,0x18, // LOAD_GLOBAL '_ow'
    0x14,0x0d, // LOAD_METHOD 'readbyte'
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'pin'
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_onewire_OneWire_readbyte = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_onewire_OneWire_readbyte,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 14,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_onewire_OneWire_readbyte + 4,
        .line_info_top = fun_data_onewire_OneWire_readbyte + 4,
        .opcodes = fun_data_onewire_OneWire_readbyte + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_onewire_OneWire_readbyte fun_data_onewire_OneWire_readbyte[0]
#endif

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_OneWire_readinto
static const byte fun_data_onewire_OneWire_readinto[38] = {
    0x3a,0x06, // prelude
    0x0e,0x1f,0x21, // names: readinto, self, buf
     // code info
    0x12,0x22, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x50, // JUMP 16
    0x57, // DUP_TOP
    0xc2, // STORE_FAST 2
    0x12,0x18, // LOAD_GLOBAL '_ow'
    0x14,0x0d, // LOAD_METHOD 'readbyte'
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'pin'
    0x36,0x01, // CALL_METHOD 1
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0x56, // STORE_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x2b, // POP_JUMP_IF_TRUE -21
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_onewire_OneWire_readinto = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_onewire_OneWire_readinto,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 38,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_onewire_OneWire_readinto + 5,
        .line_info_top = fun_data_onewire_OneWire_readinto + 5,
        .opcodes = fun_data_onewire_OneWire_readinto + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_onewire_OneWire_readinto fun_data_onewire_OneWire_readinto[0]
#endif

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_OneWire_writebit
static const byte fun_data_onewire_OneWire_writebit[16] = {
    0x2a,0x06, // prelude
    0x0f,0x1f,0x23, // names: writebit, self, value
     // code info
    0x12,0x18, // LOAD_GLOBAL '_ow'
    0x14,0x0f, // LOAD_METHOD 'writebit'
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'pin'
    0xb1, // LOAD_FAST 1
    0x36,0x02, // CALL_METHOD 2
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_onewire_OneWire_writebit = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_onewire_OneWire_writebit,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 16,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 15,
        .line_info = fun_data_onewire_OneWire_writebit + 5,
        .line_info_top = fun_data_onewire_OneWire_writebit + 5,
        .opcodes = fun_data_onewire_OneWire_writebit + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_onewire_OneWire_writebit fun_data_onewire_OneWire_writebit[0]
#endif

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_OneWire_writebyte
static const byte fun_data_onewire_OneWire_writebyte[16] = {
    0x2a,0x06, // prelude
    0x10,0x1f,0x23, // names: writebyte, self, value
     // code info
    0x12,0x18, // LOAD_GLOBAL '_ow'
    0x14,0x10, // LOAD_METHOD 'writebyte'
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'pin'
    0xb1, // LOAD_FAST 1
    0x36,0x02, // CALL_METHOD 2
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_onewire_OneWire_writebyte = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_onewire_OneWire_writebyte,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 16,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 16,
        .line_info = fun_data_onewire_OneWire_writebyte + 5,
        .line_info_top = fun_data_onewire_OneWire_writebyte + 5,
        .opcodes = fun_data_onewire_OneWire_writebyte + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_onewire_OneWire_writebyte fun_data_onewire_OneWire_writebyte[0]
#endif

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_OneWire_write
static const byte fun_data_onewire_OneWire_write[25] = {
    0x52,0x06, // prelude
    0x11,0x1f,0x21, // names: write, self, buf
     // code info
    0xb1, // LOAD_FAST 1
    0x5f, // GET_ITER_STACK
    0x4b,0x0e, // FOR_ITER 14
    0xc2, // STORE_FAST 2
    0x12,0x18, // LOAD_GLOBAL '_ow'
    0x14,0x10, // LOAD_METHOD 'writebyte'
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'pin'
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x42,0x30, // JUMP -16
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_onewire_OneWire_write = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_onewire_OneWire_write,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 25,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 17,
        .line_info = fun_data_onewire_OneWire_write + 5,
        .line_info_top = fun_data_onewire_OneWire_write + 5,
        .opcodes = fun_data_onewire_OneWire_write + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_onewire_OneWire_write fun_data_onewire_OneWire_write[0]
#endif

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_OneWire_select_rom
static const byte fun_data_onewire_OneWire_select_rom[29] = {
    0x22,0x06, // prelude
    0x12,0x1f,0x24, // names: select_rom, self, rom
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x0b, // LOAD_METHOD 'reset'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x10, // LOAD_METHOD 'writebyte'
    0xb0, // LOAD_FAST 0
    0x13,0x13, // LOAD_ATTR 'MATCH_ROM'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x11, // LOAD_METHOD 'write'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_onewire_OneWire_select_rom = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_onewire_OneWire_select_rom,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 29,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 18,
        .line_info = fun_data_onewire_OneWire_select_rom + 5,
        .line_info_top = fun_data_onewire_OneWire_select_rom + 5,
        .opcodes = fun_data_onewire_OneWire_select_rom + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_onewire_OneWire_select_rom fun_data_onewire_OneWire_select_rom[0]
#endif

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_OneWire_scan
static const byte fun_data_onewire_OneWire_scan[57] = {
    0x49,0x04, // prelude
    0x14,0x1f, // names: scan, self
     // code info
    0x2b,0x00, // BUILD_LIST 0
    0xc1, // STORE_FAST 1
    0x22,0x80,0x41, // LOAD_CONST_SMALL_INT 65
    0xc2, // STORE_FAST 2
    0x50, // LOAD_CONST_FALSE
    0xc3, // STORE_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x5f, // JUMP 31
    0x57, // DUP_TOP
    0xc4, // STORE_FAST 4
    0xb0, // LOAD_FAST 0
    0x14,0x15, // LOAD_METHOD '_search_rom'
    0xb3, // LOAD_FAST 3
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc3, // STORE_FAST 3
    0xc2, // STORE_FAST 2
    0xb3, // LOAD_FAST 3
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0xb1, // LOAD_FAST 1
    0xb3, // LOAD_FAST 3
    0x2b,0x01, // BUILD_LIST 1
    0xe5, // BINARY_OP 14 __iadd__
    0xc1, // STORE_FAST 1
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x42,0x49, // JUMP 9
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x57, // DUP_TOP
    0x22,0x81,0x7f, // LOAD_CONST_SMALL_INT 255
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x1a, // POP_JUMP_IF_TRUE -38
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_onewire_OneWire_scan = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_onewire_OneWire_scan,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 57,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 20,
        .line_info = fun_data_onewire_OneWire_scan + 4,
        .line_info_top = fun_data_onewire_OneWire_scan + 4,
        .opcodes = fun_data_onewire_OneWire_scan + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_onewire_OneWire_scan fun_data_onewire_OneWire_scan[0]
#endif

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_OneWire__search_rom
static const byte fun_data_onewire_OneWire__search_rom[154] = {
    0x73,0x08, // prelude
    0x15,0x1f,0x25,0x26, // names: _search_rom, self, l_rom, diff
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x0b, // LOAD_METHOD 'reset'
    0x36,0x00, // CALL_METHOD 0
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x14,0x10, // LOAD_METHOD 'writebyte'
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR 'SEARCH_ROM'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x43,0x46, // POP_JUMP_IF_TRUE 6
    0x12,0x27, // LOAD_GLOBAL 'bytearray'
    0x88, // LOAD_CONST_SMALL_INT 8
    0x34,0x01, // CALL_FUNCTION 1
    0xc1, // STORE_FAST 1
    0x12,0x27, // LOAD_GLOBAL 'bytearray'
    0x88, // LOAD_CONST_SMALL_INT 8
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc4, // STORE_FAST 4
    0x22,0x80,0x40, // LOAD_CONST_SMALL_INT 64
    0xc5, // STORE_FAST 5
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0xdc,0x80, // JUMP 92
    0x57, // DUP_TOP
    0xc6, // STORE_FAST 6
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc7, // STORE_FAST 7
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0xc7,0x80, // JUMP 71
    0x57, // DUP_TOP
    0xc8, // STORE_FAST 8
    0xb0, // LOAD_FAST 0
    0x14,0x0c, // LOAD_METHOD 'readbit'
    0x36,0x00, // CALL_METHOD 0
    0xc9, // STORE_FAST 9
    0xb0, // LOAD_FAST 0
    0x14,0x0c, // LOAD_METHOD 'readbit'
    0x36,0x00, // CALL_METHOD 0
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0xb9, // LOAD_FAST 9
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x63, // RETURN_VALUE
    0x42,0x5a, // JUMP 26
    0xb9, // LOAD_FAST 9
    0x43,0x57, // POP_JUMP_IF_TRUE 23
    0xb2, // LOAD_FAST 2
    0xb5, // LOAD_FAST 5
    0xd8, // BINARY_OP 1 __gt__
    0x43,0x4e, // POP_JUMP_IF_TRUE 14
    0xb1, // LOAD_FAST 1
    0xb6, // LOAD_FAST 6
    0x55, // LOAD_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xb8, // LOAD_FAST 8
    0xf0, // BINARY_OP 25 __lshift__
    0xef, // BINARY_OP 24 __and__
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0xb2, // LOAD_FAST 2
    0xb5, // LOAD_FAST 5
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0xc9, // STORE_FAST 9
    0xb5, // LOAD_FAST 5
    0xc4, // STORE_FAST 4
    0xb0, // LOAD_FAST 0
    0x14,0x0f, // LOAD_METHOD 'writebit'
    0xb9, // LOAD_FAST 9
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb9, // LOAD_FAST 9
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0xb7, // LOAD_FAST 7
    0x81, // LOAD_CONST_SMALL_INT 1
    0xb8, // LOAD_FAST 8
    0xf0, // BINARY_OP 25 __lshift__
    0xe0, // BINARY_OP 9 __ior__
    0xc7, // STORE_FAST 7
    0xb5, // LOAD_FAST 5
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe6, // BINARY_OP 15 __isub__
    0xc5, // STORE_FAST 5
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x57, // DUP_TOP
    0x88, // LOAD_CONST_SMALL_INT 8
    0xd7, // BINARY_OP 0 __lt__
    0x43,0xb3,0x7f, // POP_JUMP_IF_TRUE -77
    0x59, // POP_TOP
    0xb7, // LOAD_FAST 7
    0xb3, // LOAD_FAST 3
    0xb6, // LOAD_FAST 6
    0x56, // STORE_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x57, // DUP_TOP
    0x88, // LOAD_CONST_SMALL_INT 8
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x9e,0x7f, // POP_JUMP_IF_TRUE -98
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0x2a,0x02, // BUILD_TUPLE 2
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_onewire_OneWire__search_rom = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_onewire_OneWire__search_rom,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 154,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 15,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 21,
        .line_info = fun_data_onewire_OneWire__search_rom + 6,
        .line_info_top = fun_data_onewire_OneWire__search_rom + 6,
        .opcodes = fun_data_onewire_OneWire__search_rom + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_onewire_OneWire__search_rom fun_data_onewire_OneWire__search_rom[0]
#endif

// child of onewire_OneWire
// frozen bytecode for file onewire.py, scope onewire_OneWire_crc8
static const byte fun_data_onewire_OneWire_crc8[13] = {
    0x22,0x06, // prelude
    0x17,0x1f,0x28, // names: crc8, self, data
     // code info
    0x12,0x18, // LOAD_GLOBAL '_ow'
    0x14,0x17, // LOAD_METHOD 'crc8'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_onewire_OneWire_crc8 = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_onewire_OneWire_crc8,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 13,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 23,
        .line_info = fun_data_onewire_OneWire_crc8 + 5,
        .line_info_top = fun_data_onewire_OneWire_crc8 + 5,
        .opcodes = fun_data_onewire_OneWire_crc8 + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_onewire_OneWire_crc8 fun_data_onewire_OneWire_crc8[0]
#endif

static const mp_raw_code_t *const children_onewire_OneWire[] = {
    (const mp_raw_code_t *)&proto_fun_onewire_OneWire___init__,
    (const mp_raw_code_t *)&proto_fun_onewire_OneWire_reset,
    (const mp_raw_code_t *)&proto_fun_onewire_OneWire_readbit,
    (const mp_raw_code_t *)&proto_fun_onewire_OneWire_readbyte,
    (const mp_raw_code_t *)&proto_fun_onewire_OneWire_readinto,
    (const mp_raw_code_t *)&proto_fun_onewire_OneWire_writebit,
    (const mp_raw_code_t *)&proto_fun_onewire_OneWire_writebyte,
    (const mp_raw_code_t *)&proto_fun_onewire_OneWire_write,
    (const mp_raw_code_t *)&proto_fun_onewire_OneWire_select_rom,
    (const mp_raw_code_t *)&proto_fun_onewire_OneWire_scan,
    (const mp_raw_code_t *)&proto_fun_onewire_OneWire__search_rom,
    (const mp_raw_code_t *)&proto_fun_onewire_OneWire_crc8,
};

static const mp_raw_code_truncated_t proto_fun_onewire_OneWire = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_onewire_OneWire,
    .children = (void *)&children_onewire_OneWire,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 80,
    .n_children = 12,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_onewire_OneWire + 3,
        .line_info_top = fun_data_onewire_OneWire + 3,
        .opcodes = fun_data_onewire_OneWire + 3,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_onewire__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_onewire_OneWireError,
    (const mp_raw_code_t *)&proto_fun_onewire_OneWire,
};

static const mp_raw_code_truncated_t proto_fun_onewire__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_onewire__lt_module_gt_,
    .children = (void *)&children_onewire__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 35,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_onewire__lt_module_gt_ + 3,
        .line_info_top = fun_data_onewire__lt_module_gt_ + 3,
        .opcodes = fun_data_onewire__lt_module_gt_ + 3,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_onewire[41] = {
    MP_QSTR_onewire_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR__onewire,
    MP_QSTR_OneWireError,
    MP_QSTR_OneWire,
    MP_QSTR_0_dot_1_dot_0,
    MP_QSTR___init__,
    MP_QSTR_pin,
    MP_QSTR_init,
    MP_QSTR_OPEN_DRAIN,
    MP_QSTR_PULL_UP,
    MP_QSTR_reset,
    MP_QSTR_readbit,
    MP_QSTR_readbyte,
    MP_QSTR_readinto,
    MP_QSTR_writebit,
    MP_QSTR_writebyte,
    MP_QSTR_write,
    MP_QSTR_select_rom,
    MP_QSTR_MATCH_ROM,
    MP_QSTR_scan,
    MP_QSTR__search_rom,
    MP_QSTR_SEARCH_ROM,
    MP_QSTR_crc8,
    MP_QSTR__ow,
    MP_QSTR_Exception,
    MP_QSTR___version__,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_SKIP_ROM,
    MP_QSTR_self,
    MP_QSTR_required,
    MP_QSTR_buf,
    MP_QSTR_len,
    MP_QSTR_value,
    MP_QSTR_rom,
    MP_QSTR_l_rom,
    MP_QSTR_diff,
    MP_QSTR_bytearray,
    MP_QSTR_data,
};

// constants
static const mp_rom_obj_tuple_t const_obj_onewire_0 = {{&mp_type_tuple}, 2, {
    MP_ROM_NONE,
    MP_ROM_INT(0),
}};

// constant table
static const mp_rom_obj_t const_obj_table_data_onewire[1] = {
    MP_ROM_PTR(&const_obj_onewire_0),
};

static const mp_frozen_module_t frozen_module_onewire = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_onewire,
        .obj_table = (mp_obj_t *)&const_obj_table_data_onewire,
    },
    .proto_fun = &proto_fun_onewire__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module ds18x20
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/ds18x20.mpy
// - frozen file name: ds18x20.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file ds18x20.py, scope ds18x20__lt_module_gt_
static const byte fun_data_ds18x20__lt_module_gt_[30] = {
    0x10,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING 'const'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'micropython'
    0x1c,0x02, // IMPORT_FROM 'const'
    0x16,0x02, // STORE_NAME 'const'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'DS18X20'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x04, // STORE_NAME 'DS18X20'
    0x10,0x05, // LOAD_CONST_STRING '0.1.0'
    0x16,0x17, // STORE_NAME '__version__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ds18x20__lt_module_gt_
// frozen bytecode for file ds18x20.py, scope ds18x20_DS18X20
static const byte fun_data_ds18x20_DS18X20[37] = {
    0x00,0x02, // prelude
    0x04, // names: DS18X20
     // code info
    0x11,0x18, // LOAD_NAME '__name__'
    0x16,0x19, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'DS18X20'
    0x16,0x1a, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x06, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x09, // STORE_NAME 'scan'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0a, // STORE_NAME 'convert_temp'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x0e, // STORE_NAME 'read_scratch'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x13, // STORE_NAME 'write_scratch'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x15, // STORE_NAME 'read_temp'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ds18x20_DS18X20
// frozen bytecode for file ds18x20.py, scope ds18x20_DS18X20___init__
static const byte fun_data_ds18x20_DS18X20___init__[19] = {
    0x1a,0x06, // prelude
    0x06,0x1b,0x1c, // names: __init__, self, onewire
     // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x07, // STORE_ATTR 'ow'
    0x12,0x1d, // LOAD_GLOBAL 'bytearray'
    0x89, // LOAD_CONST_SMALL_INT 9
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x18,0x08, // STORE_ATTR 'buf'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ds18x20_DS18X20___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ds18x20_DS18X20___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 19,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_ds18x20_DS18X20___init__ + 5,
        .line_info_top = fun_data_ds18x20_DS18X20___init__ + 5,
        .opcodes = fun_data_ds18x20_DS18X20___init__ + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_ds18x20_DS18X20___init__ fun_data_ds18x20_DS18X20___init__[0]
#endif

// child of ds18x20_DS18X20
// frozen bytecode for file ds18x20.py, scope ds18x20_DS18X20_scan
static const byte fun_data_ds18x20_DS18X20_scan[16] = {
    0x19,0x04, // prelude
    0x09,0x1b, // names: scan, self
     // code info
    0x32,0x00, // MAKE_FUNCTION 0
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'ow'
    0x14,0x09, // LOAD_METHOD 'scan'
    0x36,0x00, // CALL_METHOD 0
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
// child of ds18x20_DS18X20_scan
// frozen bytecode for file ds18x20.py, scope ds18x20_DS18X20_scan__lt_listcomp_gt_
static const byte fun_data_ds18x20_DS18X20_scan__lt_listcomp_gt_[25] = {
    0x41,0x04, // prelude
    0x16,0x20, // names: <listcomp>, *
     // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x0e, // FOR_ITER 14
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x23,0x01, // LOAD_CONST_OBJ 1
    0xdd, // BINARY_OP 6 <in>
    0x44,0x35, // POP_JUMP_IF_FALSE -11
    0xb1, // LOAD_FAST 1
    0x2f,0x14, // STORE_COMP 20
    0x42,0x30, // JUMP -16
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ds18x20_DS18X20_scan__lt_listcomp_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ds18x20_DS18X20_scan__lt_listcomp_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 25,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 22,
        .line_info = fun_data_ds18x20_DS18X20_scan__lt_listcomp_gt_ + 4,
        .line_info_top = fun_data_ds18x20_DS18X20_scan__lt_listcomp_gt_ + 4,
        .opcodes = fun_data_ds18x20_DS18X20_scan__lt_listcomp_gt_ + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_ds18x20_DS18X20_scan__lt_listcomp_gt_ fun_data_ds18x20_DS18X20_scan__lt_listcomp_gt_[0]
#endif

static const mp_raw_code_t *const children_ds18x20_DS18X20_scan[] = {
    (const mp_raw_code_t *)&proto_fun_ds18x20_DS18X20_scan__lt_listcomp_gt_,
};

static const mp_raw_code_truncated_t proto_fun_ds18x20_DS18X20_scan = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ds18x20_DS18X20_scan,
    .children = (void *)&children_ds18x20_DS18X20_scan,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 16,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_ds18x20_DS18X20_scan + 4,
        .line_info_top = fun_data_ds18x20_DS18X20_scan + 4,
        .opcodes = fun_data_ds18x20_DS18X20_scan + 4,
    },
    #endif
    #endif
};

// child of ds18x20_DS18X20
// frozen bytecode for file ds18x20.py, scope ds18x20_DS18X20_convert_temp
static const byte fun_data_ds18x20_DS18X20_convert_temp[39] = {
    0x19,0x04, // prelude
    0x0a,0x1b, // names: convert_temp, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'ow'
    0x14,0x0b, // LOAD_METHOD 'reset'
    0x52, // LOAD_CONST_TRUE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'ow'
    0x14,0x0c, // LOAD_METHOD 'writebyte'
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'ow'
    0x13,0x0d, // LOAD_ATTR 'SKIP_ROM'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'ow'
    0x14,0x0c, // LOAD_METHOD 'writebyte'
    0x22,0x80,0x44, // LOAD_CONST_SMALL_INT 68
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ds18x20_DS18X20_convert_temp = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ds18x20_DS18X20_convert_temp,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 39,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_ds18x20_DS18X20_convert_temp + 4,
        .line_info_top = fun_data_ds18x20_DS18X20_convert_temp + 4,
        .opcodes = fun_data_ds18x20_DS18X20_convert_temp + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_ds18x20_DS18X20_convert_temp fun_data_ds18x20_DS18X20_convert_temp[0]
#endif

// child of ds18x20_DS18X20
// frozen bytecode for file ds18x20.py, scope ds18x20_DS18X20_read_scratch
static const byte fun_data_ds18x20_DS18X20_read_scratch[68] = {
    0x22,0x06, // prelude
    0x0e,0x1b,0x1e, // names: read_scratch, self, rom
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'ow'
    0x14,0x0b, // LOAD_METHOD 'reset'
    0x52, // LOAD_CONST_TRUE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'ow'
    0x14,0x0f, // LOAD_METHOD 'select_rom'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'ow'
    0x14,0x0c, // LOAD_METHOD 'writebyte'
    0x22,0x81,0x3e, // LOAD_CONST_SMALL_INT 190
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'ow'
    0x14,0x10, // LOAD_METHOD 'readinto'
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'buf'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'ow'
    0x14,0x11, // LOAD_METHOD 'crc8'
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'buf'
    0x36,0x01, // CALL_METHOD 1
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x1f, // LOAD_GLOBAL 'Exception'
    0x10,0x12, // LOAD_CONST_STRING 'CRC error'
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'buf'
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ds18x20_DS18X20_read_scratch = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ds18x20_DS18X20_read_scratch,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 68,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_ds18x20_DS18X20_read_scratch + 5,
        .line_info_top = fun_data_ds18x20_DS18X20_read_scratch + 5,
        .opcodes = fun_data_ds18x20_DS18X20_read_scratch + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_ds18x20_DS18X20_read_scratch fun_data_ds18x20_DS18X20_read_scratch[0]
#endif

// child of ds18x20_DS18X20
// frozen bytecode for file ds18x20.py, scope ds18x20_DS18X20_write_scratch
static const byte fun_data_ds18x20_DS18X20_write_scratch[46] = {
    0x2b,0x08, // prelude
    0x13,0x1b,0x1e,0x08, // names: write_scratch, self, rom, buf
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'ow'
    0x14,0x0b, // LOAD_METHOD 'reset'
    0x52, // LOAD_CONST_TRUE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'ow'
    0x14,0x0f, // LOAD_METHOD 'select_rom'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'ow'
    0x14,0x0c, // LOAD_METHOD 'writebyte'
    0x22,0x80,0x4e, // LOAD_CONST_SMALL_INT 78
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'ow'
    0x14,0x14, // LOAD_METHOD 'write'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ds18x20_DS18X20_write_scratch = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ds18x20_DS18X20_write_scratch,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 46,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 19,
        .line_info = fun_data_ds18x20_DS18X20_write_scratch + 6,
        .line_info_top = fun_data_ds18x20_DS18X20_write_scratch + 6,
        .opcodes = fun_data_ds18x20_DS18X20_write_scratch + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_ds18x20_DS18X20_write_scratch fun_data_ds18x20_DS18X20_write_scratch[0]
#endif

// child of ds18x20_DS18X20
// frozen bytecode for file ds18x20.py, scope ds18x20_DS18X20_read_temp
static const byte fun_data_ds18x20_DS18X20_read_temp[103] = {
    0x3a,0x06, // prelude
    0x15,0x1b,0x1e, // names: read_temp, self, rom
     // code info
    0xb0, // LOAD_FAST 0
    0x14,0x0e, // LOAD_METHOD 'read_scratch'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc2, // STORE_FAST 2
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x90, // LOAD_CONST_SMALL_INT 16
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x72, // POP_JUMP_IF_FALSE 50
    0xb2, // LOAD_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x44,0x56, // POP_JUMP_IF_FALSE 22
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf1, // BINARY_OP 26 __rshift__
    0x22,0x81,0x00, // LOAD_CONST_SMALL_INT 128
    0xed, // BINARY_OP 22 __or__
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0xd2, // UNARY_OP 2 __invert__
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x22,0x81,0x7f, // LOAD_CONST_SMALL_INT 255
    0xef, // BINARY_OP 24 __and__
    0xd1, // UNARY_OP 1 __neg__
    0xc3, // STORE_FAST 3
    0x42,0x46, // JUMP 6
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf1, // BINARY_OP 26 __rshift__
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xf3, // BINARY_OP 28 __sub__
    0xb2, // LOAD_FAST 2
    0x87, // LOAD_CONST_SMALL_INT 7
    0x55, // LOAD_SUBSCR
    0xb2, // LOAD_FAST 2
    0x86, // LOAD_CONST_SMALL_INT 6
    0x55, // LOAD_SUBSCR
    0xf3, // BINARY_OP 28 __sub__
    0xb2, // LOAD_FAST 2
    0x87, // LOAD_CONST_SMALL_INT 7
    0x55, // LOAD_SUBSCR
    0xf7, // BINARY_OP 32 __truediv__
    0xf2, // BINARY_OP 27 __add__
    0x63, // RETURN_VALUE
    0xb2, // LOAD_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x88, // LOAD_CONST_SMALL_INT 8
    0xf0, // BINARY_OP 25 __lshift__
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xed, // BINARY_OP 22 __or__
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x22,0x82,0x80,0x00, // LOAD_CONST_SMALL_INT 32768
    0xef, // BINARY_OP 24 __and__
    0x44,0x4a, // POP_JUMP_IF_FALSE 10
    0xb3, // LOAD_FAST 3
    0x22,0x83,0xff,0x7f, // LOAD_CONST_SMALL_INT 65535
    0xee, // BINARY_OP 23 __xor__
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0xd1, // UNARY_OP 1 __neg__
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x90, // LOAD_CONST_SMALL_INT 16
    0xf7, // BINARY_OP 32 __truediv__
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ds18x20_DS18X20_read_temp = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ds18x20_DS18X20_read_temp,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 103,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 21,
        .line_info = fun_data_ds18x20_DS18X20_read_temp + 5,
        .line_info_top = fun_data_ds18x20_DS18X20_read_temp + 5,
        .opcodes = fun_data_ds18x20_DS18X20_read_temp + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_ds18x20_DS18X20_read_temp fun_data_ds18x20_DS18X20_read_temp[0]
#endif

static const mp_raw_code_t *const children_ds18x20_DS18X20[] = {
    (const mp_raw_code_t *)&proto_fun_ds18x20_DS18X20___init__,
    (const mp_raw_code_t *)&proto_fun_ds18x20_DS18X20_scan,
    (const mp_raw_code_t *)&proto_fun_ds18x20_DS18X20_convert_temp,
    (const mp_raw_code_t *)&proto_fun_ds18x20_DS18X20_read_scratch,
    (const mp_raw_code_t *)&proto_fun_ds18x20_DS18X20_write_scratch,
    (const mp_raw_code_t *)&proto_fun_ds18x20_DS18X20_read_temp,
};

static const mp_raw_code_truncated_t proto_fun_ds18x20_DS18X20 = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ds18x20_DS18X20,
    .children = (void *)&children_ds18x20_DS18X20,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 37,
    .n_children = 6,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_ds18x20_DS18X20 + 3,
        .line_info_top = fun_data_ds18x20_DS18X20 + 3,
        .opcodes = fun_data_ds18x20_DS18X20 + 3,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_ds18x20__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_ds18x20_DS18X20,
};

static const mp_raw_code_truncated_t proto_fun_ds18x20__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ds18x20__lt_module_gt_,
    .children = (void *)&children_ds18x20__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 30,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_ds18x20__lt_module_gt_ + 3,
        .line_info_top = fun_data_ds18x20__lt_module_gt_ + 3,
        .opcodes = fun_data_ds18x20__lt_module_gt_ + 3,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_ds18x20[33] = {
    MP_QSTR_ds18x20_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_const,
    MP_QSTR_micropython,
    MP_QSTR_DS18X20,
    MP_QSTR_0_dot_1_dot_0,
    MP_QSTR___init__,
    MP_QSTR_ow,
    MP_QSTR_buf,
    MP_QSTR_scan,
    MP_QSTR_convert_temp,
    MP_QSTR_reset,
    MP_QSTR_writebyte,
    MP_QSTR_SKIP_ROM,
    MP_QSTR_read_scratch,
    MP_QSTR_select_rom,
    MP_QSTR_readinto,
    MP_QSTR_crc8,
    MP_QSTR_CRC_space_error,
    MP_QSTR_write_scratch,
    MP_QSTR_write,
    MP_QSTR_read_temp,
    MP_QSTR__lt_listcomp_gt_,
    MP_QSTR___version__,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_self,
    MP_QSTR_onewire,
    MP_QSTR_bytearray,
    MP_QSTR_rom,
    MP_QSTR_Exception,
    MP_QSTR__star_,
};

// constants
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ds18x20_0 = {{&mp_type_float}, (mp_float_t)0.25};
#define const_obj_ds18x20_0_macro MP_ROM_PTR(&const_obj_ds18x20_0)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ds18x20_0_macro ((mp_rom_obj_t)(0xbf000002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ds18x20_0_macro ((mp_rom_obj_t)(0xbfd4000000000000))
#endif
static const mp_rom_obj_tuple_t const_obj_ds18x20_1 = {{&mp_type_tuple}, 3, {
    MP_ROM_INT(16),
    MP_ROM_INT(34),
    MP_ROM_INT(40),
}};

// constant table
static const mp_rom_obj_t const_obj_table_data_ds18x20[2] = {
    const_obj_ds18x20_0_macro,
    MP_ROM_PTR(&const_obj_ds18x20_1),
};

static const mp_frozen_module_t frozen_module_ds18x20 = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_ds18x20,
        .obj_table = (mp_obj_t *)&const_obj_table_data_ds18x20,
    },
    .proto_fun = &proto_fun_ds18x20__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module neopixel
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/neopixel.mpy
// - frozen file name: neopixel.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file neopixel.py, scope neopixel__lt_module_gt_
static const byte fun_data_neopixel__lt_module_gt_[30] = {
    0x10,0x02, // prelude
    0x01, // names: <module>
     // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING 'bitstream'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'machine'
    0x1c,0x02, // IMPORT_FROM 'bitstream'
    0x16,0x02, // STORE_NAME 'bitstream'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'NeoPixel'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x04, // STORE_NAME 'NeoPixel'
    0x10,0x05, // LOAD_CONST_STRING '0.1.0'
    0x16,0x15, // STORE_NAME '__version__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of neopixel__lt_module_gt_
// frozen bytecode for file neopixel.py, scope neopixel_NeoPixel
static const byte fun_data_neopixel_NeoPixel[46] = {
    0x08,0x02, // prelude
    0x04, // names: NeoPixel
     // code info
    0x11,0x16, // LOAD_NAME '__name__'
    0x16,0x17, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'NeoPixel'
    0x16,0x18, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x16,0x10, // STORE_NAME 'ORDER'
    0x83, // LOAD_CONST_SMALL_INT 3
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x06, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x0e, // STORE_NAME '__len__'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x0f, // STORE_NAME '__setitem__'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x11, // STORE_NAME '__getitem__'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x12, // STORE_NAME 'fill'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x13, // STORE_NAME 'write'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of neopixel_NeoPixel
// frozen bytecode for file neopixel.py, scope neopixel_NeoPixel___init__
static const byte fun_data_neopixel_NeoPixel___init__[69] = {
    0xb9,0x84,0x01,0x0c, // prelude
    0x06,0x19,0x07,0x08,0x09,0x0d, // names: __init__, self, pin, n, bpp, timing
     // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x07, // STORE_ATTR 'pin'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x08, // STORE_ATTR 'n'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x09, // STORE_ATTR 'bpp'
    0x12,0x1a, // LOAD_GLOBAL 'bytearray'
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xf4, // BINARY_OP 29 __mul__
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x18,0x0a, // STORE_ATTR 'buf'
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'pin'
    0x14,0x0b, // LOAD_METHOD 'init'
    0xb1, // LOAD_FAST 1
    0x13,0x0c, // LOAD_ATTR 'OUT'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x1b, // LOAD_GLOBAL 'isinstance'
    0xb4, // LOAD_FAST 4
    0x12,0x1c, // LOAD_GLOBAL 'int'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0xb4, // LOAD_FAST 4
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x42,0x42, // JUMP 2
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x42,0x41, // JUMP 1
    0xb4, // LOAD_FAST 4
    0xb0, // LOAD_FAST 0
    0x18,0x0d, // STORE_ATTR 'timing'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_neopixel_NeoPixel___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_neopixel_NeoPixel___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 69,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 5,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_neopixel_NeoPixel___init__ + 10,
        .line_info_top = fun_data_neopixel_NeoPixel___init__ + 10,
        .opcodes = fun_data_neopixel_NeoPixel___init__ + 10,
    },
    #endif
    #endif
};
#else
#define proto_fun_neopixel_NeoPixel___init__ fun_data_neopixel_NeoPixel___init__[0]
#endif

// child of neopixel_NeoPixel
// frozen bytecode for file neopixel.py, scope neopixel_NeoPixel___len__
static const byte fun_data_neopixel_NeoPixel___len__[8] = {
    0x09,0x04, // prelude
    0x0e,0x19, // names: __len__, self
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'n'
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_neopixel_NeoPixel___len__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_neopixel_NeoPixel___len__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 8,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_neopixel_NeoPixel___len__ + 4,
        .line_info_top = fun_data_neopixel_NeoPixel___len__ + 4,
        .opcodes = fun_data_neopixel_NeoPixel___len__ + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_neopixel_NeoPixel___len__ fun_data_neopixel_NeoPixel___len__[0]
#endif

// child of neopixel_NeoPixel
// frozen bytecode for file neopixel.py, scope neopixel_NeoPixel___setitem__
static const byte fun_data_neopixel_NeoPixel___setitem__[45] = {
    0x53,0x08, // prelude
    0x0f,0x19,0x1d,0x1e, // names: __setitem__, self, i, v
     // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'bpp'
    0xf4, // BINARY_OP 29 __mul__
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'bpp'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x52, // JUMP 18
    0x57, // DUP_TOP
    0xc1, // STORE_FAST 1
    0xb2, // LOAD_FAST 2
    0xb1, // LOAD_FAST 1
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x0a, // LOAD_ATTR 'buf'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'ORDER'
    0xb1, // LOAD_FAST 1
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0x56, // STORE_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x29, // POP_JUMP_IF_TRUE -23
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_neopixel_NeoPixel___setitem__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_neopixel_NeoPixel___setitem__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 45,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 15,
        .line_info = fun_data_neopixel_NeoPixel___setitem__ + 6,
        .line_info_top = fun_data_neopixel_NeoPixel___setitem__ + 6,
        .opcodes = fun_data_neopixel_NeoPixel___setitem__ + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_neopixel_NeoPixel___setitem__ fun_data_neopixel_NeoPixel___setitem__[0]
#endif

// child of neopixel_NeoPixel
// frozen bytecode for file neopixel.py, scope neopixel_NeoPixel___getitem__
static const byte fun_data_neopixel_NeoPixel___getitem__[37] = {
    0x32,0x86,0x01, // prelude
    0x11,0x19,0x1d, // names: __getitem__, self, i
    0x00,0x02, // code info
    0xb1, // LOAD_FAST 1
    0x25,0x00, // LOAD_DEREF 0
    0x13,0x09, // LOAD_ATTR 'bpp'
    0xf4, // BINARY_OP 29 __mul__
    0x27,0x02, // STORE_DEREF 2
    0x12,0x1f, // LOAD_GLOBAL 'tuple'
    0xb0, // LOAD_FAST 0
    0xb2, // LOAD_FAST 2
    0x20,0x00,0x02, // MAKE_CLOSURE 0
    0x12,0x20, // LOAD_GLOBAL 'range'
    0x25,0x00, // LOAD_DEREF 0
    0x13,0x09, // LOAD_ATTR 'bpp'
    0x34,0x01, // CALL_FUNCTION 1
    0x5e, // GET_ITER
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x63, // RETURN_VALUE
};
// child of neopixel_NeoPixel___getitem__
// frozen bytecode for file neopixel.py, scope neopixel_NeoPixel___getitem____lt_genexpr_gt_
static const byte fun_data_neopixel_NeoPixel___getitem____lt_genexpr_gt_[34] = {
    0xdb,0x40,0x08, // prelude
    0x14,0x22,0x22,0x22, // names: <genexpr>, *, *, *
     // code info
    0x53, // LOAD_NULL
    0xb2, // LOAD_FAST 2
    0x53, // LOAD_NULL
    0x53, // LOAD_NULL
    0x4b,0x13, // FOR_ITER 19
    0xc3, // STORE_FAST 3
    0x25,0x00, // LOAD_DEREF 0
    0x13,0x0a, // LOAD_ATTR 'buf'
    0x25,0x01, // LOAD_DEREF 1
    0x25,0x00, // LOAD_DEREF 0
    0x13,0x10, // LOAD_ATTR 'ORDER'
    0xb3, // LOAD_FAST 3
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0x55, // LOAD_SUBSCR
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x42,0x2b, // JUMP -21
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_neopixel_NeoPixel___getitem____lt_genexpr_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_neopixel_NeoPixel___getitem____lt_genexpr_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 34,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 20,
        .line_info = fun_data_neopixel_NeoPixel___getitem____lt_genexpr_gt_ + 7,
        .line_info_top = fun_data_neopixel_NeoPixel___getitem____lt_genexpr_gt_ + 7,
        .opcodes = fun_data_neopixel_NeoPixel___getitem____lt_genexpr_gt_ + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_neopixel_NeoPixel___getitem____lt_genexpr_gt_ fun_data_neopixel_NeoPixel___getitem____lt_genexpr_gt_[0]
#endif

static const mp_raw_code_t *const children_neopixel_NeoPixel___getitem__[] = {
    (const mp_raw_code_t *)&proto_fun_neopixel_NeoPixel___getitem____lt_genexpr_gt_,
};

static const mp_raw_code_truncated_t proto_fun_neopixel_NeoPixel___getitem__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_neopixel_NeoPixel___getitem__,
    .children = (void *)&children_neopixel_NeoPixel___getitem__,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 37,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 17,
        .line_info = fun_data_neopixel_NeoPixel___getitem__ + 6,
        .line_info_top = fun_data_neopixel_NeoPixel___getitem__ + 6,
        .opcodes = fun_data_neopixel_NeoPixel___getitem__ + 8,
    },
    #endif
    #endif
};

// child of neopixel_NeoPixel
// frozen bytecode for file neopixel.py, scope neopixel_NeoPixel_fill
static const byte fun_data_neopixel_NeoPixel_fill[63] = {
    0x62,0x06, // prelude
    0x12,0x19,0x1e, // names: fill, self, v
     // code info
    0xb0, // LOAD_FAST 0
    0x13,0x0a, // LOAD_ATTR 'buf'
    0xc2, // STORE_FAST 2
    0x12,0x21, // LOAD_GLOBAL 'len'
    0xb0, // LOAD_FAST 0
    0x13,0x0a, // LOAD_ATTR 'buf'
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'bpp'
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x5d, // JUMP 29
    0x57, // DUP_TOP
    0xc5, // STORE_FAST 5
    0xb1, // LOAD_FAST 1
    0xb5, // LOAD_FAST 5
    0x55, // LOAD_SUBSCR
    0xc6, // STORE_FAST 6
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'ORDER'
    0xb5, // LOAD_FAST 5
    0x55, // LOAD_SUBSCR
    0xc7, // STORE_FAST 7
    0x42,0x48, // JUMP 8
    0xb6, // LOAD_FAST 6
    0xb2, // LOAD_FAST 2
    0xb7, // LOAD_FAST 7
    0x56, // STORE_SUBSCR
    0xb7, // LOAD_FAST 7
    0xb4, // LOAD_FAST 4
    0xe5, // BINARY_OP 14 __iadd__
    0xc7, // STORE_FAST 7
    0xb7, // LOAD_FAST 7
    0xb3, // LOAD_FAST 3
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x33, // POP_JUMP_IF_TRUE -13
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x1e, // POP_JUMP_IF_TRUE -34
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_neopixel_NeoPixel_fill = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_neopixel_NeoPixel_fill,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 63,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 18,
        .line_info = fun_data_neopixel_NeoPixel_fill + 5,
        .line_info_top = fun_data_neopixel_NeoPixel_fill + 5,
        .opcodes = fun_data_neopixel_NeoPixel_fill + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_neopixel_NeoPixel_fill fun_data_neopixel_NeoPixel_fill[0]
#endif

// child of neopixel_NeoPixel
// frozen bytecode for file neopixel.py, scope neopixel_NeoPixel_write
static const byte fun_data_neopixel_NeoPixel_write[21] = {
    0x29,0x04, // prelude
    0x13,0x19, // names: write, self
     // code info
    0x12,0x02, // LOAD_GLOBAL 'bitstream'
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'pin'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x13,0x0d, // LOAD_ATTR 'timing'
    0xb0, // LOAD_FAST 0
    0x13,0x0a, // LOAD_ATTR 'buf'
    0x34,0x04, // CALL_FUNCTION 4
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_neopixel_NeoPixel_write = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_neopixel_NeoPixel_write,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 21,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 19,
        .line_info = fun_data_neopixel_NeoPixel_write + 4,
        .line_info_top = fun_data_neopixel_NeoPixel_write + 4,
        .opcodes = fun_data_neopixel_NeoPixel_write + 4,
    },
    #endif
    #endif
};
#else
#define proto_fun_neopixel_NeoPixel_write fun_data_neopixel_NeoPixel_write[0]
#endif

static const mp_raw_code_t *const children_neopixel_NeoPixel[] = {
    (const mp_raw_code_t *)&proto_fun_neopixel_NeoPixel___init__,
    (const mp_raw_code_t *)&proto_fun_neopixel_NeoPixel___len__,
    (const mp_raw_code_t *)&proto_fun_neopixel_NeoPixel___setitem__,
    (const mp_raw_code_t *)&proto_fun_neopixel_NeoPixel___getitem__,
    (const mp_raw_code_t *)&proto_fun_neopixel_NeoPixel_fill,
    (const mp_raw_code_t *)&proto_fun_neopixel_NeoPixel_write,
};

static const mp_raw_code_truncated_t proto_fun_neopixel_NeoPixel = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_neopixel_NeoPixel,
    .children = (void *)&children_neopixel_NeoPixel,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 46,
    .n_children = 6,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_neopixel_NeoPixel + 3,
        .line_info_top = fun_data_neopixel_NeoPixel + 3,
        .opcodes = fun_data_neopixel_NeoPixel + 3,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_neopixel__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_neopixel_NeoPixel,
};

static const mp_raw_code_truncated_t proto_fun_neopixel__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_neopixel__lt_module_gt_,
    .children = (void *)&children_neopixel__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 30,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_neopixel__lt_module_gt_ + 3,
        .line_info_top = fun_data_neopixel__lt_module_gt_ + 3,
        .opcodes = fun_data_neopixel__lt_module_gt_ + 3,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_neopixel[35] = {
    MP_QSTR_neopixel_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_bitstream,
    MP_QSTR_machine,
    MP_QSTR_NeoPixel,
    MP_QSTR_0_dot_1_dot_0,
    MP_QSTR___init__,
    MP_QSTR_pin,
    MP_QSTR_n,
    MP_QSTR_bpp,
    MP_QSTR_buf,
    MP_QSTR_init,
    MP_QSTR_OUT,
    MP_QSTR_timing,
    MP_QSTR___len__,
    MP_QSTR___setitem__,
    MP_QSTR_ORDER,
    MP_QSTR___getitem__,
    MP_QSTR_fill,
    MP_QSTR_write,
    MP_QSTR__lt_genexpr_gt_,
    MP_QSTR___version__,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_self,
    MP_QSTR_bytearray,
    MP_QSTR_isinstance,
    MP_QSTR_int,
    MP_QSTR_i,
    MP_QSTR_v,
    MP_QSTR_tuple,
    MP_QSTR_range,
    MP_QSTR_len,
    MP_QSTR__star_,
};

// constants
static const mp_rom_obj_tuple_t const_obj_neopixel_0 = {{&mp_type_tuple}, 4, {
    MP_ROM_INT(1),
    MP_ROM_INT(0),
    MP_ROM_INT(2),
    MP_ROM_INT(3),
}};
static const mp_rom_obj_tuple_t const_obj_neopixel_1 = {{&mp_type_tuple}, 4, {
    MP_ROM_INT(400),
    MP_ROM_INT(850),
    MP_ROM_INT(800),
    MP_ROM_INT(450),
}};
static const mp_rom_obj_tuple_t const_obj_neopixel_2 = {{&mp_type_tuple}, 4, {
    MP_ROM_INT(800),
    MP_ROM_INT(1700),
    MP_ROM_INT(1600),
    MP_ROM_INT(900),
}};

// constant table
static const mp_rom_obj_t const_obj_table_data_neopixel[3] = {
    MP_ROM_PTR(&const_obj_neopixel_0),
    MP_ROM_PTR(&const_obj_neopixel_1),
    MP_ROM_PTR(&const_obj_neopixel_2),
};

static const mp_frozen_module_t frozen_module_neopixel = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_neopixel,
        .obj_table = (mp_obj_t *)&const_obj_table_data_neopixel,
    },
    .proto_fun = &proto_fun_neopixel__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module umqtt_robust
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/umqtt/robust.mpy
// - frozen file name: umqtt/robust.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file umqtt/robust.py, scope umqtt_robust__lt_module_gt_
static const byte fun_data_umqtt_robust__lt_module_gt_[44] = {
    0x18,0x0a, // prelude
    0x01, // names: <module>
    0x26,0x6c,0x8d,0x33, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'time'
    0x16,0x02, // STORE_NAME 'time'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x03, // LOAD_CONST_STRING 'simple'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x04, // IMPORT_NAME ''
    0x1c,0x03, // IMPORT_FROM 'simple'
    0x16,0x03, // STORE_NAME 'simple'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x05, // LOAD_CONST_STRING 'MQTTClient'
    0x11,0x03, // LOAD_NAME 'simple'
    0x13,0x05, // LOAD_ATTR 'MQTTClient'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x05, // STORE_NAME 'MQTTClient'
    0x10,0x06, // LOAD_CONST_STRING '1.0.2'
    0x16,0x14, // STORE_NAME '__version__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of umqtt_robust__lt_module_gt_
// frozen bytecode for file umqtt/robust.py, scope umqtt_robust_MQTTClient
static const byte fun_data_umqtt_robust_MQTTClient[74] = {
    0x18,0x1d, // prelude
    0x05, // names: MQTTClient
    0x68,0x40,0x23,0x43,0x64,0x84,0x07,0x86,0x0a,0x8b,0x08,0x86,0x08,0x00, // code info
    0x11,0x15, // LOAD_NAME '__name__'
    0x16,0x16, // STORE_NAME '__module__'
    0x10,0x05, // LOAD_CONST_STRING 'MQTTClient'
    0x16,0x17, // STORE_NAME '__qualname__'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x16,0x09, // STORE_NAME 'DELAY'
    0x50, // LOAD_CONST_FALSE
    0x16,0x0b, // STORE_NAME 'DEBUG'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x07, // STORE_NAME 'delay'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x0a, // STORE_NAME 'log'
    0xb0, // LOAD_FAST 0
    0x20,0x02,0x01, // MAKE_CLOSURE 2
    0x16,0x0d, // STORE_NAME 'reconnect'
    0x50, // LOAD_CONST_FALSE
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0xb0, // LOAD_FAST 0
    0x21,0x03,0x01, // MAKE_CLOSURE_DEFARGS 3
    0x16,0x0f, // STORE_NAME 'publish'
    0xb0, // LOAD_FAST 0
    0x20,0x04,0x01, // MAKE_CLOSURE 4
    0x16,0x10, // STORE_NAME 'wait_msg'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0xb0, // LOAD_FAST 0
    0x21,0x05,0x01, // MAKE_CLOSURE_DEFARGS 5
    0x16,0x11, // STORE_NAME 'check_msg'
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
// child of umqtt_robust_MQTTClient
// frozen bytecode for file umqtt/robust.py, scope umqtt_robust_MQTTClient_delay
static const byte fun_data_umqtt_robust_MQTTClient_delay[19] = {
    0x22,0x0a, // prelude
    0x07,0x18,0x19, // names: delay, self, i
    0x80,0x09, // code info
    0x12,0x02, // LOAD_GLOBAL 'time'
    0x14,0x08, // LOAD_METHOD 'sleep'
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'DELAY'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_robust_MQTTClient_delay = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_robust_MQTTClient_delay,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 19,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_umqtt_robust_MQTTClient_delay + 5,
        .line_info_top = fun_data_umqtt_robust_MQTTClient_delay + 7,
        .opcodes = fun_data_umqtt_robust_MQTTClient_delay + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_robust_MQTTClient_delay fun_data_umqtt_robust_MQTTClient_delay[0]
#endif

// child of umqtt_robust_MQTTClient
// frozen bytecode for file umqtt/robust.py, scope umqtt_robust_MQTTClient_log
static const byte fun_data_umqtt_robust_MQTTClient_log[41] = {
    0x2b,0x12, // prelude
    0x0a,0x18,0x1a,0x1b, // names: log, self, in_reconnect, e
    0x80,0x0c,0x25,0x23,0x4b, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'DEBUG'
    0x44,0x57, // POP_JUMP_IF_FALSE 23
    0xb1, // LOAD_FAST 1
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0x12,0x1c, // LOAD_GLOBAL 'print'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0xb2, // LOAD_FAST 2
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x49, // JUMP 9
    0x12,0x1c, // LOAD_GLOBAL 'print'
    0x10,0x0c, // LOAD_CONST_STRING 'mqtt: %r'
    0xb2, // LOAD_FAST 2
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_robust_MQTTClient_log = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_robust_MQTTClient_log,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 41,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_umqtt_robust_MQTTClient_log + 6,
        .line_info_top = fun_data_umqtt_robust_MQTTClient_log + 11,
        .opcodes = fun_data_umqtt_robust_MQTTClient_log + 11,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_robust_MQTTClient_log fun_data_umqtt_robust_MQTTClient_log[0]
#endif

// child of umqtt_robust_MQTTClient
// frozen bytecode for file umqtt/robust.py, scope umqtt_robust_MQTTClient_reconnect
static const byte fun_data_umqtt_robust_MQTTClient_reconnect[70] = {
    0xd2,0x02,0x16, // prelude
    0x0d,0x1d,0x18, // names: reconnect, *, self
    0x80,0x13,0x22,0x20,0x22,0x54,0x28,0x24, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc2, // STORE_FAST 2
    0x48,0x0b, // SETUP_EXCEPT 11
    0x12,0x1e, // LOAD_GLOBAL 'super'
    0x25,0x00, // LOAD_DEREF 0
    0xb1, // LOAD_FAST 1
    0x15,0x0e, // LOAD_SUPER_METHOD 'connect'
    0x50, // LOAD_CONST_FALSE
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
    0x57, // DUP_TOP
    0x12,0x1f, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x5e, // POP_JUMP_IF_FALSE 30
    0xc3, // STORE_FAST 3
    0x49,0x14, // SETUP_FINALLY 20
    0xb1, // LOAD_FAST 1
    0x14,0x0a, // LOAD_METHOD 'log'
    0x52, // LOAD_CONST_TRUE
    0xb3, // LOAD_FAST 3
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0xc2, // STORE_FAST 2
    0xb1, // LOAD_FAST 1
    0x14,0x07, // LOAD_METHOD 'delay'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc3, // STORE_FAST 3
    0x28,0x03, // DELETE_FAST 3
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x42,0x0c, // JUMP -52
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_robust_MQTTClient_reconnect = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_robust_MQTTClient_reconnect,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 70,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_umqtt_robust_MQTTClient_reconnect + 6,
        .line_info_top = fun_data_umqtt_robust_MQTTClient_reconnect + 14,
        .opcodes = fun_data_umqtt_robust_MQTTClient_reconnect + 14,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_robust_MQTTClient_reconnect fun_data_umqtt_robust_MQTTClient_reconnect[0]
#endif

// child of umqtt_robust_MQTTClient
// frozen bytecode for file umqtt/robust.py, scope umqtt_robust_MQTTClient_publish
static const byte fun_data_umqtt_robust_MQTTClient_publish[69] = {
    0xea,0x86,0x01,0x1a, // prelude
    0x0f,0x1d,0x18,0x20,0x21,0x22,0x23, // names: publish, *, self, topic, msg, retain, qos
    0x80,0x1d,0x20,0x22,0x57,0x31, // code info
    0x48,0x0e, // SETUP_EXCEPT 14
    0x12,0x1e, // LOAD_GLOBAL 'super'
    0x25,0x00, // LOAD_DEREF 0
    0xb1, // LOAD_FAST 1
    0x15,0x0f, // LOAD_SUPER_METHOD 'publish'
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0xb5, // LOAD_FAST 5
    0x36,0x04, // CALL_METHOD 4
    0x63, // RETURN_VALUE
    0x57, // DUP_TOP
    0x12,0x1f, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x53, // POP_JUMP_IF_FALSE 19
    0xc6, // STORE_FAST 6
    0x49,0x09, // SETUP_FINALLY 9
    0xb1, // LOAD_FAST 1
    0x14,0x0a, // LOAD_METHOD 'log'
    0x50, // LOAD_CONST_FALSE
    0xb6, // LOAD_FAST 6
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc6, // STORE_FAST 6
    0x28,0x06, // DELETE_FAST 6
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xb1, // LOAD_FAST 1
    0x14,0x0d, // LOAD_METHOD 'reconnect'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x42,0x0e, // JUMP -50
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_robust_MQTTClient_publish = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_robust_MQTTClient_publish,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 69,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 6,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 15,
        .line_info = fun_data_umqtt_robust_MQTTClient_publish + 11,
        .line_info_top = fun_data_umqtt_robust_MQTTClient_publish + 17,
        .opcodes = fun_data_umqtt_robust_MQTTClient_publish + 17,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_robust_MQTTClient_publish fun_data_umqtt_robust_MQTTClient_publish[0]
#endif

// child of umqtt_robust_MQTTClient
// frozen bytecode for file umqtt/robust.py, scope umqtt_robust_MQTTClient_wait_msg
static const byte fun_data_umqtt_robust_MQTTClient_wait_msg[60] = {
    0xca,0x02,0x12, // prelude
    0x10,0x1d,0x18, // names: wait_msg, *, self
    0x80,0x25,0x20,0x22,0x53,0x31, // code info
    0x48,0x0a, // SETUP_EXCEPT 10
    0x12,0x1e, // LOAD_GLOBAL 'super'
    0x25,0x00, // LOAD_DEREF 0
    0xb1, // LOAD_FAST 1
    0x15,0x10, // LOAD_SUPER_METHOD 'wait_msg'
    0x36,0x00, // CALL_METHOD 0
    0x63, // RETURN_VALUE
    0x57, // DUP_TOP
    0x12,0x1f, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x53, // POP_JUMP_IF_FALSE 19
    0xc2, // STORE_FAST 2
    0x49,0x09, // SETUP_FINALLY 9
    0xb1, // LOAD_FAST 1
    0x14,0x0a, // LOAD_METHOD 'log'
    0x50, // LOAD_CONST_FALSE
    0xb2, // LOAD_FAST 2
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc2, // STORE_FAST 2
    0x28,0x02, // DELETE_FAST 2
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xb1, // LOAD_FAST 1
    0x14,0x0d, // LOAD_METHOD 'reconnect'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x42,0x12, // JUMP -46
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_robust_MQTTClient_wait_msg = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_robust_MQTTClient_wait_msg,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 60,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 16,
        .line_info = fun_data_umqtt_robust_MQTTClient_wait_msg + 6,
        .line_info_top = fun_data_umqtt_robust_MQTTClient_wait_msg + 12,
        .opcodes = fun_data_umqtt_robust_MQTTClient_wait_msg + 12,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_robust_MQTTClient_wait_msg fun_data_umqtt_robust_MQTTClient_wait_msg[0]
#endif

// child of umqtt_robust_MQTTClient
// frozen bytecode for file umqtt/robust.py, scope umqtt_robust_MQTTClient_check_msg
static const byte fun_data_umqtt_robust_MQTTClient_check_msg[79] = {
    0xd3,0x03,0x18, // prelude
    0x11,0x1d,0x18,0x24, // names: check_msg, *, self, attempts
    0x80,0x2d,0x22,0x29,0x22,0x53,0x31,0x26, // code info
    0x42,0x79, // JUMP 57
    0xb1, // LOAD_FAST 1
    0x13,0x12, // LOAD_ATTR 'sock'
    0x14,0x13, // LOAD_METHOD 'setblocking'
    0x50, // LOAD_CONST_FALSE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x48,0x0a, // SETUP_EXCEPT 10
    0x12,0x1e, // LOAD_GLOBAL 'super'
    0x25,0x00, // LOAD_DEREF 0
    0xb1, // LOAD_FAST 1
    0x15,0x10, // LOAD_SUPER_METHOD 'wait_msg'
    0x36,0x00, // CALL_METHOD 0
    0x63, // RETURN_VALUE
    0x57, // DUP_TOP
    0x12,0x1f, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x53, // POP_JUMP_IF_FALSE 19
    0xc3, // STORE_FAST 3
    0x49,0x09, // SETUP_FINALLY 9
    0xb1, // LOAD_FAST 1
    0x14,0x0a, // LOAD_METHOD 'log'
    0x50, // LOAD_CONST_FALSE
    0xb3, // LOAD_FAST 3
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0xc3, // STORE_FAST 3
    0x28,0x03, // DELETE_FAST 3
    0x5d, // END_FINALLY
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0xb1, // LOAD_FAST 1
    0x14,0x0d, // LOAD_METHOD 'reconnect'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe6, // BINARY_OP 15 __isub__
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x43,0x04, // POP_JUMP_IF_TRUE -60
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_robust_MQTTClient_check_msg = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_robust_MQTTClient_check_msg,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 79,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 17,
        .line_info = fun_data_umqtt_robust_MQTTClient_check_msg + 7,
        .line_info_top = fun_data_umqtt_robust_MQTTClient_check_msg + 15,
        .opcodes = fun_data_umqtt_robust_MQTTClient_check_msg + 15,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_robust_MQTTClient_check_msg fun_data_umqtt_robust_MQTTClient_check_msg[0]
#endif

static const mp_raw_code_t *const children_umqtt_robust_MQTTClient[] = {
    (const mp_raw_code_t *)&proto_fun_umqtt_robust_MQTTClient_delay,
    (const mp_raw_code_t *)&proto_fun_umqtt_robust_MQTTClient_log,
    (const mp_raw_code_t *)&proto_fun_umqtt_robust_MQTTClient_reconnect,
    (const mp_raw_code_t *)&proto_fun_umqtt_robust_MQTTClient_publish,
    (const mp_raw_code_t *)&proto_fun_umqtt_robust_MQTTClient_wait_msg,
    (const mp_raw_code_t *)&proto_fun_umqtt_robust_MQTTClient_check_msg,
};

static const mp_raw_code_truncated_t proto_fun_umqtt_robust_MQTTClient = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_robust_MQTTClient,
    .children = (void *)&children_umqtt_robust_MQTTClient,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 74,
    .n_children = 6,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 5,
        .line_info = fun_data_umqtt_robust_MQTTClient + 3,
        .line_info_top = fun_data_umqtt_robust_MQTTClient + 16,
        .opcodes = fun_data_umqtt_robust_MQTTClient + 17,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_umqtt_robust__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_umqtt_robust_MQTTClient,
};

static const mp_raw_code_truncated_t proto_fun_umqtt_robust__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_robust__lt_module_gt_,
    .children = (void *)&children_umqtt_robust__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 44,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_umqtt_robust__lt_module_gt_ + 3,
        .line_info_top = fun_data_umqtt_robust__lt_module_gt_ + 7,
        .opcodes = fun_data_umqtt_robust__lt_module_gt_ + 7,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_umqtt_robust[37] = {
    MP_QSTR_umqtt_slash_robust_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_time,
    MP_QSTR_simple,
    MP_QSTR_,
    MP_QSTR_MQTTClient,
    MP_QSTR_1_dot_0_dot_2,
    MP_QSTR_delay,
    MP_QSTR_sleep,
    MP_QSTR_DELAY,
    MP_QSTR_log,
    MP_QSTR_DEBUG,
    MP_QSTR_mqtt_colon__space__percent_r,
    MP_QSTR_reconnect,
    MP_QSTR_connect,
    MP_QSTR_publish,
    MP_QSTR_wait_msg,
    MP_QSTR_check_msg,
    MP_QSTR_sock,
    MP_QSTR_setblocking,
    MP_QSTR___version__,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_self,
    MP_QSTR_i,
    MP_QSTR_in_reconnect,
    MP_QSTR_e,
    MP_QSTR_print,
    MP_QSTR__star_,
    MP_QSTR_super,
    MP_QSTR_OSError,
    MP_QSTR_topic,
    MP_QSTR_msg,
    MP_QSTR_retain,
    MP_QSTR_qos,
    MP_QSTR_attempts,
};

// constants

// constant table
static const mp_rom_obj_t const_obj_table_data_umqtt_robust[1] = {
    MP_ROM_QSTR(MP_QSTR_mqtt_space_reconnect_colon__space__percent_r),
};

static const mp_frozen_module_t frozen_module_umqtt_robust = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_umqtt_robust,
        .obj_table = (mp_obj_t *)&const_obj_table_data_umqtt_robust,
    },
    .proto_fun = &proto_fun_umqtt_robust__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module umqtt_simple
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/umqtt/simple.mpy
// - frozen file name: umqtt/simple.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file umqtt/simple.py, scope umqtt_simple__lt_module_gt_
static const byte fun_data_umqtt_simple__lt_module_gt_[60] = {
    0x18,0x10, // prelude
    0x01, // names: <module>
    0x26,0x26,0x6c,0x6b,0x20,0x89,0xe2, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'socket'
    0x16,0x02, // STORE_NAME 'socket'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'struct'
    0x16,0x03, // STORE_NAME 'struct'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x04, // LOAD_CONST_STRING 'hexlify'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x05, // IMPORT_NAME 'binascii'
    0x1c,0x04, // IMPORT_FROM 'hexlify'
    0x16,0x04, // STORE_NAME 'hexlify'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x06, // LOAD_CONST_STRING 'MQTTException'
    0x11,0x34, // LOAD_NAME 'Exception'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x06, // STORE_NAME 'MQTTException'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x07, // LOAD_CONST_STRING 'MQTTClient'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x07, // STORE_NAME 'MQTTClient'
    0x10,0x08, // LOAD_CONST_STRING '1.7.0'
    0x16,0x35, // STORE_NAME '__version__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of umqtt_simple__lt_module_gt_
// frozen bytecode for file umqtt/simple.py, scope umqtt_simple_MQTTException
static const byte fun_data_umqtt_simple_MQTTException[15] = {
    0x00,0x06, // prelude
    0x06, // names: MQTTException
    0x68,0x60, // code info
    0x11,0x36, // LOAD_NAME '__name__'
    0x16,0x37, // STORE_NAME '__module__'
    0x10,0x06, // LOAD_CONST_STRING 'MQTTException'
    0x16,0x38, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_simple_MQTTException = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_simple_MQTTException,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 15,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_umqtt_simple_MQTTException + 3,
        .line_info_top = fun_data_umqtt_simple_MQTTException + 5,
        .opcodes = fun_data_umqtt_simple_MQTTException + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_simple_MQTTException fun_data_umqtt_simple_MQTTException[0]
#endif

// child of umqtt_simple__lt_module_gt_
// frozen bytecode for file umqtt/simple.py, scope umqtt_simple_MQTTClient
static const byte fun_data_umqtt_simple_MQTTClient[120] = {
    0x28,0x36, // prelude
    0x07, // names: MQTTClient
    0x88,0x0a,0x85,0x09,0x89,0x14,0x64,0x20,0x84,0x0a,0x64,0x89,0x08,0x89,0x34,0x64,0x20,0x64,0x89,0x23,0x88,0x13,0x84,0x11,0x84,0x24, // code info
    0x11,0x36, // LOAD_NAME '__name__'
    0x16,0x37, // STORE_NAME '__module__'
    0x10,0x07, // LOAD_CONST_STRING 'MQTTClient'
    0x16,0x38, // STORE_NAME '__qualname__'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x2c,0x00, // BUILD_MAP 0
    0x2a,0x06, // BUILD_TUPLE 6
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x09, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x19, // STORE_NAME '_send_str'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x1d, // STORE_NAME '_recv_len'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x1f, // STORE_NAME 'set_callback'
    0x50, // LOAD_CONST_FALSE
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x04, // MAKE_FUNCTION_DEFARGS 4
    0x16,0x20, // STORE_NAME 'set_last_will'
    0x52, // LOAD_CONST_TRUE
    0x51, // LOAD_CONST_NONE
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x05, // MAKE_FUNCTION_DEFARGS 5
    0x16,0x22, // STORE_NAME 'connect'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x27, // STORE_NAME 'disconnect'
    0x32,0x07, // MAKE_FUNCTION 7
    0x16,0x29, // STORE_NAME 'ping'
    0x50, // LOAD_CONST_FALSE
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x08, // MAKE_FUNCTION_DEFARGS 8
    0x16,0x2a, // STORE_NAME 'publish'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x09, // MAKE_FUNCTION_DEFARGS 9
    0x16,0x2d, // STORE_NAME 'subscribe'
    0x32,0x0a, // MAKE_FUNCTION 10
    0x16,0x31, // STORE_NAME 'unsubscribe'
    0x32,0x0b, // MAKE_FUNCTION 11
    0x16,0x2c, // STORE_NAME 'wait_msg'
    0x32,0x0c, // MAKE_FUNCTION 12
    0x16,0x33, // STORE_NAME 'check_msg'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of umqtt_simple_MQTTClient
// frozen bytecode for file umqtt/simple.py, scope umqtt_simple_MQTTClient___init__
static const byte fun_data_umqtt_simple_MQTTClient___init__[113] = {
    0xd1,0x80,0x85,0x01,0x38, // prelude
    0x09,0x39,0x0a,0x0c,0x0d,0x12,0x3a,0x14,0x0e,0x0f, // names: __init__, self, client_id, server, port, user, password, keepalive, ssl, ssl_params
    0x80,0x15,0x25,0x2d,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24, // code info
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0xb7, // LOAD_FAST 7
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x22,0x80,0xc5,0x33, // LOAD_CONST_SMALL_INT 8883
    0x42,0x43, // JUMP 3
    0x22,0x8e,0x5b, // LOAD_CONST_SMALL_INT 1883
    0xc3, // STORE_FAST 3
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x0a, // STORE_ATTR 'client_id'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x0b, // STORE_ATTR 'sock'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x0c, // STORE_ATTR 'server'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x0d, // STORE_ATTR 'port'
    0xb7, // LOAD_FAST 7
    0xb0, // LOAD_FAST 0
    0x18,0x0e, // STORE_ATTR 'ssl'
    0xb8, // LOAD_FAST 8
    0xb0, // LOAD_FAST 0
    0x18,0x0f, // STORE_ATTR 'ssl_params'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x10, // STORE_ATTR 'pid'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x11, // STORE_ATTR 'cb'
    0xb4, // LOAD_FAST 4
    0xb0, // LOAD_FAST 0
    0x18,0x12, // STORE_ATTR 'user'
    0xb5, // LOAD_FAST 5
    0xb0, // LOAD_FAST 0
    0x18,0x13, // STORE_ATTR 'pswd'
    0xb6, // LOAD_FAST 6
    0xb0, // LOAD_FAST 0
    0x18,0x14, // STORE_ATTR 'keepalive'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x15, // STORE_ATTR 'lw_topic'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x16, // STORE_ATTR 'lw_msg'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x18,0x17, // STORE_ATTR 'lw_qos'
    0x50, // LOAD_CONST_FALSE
    0xb0, // LOAD_FAST 0
    0x18,0x18, // STORE_ATTR 'lw_retain'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_simple_MQTTClient___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_simple_MQTTClient___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 113,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 9,
        .n_kwonly_args = 0,
        .n_def_pos_args = 6,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_umqtt_simple_MQTTClient___init__ + 15,
        .line_info_top = fun_data_umqtt_simple_MQTTClient___init__ + 33,
        .opcodes = fun_data_umqtt_simple_MQTTClient___init__ + 33,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_simple_MQTTClient___init__ fun_data_umqtt_simple_MQTTClient___init__[0]
#endif

// child of umqtt_simple_MQTTClient
// frozen bytecode for file umqtt/simple.py, scope umqtt_simple_MQTTClient__send_str
static const byte fun_data_umqtt_simple_MQTTClient__send_str[40] = {
    0x42,0x0c, // prelude
    0x19,0x39,0x3b, // names: _send_str, self, s
    0x80,0x28,0x35, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1a, // LOAD_METHOD 'write'
    0x12,0x03, // LOAD_GLOBAL 'struct'
    0x14,0x1b, // LOAD_METHOD 'pack'
    0x10,0x1c, // LOAD_CONST_STRING '!H'
    0x12,0x3c, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x36,0x02, // CALL_METHOD 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1a, // LOAD_METHOD 'write'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_simple_MQTTClient__send_str = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_simple_MQTTClient__send_str,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 40,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 25,
        .line_info = fun_data_umqtt_simple_MQTTClient__send_str + 5,
        .line_info_top = fun_data_umqtt_simple_MQTTClient__send_str + 8,
        .opcodes = fun_data_umqtt_simple_MQTTClient__send_str + 8,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_simple_MQTTClient__send_str fun_data_umqtt_simple_MQTTClient__send_str[0]
#endif

// child of umqtt_simple_MQTTClient
// frozen bytecode for file umqtt/simple.py, scope umqtt_simple_MQTTClient__recv_len
static const byte fun_data_umqtt_simple_MQTTClient__recv_len[55] = {
    0x31,0x16, // prelude
    0x1d,0x39, // names: _recv_len, self
    0x80,0x2c,0x22,0x22,0x20,0x2b,0x2a,0x27,0x22, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc1, // STORE_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1e, // LOAD_METHOD 'read'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x01, // CALL_METHOD 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc3, // STORE_FAST 3
    0xb1, // LOAD_FAST 1
    0xb3, // LOAD_FAST 3
    0x22,0x80,0x7f, // LOAD_CONST_SMALL_INT 127
    0xef, // BINARY_OP 24 __and__
    0xb2, // LOAD_FAST 2
    0xf0, // BINARY_OP 25 __lshift__
    0xe0, // BINARY_OP 9 __ior__
    0xc1, // STORE_FAST 1
    0xb3, // LOAD_FAST 3
    0x22,0x81,0x00, // LOAD_CONST_SMALL_INT 128
    0xef, // BINARY_OP 24 __and__
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
    0xb2, // LOAD_FAST 2
    0x87, // LOAD_CONST_SMALL_INT 7
    0xe5, // BINARY_OP 14 __iadd__
    0xc2, // STORE_FAST 2
    0x42,0x1c, // JUMP -36
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_simple_MQTTClient__recv_len = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_simple_MQTTClient__recv_len,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 55,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 29,
        .line_info = fun_data_umqtt_simple_MQTTClient__recv_len + 4,
        .line_info_top = fun_data_umqtt_simple_MQTTClient__recv_len + 13,
        .opcodes = fun_data_umqtt_simple_MQTTClient__recv_len + 13,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_simple_MQTTClient__recv_len fun_data_umqtt_simple_MQTTClient__recv_len[0]
#endif

// child of umqtt_simple_MQTTClient
// frozen bytecode for file umqtt/simple.py, scope umqtt_simple_MQTTClient_set_callback
static const byte fun_data_umqtt_simple_MQTTClient_set_callback[13] = {
    0x1a,0x0a, // prelude
    0x1f,0x39,0x3d, // names: set_callback, self, f
    0x80,0x36, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x11, // STORE_ATTR 'cb'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_simple_MQTTClient_set_callback = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_simple_MQTTClient_set_callback,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 13,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 31,
        .line_info = fun_data_umqtt_simple_MQTTClient_set_callback + 5,
        .line_info_top = fun_data_umqtt_simple_MQTTClient_set_callback + 7,
        .opcodes = fun_data_umqtt_simple_MQTTClient_set_callback + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_simple_MQTTClient_set_callback fun_data_umqtt_simple_MQTTClient_set_callback[0]
#endif

// child of umqtt_simple_MQTTClient
// frozen bytecode for file umqtt/simple.py, scope umqtt_simple_MQTTClient_set_last_will
static const byte fun_data_umqtt_simple_MQTTClient_set_last_will[59] = {
    0xb9,0x84,0x01,0x1a, // prelude
    0x20,0x39,0x3e,0x3f,0x40,0x41, // names: set_last_will, self, topic, msg, retain, qos
    0x80,0x39,0x32,0x26,0x24,0x24,0x24, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb4, // LOAD_FAST 4
    0x57, // DUP_TOP
    0x5b, // ROT_THREE
    0xda, // BINARY_OP 3 __le__
    0x46,0x04, // JUMP_IF_FALSE_OR_POP 4
    0x82, // LOAD_CONST_SMALL_INT 2
    0xda, // BINARY_OP 3 __le__
    0x42,0x42, // JUMP 2
    0x5a, // ROT_TWO
    0x59, // POP_TOP
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x21, // LOAD_GLOBAL 'AssertionError'
    0x65, // RAISE_OBJ
    0xb1, // LOAD_FAST 1
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x21, // LOAD_GLOBAL 'AssertionError'
    0x65, // RAISE_OBJ
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x15, // STORE_ATTR 'lw_topic'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x16, // STORE_ATTR 'lw_msg'
    0xb4, // LOAD_FAST 4
    0xb0, // LOAD_FAST 0
    0x18,0x17, // STORE_ATTR 'lw_qos'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x18, // STORE_ATTR 'lw_retain'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_simple_MQTTClient_set_last_will = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_simple_MQTTClient_set_last_will,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 59,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 5,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 32,
        .line_info = fun_data_umqtt_simple_MQTTClient_set_last_will + 10,
        .line_info_top = fun_data_umqtt_simple_MQTTClient_set_last_will + 17,
        .opcodes = fun_data_umqtt_simple_MQTTClient_set_last_will + 17,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_simple_MQTTClient_set_last_will fun_data_umqtt_simple_MQTTClient_set_last_will[0]
#endif

// child of umqtt_simple_MQTTClient
// frozen bytecode for file umqtt/simple.py, scope umqtt_simple_MQTTClient_connect
static const byte fun_data_umqtt_simple_MQTTClient_connect[488] = {
    0xfb,0x80,0x01,0x62, // prelude
    0x22,0x39,0x42,0x43, // names: connect, self, clean_session, timeout
    0x80,0x41,0x29,0x29,0x31,0x29,0x47,0x45,0x33,0x25,0x35,0x27,0x47,0x2c,0x26,0x25,0x36,0x2a,0x25,0x2d,0x2c,0x2e,0x25,0x36,0x38,0x4c,0x22,0x22,0x2c,0x24,0x2b,0x44,0x2c,0x49,0x29,0x25,0x29,0x29,0x25,0x29,0x29,0x29,0x31,0x27,0x28, // code info
    0x12,0x02, // LOAD_GLOBAL 'socket'
    0x14,0x02, // LOAD_METHOD 'socket'
    0x36,0x00, // CALL_METHOD 0
    0xb0, // LOAD_FAST 0
    0x18,0x0b, // STORE_ATTR 'sock'
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x23, // LOAD_METHOD 'settimeout'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x02, // LOAD_GLOBAL 'socket'
    0x14,0x24, // LOAD_METHOD 'getaddrinfo'
    0xb0, // LOAD_FAST 0
    0x13,0x0c, // LOAD_ATTR 'server'
    0xb0, // LOAD_FAST 0
    0x13,0x0d, // LOAD_ATTR 'port'
    0x36,0x02, // CALL_METHOD 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x55, // LOAD_SUBSCR
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x22, // LOAD_METHOD 'connect'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR 'ssl'
    0x52, // LOAD_CONST_TRUE
    0xde, // BINARY_OP 7 <is>
    0x44,0x58, // POP_JUMP_IF_FALSE 24
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x0e, // IMPORT_NAME 'ssl'
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x14,0x25, // LOAD_METHOD 'wrap_socket'
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x53, // LOAD_NULL
    0xb0, // LOAD_FAST 0
    0x13,0x0f, // LOAD_ATTR 'ssl_params'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x37,0x82,0x01, // CALL_METHOD_VAR_KW 257
    0xb0, // LOAD_FAST 0
    0x18,0x0b, // STORE_ATTR 'sock'
    0x42,0x5a, // JUMP 26
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR 'ssl'
    0x44,0x55, // POP_JUMP_IF_FALSE 21
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR 'ssl'
    0x14,0x25, // LOAD_METHOD 'wrap_socket'
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x10,0x26, // LOAD_CONST_STRING 'server_hostname'
    0xb0, // LOAD_FAST 0
    0x13,0x0c, // LOAD_ATTR 'server'
    0x36,0x82,0x01, // CALL_METHOD 257
    0xb0, // LOAD_FAST 0
    0x18,0x0b, // STORE_ATTR 'sock'
    0x42,0x40, // JUMP 0
    0x12,0x44, // LOAD_GLOBAL 'bytearray'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x34,0x01, // CALL_FUNCTION 1
    0xc5, // STORE_FAST 5
    0x12,0x44, // LOAD_GLOBAL 'bytearray'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc6, // STORE_FAST 6
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf2, // BINARY_OP 27 __add__
    0x12,0x3c, // LOAD_GLOBAL 'len'
    0xb0, // LOAD_FAST 0
    0x13,0x0a, // LOAD_ATTR 'client_id'
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0xc7, // STORE_FAST 7
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf0, // BINARY_OP 25 __lshift__
    0xb6, // LOAD_FAST 6
    0x86, // LOAD_CONST_SMALL_INT 6
    0x56, // STORE_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x12, // LOAD_ATTR 'user'
    0x44,0x60, // POP_JUMP_IF_FALSE 32
    0xb7, // LOAD_FAST 7
    0x82, // LOAD_CONST_SMALL_INT 2
    0x12,0x3c, // LOAD_GLOBAL 'len'
    0xb0, // LOAD_FAST 0
    0x13,0x12, // LOAD_ATTR 'user'
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf2, // BINARY_OP 27 __add__
    0x12,0x3c, // LOAD_GLOBAL 'len'
    0xb0, // LOAD_FAST 0
    0x13,0x13, // LOAD_ATTR 'pswd'
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0xe5, // BINARY_OP 14 __iadd__
    0xc7, // STORE_FAST 7
    0xb6, // LOAD_FAST 6
    0x86, // LOAD_CONST_SMALL_INT 6
    0x58, // DUP_TOP_TWO
    0x55, // LOAD_SUBSCR
    0x22,0x81,0x40, // LOAD_CONST_SMALL_INT 192
    0xe0, // BINARY_OP 9 __ior__
    0x5b, // ROT_THREE
    0x56, // STORE_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x14, // LOAD_ATTR 'keepalive'
    0x44,0x67, // POP_JUMP_IF_FALSE 39
    0xb0, // LOAD_FAST 0
    0x13,0x14, // LOAD_ATTR 'keepalive'
    0x22,0x84,0x80,0x00, // LOAD_CONST_SMALL_INT 65536
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x21, // LOAD_GLOBAL 'AssertionError'
    0x65, // RAISE_OBJ
    0xb6, // LOAD_FAST 6
    0x87, // LOAD_CONST_SMALL_INT 7
    0x58, // DUP_TOP_TWO
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x14, // LOAD_ATTR 'keepalive'
    0x88, // LOAD_CONST_SMALL_INT 8
    0xf1, // BINARY_OP 26 __rshift__
    0xe0, // BINARY_OP 9 __ior__
    0x5b, // ROT_THREE
    0x56, // STORE_SUBSCR
    0xb6, // LOAD_FAST 6
    0x88, // LOAD_CONST_SMALL_INT 8
    0x58, // DUP_TOP_TWO
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x14, // LOAD_ATTR 'keepalive'
    0x22,0x81,0x7f, // LOAD_CONST_SMALL_INT 255
    0xef, // BINARY_OP 24 __and__
    0xe0, // BINARY_OP 9 __ior__
    0x5b, // ROT_THREE
    0x56, // STORE_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'lw_topic'
    0x44,0x7a, // POP_JUMP_IF_FALSE 58
    0xb7, // LOAD_FAST 7
    0x82, // LOAD_CONST_SMALL_INT 2
    0x12,0x3c, // LOAD_GLOBAL 'len'
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'lw_topic'
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf2, // BINARY_OP 27 __add__
    0x12,0x3c, // LOAD_GLOBAL 'len'
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR 'lw_msg'
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0xe5, // BINARY_OP 14 __iadd__
    0xc7, // STORE_FAST 7
    0xb6, // LOAD_FAST 6
    0x86, // LOAD_CONST_SMALL_INT 6
    0x58, // DUP_TOP_TWO
    0x55, // LOAD_SUBSCR
    0x84, // LOAD_CONST_SMALL_INT 4
    0xb0, // LOAD_FAST 0
    0x13,0x17, // LOAD_ATTR 'lw_qos'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xef, // BINARY_OP 24 __and__
    0x83, // LOAD_CONST_SMALL_INT 3
    0xf0, // BINARY_OP 25 __lshift__
    0xed, // BINARY_OP 22 __or__
    0xb0, // LOAD_FAST 0
    0x13,0x17, // LOAD_ATTR 'lw_qos'
    0x82, // LOAD_CONST_SMALL_INT 2
    0xef, // BINARY_OP 24 __and__
    0x83, // LOAD_CONST_SMALL_INT 3
    0xf0, // BINARY_OP 25 __lshift__
    0xed, // BINARY_OP 22 __or__
    0xe0, // BINARY_OP 9 __ior__
    0x5b, // ROT_THREE
    0x56, // STORE_SUBSCR
    0xb6, // LOAD_FAST 6
    0x86, // LOAD_CONST_SMALL_INT 6
    0x58, // DUP_TOP_TWO
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x18, // LOAD_ATTR 'lw_retain'
    0x85, // LOAD_CONST_SMALL_INT 5
    0xf0, // BINARY_OP 25 __lshift__
    0xe0, // BINARY_OP 9 __ior__
    0x5b, // ROT_THREE
    0x56, // STORE_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xc8, // STORE_FAST 8
    0x42,0x54, // JUMP 20
    0xb7, // LOAD_FAST 7
    0x22,0x80,0x7f, // LOAD_CONST_SMALL_INT 127
    0xef, // BINARY_OP 24 __and__
    0x22,0x81,0x00, // LOAD_CONST_SMALL_INT 128
    0xed, // BINARY_OP 22 __or__
    0xb5, // LOAD_FAST 5
    0xb8, // LOAD_FAST 8
    0x56, // STORE_SUBSCR
    0xb7, // LOAD_FAST 7
    0x87, // LOAD_CONST_SMALL_INT 7
    0xe4, // BINARY_OP 13 __irshift__
    0xc7, // STORE_FAST 7
    0xb8, // LOAD_FAST 8
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0xc8, // STORE_FAST 8
    0xb7, // LOAD_FAST 7
    0x22,0x80,0x7f, // LOAD_CONST_SMALL_INT 127
    0xd8, // BINARY_OP 1 __gt__
    0x43,0x25, // POP_JUMP_IF_TRUE -27
    0xb7, // LOAD_FAST 7
    0xb5, // LOAD_FAST 5
    0xb8, // LOAD_FAST 8
    0x56, // STORE_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1a, // LOAD_METHOD 'write'
    0xb5, // LOAD_FAST 5
    0xb8, // LOAD_FAST 8
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf2, // BINARY_OP 27 __add__
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1a, // LOAD_METHOD 'write'
    0xb6, // LOAD_FAST 6
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD '_send_str'
    0xb0, // LOAD_FAST 0
    0x13,0x0a, // LOAD_ATTR 'client_id'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'lw_topic'
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD '_send_str'
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'lw_topic'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD '_send_str'
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR 'lw_msg'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x12, // LOAD_ATTR 'user'
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD '_send_str'
    0xb0, // LOAD_FAST 0
    0x13,0x12, // LOAD_ATTR 'user'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD '_send_str'
    0xb0, // LOAD_FAST 0
    0x13,0x13, // LOAD_ATTR 'pswd'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1e, // LOAD_METHOD 'read'
    0x84, // LOAD_CONST_SMALL_INT 4
    0x36,0x01, // CALL_METHOD 1
    0xc9, // STORE_FAST 9
    0xb9, // LOAD_FAST 9
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xa0, // LOAD_CONST_SMALL_INT 32
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0xb9, // LOAD_FAST 9
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x82, // LOAD_CONST_SMALL_INT 2
    0xd9, // BINARY_OP 2 __eq__
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x21, // LOAD_GLOBAL 'AssertionError'
    0x65, // RAISE_OBJ
    0xb9, // LOAD_FAST 9
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x12,0x06, // LOAD_GLOBAL 'MQTTException'
    0xb9, // LOAD_FAST 9
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb9, // LOAD_FAST 9
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xef, // BINARY_OP 24 __and__
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_simple_MQTTClient_connect = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_simple_MQTTClient_connect,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 488,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 16,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 34,
        .line_info = fun_data_umqtt_simple_MQTTClient_connect + 8,
        .line_info_top = fun_data_umqtt_simple_MQTTClient_connect + 53,
        .opcodes = fun_data_umqtt_simple_MQTTClient_connect + 53,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_simple_MQTTClient_connect fun_data_umqtt_simple_MQTTClient_connect[0]
#endif

// child of umqtt_simple_MQTTClient
// frozen bytecode for file umqtt/simple.py, scope umqtt_simple_MQTTClient_disconnect
static const byte fun_data_umqtt_simple_MQTTClient_disconnect[27] = {
    0x19,0x0a, // prelude
    0x27,0x39, // names: disconnect, self
    0x80,0x75,0x2a, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1a, // LOAD_METHOD 'write'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x28, // LOAD_METHOD 'close'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_simple_MQTTClient_disconnect = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_simple_MQTTClient_disconnect,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 27,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 39,
        .line_info = fun_data_umqtt_simple_MQTTClient_disconnect + 4,
        .line_info_top = fun_data_umqtt_simple_MQTTClient_disconnect + 7,
        .opcodes = fun_data_umqtt_simple_MQTTClient_disconnect + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_simple_MQTTClient_disconnect fun_data_umqtt_simple_MQTTClient_disconnect[0]
#endif

// child of umqtt_simple_MQTTClient
// frozen bytecode for file umqtt/simple.py, scope umqtt_simple_MQTTClient_ping
static const byte fun_data_umqtt_simple_MQTTClient_ping[18] = {
    0x19,0x08, // prelude
    0x29,0x39, // names: ping, self
    0x80,0x79, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1a, // LOAD_METHOD 'write'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_simple_MQTTClient_ping = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_simple_MQTTClient_ping,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 18,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 41,
        .line_info = fun_data_umqtt_simple_MQTTClient_ping + 4,
        .line_info_top = fun_data_umqtt_simple_MQTTClient_ping + 6,
        .opcodes = fun_data_umqtt_simple_MQTTClient_ping + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_simple_MQTTClient_ping fun_data_umqtt_simple_MQTTClient_ping[0]
#endif

// child of umqtt_simple_MQTTClient
// frozen bytecode for file umqtt/simple.py, scope umqtt_simple_MQTTClient_publish
static const byte fun_data_umqtt_simple_MQTTClient_publish[278] = {
    0x81,0x94,0x01,0x4e, // prelude
    0x2a,0x39,0x3e,0x3f,0x40,0x41, // names: publish, self, topic, msg, retain, qos
    0x80,0x7c,0x27,0x2c,0x2e,0x25,0x24,0x2c,0x22,0x22,0x2c,0x24,0x2b,0x44,0x2c,0x27,0x25,0x29,0x24,0x2c,0x2a,0x29,0x25,0x20,0x26,0x27,0x29,0x29,0x29,0x2a,0x25,0x26,0x25, // code info
    0x12,0x44, // LOAD_GLOBAL 'bytearray'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x34,0x01, // CALL_FUNCTION 1
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x80, // LOAD_CONST_SMALL_INT 0
    0x58, // DUP_TOP_TWO
    0x55, // LOAD_SUBSCR
    0xb4, // LOAD_FAST 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf0, // BINARY_OP 25 __lshift__
    0xb3, // LOAD_FAST 3
    0xed, // BINARY_OP 22 __or__
    0xe0, // BINARY_OP 9 __ior__
    0x5b, // ROT_THREE
    0x56, // STORE_SUBSCR
    0x82, // LOAD_CONST_SMALL_INT 2
    0x12,0x3c, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x12,0x3c, // LOAD_GLOBAL 'len'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0xc6, // STORE_FAST 6
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0xb6, // LOAD_FAST 6
    0x82, // LOAD_CONST_SMALL_INT 2
    0xe5, // BINARY_OP 14 __iadd__
    0xc6, // STORE_FAST 6
    0xb6, // LOAD_FAST 6
    0x22,0x81,0x80,0x80,0x00, // LOAD_CONST_SMALL_INT 2097152
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x21, // LOAD_GLOBAL 'AssertionError'
    0x65, // RAISE_OBJ
    0x81, // LOAD_CONST_SMALL_INT 1
    0xc7, // STORE_FAST 7
    0x42,0x54, // JUMP 20
    0xb6, // LOAD_FAST 6
    0x22,0x80,0x7f, // LOAD_CONST_SMALL_INT 127
    0xef, // BINARY_OP 24 __and__
    0x22,0x81,0x00, // LOAD_CONST_SMALL_INT 128
    0xed, // BINARY_OP 22 __or__
    0xb5, // LOAD_FAST 5
    0xb7, // LOAD_FAST 7
    0x56, // STORE_SUBSCR
    0xb6, // LOAD_FAST 6
    0x87, // LOAD_CONST_SMALL_INT 7
    0xe4, // BINARY_OP 13 __irshift__
    0xc6, // STORE_FAST 6
    0xb7, // LOAD_FAST 7
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0xc7, // STORE_FAST 7
    0xb6, // LOAD_FAST 6
    0x22,0x80,0x7f, // LOAD_CONST_SMALL_INT 127
    0xd8, // BINARY_OP 1 __gt__
    0x43,0x25, // POP_JUMP_IF_TRUE -27
    0xb6, // LOAD_FAST 6
    0xb5, // LOAD_FAST 5
    0xb7, // LOAD_FAST 7
    0x56, // STORE_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1a, // LOAD_METHOD 'write'
    0xb5, // LOAD_FAST 5
    0xb7, // LOAD_FAST 7
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD '_send_str'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x63, // POP_JUMP_IF_FALSE 35
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x10, // LOAD_ATTR 'pid'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x10, // STORE_ATTR 'pid'
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'pid'
    0xc8, // STORE_FAST 8
    0x12,0x03, // LOAD_GLOBAL 'struct'
    0x14,0x2b, // LOAD_METHOD 'pack_into'
    0x10,0x1c, // LOAD_CONST_STRING '!H'
    0xb5, // LOAD_FAST 5
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb8, // LOAD_FAST 8
    0x36,0x04, // CALL_METHOD 4
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1a, // LOAD_METHOD 'write'
    0xb5, // LOAD_FAST 5
    0x82, // LOAD_CONST_SMALL_INT 2
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1a, // LOAD_METHOD 'write'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb4, // LOAD_FAST 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x7d, // POP_JUMP_IF_FALSE 61
    0xb0, // LOAD_FAST 0
    0x14,0x2c, // LOAD_METHOD 'wait_msg'
    0x36,0x00, // CALL_METHOD 0
    0xc9, // STORE_FAST 9
    0xb9, // LOAD_FAST 9
    0x22,0x80,0x40, // LOAD_CONST_SMALL_INT 64
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x6c, // POP_JUMP_IF_FALSE 44
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1e, // LOAD_METHOD 'read'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x01, // CALL_METHOD 1
    0xc6, // STORE_FAST 6
    0xb6, // LOAD_FAST 6
    0x23,0x05, // LOAD_CONST_OBJ 5
    0xd9, // BINARY_OP 2 __eq__
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x21, // LOAD_GLOBAL 'AssertionError'
    0x65, // RAISE_OBJ
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1e, // LOAD_METHOD 'read'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x36,0x01, // CALL_METHOD 1
    0xca, // STORE_FAST 10
    0xba, // LOAD_FAST 10
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x88, // LOAD_CONST_SMALL_INT 8
    0xf0, // BINARY_OP 25 __lshift__
    0xba, // LOAD_FAST 10
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xed, // BINARY_OP 22 __or__
    0xca, // STORE_FAST 10
    0xb8, // LOAD_FAST 8
    0xba, // LOAD_FAST 10
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x42,0x05, // JUMP -59
    0x42,0x4a, // JUMP 10
    0xb4, // LOAD_FAST 4
    0x82, // LOAD_CONST_SMALL_INT 2
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0x12,0x21, // LOAD_GLOBAL 'AssertionError'
    0x65, // RAISE_OBJ
    0x42,0x40, // JUMP 0
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_simple_MQTTClient_publish = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_simple_MQTTClient_publish,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 278,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 17,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 5,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 42,
        .line_info = fun_data_umqtt_simple_MQTTClient_publish + 10,
        .line_info_top = fun_data_umqtt_simple_MQTTClient_publish + 43,
        .opcodes = fun_data_umqtt_simple_MQTTClient_publish + 43,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_simple_MQTTClient_publish fun_data_umqtt_simple_MQTTClient_publish[0]
#endif

// child of umqtt_simple_MQTTClient
// frozen bytecode for file umqtt/simple.py, scope umqtt_simple_MQTTClient_subscribe
static const byte fun_data_umqtt_simple_MQTTClient_subscribe[177] = {
    0xeb,0x01,0x28, // prelude
    0x2d,0x39,0x3e,0x41, // names: subscribe, self, topic, qos
    0x80,0x9f,0x2f,0x27,0x29,0x59,0x29,0x27,0x30,0x20,0x26,0x27,0x49,0x35,0x29,0x28, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x11, // LOAD_ATTR 'cb'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0x12,0x21, // LOAD_GLOBAL 'AssertionError'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x12,0x44, // LOAD_GLOBAL 'bytearray'
    0x23,0x07, // LOAD_CONST_OBJ 7
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x10, // LOAD_ATTR 'pid'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x10, // STORE_ATTR 'pid'
    0x12,0x03, // LOAD_GLOBAL 'struct'
    0x14,0x2b, // LOAD_METHOD 'pack_into'
    0x10,0x2e, // LOAD_CONST_STRING '!BH'
    0xb3, // LOAD_FAST 3
    0x81, // LOAD_CONST_SMALL_INT 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf2, // BINARY_OP 27 __add__
    0x12,0x3c, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'pid'
    0x36,0x05, // CALL_METHOD 5
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1a, // LOAD_METHOD 'write'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD '_send_str'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1a, // LOAD_METHOD 'write'
    0xb2, // LOAD_FAST 2
    0x14,0x2f, // LOAD_METHOD 'to_bytes'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x30, // LOAD_CONST_STRING 'little'
    0x36,0x02, // CALL_METHOD 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x2c, // LOAD_METHOD 'wait_msg'
    0x36,0x00, // CALL_METHOD 0
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x22,0x81,0x10, // LOAD_CONST_SMALL_INT 144
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x71, // POP_JUMP_IF_FALSE 49
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1e, // LOAD_METHOD 'read'
    0x84, // LOAD_CONST_SMALL_INT 4
    0x36,0x01, // CALL_METHOD 1
    0xc5, // STORE_FAST 5
    0xb5, // LOAD_FAST 5
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb3, // LOAD_FAST 3
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0xb5, // LOAD_FAST 5
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0xb3, // LOAD_FAST 3
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xd9, // BINARY_OP 2 __eq__
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x21, // LOAD_GLOBAL 'AssertionError'
    0x65, // RAISE_OBJ
    0xb5, // LOAD_FAST 5
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0x22,0x81,0x00, // LOAD_CONST_SMALL_INT 128
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x48, // POP_JUMP_IF_FALSE 8
    0x12,0x06, // LOAD_GLOBAL 'MQTTException'
    0xb5, // LOAD_FAST 5
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x42,0x00, // JUMP -64
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_simple_MQTTClient_subscribe = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_simple_MQTTClient_subscribe,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 177,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 45,
        .line_info = fun_data_umqtt_simple_MQTTClient_subscribe + 7,
        .line_info_top = fun_data_umqtt_simple_MQTTClient_subscribe + 23,
        .opcodes = fun_data_umqtt_simple_MQTTClient_subscribe + 23,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_simple_MQTTClient_subscribe fun_data_umqtt_simple_MQTTClient_subscribe[0]
#endif

// child of umqtt_simple_MQTTClient
// frozen bytecode for file umqtt/simple.py, scope umqtt_simple_MQTTClient_unsubscribe
static const byte fun_data_umqtt_simple_MQTTClient_unsubscribe[121] = {
    0x62,0x1e, // prelude
    0x31,0x39,0x3e, // names: unsubscribe, self, topic
    0x80,0xb2,0x27,0x29,0x37,0x29,0x27,0x20,0x26,0x27,0x29,0x35, // code info
    0x12,0x44, // LOAD_GLOBAL 'bytearray'
    0x23,0x08, // LOAD_CONST_OBJ 8
    0x34,0x01, // CALL_FUNCTION 1
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x57, // DUP_TOP
    0x13,0x10, // LOAD_ATTR 'pid'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x5a, // ROT_TWO
    0x18,0x10, // STORE_ATTR 'pid'
    0x12,0x03, // LOAD_GLOBAL 'struct'
    0x14,0x2b, // LOAD_METHOD 'pack_into'
    0x10,0x2e, // LOAD_CONST_STRING '!BH'
    0xb2, // LOAD_FAST 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf2, // BINARY_OP 27 __add__
    0x12,0x3c, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'pid'
    0x36,0x05, // CALL_METHOD 5
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1a, // LOAD_METHOD 'write'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD '_send_str'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x2c, // LOAD_METHOD 'wait_msg'
    0x36,0x00, // CALL_METHOD 0
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x22,0x81,0x30, // LOAD_CONST_SMALL_INT 176
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x60, // POP_JUMP_IF_FALSE 32
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1e, // LOAD_METHOD 'read'
    0x83, // LOAD_CONST_SMALL_INT 3
    0x36,0x01, // CALL_METHOD 1
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb2, // LOAD_FAST 2
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0xb4, // LOAD_FAST 4
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0xb2, // LOAD_FAST 2
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xd9, // BINARY_OP 2 __eq__
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x21, // LOAD_GLOBAL 'AssertionError'
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0x42,0x11, // JUMP -47
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_simple_MQTTClient_unsubscribe = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_simple_MQTTClient_unsubscribe,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 121,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 49,
        .line_info = fun_data_umqtt_simple_MQTTClient_unsubscribe + 5,
        .line_info_top = fun_data_umqtt_simple_MQTTClient_unsubscribe + 17,
        .opcodes = fun_data_umqtt_simple_MQTTClient_unsubscribe + 17,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_simple_MQTTClient_unsubscribe fun_data_umqtt_simple_MQTTClient_unsubscribe[0]
#endif

// child of umqtt_simple_MQTTClient
// frozen bytecode for file umqtt/simple.py, scope umqtt_simple_MQTTClient_wait_msg
static const byte fun_data_umqtt_simple_MQTTClient_wait_msg[252] = {
    0x71,0x44, // prelude
    0x2c,0x39, // names: wait_msg, self
    0x80,0xc3,0x29,0x29,0x25,0x22,0x26,0x26,0x26,0x2b,0x28,0x22,0x24,0x2a,0x22,0x26,0x29,0x2a,0x29,0x26,0x25,0x29,0x2a,0x24,0x29,0x28,0x27,0x27,0x2c,0x2b,0x27,0x25, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1e, // LOAD_METHOD 'read'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x01, // CALL_METHOD 1
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x32, // LOAD_METHOD 'setblocking'
    0x52, // LOAD_CONST_TRUE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb1, // LOAD_FAST 1
    0x23,0x09, // LOAD_CONST_OBJ 9
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x12,0x45, // LOAD_GLOBAL 'OSError'
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb1, // LOAD_FAST 1
    0x23,0x0a, // LOAD_CONST_OBJ 10
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x55, // POP_JUMP_IF_FALSE 21
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1e, // LOAD_METHOD 'read'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x01, // CALL_METHOD 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x12,0x21, // LOAD_GLOBAL 'AssertionError'
    0x65, // RAISE_OBJ
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x22,0x81,0x70, // LOAD_CONST_SMALL_INT 240
    0xef, // BINARY_OP 24 __and__
    0x22,0x30, // LOAD_CONST_SMALL_INT 48
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb3, // LOAD_FAST 3
    0x63, // RETURN_VALUE
    0xb0, // LOAD_FAST 0
    0x14,0x1d, // LOAD_METHOD '_recv_len'
    0x36,0x00, // CALL_METHOD 0
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1e, // LOAD_METHOD 'read'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x36,0x01, // CALL_METHOD 1
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x88, // LOAD_CONST_SMALL_INT 8
    0xf0, // BINARY_OP 25 __lshift__
    0xb4, // LOAD_FAST 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xed, // BINARY_OP 22 __or__
    0xc4, // STORE_FAST 4
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1e, // LOAD_METHOD 'read'
    0xb4, // LOAD_FAST 4
    0x36,0x01, // CALL_METHOD 1
    0xc5, // STORE_FAST 5
    0xb2, // LOAD_FAST 2
    0xb4, // LOAD_FAST 4
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf2, // BINARY_OP 27 __add__
    0xe6, // BINARY_OP 15 __isub__
    0xc2, // STORE_FAST 2
    0xb3, // LOAD_FAST 3
    0x86, // LOAD_CONST_SMALL_INT 6
    0xef, // BINARY_OP 24 __and__
    0x44,0x57, // POP_JUMP_IF_FALSE 23
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1e, // LOAD_METHOD 'read'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x36,0x01, // CALL_METHOD 1
    0xc6, // STORE_FAST 6
    0xb6, // LOAD_FAST 6
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x88, // LOAD_CONST_SMALL_INT 8
    0xf0, // BINARY_OP 25 __lshift__
    0xb6, // LOAD_FAST 6
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xed, // BINARY_OP 22 __or__
    0xc6, // STORE_FAST 6
    0xb2, // LOAD_FAST 2
    0x82, // LOAD_CONST_SMALL_INT 2
    0xe6, // BINARY_OP 15 __isub__
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1e, // LOAD_METHOD 'read'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0xc7, // STORE_FAST 7
    0xb0, // LOAD_FAST 0
    0x14,0x11, // LOAD_METHOD 'cb'
    0xb5, // LOAD_FAST 5
    0xb7, // LOAD_FAST 7
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0xb3, // LOAD_FAST 3
    0x86, // LOAD_CONST_SMALL_INT 6
    0xef, // BINARY_OP 24 __and__
    0x82, // LOAD_CONST_SMALL_INT 2
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x5e, // POP_JUMP_IF_FALSE 30
    0x12,0x44, // LOAD_GLOBAL 'bytearray'
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0x34,0x01, // CALL_FUNCTION 1
    0xc8, // STORE_FAST 8
    0x12,0x03, // LOAD_GLOBAL 'struct'
    0x14,0x2b, // LOAD_METHOD 'pack_into'
    0x10,0x1c, // LOAD_CONST_STRING '!H'
    0xb8, // LOAD_FAST 8
    0x82, // LOAD_CONST_SMALL_INT 2
    0xb6, // LOAD_FAST 6
    0x36,0x04, // CALL_METHOD 4
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x1a, // LOAD_METHOD 'write'
    0xb8, // LOAD_FAST 8
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x4c, // JUMP 12
    0xb3, // LOAD_FAST 3
    0x86, // LOAD_CONST_SMALL_INT 6
    0xef, // BINARY_OP 24 __and__
    0x84, // LOAD_CONST_SMALL_INT 4
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0x12,0x21, // LOAD_GLOBAL 'AssertionError'
    0x65, // RAISE_OBJ
    0x42,0x40, // JUMP 0
    0xb3, // LOAD_FAST 3
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_simple_MQTTClient_wait_msg = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_simple_MQTTClient_wait_msg,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 252,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 15,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 44,
        .line_info = fun_data_umqtt_simple_MQTTClient_wait_msg + 4,
        .line_info_top = fun_data_umqtt_simple_MQTTClient_wait_msg + 36,
        .opcodes = fun_data_umqtt_simple_MQTTClient_wait_msg + 36,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_simple_MQTTClient_wait_msg fun_data_umqtt_simple_MQTTClient_wait_msg[0]
#endif

// child of umqtt_simple_MQTTClient
// frozen bytecode for file umqtt/simple.py, scope umqtt_simple_MQTTClient_check_msg
static const byte fun_data_umqtt_simple_MQTTClient_check_msg[22] = {
    0x19,0x0a, // prelude
    0x33,0x39, // names: check_msg, self
    0x80,0xe7,0x29, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'sock'
    0x14,0x32, // LOAD_METHOD 'setblocking'
    0x50, // LOAD_CONST_FALSE
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x2c, // LOAD_METHOD 'wait_msg'
    0x36,0x00, // CALL_METHOD 0
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_umqtt_simple_MQTTClient_check_msg = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_simple_MQTTClient_check_msg,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 22,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 51,
        .line_info = fun_data_umqtt_simple_MQTTClient_check_msg + 4,
        .line_info_top = fun_data_umqtt_simple_MQTTClient_check_msg + 7,
        .opcodes = fun_data_umqtt_simple_MQTTClient_check_msg + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_umqtt_simple_MQTTClient_check_msg fun_data_umqtt_simple_MQTTClient_check_msg[0]
#endif

static const mp_raw_code_t *const children_umqtt_simple_MQTTClient[] = {
    (const mp_raw_code_t *)&proto_fun_umqtt_simple_MQTTClient___init__,
    (const mp_raw_code_t *)&proto_fun_umqtt_simple_MQTTClient__send_str,
    (const mp_raw_code_t *)&proto_fun_umqtt_simple_MQTTClient__recv_len,
    (const mp_raw_code_t *)&proto_fun_umqtt_simple_MQTTClient_set_callback,
    (const mp_raw_code_t *)&proto_fun_umqtt_simple_MQTTClient_set_last_will,
    (const mp_raw_code_t *)&proto_fun_umqtt_simple_MQTTClient_connect,
    (const mp_raw_code_t *)&proto_fun_umqtt_simple_MQTTClient_disconnect,
    (const mp_raw_code_t *)&proto_fun_umqtt_simple_MQTTClient_ping,
    (const mp_raw_code_t *)&proto_fun_umqtt_simple_MQTTClient_publish,
    (const mp_raw_code_t *)&proto_fun_umqtt_simple_MQTTClient_subscribe,
    (const mp_raw_code_t *)&proto_fun_umqtt_simple_MQTTClient_unsubscribe,
    (const mp_raw_code_t *)&proto_fun_umqtt_simple_MQTTClient_wait_msg,
    (const mp_raw_code_t *)&proto_fun_umqtt_simple_MQTTClient_check_msg,
};

static const mp_raw_code_truncated_t proto_fun_umqtt_simple_MQTTClient = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_simple_MQTTClient,
    .children = (void *)&children_umqtt_simple_MQTTClient,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 120,
    .n_children = 13,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_umqtt_simple_MQTTClient + 3,
        .line_info_top = fun_data_umqtt_simple_MQTTClient + 29,
        .opcodes = fun_data_umqtt_simple_MQTTClient + 29,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_umqtt_simple__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_umqtt_simple_MQTTException,
    (const mp_raw_code_t *)&proto_fun_umqtt_simple_MQTTClient,
};

static const mp_raw_code_truncated_t proto_fun_umqtt_simple__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_umqtt_simple__lt_module_gt_,
    .children = (void *)&children_umqtt_simple__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 60,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_umqtt_simple__lt_module_gt_ + 3,
        .line_info_top = fun_data_umqtt_simple__lt_module_gt_ + 10,
        .opcodes = fun_data_umqtt_simple__lt_module_gt_ + 10,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_umqtt_simple[70] = {
    MP_QSTR_umqtt_slash_simple_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_socket,
    MP_QSTR_struct,
    MP_QSTR_hexlify,
    MP_QSTR_binascii,
    MP_QSTR_MQTTException,
    MP_QSTR_MQTTClient,
    MP_QSTR_1_dot_7_dot_0,
    MP_QSTR___init__,
    MP_QSTR_client_id,
    MP_QSTR_sock,
    MP_QSTR_server,
    MP_QSTR_port,
    MP_QSTR_ssl,
    MP_QSTR_ssl_params,
    MP_QSTR_pid,
    MP_QSTR_cb,
    MP_QSTR_user,
    MP_QSTR_pswd,
    MP_QSTR_keepalive,
    MP_QSTR_lw_topic,
    MP_QSTR_lw_msg,
    MP_QSTR_lw_qos,
    MP_QSTR_lw_retain,
    MP_QSTR__send_str,
    MP_QSTR_write,
    MP_QSTR_pack,
    MP_QSTR__bang_H,
    MP_QSTR__recv_len,
    MP_QSTR_read,
    MP_QSTR_set_callback,
    MP_QSTR_set_last_will,
    MP_QSTR_AssertionError,
    MP_QSTR_connect,
    MP_QSTR_settimeout,
    MP_QSTR_getaddrinfo,
    MP_QSTR_wrap_socket,
    MP_QSTR_server_hostname,
    MP_QSTR_disconnect,
    MP_QSTR_close,
    MP_QSTR_ping,
    MP_QSTR_publish,
    MP_QSTR_pack_into,
    MP_QSTR_wait_msg,
    MP_QSTR_subscribe,
    MP_QSTR__bang_BH,
    MP_QSTR_to_bytes,
    MP_QSTR_little,
    MP_QSTR_unsubscribe,
    MP_QSTR_setblocking,
    MP_QSTR_check_msg,
    MP_QSTR_Exception,
    MP_QSTR___version__,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_self,
    MP_QSTR_password,
    MP_QSTR_s,
    MP_QSTR_len,
    MP_QSTR_f,
    MP_QSTR_topic,
    MP_QSTR_msg,
    MP_QSTR_retain,
    MP_QSTR_qos,
    MP_QSTR_clean_session,
    MP_QSTR_timeout,
    MP_QSTR_bytearray,
    MP_QSTR_OSError,
};

// constants
static const mp_obj_str_t const_obj_umqtt_simple_0 = {{&mp_type_bytes}, 36565, 6, (const byte*)"\x10\x00\x00\x00\x00\x00"};
static const mp_obj_str_t const_obj_umqtt_simple_1 = {{&mp_type_bytes}, 14619, 9, (const byte*)"\x04\x4d\x51\x54\x54\x04\x02\x00\x00"};
static const mp_obj_str_t const_obj_umqtt_simple_2 = {{&mp_type_bytes}, 24037, 2, (const byte*)"\xe0\x00"};
static const mp_obj_str_t const_obj_umqtt_simple_3 = {{&mp_type_bytes}, 25093, 2, (const byte*)"\xc0\x00"};
static const mp_obj_str_t const_obj_umqtt_simple_4 = {{&mp_type_bytes}, 18805, 4, (const byte*)"\x30\x00\x00\x00"};
static const mp_obj_str_t const_obj_umqtt_simple_5 = {{&mp_type_bytes}, 46503, 1, (const byte*)"\x02"};
static const mp_obj_str_t const_obj_umqtt_simple_6 = {{&mp_type_str}, 9577, 29, (const byte*)"\x53\x75\x62\x73\x63\x72\x69\x62\x65\x20\x63\x61\x6c\x6c\x62\x61\x63\x6b\x20\x69\x73\x20\x6e\x6f\x74\x20\x73\x65\x74"};
static const mp_obj_str_t const_obj_umqtt_simple_7 = {{&mp_type_bytes}, 63431, 4, (const byte*)"\x82\x00\x00\x00"};
static const mp_obj_str_t const_obj_umqtt_simple_8 = {{&mp_type_bytes}, 27559, 4, (const byte*)"\xa2\x00\x00\x00"};
static const mp_obj_str_t const_obj_umqtt_simple_10 = {{&mp_type_bytes}, 46453, 1, (const byte*)"\xd0"};
static const mp_obj_str_t const_obj_umqtt_simple_11 = {{&mp_type_bytes}, 12359, 4, (const byte*)"\x40\x02\x00\x00"};

// constant table
static const mp_rom_obj_t const_obj_table_data_umqtt_simple[12] = {
    MP_ROM_PTR(&const_obj_umqtt_simple_0),
    MP_ROM_PTR(&const_obj_umqtt_simple_1),
    MP_ROM_PTR(&const_obj_umqtt_simple_2),
    MP_ROM_PTR(&const_obj_umqtt_simple_3),
    MP_ROM_PTR(&const_obj_umqtt_simple_4),
    MP_ROM_PTR(&const_obj_umqtt_simple_5),
    MP_ROM_PTR(&const_obj_umqtt_simple_6),
    MP_ROM_PTR(&const_obj_umqtt_simple_7),
    MP_ROM_PTR(&const_obj_umqtt_simple_8),
    MP_ROM_PTR(&mp_const_empty_bytes_obj),
    MP_ROM_PTR(&const_obj_umqtt_simple_10),
    MP_ROM_PTR(&const_obj_umqtt_simple_11),
};

static const mp_frozen_module_t frozen_module_umqtt_simple = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_umqtt_simple,
        .obj_table = (mp_obj_t *)&const_obj_table_data_umqtt_simple,
    },
    .proto_fun = &proto_fun_umqtt_simple__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module upysh
// - original source file: /home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/frozen_mpy/upysh.mpy
// - frozen file name: upysh.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file upysh.py, scope upysh__lt_module_gt_
static const byte fun_data_upysh__lt_module_gt_[169] = {
    0x10,0x3c, // prelude
    0x01, // names: <module>
    0x26,0x66,0x89,0x18,0x89,0x08,0x89,0x08,0x88,0x09,0x64,0x20,0x84,0x10,0x84,0x0c,0x88,0x0d,0x89,0x0f,0x26,0x26,0x26,0x46,0x26,0x26,0x26,0x46,0x67, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'sys'
    0x16,0x02, // STORE_NAME 'sys'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'os'
    0x16,0x03, // STORE_NAME 'os'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x04, // LOAD_CONST_STRING 'LS'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x04, // STORE_NAME 'LS'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x05, // LOAD_CONST_STRING 'PWD'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x05, // STORE_NAME 'PWD'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x06, // LOAD_CONST_STRING 'CLEAR'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x06, // STORE_NAME 'CLEAR'
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x03, // MAKE_FUNCTION_DEFARGS 3
    0x16,0x0e, // STORE_NAME 'head'
    0x32,0x04, // MAKE_FUNCTION 4
    0x16,0x12, // STORE_NAME 'cat'
    0x32,0x05, // MAKE_FUNCTION 5
    0x16,0x13, // STORE_NAME 'cp'
    0x32,0x06, // MAKE_FUNCTION 6
    0x16,0x1a, // STORE_NAME 'newfile'
    0x50, // LOAD_CONST_FALSE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x07, // MAKE_FUNCTION_DEFARGS 7
    0x16,0x1d, // STORE_NAME 'rm'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x08, // MAKE_FUNCTION 8
    0x10,0x07, // LOAD_CONST_STRING 'Man'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x07, // STORE_NAME 'Man'
    0x11,0x07, // LOAD_NAME 'Man'
    0x34,0x00, // CALL_FUNCTION 0
    0x16,0x2b, // STORE_NAME 'man'
    0x11,0x05, // LOAD_NAME 'PWD'
    0x34,0x00, // CALL_FUNCTION 0
    0x16,0x2c, // STORE_NAME 'pwd'
    0x11,0x04, // LOAD_NAME 'LS'
    0x34,0x00, // CALL_FUNCTION 0
    0x16,0x2d, // STORE_NAME 'ls'
    0x11,0x06, // LOAD_NAME 'CLEAR'
    0x34,0x00, // CALL_FUNCTION 0
    0x16,0x2e, // STORE_NAME 'clear'
    0x11,0x03, // LOAD_NAME 'os'
    0x13,0x08, // LOAD_ATTR 'chdir'
    0x16,0x2f, // STORE_NAME 'cd'
    0x11,0x03, // LOAD_NAME 'os'
    0x13,0x09, // LOAD_ATTR 'mkdir'
    0x16,0x09, // STORE_NAME 'mkdir'
    0x11,0x03, // LOAD_NAME 'os'
    0x13,0x0a, // LOAD_ATTR 'rename'
    0x16,0x30, // STORE_NAME 'mv'
    0x11,0x03, // LOAD_NAME 'os'
    0x13,0x0b, // LOAD_ATTR 'rmdir'
    0x16,0x0b, // STORE_NAME 'rmdir'
    0x11,0x31, // LOAD_NAME 'print'
    0x11,0x2b, // LOAD_NAME 'man'
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x10,0x0c, // LOAD_CONST_STRING '0.6.1'
    0x16,0x32, // STORE_NAME '__version__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_LS
static const byte fun_data_upysh_LS[30] = {
    0x08,0x0a, // prelude
    0x04, // names: LS
    0x68,0x40,0x64,0x20, // code info
    0x11,0x33, // LOAD_NAME '__name__'
    0x16,0x34, // STORE_NAME '__module__'
    0x10,0x04, // LOAD_CONST_STRING 'LS'
    0x16,0x35, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x22, // STORE_NAME '__repr__'
    0x10,0x0d, // LOAD_CONST_STRING '.'
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x23, // STORE_NAME '__call__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of upysh_LS
// frozen bytecode for file upysh.py, scope upysh_LS___repr__
static const byte fun_data_upysh_LS___repr__[16] = {
    0x11,0x0a, // prelude
    0x22,0x43, // names: __repr__, self
    0x60,0x60,0x26, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x23, // LOAD_METHOD '__call__'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x10,0x24, // LOAD_CONST_STRING ''
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_upysh_LS___repr__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh_LS___repr__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 16,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 34,
        .line_info = fun_data_upysh_LS___repr__ + 4,
        .line_info_top = fun_data_upysh_LS___repr__ + 7,
        .opcodes = fun_data_upysh_LS___repr__ + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_upysh_LS___repr__ fun_data_upysh_LS___repr__[0]
#endif

// child of upysh_LS
// frozen bytecode for file upysh.py, scope upysh_LS___call__
static const byte fun_data_upysh_LS___call__[162] = {
    0xee,0x01,0x24, // prelude
    0x23,0x43,0x3e, // names: __call__, self, path
    0x80,0x0a,0x2c,0x26,0x25,0x2a,0x2d,0x25,0x2a,0x29,0x52,0x2d,0x22,0x28,0x59, // code info
    0x12,0x44, // LOAD_GLOBAL 'list'
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x1e, // LOAD_METHOD 'ilistdir'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x14,0x25, // LOAD_METHOD 'sort'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x5f, // GET_ITER_STACK
    0x4b,0x18, // FOR_ITER 24
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x22,0x81,0x80,0x00, // LOAD_CONST_SMALL_INT 16384
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0x12,0x31, // LOAD_GLOBAL 'print'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x26, // JUMP -26
    0xb2, // LOAD_FAST 2
    0x5f, // GET_ITER_STACK
    0x4b,0x33, // FOR_ITER 51
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x22,0x81,0x80,0x00, // LOAD_CONST_SMALL_INT 16384
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x66, // POP_JUMP_IF_FALSE 38
    0x12,0x45, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x83, // LOAD_CONST_SMALL_INT 3
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x52, // POP_JUMP_IF_FALSE 18
    0x12,0x31, // LOAD_GLOBAL 'print'
    0x10,0x26, // LOAD_CONST_STRING '% 9d %s'
    0xb3, // LOAD_FAST 3
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x4b, // JUMP 11
    0x12,0x31, // LOAD_GLOBAL 'print'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x0b, // JUMP -53
    0x48,0x20, // SETUP_EXCEPT 32
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x27, // LOAD_METHOD 'statvfs'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0xc4, // STORE_FAST 4
    0x12,0x31, // LOAD_GLOBAL 'print'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x14,0x28, // LOAD_METHOD 'format'
    0xb4, // LOAD_FAST 4
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb4, // LOAD_FAST 4
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xf4, // BINARY_OP 29 __mul__
    0x22,0x88,0x00, // LOAD_CONST_SMALL_INT 1024
    0xf6, // BINARY_OP 31 __floordiv__
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x04, // POP_EXCEPT_JUMP 4
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_upysh_LS___call__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh_LS___call__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 162,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 35,
        .line_info = fun_data_upysh_LS___call__ + 6,
        .line_info_top = fun_data_upysh_LS___call__ + 21,
        .opcodes = fun_data_upysh_LS___call__ + 21,
    },
    #endif
    #endif
};
#else
#define proto_fun_upysh_LS___call__ fun_data_upysh_LS___call__[0]
#endif

static const mp_raw_code_t *const children_upysh_LS[] = {
    (const mp_raw_code_t *)&proto_fun_upysh_LS___repr__,
    (const mp_raw_code_t *)&proto_fun_upysh_LS___call__,
};

static const mp_raw_code_truncated_t proto_fun_upysh_LS = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh_LS,
    .children = (void *)&children_upysh_LS,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 30,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 4,
        .line_info = fun_data_upysh_LS + 3,
        .line_info_top = fun_data_upysh_LS + 7,
        .opcodes = fun_data_upysh_LS + 7,
    },
    #endif
    #endif
};

// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_PWD
static const byte fun_data_upysh_PWD[24] = {
    0x00,0x08, // prelude
    0x05, // names: PWD
    0x88,0x1d,0x64, // code info
    0x11,0x33, // LOAD_NAME '__name__'
    0x16,0x34, // STORE_NAME '__module__'
    0x10,0x05, // LOAD_CONST_STRING 'PWD'
    0x16,0x35, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x22, // STORE_NAME '__repr__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x23, // STORE_NAME '__call__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of upysh_PWD
// frozen bytecode for file upysh.py, scope upysh_PWD___repr__
static const byte fun_data_upysh_PWD___repr__[13] = {
    0x11,0x08, // prelude
    0x22,0x43, // names: __repr__, self
    0x80,0x1e, // code info
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x29, // LOAD_METHOD 'getcwd'
    0x36,0x00, // CALL_METHOD 0
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_upysh_PWD___repr__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh_PWD___repr__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 13,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 34,
        .line_info = fun_data_upysh_PWD___repr__ + 4,
        .line_info_top = fun_data_upysh_PWD___repr__ + 6,
        .opcodes = fun_data_upysh_PWD___repr__ + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_upysh_PWD___repr__ fun_data_upysh_PWD___repr__[0]
#endif

// child of upysh_PWD
// frozen bytecode for file upysh.py, scope upysh_PWD___call__
static const byte fun_data_upysh_PWD___call__[12] = {
    0x11,0x08, // prelude
    0x23,0x43, // names: __call__, self
    0x80,0x21, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x22, // LOAD_METHOD '__repr__'
    0x36,0x00, // CALL_METHOD 0
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_upysh_PWD___call__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh_PWD___call__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 12,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 35,
        .line_info = fun_data_upysh_PWD___call__ + 4,
        .line_info_top = fun_data_upysh_PWD___call__ + 6,
        .opcodes = fun_data_upysh_PWD___call__ + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_upysh_PWD___call__ fun_data_upysh_PWD___call__[0]
#endif

static const mp_raw_code_t *const children_upysh_PWD[] = {
    (const mp_raw_code_t *)&proto_fun_upysh_PWD___repr__,
    (const mp_raw_code_t *)&proto_fun_upysh_PWD___call__,
};

static const mp_raw_code_truncated_t proto_fun_upysh_PWD = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh_PWD,
    .children = (void *)&children_upysh_PWD,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 24,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 5,
        .line_info = fun_data_upysh_PWD + 3,
        .line_info_top = fun_data_upysh_PWD + 6,
        .opcodes = fun_data_upysh_PWD + 6,
    },
    #endif
    #endif
};

// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_CLEAR
static const byte fun_data_upysh_CLEAR[24] = {
    0x00,0x08, // prelude
    0x06, // names: CLEAR
    0x88,0x25,0x64, // code info
    0x11,0x33, // LOAD_NAME '__name__'
    0x16,0x34, // STORE_NAME '__module__'
    0x10,0x06, // LOAD_CONST_STRING 'CLEAR'
    0x16,0x35, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x22, // STORE_NAME '__repr__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x23, // STORE_NAME '__call__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of upysh_CLEAR
// frozen bytecode for file upysh.py, scope upysh_CLEAR___repr__
static const byte fun_data_upysh_CLEAR___repr__[9] = {
    0x09,0x08, // prelude
    0x22,0x43, // names: __repr__, self
    0x80,0x26, // code info
    0x10,0x2a, // LOAD_CONST_STRING '\x1b[2J\x1b[H'
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_upysh_CLEAR___repr__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh_CLEAR___repr__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 9,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 34,
        .line_info = fun_data_upysh_CLEAR___repr__ + 4,
        .line_info_top = fun_data_upysh_CLEAR___repr__ + 6,
        .opcodes = fun_data_upysh_CLEAR___repr__ + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_upysh_CLEAR___repr__ fun_data_upysh_CLEAR___repr__[0]
#endif

// child of upysh_CLEAR
// frozen bytecode for file upysh.py, scope upysh_CLEAR___call__
static const byte fun_data_upysh_CLEAR___call__[12] = {
    0x11,0x08, // prelude
    0x23,0x43, // names: __call__, self
    0x80,0x29, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x22, // LOAD_METHOD '__repr__'
    0x36,0x00, // CALL_METHOD 0
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_upysh_CLEAR___call__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh_CLEAR___call__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 12,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 35,
        .line_info = fun_data_upysh_CLEAR___call__ + 4,
        .line_info_top = fun_data_upysh_CLEAR___call__ + 6,
        .opcodes = fun_data_upysh_CLEAR___call__ + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_upysh_CLEAR___call__ fun_data_upysh_CLEAR___call__[0]
#endif

static const mp_raw_code_t *const children_upysh_CLEAR[] = {
    (const mp_raw_code_t *)&proto_fun_upysh_CLEAR___repr__,
    (const mp_raw_code_t *)&proto_fun_upysh_CLEAR___call__,
};

static const mp_raw_code_truncated_t proto_fun_upysh_CLEAR = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh_CLEAR,
    .children = (void *)&children_upysh_CLEAR,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 24,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_upysh_CLEAR + 3,
        .line_info_top = fun_data_upysh_CLEAR + 6,
        .opcodes = fun_data_upysh_CLEAR + 6,
    },
    #endif
    #endif
};

// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_head
static const byte fun_data_upysh_head[62] = {
    0xd6,0x01,0x14, // prelude
    0x0e,0x36,0x37, // names: head, f, n
    0x80,0x2d,0x28,0x26,0x26,0x23,0x22, // code info
    0x12,0x38, // LOAD_GLOBAL 'open'
    0xb0, // LOAD_FAST 0
    0x34,0x01, // CALL_FUNCTION 1
    0x47,0x26, // SETUP_WITH 38
    0xc0, // STORE_FAST 0
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x59, // JUMP 25
    0x57, // DUP_TOP
    0xc2, // STORE_FAST 2
    0xb0, // LOAD_FAST 0
    0x14,0x0f, // LOAD_METHOD 'readline'
    0x36,0x00, // CALL_METHOD 0
    0xc3, // STORE_FAST 3
    0xb3, // LOAD_FAST 3
    0x43,0x42, // POP_JUMP_IF_TRUE 2
    0x42,0x51, // JUMP 17
    0x12,0x02, // LOAD_GLOBAL 'sys'
    0x13,0x10, // LOAD_ATTR 'stdout'
    0x14,0x11, // LOAD_METHOD 'write'
    0xb3, // LOAD_FAST 3
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x22, // POP_JUMP_IF_TRUE -30
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_upysh_head = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh_head,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 62,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_upysh_head + 6,
        .line_info_top = fun_data_upysh_head + 13,
        .opcodes = fun_data_upysh_head + 13,
    },
    #endif
    #endif
};
#else
#define proto_fun_upysh_head fun_data_upysh_head[0]
#endif

// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_cat
static const byte fun_data_upysh_cat[16] = {
    0x19,0x08, // prelude
    0x12,0x36, // names: cat, f
    0x80,0x36, // code info
    0x12,0x0e, // LOAD_GLOBAL 'head'
    0xb0, // LOAD_FAST 0
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_upysh_cat = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh_cat,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 16,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 18,
        .line_info = fun_data_upysh_cat + 4,
        .line_info_top = fun_data_upysh_cat + 6,
        .opcodes = fun_data_upysh_cat + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_upysh_cat fun_data_upysh_cat[0]
#endif

// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_cp
static const byte fun_data_upysh_cp[132] = {
    0xea,0x02,0x20, // prelude
    0x13,0x39,0x3a, // names: cp, s, t
    0x80,0x3a,0x22,0x30,0x56,0x23,0x28,0x26,0x34,0x20,0x27,0x25,0x22, // code info
    0x48,0x1f, // SETUP_EXCEPT 31
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x14, // LOAD_METHOD 'stat'
    0xb1, // LOAD_FAST 1
    0x36,0x01, // CALL_METHOD 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x22,0x81,0x80,0x00, // LOAD_CONST_SMALL_INT 16384
    0xef, // BINARY_OP 24 __and__
    0x44,0x4d, // POP_JUMP_IF_FALSE 13
    0xb1, // LOAD_FAST 1
    0x14,0x15, // LOAD_METHOD 'rstrip'
    0x10,0x16, // LOAD_CONST_STRING '/'
    0x36,0x01, // CALL_METHOD 1
    0x10,0x16, // LOAD_CONST_STRING '/'
    0xf2, // BINARY_OP 27 __add__
    0xb0, // LOAD_FAST 0
    0xf2, // BINARY_OP 27 __add__
    0xc1, // STORE_FAST 1
    0x4a,0x0a, // POP_EXCEPT_JUMP 10
    0x57, // DUP_TOP
    0x12,0x3b, // LOAD_GLOBAL 'OSError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x12,0x3c, // LOAD_GLOBAL 'bytearray'
    0x22,0x84,0x00, // LOAD_CONST_SMALL_INT 512
    0x34,0x01, // CALL_FUNCTION 1
    0xc2, // STORE_FAST 2
    0x12,0x3d, // LOAD_GLOBAL 'memoryview'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0x12,0x38, // LOAD_GLOBAL 'open'
    0xb0, // LOAD_FAST 0
    0x10,0x17, // LOAD_CONST_STRING 'rb'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x2b, // SETUP_WITH 43
    0xc0, // STORE_FAST 0
    0x12,0x38, // LOAD_GLOBAL 'open'
    0xb1, // LOAD_FAST 1
    0x10,0x18, // LOAD_CONST_STRING 'wb'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x1e, // SETUP_WITH 30
    0xc1, // STORE_FAST 1
    0xb0, // LOAD_FAST 0
    0x14,0x19, // LOAD_METHOD 'readinto'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0xc4, // STORE_FAST 4
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0xda, // BINARY_OP 3 __le__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0x42,0x4e, // JUMP 14
    0xb1, // LOAD_FAST 1
    0x14,0x11, // LOAD_METHOD 'write'
    0xb3, // LOAD_FAST 3
    0x51, // LOAD_CONST_NONE
    0xb4, // LOAD_FAST 4
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x24, // JUMP -28
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_upysh_cp = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh_cp,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 132,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 19,
        .line_info = fun_data_upysh_cp + 6,
        .line_info_top = fun_data_upysh_cp + 19,
        .opcodes = fun_data_upysh_cp + 19,
    },
    #endif
    #endif
};
#else
#define proto_fun_upysh_cp fun_data_upysh_cp[0]
#endif

// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_newfile
static const byte fun_data_upysh_newfile[73] = {
    0xd1,0x02,0x16, // prelude
    0x1a,0x3e, // names: newfile, path
    0x80,0x4a,0x27,0x2a,0x20,0x22,0x4e,0x24,0x27, // code info
    0x12,0x31, // LOAD_GLOBAL 'print'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x12,0x38, // LOAD_GLOBAL 'open'
    0xb0, // LOAD_FAST 0
    0x10,0x1b, // LOAD_CONST_STRING 'w'
    0x34,0x02, // CALL_FUNCTION 2
    0x47,0x27, // SETUP_WITH 39
    0xc1, // STORE_FAST 1
    0x48,0x07, // SETUP_EXCEPT 7
    0x12,0x3f, // LOAD_GLOBAL 'input'
    0x34,0x00, // CALL_FUNCTION 0
    0xc2, // STORE_FAST 2
    0x4a,0x0b, // POP_EXCEPT_JUMP 11
    0x57, // DUP_TOP
    0x12,0x40, // LOAD_GLOBAL 'EOFError'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x44, // POP_JUMP_IF_FALSE 4
    0x59, // POP_TOP
    0x40,0x53,0x01, // UNWIND_JUMP 19
    0x5d, // END_FINALLY
    0xb1, // LOAD_FAST 1
    0x14,0x11, // LOAD_METHOD 'write'
    0xb2, // LOAD_FAST 2
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x14,0x11, // LOAD_METHOD 'write'
    0x10,0x1c, // LOAD_CONST_STRING '\n'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x1b, // JUMP -37
    0x51, // LOAD_CONST_NONE
    0x5c, // WITH_CLEANUP
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_upysh_newfile = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh_newfile,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 73,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 11,
        .n_exc_stack = 2,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 26,
        .line_info = fun_data_upysh_newfile + 5,
        .line_info_top = fun_data_upysh_newfile + 14,
        .opcodes = fun_data_upysh_newfile + 14,
    },
    #endif
    #endif
};
#else
#define proto_fun_upysh_newfile fun_data_upysh_newfile[0]
#endif

// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_rm
static const byte fun_data_upysh_rm[117] = {
    0xe6,0x01,0x18, // prelude
    0x1d,0x41,0x42, // names: rm, d, recursive
    0x80,0x56,0x22,0x33,0x2b,0x30,0x33,0x4a,0x4b, // code info
    0x48,0x55, // SETUP_EXCEPT 85
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x14, // LOAD_METHOD 'stat'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x22,0x81,0x80,0x00, // LOAD_CONST_SMALL_INT 16384
    0xef, // BINARY_OP 24 __and__
    0x44,0x7b, // POP_JUMP_IF_FALSE 59
    0xb1, // LOAD_FAST 1
    0x44,0x78, // POP_JUMP_IF_FALSE 56
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x1e, // LOAD_METHOD 'ilistdir'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x5f, // GET_ITER_STACK
    0x4b,0x24, // FOR_ITER 36
    0xc2, // STORE_FAST 2
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x10,0x0d, // LOAD_CONST_STRING '.'
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x10,0x1f, // LOAD_CONST_STRING '..'
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x51, // POP_JUMP_IF_FALSE 17
    0x12,0x1d, // LOAD_GLOBAL 'rm'
    0x10,0x16, // LOAD_CONST_STRING '/'
    0x14,0x20, // LOAD_METHOD 'join'
    0xb0, // LOAD_FAST 0
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x2a,0x02, // BUILD_TUPLE 2
    0x36,0x01, // CALL_METHOD 1
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x42,0x1a, // JUMP -38
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x0b, // LOAD_METHOD 'rmdir'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0x48, // JUMP 8
    0x12,0x03, // LOAD_GLOBAL 'os'
    0x14,0x21, // LOAD_METHOD 'remove'
    0xb0, // LOAD_FAST 0
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x4a,0x0d, // POP_EXCEPT_JUMP 13
    0x59, // POP_TOP
    0x12,0x31, // LOAD_GLOBAL 'print'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xb0, // LOAD_FAST 0
    0xf8, // BINARY_OP 33 __mod__
    0x34,0x01, // CALL_FUNCTION 1
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_upysh_rm = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh_rm,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 117,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 29,
        .line_info = fun_data_upysh_rm + 6,
        .line_info_top = fun_data_upysh_rm + 15,
        .opcodes = fun_data_upysh_rm + 15,
    },
    #endif
    #endif
};
#else
#define proto_fun_upysh_rm fun_data_upysh_rm[0]
#endif

// child of upysh__lt_module_gt_
// frozen bytecode for file upysh.py, scope upysh_Man
static const byte fun_data_upysh_Man[19] = {
    0x00,0x06, // prelude
    0x07, // names: Man
    0x88,0x63, // code info
    0x11,0x33, // LOAD_NAME '__name__'
    0x16,0x34, // STORE_NAME '__module__'
    0x10,0x07, // LOAD_CONST_STRING 'Man'
    0x16,0x35, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x22, // STORE_NAME '__repr__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of upysh_Man
// frozen bytecode for file upysh.py, scope upysh_Man___repr__
static const byte fun_data_upysh_Man___repr__[9] = {
    0x09,0x08, // prelude
    0x22,0x43, // names: __repr__, self
    0x80,0x64, // code info
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_upysh_Man___repr__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh_Man___repr__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 9,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 34,
        .line_info = fun_data_upysh_Man___repr__ + 4,
        .line_info_top = fun_data_upysh_Man___repr__ + 6,
        .opcodes = fun_data_upysh_Man___repr__ + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_upysh_Man___repr__ fun_data_upysh_Man___repr__[0]
#endif

static const mp_raw_code_t *const children_upysh_Man[] = {
    (const mp_raw_code_t *)&proto_fun_upysh_Man___repr__,
};

static const mp_raw_code_truncated_t proto_fun_upysh_Man = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh_Man,
    .children = (void *)&children_upysh_Man,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 19,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 1,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_upysh_Man + 3,
        .line_info_top = fun_data_upysh_Man + 5,
        .opcodes = fun_data_upysh_Man + 5,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_upysh__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_upysh_LS,
    (const mp_raw_code_t *)&proto_fun_upysh_PWD,
    (const mp_raw_code_t *)&proto_fun_upysh_CLEAR,
    (const mp_raw_code_t *)&proto_fun_upysh_head,
    (const mp_raw_code_t *)&proto_fun_upysh_cat,
    (const mp_raw_code_t *)&proto_fun_upysh_cp,
    (const mp_raw_code_t *)&proto_fun_upysh_newfile,
    (const mp_raw_code_t *)&proto_fun_upysh_rm,
    (const mp_raw_code_t *)&proto_fun_upysh_Man,
};

static const mp_raw_code_truncated_t proto_fun_upysh__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_upysh__lt_module_gt_,
    .children = (void *)&children_upysh__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 169,
    .n_children = 9,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_upysh__lt_module_gt_ + 3,
        .line_info_top = fun_data_upysh__lt_module_gt_ + 32,
        .opcodes = fun_data_upysh__lt_module_gt_ + 32,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_upysh[70] = {
    MP_QSTR_upysh_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_sys,
    MP_QSTR_os,
    MP_QSTR_LS,
    MP_QSTR_PWD,
    MP_QSTR_CLEAR,
    MP_QSTR_Man,
    MP_QSTR_chdir,
    MP_QSTR_mkdir,
    MP_QSTR_rename,
    MP_QSTR_rmdir,
    MP_QSTR_0_dot_6_dot_1,
    MP_QSTR__dot_,
    MP_QSTR_head,
    MP_QSTR_readline,
    MP_QSTR_stdout,
    MP_QSTR_write,
    MP_QSTR_cat,
    MP_QSTR_cp,
    MP_QSTR_stat,
    MP_QSTR_rstrip,
    MP_QSTR__slash_,
    MP_QSTR_rb,
    MP_QSTR_wb,
    MP_QSTR_readinto,
    MP_QSTR_newfile,
    MP_QSTR_w,
    MP_QSTR__0x0a_,
    MP_QSTR_rm,
    MP_QSTR_ilistdir,
    MP_QSTR__dot__dot_,
    MP_QSTR_join,
    MP_QSTR_remove,
    MP_QSTR___repr__,
    MP_QSTR___call__,
    MP_QSTR_,
    MP_QSTR_sort,
    MP_QSTR__percent__space_9d_space__percent_s,
    MP_QSTR_statvfs,
    MP_QSTR_format,
    MP_QSTR_getcwd,
    MP_QSTR__0x1b__bracket_open_2J_0x1b__bracket_open_H,
    MP_QSTR_man,
    MP_QSTR_pwd,
    MP_QSTR_ls,
    MP_QSTR_clear,
    MP_QSTR_cd,
    MP_QSTR_mv,
    MP_QSTR_print,
    MP_QSTR___version__,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_f,
    MP_QSTR_n,
    MP_QSTR_open,
    MP_QSTR_s,
    MP_QSTR_t,
    MP_QSTR_OSError,
    MP_QSTR_bytearray,
    MP_QSTR_memoryview,
    MP_QSTR_path,
    MP_QSTR_input,
    MP_QSTR_EOFError,
    MP_QSTR_d,
    MP_QSTR_recursive,
    MP_QSTR_self,
    MP_QSTR_list,
    MP_QSTR_len,
};

// constants
static const mp_obj_int_t const_obj_upysh_0 = {{&mp_type_int}, {.neg=0, .fixed_dig=1, .alloc=2, .len=2, .dig=(uint16_t*)(const uint16_t[]){0x0,0x4000}}};
static const mp_obj_str_t const_obj_upysh_1 = {{&mp_type_str}, 25826, 58, (const byte*)"\x54\x79\x70\x65\x20\x66\x69\x6c\x65\x20\x63\x6f\x6e\x74\x65\x6e\x74\x73\x20\x6c\x69\x6e\x65\x20\x62\x79\x20\x6c\x69\x6e\x65\x2c\x20\x66\x69\x6e\x69\x73\x68\x20\x77\x69\x74\x68\x20\x45\x4f\x46\x20\x28\x43\x74\x72\x6c\x2b\x44\x29\x2e"};
static const mp_obj_str_t const_obj_upysh_6 = {{&mp_type_str}, 30724, 256, (const byte*)"\x0a\x75\x70\x79\x73\x68\x20\x69\x73\x20\x69\x6e\x74\x65\x6e\x64\x65\x64\x20\x74\x6f\x20\x62\x65\x20\x69\x6d\x70\x6f\x72\x74\x65\x64\x20\x75\x73\x69\x6e\x67\x3a\x0a\x66\x72\x6f\x6d\x20\x75\x70\x79\x73\x68\x20\x69\x6d\x70\x6f\x72\x74\x20\x2a\x0a\x0a\x54\x6f\x20\x73\x65\x65\x20\x74\x68\x69\x73\x20\x68\x65\x6c\x70\x20\x74\x65\x78\x74\x20\x61\x67\x61\x69\x6e\x2c\x20\x74\x79\x70\x65\x20\x22\x6d\x61\x6e\x22\x2e\x0a\x0a\x75\x70\x79\x73\x68\x20\x63\x6f\x6d\x6d\x61\x6e\x64\x73\x3a\x0a\x63\x6c\x65\x61\x72\x2c\x20\x6c\x73\x2c\x20\x6c\x73\x28\x2e\x2e\x2e\x29\x2c\x20\x68\x65\x61\x64\x28\x2e\x2e\x2e\x29\x2c\x20\x63\x61\x74\x28\x2e\x2e\x2e\x29\x2c\x20\x6e\x65\x77\x66\x69\x6c\x65\x28\x2e\x2e\x2e\x29\x0a\x63\x70\x28\x27\x73\x72\x63\x27\x2c\x20\x27\x64\x65\x73\x74\x27\x29\x2c\x20\x6d\x76\x28\x27\x6f\x6c\x64\x27\x2c\x20\x27\x6e\x65\x77\x27\x29\x2c\x20\x72\x6d\x28\x2e\x2e\x2e\x29\x0a\x70\x77\x64\x2c\x20\x63\x64\x28\x2e\x2e\x2e\x29\x2c\x20\x6d\x6b\x64\x69\x72\x28\x2e\x2e\x2e\x29\x2c\x20\x72\x6d\x64\x69\x72\x28\x2e\x2e\x2e\x29\x0a"};

// constant table
static const mp_rom_obj_t const_obj_table_data_upysh[7] = {
    MP_ROM_PTR(&const_obj_upysh_0),
    MP_ROM_PTR(&const_obj_upysh_1),
    MP_ROM_QSTR(MP_QSTR_rm_space_of_space__squot__percent_s_squot__space_failed),
    MP_ROM_QSTR(MP_QSTR__space__space__space__space__lt_dir_gt__space__percent_s),
    MP_ROM_QSTR(MP_QSTR__space__space__space__space__space__space__space__space__space__space__percent_s),
    MP_ROM_QSTR(MP_QSTR__0x0a__brace_open__colon__comma_d_brace_close_k_space_free),
    MP_ROM_PTR(&const_obj_upysh_6),
};

static const mp_frozen_module_t frozen_module_upysh = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_upysh,
        .obj_table = (mp_obj_t *)&const_obj_table_data_upysh,
    },
    .proto_fun = &proto_fun_upysh__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// collection of all frozen modules

const char mp_frozen_names[] = {
    #ifdef MP_FROZEN_STR_NAMES
    MP_FROZEN_STR_NAMES
    #endif
    "flashbdev.py\0"
    "apa106.py\0"
    "espnow.py\0"
    "machine.py\0"
    "_boot.py\0"
    "inisetup.py\0"
    "recovery.py\0"
    "core.py\0"
    "asyncio/__init__.py\0"
    "asyncio/core.py\0"
    "asyncio/event.py\0"
    "asyncio/funcs.py\0"
    "asyncio/lock.py\0"
    "asyncio/stream.py\0"
    "uasyncio.py\0"
    "requests/__init__.py\0"
    "mip/__init__.py\0"
    "ntptime.py\0"
    "ssl.py\0"
    "webrepl.py\0"
    "webrepl_setup.py\0"
    "urequests.py\0"
    "aioespnow.py\0"
    "dht.py\0"
    "onewire.py\0"
    "ds18x20.py\0"
    "neopixel.py\0"
    "umqtt/robust.py\0"
    "umqtt/simple.py\0"
    "upysh.py\0"
    "\0"
};

const mp_frozen_module_t *const mp_frozen_mpy_content[] = {
    &frozen_module_flashbdev,
    &frozen_module_apa106,
    &frozen_module_espnow,
    &frozen_module_machine,
    &frozen_module__boot,
    &frozen_module_inisetup,
    &frozen_module_recovery,
    &frozen_module_core,
    &frozen_module_asyncio___init__,
    &frozen_module_asyncio_core,
    &frozen_module_asyncio_event,
    &frozen_module_asyncio_funcs,
    &frozen_module_asyncio_lock,
    &frozen_module_asyncio_stream,
    &frozen_module_uasyncio,
    &frozen_module_requests___init__,
    &frozen_module_mip___init__,
    &frozen_module_ntptime,
    &frozen_module_ssl,
    &frozen_module_webrepl,
    &frozen_module_webrepl_setup,
    &frozen_module_urequests,
    &frozen_module_aioespnow,
    &frozen_module_dht,
    &frozen_module_onewire,
    &frozen_module_ds18x20,
    &frozen_module_neopixel,
    &frozen_module_umqtt_robust,
    &frozen_module_umqtt_simple,
    &frozen_module_upysh,
};

#ifdef MICROPY_FROZEN_LIST_ITEM
MICROPY_FROZEN_LIST_ITEM("flashbdev", "flashbdev.py")
MICROPY_FROZEN_LIST_ITEM("apa106", "apa106.py")
MICROPY_FROZEN_LIST_ITEM("espnow", "espnow.py")
MICROPY_FROZEN_LIST_ITEM("machine", "machine.py")
MICROPY_FROZEN_LIST_ITEM("_boot", "_boot.py")
MICROPY_FROZEN_LIST_ITEM("inisetup", "inisetup.py")
MICROPY_FROZEN_LIST_ITEM("recovery", "recovery.py")
MICROPY_FROZEN_LIST_ITEM("core", "core.py")
MICROPY_FROZEN_LIST_ITEM("asyncio", "asyncio/__init__.py")
MICROPY_FROZEN_LIST_ITEM("asyncio/core", "asyncio/core.py")
MICROPY_FROZEN_LIST_ITEM("asyncio/event", "asyncio/event.py")
MICROPY_FROZEN_LIST_ITEM("asyncio/funcs", "asyncio/funcs.py")
MICROPY_FROZEN_LIST_ITEM("asyncio/lock", "asyncio/lock.py")
MICROPY_FROZEN_LIST_ITEM("asyncio/stream", "asyncio/stream.py")
MICROPY_FROZEN_LIST_ITEM("uasyncio", "uasyncio.py")
MICROPY_FROZEN_LIST_ITEM("requests", "requests/__init__.py")
MICROPY_FROZEN_LIST_ITEM("mip", "mip/__init__.py")
MICROPY_FROZEN_LIST_ITEM("ntptime", "ntptime.py")
MICROPY_FROZEN_LIST_ITEM("ssl", "ssl.py")
MICROPY_FROZEN_LIST_ITEM("webrepl", "webrepl.py")
MICROPY_FROZEN_LIST_ITEM("webrepl_setup", "webrepl_setup.py")
MICROPY_FROZEN_LIST_ITEM("urequests", "urequests.py")
MICROPY_FROZEN_LIST_ITEM("aioespnow", "aioespnow.py")
MICROPY_FROZEN_LIST_ITEM("dht", "dht.py")
MICROPY_FROZEN_LIST_ITEM("onewire", "onewire.py")
MICROPY_FROZEN_LIST_ITEM("ds18x20", "ds18x20.py")
MICROPY_FROZEN_LIST_ITEM("neopixel", "neopixel.py")
MICROPY_FROZEN_LIST_ITEM("umqtt/robust", "umqtt/robust.py")
MICROPY_FROZEN_LIST_ITEM("umqtt/simple", "umqtt/simple.py")
MICROPY_FROZEN_LIST_ITEM("upysh", "upysh.py")
#endif

/*
byte sizes:
qstr content: 532 unique, 6876 bytes
bc content: 20228
const str content: 4395
const int content: 22
const obj content: 1784
const table qstr content: 0 entries, 0 bytes
const table ptr content: 199 entries, 796 bytes
raw code content: 314 * 4 = 5024
mp_frozen_mpy_names_content: 393
mp_frozen_mpy_content_size: 120
total: 39638
*/
