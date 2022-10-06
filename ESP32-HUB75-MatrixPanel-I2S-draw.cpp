#include <Arduino.h>
#include "ESP32-HUB75-MatrixPanel-I2S-DMA-icn2053.h"
#include "ESP32-HUB75-MatrixPanel-leddrivers_v2.h"

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

#ifndef ABS
#define ABS(a)  (a >= 0)?(a):(-(a))  
#endif

#ifndef MIN
#define MIN(a,b) ((a) < (b))?(a):(b)  
#endif

#ifndef MAX
#define MIN(a,b) ((a) > (b))?(a):(b)  
#endif


//вынесенные процедуры отрисовки

void MatrixPanel_I2S_DMA::setColor(uint8_t r, uint8_t g, uint8_t b)
{
  if ((CurRGB.r == r)&&(CurRGB.g == g)&&(CurRGB.b == b)) return;
	CurRGB.r = r;
	CurRGB.g = g;
	CurRGB.b = b; 	
  CurColor = color565(r, g, b);    
}

void MatrixPanel_I2S_DMA::setColor(uint16_t color)
{
  if (color == CurColor) return;
  CurColor = color;
  color565to888(color, CurRGB.r, CurRGB.g, CurRGB.b);
}

void MatrixPanel_I2S_DMA::setTextColorRGB(uint8_t r, uint8_t g, uint8_t b)
{  
  setTextColor(color565(r,g,b));
}

void MatrixPanel_I2S_DMA::clearScreen()
{
  //Serial.println("clear");
  clearBuffer(0);
  if (m_cfg.double_buff)
  {
    clearBuffer(1);
  }
}

void MatrixPanel_I2S_DMA::drawPixel(int16_t x, int16_t y, uint16_t color) // adafruit virtual void override
{
  setColor(color);
  fillRectBuffer(x,y, 1, 1);
} 

void MatrixPanel_I2S_DMA::drawPixelRGB888(int16_t x, int16_t y, uint8_t r, uint8_t g,uint8_t b) 
{
  setColor(r, g, b);
  fillRectBuffer(x,y, 1, 1);
}

#ifdef USE_GFX_ROOT
// Support for CRGB values provided via FastLED
void MatrixPanel_I2S_DMA::drawPixel(int16_t x, int16_t y, CRGB color) 
{
  drawPixel( x, y, color.red, color.green, color.blue);
}

void MatrixPanel_I2S_DMA::fillScreen(CRGB color) 
{
  updateMatrixDMABuffer(color.red, color.green, color.blue);
}
#endif

void MatrixPanel_I2S_DMA::fillScreen(uint16_t color)  // adafruit virtual void override
{  
  //Serial.println("Fill");
  setColor(color);
  //Serial.println(back_buffer_id);
  if (color == 0) 
    clearBuffer(1);
  else 
    MatrixPanel_I2S_DMA::fillRectBuffer(0,0,pixels_per_row, m_cfg.mx_height); // RGB only (no pixel coordinate) version of 'updateMatrixDMABuffer'
} 

void MatrixPanel_I2S_DMA::fillScreenRGB888(uint8_t r, uint8_t g,uint8_t b)
{
  setColor(r, g ,b);
  MatrixPanel_I2S_DMA::fillRectBuffer(0,0,pixels_per_row, m_cfg.mx_height); // RGB only (no pixel coordinate) version of 'updateMatrixDMABuffer'
} 

#ifndef NO_FAST_FUNCTIONS

void MatrixPanel_I2S_DMA::fillRect(int16_t x, int16_t y, int16_t w, int16_t h)
{
  fillRectBuffer(x,y,w,h);
}

void MatrixPanel_I2S_DMA::drawFastVLine(int16_t x, int16_t y, int16_t h)
{
  fillRect(x, y, 1, h);
}

void MatrixPanel_I2S_DMA::drawFastVLine(int16_t x, int16_t y, int16_t h, uint8_t r, uint8_t g, uint8_t b)
{ 
  setColor(r, g, b);
  fillRect(x, y, 1, h); 
};
void MatrixPanel_I2S_DMA::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  setColor(color);
  fillRect(x, y, 1, h);
}

void MatrixPanel_I2S_DMA::drawFastHLine(int16_t x, int16_t y, int16_t w)
{
  fillRect(x, y, w, 1);
}

