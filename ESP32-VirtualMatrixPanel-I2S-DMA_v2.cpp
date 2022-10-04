#include "ESP32-VirtualMatrixPanel-I2S-DMA_v2.h"

struct VirtualCoords {
  int16_t x;
  int16_t y;
};


void VirtualMatrixPanel::init(bool serpentine_chain, bool top_down_chain)
{
  virtualResY = m_cfg.chain_length_y*m_cfg.mx_height;
  virtualResX = m_cfg.chain_length_x*m_cfg.mx_width;     

  _s_chain_party = serpentine_chain; // serpentine, or 'S' chain?
  _chain_top_down = top_down_chain;
}

VirtualMatrixPanel::VirtualMatrixPanel(bool serpentine_chain, bool top_down_chain)
  #if defined(USE_GFX_ROOT)
  :GFX(vmodule_cols*panelX, vmodule_rows*panelY);
  #elif !defined(NO_GFX)
  :MatrixPanel_I2S_DMA(true)
  #endif         
{
  init(serpentine_chain, top_down_chain);
  //MatrixPanel_I2S_DMA::begin();
}

VirtualMatrixPanel::VirtualMatrixPanel(const hub75_i2s_cfg_t& opts, bool serpentine_chain, bool top_down_chain)
  #if defined(USE_GFX_ROOT)
  :GFX(vmodule_cols*panelX, vmodule_rows*panelY);
  #elif !defined(NO_GFX)
  :MatrixPanel_I2S_DMA(opts, true)
  #endif 
{
  init(serpentine_chain, top_down_chain);
  //MatrixPanel_I2S_DMA::begin();
}


void VirtualMatrixPanel::fillRectBuffer(int16_t x, int16_t y, int16_t w, int16_t h)
{  
  if (_rotate)
  {
    int16_t temp = h;
    h = w;
    w = temp;
    temp = x;
    x = y;
    y = temp;
  }else if (m_cfg.chain_length_y == 1)
  {
    //стандартные горизонтальные последовательно соединенные панели
    MatrixPanel_I2S_DMA::fillRectBuffer(x,y,w,h);
  }

  VirtualCoords coords;

  //Serial.println();Serial.print(x);Serial.print(":");Serial.println(y);
  uint8_t row = (y / m_cfg.mx_height); //a non indexed 0 row number
  int16_t _h;
  if((uint8_t)_s_chain_party & (row & 1))
  {
    coords.x = (row+1)*virtualResX - x - w;
    coords.y = y - row*m_cfg.mx_height;
    _h = m_cfg.mx_height - coords.y;
    if (_h > h) _h = h;
  }else 
  {
    coords.x = x + (row)*virtualResX;
    coords.y = m_cfg.mx_height - (y - row*m_cfg.mx_height) - 1;
    _h = coords.y + 1;
    if (_h > h) _h = h;
    coords.y -= _h - 1;
  }
  x = (x << 1) + w;
  while(true)
  {
    MatrixPanel_I2S_DMA::fillRectBuffer(coords.x,coords.y,w,_h);
    //Serial.print(coords.x);Serial.print(":");Serial.print(coords.y);Serial.print(":");Serial.print(w);Serial.print(":");Serial.print(_h);Serial.print("-");  Serial.println(_h);
    h -= _h;
    if (h <= 0) break;
    row++;
    _h = m_cfg.mx_height;
    if (_h > h) _h = h;
    if(_s_chain_party)
    {
      if(row & 1)
      {
        coords.x += (virtualResX << 1) - x;
        coords.y = 0;
      }else
      {
        coords.x += x;
        coords.y = m_cfg.mx_height - _h;
      }
    }else 
    {
      coords.x += virtualResX;
      coords.y = m_cfg.mx_height - _h;
    }
  }  
}

void VirtualMatrixPanel::setRotate(bool rotate) 
{
	_rotate=rotate;	
		// We don't support rotation by degrees.
    setRotation(rotate & 1); 
}



