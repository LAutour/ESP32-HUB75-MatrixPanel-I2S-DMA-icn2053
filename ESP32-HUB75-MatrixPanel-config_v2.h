#ifndef _ESP32_HUB75_RGB_MATRIXPANEL_CONFIG_V2
#define _ESP32_HUB75_RGB_MATRIXPANEL_CONFIG_V2

#include <stdint.h>
#include <stdbool.h>
#include "color_convert.h"ы

//сообщения отладки по UART
#define SERIAL_DEBUG
//прроцедуры инциализации регистров драйверов чрезе прямой доступ к выводам интерфейса 
//оставлено на всякий случай
//#define DIRECT_DRIVER_INIT

//оптимизация использования видеопямяти и кода для ICN2053
#define USE_COLORx16 //16-битный цвет для ICN2053 

enum{BRIGHTNESS_DEFAULT = 255};

#ifndef MATRIX_WIDTH
 #define MATRIX_WIDTH                80   // Single panel of 64 pixel width
#endif
enum{PANEL_WIDTH = MATRIX_WIDTH};

#ifndef MATRIX_HEIGHT
 #define MATRIX_HEIGHT               40   // CHANGE THIS VALUE to 64 IF USING 64px HIGH panel(s) with E PIN
#endif
enum{PANEL_HEIGHT = MATRIX_HEIGHT};

#ifndef CHAIN_LENGTH_X
 #define CHAIN_LENGTH_X                1   // Number of modules chained together, i.e. 4 panels chained result in virtualmatrix 64x4=256 px long
#endif
enum{PANEL_COUNT_X = CHAIN_LENGTH_X};

#ifndef CHAIN_LENGTH_Y
 #define CHAIN_LENGTH_Y                1   // Number of modules chained together, i.e. 4 panels chained result in virtualmatrix 64x4=256 px long
#endif
enum{PANEL_COUNT_Y = CHAIN_LENGTH_Y};


#ifndef MATRIX_ROWS_IN_PARALLEL
 #define MATRIX_ROWS_IN_PARALLEL     2
#endif
enum{ROWS_IN_PARALLEL = MATRIX_ROWS_IN_PARALLEL};

#ifndef PIXEL_COLOR_DEPTH
 #define PIXEL_COLOR_DEPTH      COLORx16
#endif

#ifndef LAT_BLANKING
#define LAT_BLANKING 1
#endif

#ifndef R1_PIN
#define R1_PIN 32
#endif
#ifndef G1_PIN
#define G1_PIN 33
#endif
#ifndef B1_PIN
#define B1_PIN 25
#endif
#ifndef R2_PIN
#define R2_PIN 26
#endif
#ifndef G2_PIN
#define G2_PIN 27
#endif
#ifndef B2_PIN
#define B2_PIN 14
#endif
#ifndef A_PIN
#define A_PIN 15
#endif
#ifndef B_PIN
#define B_PIN 13
#endif
#ifndef C_PIN
#define C_PIN 4
#endif
#ifndef D_PIN
#define D_PIN 2
#endif
#ifndef E_PIN
#define E_PIN 12
#endif
#ifndef LAT_PIN
#define LAT_PIN 16
#endif
#ifndef OE_PIN
#define OE_PIN 5
#endif
#ifndef CLK_PIN
#define CLK_PIN 17
#endif
#ifdef ADD_VSYNC_OUT
#ifndef VSYNC_PIN
#define VSYNC_PIN 18
#endif
#endif
#ifdef ADD_HSYNC_OUT
#ifndef HSYNC_PIN
#define HSYNC_PIN 18
#endif
#endif

typedef enum{CLK_POZITIVE = 0, CLK_NEGATIVE} clk_phase_t;
enum{DOUBLE_BUFF_OFF = false, DOUBLE_BUFF_ON = true};

enum {
  SHIFTREG = 0, 
  MBI5124,
  ICN2038,
  ICN2038S = ICN2038,
  FM6124 = ICN2038S,
  FM6126A = ICN2038S,    
  ICN2053,
  FM6353 = ICN2053,
  FM6353B = ICN2053,
  };
enum clk_clock_t{HZ_5M=5000000, HZ_10M=10000000, HZ_13M=13000000, HZ_20M=20000000};

//enum{ICN2053_VSYNK = 3};

#ifndef SHIFT_DRIVER
#define SHIFT_DRIVER ICN2053
#endif

#ifndef CLK_CLOCK
#define CLK_CLOCK HZ_13M
#endif

#ifndef DOUBLE_BUFF
#define DOUBLE_BUFF true
#endif

#ifndef LATCH_BLANK
#define LATCH_BLANK 1
#endif

