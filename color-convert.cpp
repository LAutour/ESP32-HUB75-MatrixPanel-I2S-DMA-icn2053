#include "color_convert.h"

//C/p'ed from https://ledshield.wordpress.com/2012/11/13/led-brightness-to-your-eye-gamma-correction-no/
// Example calculator: https://gist.github.com/mathiasvr/19ce1d7b6caeab230934080ae1f1380e
// need to make sure this would end up in RAM for fastest access
const uint8_t lumConvTab[256] = { 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   
  1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3, 
  3,   3,   4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,   6,   6,   6,   
  6,   7,   7,   7,   7,   8,   8,   8,   8,   9,   9,   9,   10,  10,  10,  11, 
  11,  11,  12,  12,  12,  13,  13,  13,  14,  14,  14,  15,  15,  16,  16,  17, 
  17,  17,  18,  18,  19,  19,  20,  20,  21,  21,  22,  22,  23,  23,  24,  24, 
  25,  25,  26,  27,  27,  28,  28,  29,  30,  30,  31,  31,  32,  33,  33,  34, 
  35,  35,  36,  37,  38,  38,  39,  40,  41,  41,  42,  43,  44,  45,  45,  46,
  47,  48,  49,  50,  51,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 
  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  73,  74,  75,  76,  77,  78, 
  80,  81,  82,  83,  84,  86,  87,  88,  90,  91,  92,  93,  95,  96,  98,  99, 
  100, 102, 103, 105, 106, 107, 109, 110, 112, 113, 115, 116, 118, 120, 121, 123, 
  124, 126, 128, 129, 131, 133, 134, 136, 138, 139, 141, 143, 145, 146, 148, 150, 
  152, 154, 156, 157, 159, 161, 163, 165, 167, 169, 171, 173, 175, 177, 179, 181, 
  183, 185, 187, 189, 192, 194, 196, 198, 200, 203, 205, 207, 209, 212, 214, 216, 
  218, 221, 223, 226, 228, 230, 233, 235, 238, 240, 243, 245, 248, 250, 253, 255, 
  //255
};

//from https://github.com/SebiTimeWaster/ICN2053_ESP32_LedWall
uint16_t Translate8To16Bit[256] = {
  0,46,92,139,186,233,280,327,375,422,470,519,567,615,664,713,
  762,812,861,911,961,1011,1061,1112,1163,1214,1265,1317,1368,1420,1473,1525,
  1578,1631,1684,1737,1791,1844,1899,1953,2007,2062,2117,2173,2228,2284,2340,2397,
  2453,2510,2568,2625,2683,2741,2799,2858,2917,2976,3036,3096,3156,3216,3277,3338,
  3399,3461,3523,3586,3648,3711,3775,3838,3902,3967,4032,4097,4162,4228,4294,4361,
  4428,4495,4563,4631,4699,4768,4838,4907,4978,5048,5119,5191,5262,5335,5407,5481,
  5554,5628,5703,5778,5853,5929,6006,6083,6160,6238,6317,6396,6476,6556,6636,6718,
  6799,6882,6965,7048,7132,7217,7302,7388,7475,7562,7650,7739,7828,7918,8008,8099,
  8191,8284,8377,8472,8567,8662,8759,8856,8954,9053,9153,9253,9355,9457,9560,9664,
  9769,9875,9982,10090,10199,10309,10420,10532,10645,10760,10875,10991,11109,11228,
  11348,11469,11591,11715,11840,11967,12094,12223,12354,12486,12620,12755,12891,13030,
  13169,13311,13454,13599,13746,13895,14045,14198,14352,14509,14667,14828,14991,15157,
  15324,15494,15667,15842,16020,16200,16383,16569,16758,16951,17146,17345,17547,17752,
  17961,18174,18391,18612,18837,19067,19301,19539,19783,20032,20286,20546,20812,21083,
  21361,21646,21938,22237,22544,22859,23183,23516,23859,24211,24575,24950,25338,25739,
  26153,26583,27029,27493,27975,28478,29003,29553,30130,30736,31375,32051,32767,33530,
  34345,35221,36167,37195,38322,39567,40959,42537,44359,46514,49151,52551,57343,65535
  };

const uint8_t mask_x[9] = {0,1,3,7,15,31,63,127,255};

uint16_t color111to565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r == 0)?0:0xF800)|((g == 0)?0:0x07E0)|((b == 0)?0:0x001F);
}

