# Host (unix-port) build of the REAL zeno_lcd drawing/text code against a mock bus.
ZL_HOST_DIR := $(USERMOD_DIR)
ZL_ROOT := $(USERMOD_DIR)/../..

SRC_USERMOD += $(ZL_ROOT)/modzlcd.c
SRC_USERMOD += $(ZL_ROOT)/ili9488_panel.c
SRC_USERMOD += $(ZL_ROOT)/zeno_gfx.c
SRC_USERMOD += $(ZL_ROOT)/zeno_font.c
SRC_USERMOD += $(ZL_ROOT)/zeno_font_data.c
SRC_USERMOD += $(ZL_HOST_DIR)/mock_bus.c

CFLAGS_USERMOD += -I$(ZL_ROOT) -I$(ZL_HOST_DIR)/stubs