void MatrixPanel_I2S_DMA::drawFastHLine(int16_t x, int16_t y, int16_t w, uint8_t r, uint8_t g, uint8_t b)
{ 
  setColor(r, g, b);
  fillRect(x, y, w, 1);
};
void MatrixPanel_I2S_DMA::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  setColor(color);
  fillRect(x, y, w, 1);
}

void MatrixPanel_I2S_DMA::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t r, uint8_t g, uint8_t b)
{
  setColor(r, g, b);
  fillRect(x, y, w, h);
}

void MatrixPanel_I2S_DMA::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  setColor(color);
  fillRect(x, y, w, h);
}


void MatrixPanel_I2S_DMA::drawRect(int16_t x, int16_t y, int16_t w, int16_t h)
{
  if ((w > 0)&&(h > 0))
  {
    drawFastHLine(x, y, w);
    drawFastHLine(x, y+h-1, w);
    drawFastVLine(x, y, h);
    drawFastVLine(x+w-1, y, h);
  }
}

void MatrixPanel_I2S_DMA::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t r, uint8_t g, uint8_t b)
{
  setColor(r, g, b);
  drawRect(x, y, w, h);
}

void MatrixPanel_I2S_DMA::drawRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color)
{
  setColor(color);
  drawRect(x, y, w, h);
}
#endif

void MatrixPanel_I2S_DMA::drawIcon (int *ico, int16_t x, int16_t y, int16_t cols, int16_t rows) 
{
/*  drawIcon draws a C style bitmap.  
//  Example 10x5px bitmap of a yellow sun 
//
  int half_sun [50] = {
      0x0000, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0x0000,
      0x0000, 0xffe0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffe0, 0x0000,
      0x0000, 0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000, 0x0000,
      0xffe0, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0xffe0,
      0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000,
  };
  
  MatrixPanel_I2S_DMA matrix;

  matrix.drawIcon (half_sun, 0,0,10,5);
*/

  int i, j;
  for (i = 0; i < rows; i++) 
  {
    for (j = 0; j < cols; j++) 
    {
      drawPixel(x + j, y + i, (uint16_t) ico[i * cols + j]);
    }
  }  
}

void MatrixPanel_I2S_DMA::_steepDrawPixelRGB(bool steep, int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b)
{
  if (steep) drawPixelRGB888(y,x,r,g,b);
  else drawPixelRGB888(x,y,r,g,b);
}

//Wu's line algorithm, fix point version
void MatrixPanel_I2S_DMA::writeLineAA(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) 
{
  int32_t dx = x1 - x0; 
  if (dx < 0) dx = -dx;
  int32_t dy = y1 - y0; 
  if (dy < 0) dy = -dy;
  if (dx == 0) 
  {
    drawFastVLine(x0, min(y0,y1), dy, color);
    return;
  }
  if (dy == 0) 
  {
    drawFastHLine(min(x0,x1), y0, dx, color);
    return;
  }
  if (dy == dx)
  {
    drawLine(x0,y0,x1,y1, color);
    return;
  }

  bool steep = dy > dx;
  if (steep)
  {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
    _swap_int16_t(dx, dy);
  }
  if (x0 > x1)
  {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }
  rgb888_t c;
  color565to888(color, c.r, c.g, c.b);

  _steepDrawPixelRGB(steep, x0, y0, c.r, c.g, c.b);
  _steepDrawPixelRGB(steep, x1, y1, c.r, c.g, c.b);
  
  dy <<= 16;
  int32_t gradient = dy / dx;
  int32_t y = (y0 << 16) + gradient;
  int16_t gcolor;
  rgb888_t g;
  for (int x = x0 + 1; x <= x1 - 1; x++)
  {
    gcolor = (y >> 8)& 255;
    g.r = (c.r * gcolor) >> 8;
    g.g = (c.g * gcolor) >> 8;
    g.b = (c.b * gcolor) >> 8;
    int16_t _y =  y >> 16;
    _steepDrawPixelRGB(steep, x, _y, c.r-g.r, c.g-g.g, c.b-g.b);
    _steepDrawPixelRGB(steep, x, _y + 1, g.r, g.g, g.b);
    y += gradient;
  }
}
