#pragma once
/* Runtime pin map. Filled by zenotft.init(); read by vendored lcd_com.c via CONFIG_*_GPIO macros. */
typedef struct { int d0,d1,d2,d3,d4,d5,d6,d7,wr,rd,rs,cs,rst; } zeno_pins_t;
extern zeno_pins_t zeno_pins;