uint16_t color222to565(uint8_t r, uint8_t g, uint8_t b)
{
  r &= 3;
  r = (r << 3)|(r << 1)|(r >> 1);
  g &= 3;
  g |= (g << 4)||(g << 2);
  b &= 3;
  b = (b << 3)|(b << 1)|(b >> 1);
  return (r << 11)|(g << 5)|b;   
}

// Promote 3/3/3 RGB to Adafruit_GFX 5/6/5 RRRrrGGGgggBBBbb
uint16_t color333to565(uint8_t r, uint8_t g, uint8_t b) 
{ 
  r &= 7;
  r = (r << 2)|(r >> 1);
  g &= 7;
  g |= (g << 3);
  b &= 7;
  b = (b << 2)|(b >> 1);
  return (r << 11)|(g << 5)|b; 
}

// Promote 4/4/4 RGB to Adafruit_GFX 5/6/5 RRRRrGGGGggBBBBb
uint16_t color444to565(uint8_t r, uint8_t g, uint8_t b) 
{
  r &= 0x0F;
  r = (r << 1)|(r >> 3);
  g &= 0x0F;;
  g = (g << 2)|(g >> 2);
  b &= 0x0F;
  b = (b << 1)|(b >> 3);; 
  return (r << 11)|(g << 5)|b;
}

uint16_t color555to565(uint8_t r, uint8_t g, uint8_t b)
{
  g &= 0x1F;
  g = (g << 1)|(g >> 4);
  return (r << 11)|(g << 5)|(b & 0x1F);
}

uint16_t color666to565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0x3E) << 10)|((g &= 0x3F) << 5)|((b &= 0x3E) >> 1);
}

uint16_t color777to565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0x7C) << 9)|((g &= 0x7E) << 4)|((b &= 0x7C) >> 2);
}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
//https://github.com/squix78/ILI9341Buffer/blob/master/ILI9341_SPI.cpp
uint16_t color565(uint8_t r, uint8_t g, uint8_t b) 
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

/**
 * @brief - convert RGB565 to RGB888
 * @param uint16_t color - RGB565 input color
 * @param uint8_t &r, &g, &b - refs to variables where converted colours would be emplaced
 */
void color565to888(const uint16_t color, uint8_t& r, uint8_t& g, uint8_t& b)
{
  r = ((((color >> 11)) * 527) + 23) >> 6;
  g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
  b = (((color & 0x1F) * 527) + 23) >> 6;
}

void color565toXXX(const uint16_t color, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t X)
{
  if (X <= 5)
  {
    b = (color & 0x31) >> (5-X);
    g = (color & 0x7E0) >> (6-X);
    r = (color & 0xF800) >> (5-X);
  }else
  {    
    color565to888(color, r, g, b);
    if (X < 8)
    {
      X = 8 - X;
      b >>= X;
      g >>= X;
      b >>= X;
    }
  }
}


// Promote 4/4/4 RGB to Adafruit_GFX 5/6/5 RRRRRGGGGGgBBBBB
uint16_t colorXXXto565(uint8_t r, uint8_t g, uint8_t b, uint8_t X) 
{
  switch (X)
  {
    case 1:
      return color111to565(r, g, b);
    case 2:
      return color222to565(r, g, b);
    case 3: 
      return color333to565(r, g, b);
    case 4: 
      return color444to565(r, g, b);
    case 5: 
      return color555to565(r, g, b);     
    case 6:
      return color666to565(r, g, b); 
    case 7:
      return color777to565(r, g, b); 
  }
  return color888to565(r, g, b); 
}

// Converts packet ColorXXX to RXGXBX
void colorXXXtoRXGXBX(uint16_t colorXXX, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t X)
{
  if (X > 5) 
  {
    color565to888(colorXXX, r, b, g);
  }else 
  {
    uint8_t m = mask_x[X];
    b = colorXXX & m;
    colorXXX >>= X;     
    g = colorXXX & m;
    colorXXX >>= X;
    r = colorXXX & m;
  }  
}

// Converts ColorXXX to RXGXBX
uint16_t colorRXGXBXtoXXX(uint8_t r, uint8_t g, uint8_t b, uint8_t X)
{
  uint8_t m = mask_x[X];
  r &= m; g &= m; b &= m;
  if (X > 5)
  {
    X = 8-X;
    r <<= X; 
    g <<= X;
    b <<= X;
    return colorXXXto565(r,g,b,X);
  }  
  return ((r << X)<<X)|(g << X)|(b << X);
}

