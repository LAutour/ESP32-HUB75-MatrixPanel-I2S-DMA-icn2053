#include <Arduino.h>
#include "ESP32-HUB75-MatrixPanel-I2S-DMA-icn2053.h"
#include "ESP32-HUB75-MatrixPanel-leddrivers_v2.h"

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