#ifndef CLK_PHASE
#define CLK_PHASE CLK_POZITIVE
#endif


typedef struct{
  #ifdef ADD_VSYNC_OUT
  uint8_t r1, g1, b1, r2, g2, b2, a, b, c, d, e, lat, oe, clk, vsync;
  #else
  uint8_t r1, g1, b1, r2, g2, b2, a, b, c, d, e, lat, oe, clk;
  #endif
} hub75_pins_t;

//#define COLOR_CHANNELS_PER_PIXEL     3

typedef struct  HUB75_I2S_CFG_S{
  // physical width of a single matrix panel module (in pixels, usually it is 64 ;) )
  uint16_t mx_width;
  // physical height of a single matrix panel module (in pixels, usually amost always it is either 32 or 64)
  uint16_t mx_height;
  // number of chained panels regardless of the topology, default 1 - a single matrix module
  uint16_t chain_length_x;
  //
  uint16_t chain_length_y;
    //GPIO pins mapping
  hub75_pins_t gpio;
  // Matrix driver chip type - default is a plain shift register
  uint8_t driver;
  // I2S clock speed
  clk_clock_t i2sspeed;

  // use DMA double buffer (twice as much RAM required)
  // для Shift драйверов двойной буфер включатеся по double_buff или double_dma_buff 
  // для ICN2053 драйверов двойные буфера double_buff (первичный видео буфер) и double_dma_buff независимы (буфер вывода)
  //двойной DMA буфер(SHIFT) или двойной первичный видео буфер (ICN2053)
  bool double_buff;
  //двойной DMA буфер
  bool double_dma_buff;

  // How many clock cycles to blank OE before/after LAT signal change, default is 1 clock
  uint8_t latch_blanking;
  //
  clk_phase_t clk_phase;
  //
  color_depth_t color_depth;
    /**
   *  I2S clock phase
   *  0 (default) - data lines are clocked with negative edge
   *  Clk  /¯\_/¯\_/
   *  LAT  __/¯¯¯\__
   *  EO   ¯¯¯¯¯¯\___
   *
   *  1 - data lines are clocked with positive edge
   *  Clk  \_/¯\_/¯\
   *  LAT  __/¯¯¯\__
   *  EO   ¯¯¯¯¯¯\__
   *
   */
}hub75_i2s_cfg_t; // end of structure HUB75_I2S_CFG

extern const hub75_i2s_cfg_t hub75_i2s_cfg_default;

// Definitions below should NOT be ever changed without rewriting library logic
typedef uint16_t ESP32_I2S_DMA_STORAGE_TYPE;  // DMA output of one uint16_t at a time.
enum{SIZE_DMA_TYPE = sizeof(ESP32_I2S_DMA_STORAGE_TYPE)};

#define ESP32_I2S_DMA_MODE I2S_PARALLEL_WIDTH_16    // From esp32_i2s_parallel_v2.h = 16 bits in parallel

// Panel Upper half RGB (numbering according to order in DMA gpio_bus configuration)
enum{
  BITS_RGB1_OFFSET = 0, // Start point of RGB_X1 bits
  BIT_R1 = (1<<0),   
  BIT_G1 = (1<<1),   
  BIT_B1 = (1<<2),   

// Panel Lower half RGB
  BITS_RGB2_OFFSET = 3, // Start point of RGB_X2 bits
  BIT_R2 = (1<<3),   
  BIT_G2 = (1<<4),   
  BIT_B2 = (1<<5),   

// Panel Control Signals
  BIT_LAT = (1<<6), 
  BIT_OE = (1<<7),  

// Panel GPIO Pin Addresses (A, B, C, D etc..)
  BITS_ADDR_OFFSET = 8,  // Start point of address bits
  BIT_A = (1<<8),    
  BIT_B = (1<<9),    
  BIT_C = (1<<10),   
  BIT_D = (1<<11),  
  BIT_E = (1<<12),  
  #ifdef ADD_HSYNC_OUT 
  BIT_HSYNC = (1<<14),
  #endif
  #ifdef ADD_VSYNC_OUT 
  BIT_VSYNC = (1<<15),
  #endif

  BITMASK_ADDR  = BIT_A + BIT_B + BIT_C + BIT_D + BIT_E,  //битовая маска адреса строки
  BITMASK_RGB1  = BIT_R1 + BIT_G1 + BIT_B1,    //битовая маска цвета верхних строк
  BITMASK_RGB2  = BIT_R2 + BIT_G2 + BIT_B2,    //битовая маска цвета нижних строк
  BITMASK_RGB12 = BITMASK_RGB1 + BITMASK_RGB2, //битовая маска цвета
};

#endif