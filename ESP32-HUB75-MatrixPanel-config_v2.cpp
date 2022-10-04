#include "ESP32-HUB75-MatrixPanel-config_v2.h"

const hub75_i2s_cfg_t hub75_i2s_cfg_default= {
  .mx_width = PANEL_WIDTH,
  .mx_height = PANEL_HEIGHT,
  .chain_length_x = PANEL_COUNT_X,
  .chain_length_y = PANEL_COUNT_Y,
  .gpio = {
  .r1 = R1_PIN,
  .g1 = G1_PIN,
  .b1 = B1_PIN,
  .r2 = R2_PIN,
  .g2 = G2_PIN,
  .b2 = B2_PIN,
  .a = A_PIN,
  .b = B_PIN,
  .c = C_PIN,
  .d = D_PIN,
  .e = E_PIN,
  .lat = LAT_PIN,
  .oe = OE_PIN,
  .clk = CLK_PIN,
  #ifdef ADD_VSYNC_OUT
  .vsync = VSYNC_PIN,
  #endif
  #ifdef ADD_VSYNC_OUT
  .hsync = HSYNC_PIN,
  #endif
  },
  .driver = SHIFT_DRIVER,
  .i2sspeed = CLK_CLOCK, 
  .double_buff = false,
  .double_dma_buff = DOUBLE_BUFF,
  .latch_blanking = LAT_BLANKING,
  .clk_phase = CLK_PHASE,
  .color_depth = PIXEL_COLOR_DEPTH, 
};
