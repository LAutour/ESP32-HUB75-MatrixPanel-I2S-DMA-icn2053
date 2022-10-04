#ifndef _ESP32_VIRTUAL_MATRIX_PANEL_I2S_DMA
#define _ESP32_VIRTUAL_MATRIX_PANEL_I2S_DMA

//переделанная библиотека под ESP32-HUB75-MatrixPanel-I2S-DMA
//наследование cразу от ESP32-HUB75-MatrixPanel, то позволяет убрать лишние переопределения виртуальных методов
//одновременно пришлось отрисовку пиксела заменить на отрисовку прямоугольника
//теоретически можно вообще убрать внутрь ESP32-HUB75-MatrixPanel

#include "ESP32-HUB75-MatrixPanel-I2S-DMA_v2.h"

#ifndef NO_GFX
 #include <Fonts/FreeSansBold12pt7b.h>
#endif

 
#ifdef USE_GFX_ROOT
class VirtualMatrixPanel : public GFX
#elif !defined NO_GFX
class VirtualMatrixPanel : public MatrixPanel_I2S_DMA
#else
class VirtualMatrixPanel
#endif	
{
  public:
    int16_t virtualResX;
    int16_t virtualResY;

    VirtualMatrixPanel(bool serpentine_chain = true, bool top_down_chain = false);
    VirtualMatrixPanel(const hub75_i2s_cfg_t& opts, bool serpentine_chain = true, bool top_down_chain = false);
    //VirtualCoords getCoords_(int16_t x, int16_t y);
    virtual void fillRectBuffer(int16_t x, int16_t y, int16_t w, int16_t h);
		
  // Rotate display
  void setRotate(bool rotate);
  // Mirror display
  void setMirrorX(bool mirror_x);
  void setMirrorY(bool mirror_y);

  private:
    bool _s_chain_party  = true; // Are we chained? Ain't no party like a... 
    bool _chain_top_down = false; // is the ESP at the top or bottom of the matrix of devices?
    bool _mirror_x = false;
    bool _mirror_y = false;
	  bool _rotate = false;
    void init(bool serpentine_chain, bool top_down_chain);
}; // end Class header

#endif

