#include "graphic16x16.h"     
#include "FastLED.h" 

#define NUM_LEDS 256
#define DATA_PIN 14
CRGB leds[NUM_LEDS];

void Graphic16x16::setup(){
  FastLED.addLeds<WS2812B, DATA_PIN, GBR>(leds, NUM_LEDS);
  FastLED.setBrightness(20);
}

void Graphic16x16::display(){
  FastLED.clear();
  for(int i = 0; i < 16; i++) {
    for(int j = 0; j < 16; j++){
      leds[XY(j, i)] = MAIN_FRAME[XY(j, i)];
    }
  }
  FastLED.show(); 
}

uint8_t Graphic16x16::XY(uint8_t x, uint8_t y) {
  const uint8_t XYTable[] = {
   255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240,
   224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
   223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208,
   192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
   191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176,
   160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
   159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144,
   128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
   127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112,
    96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    95,  94,  93,  92,  91,  90,  89,  88,  87,  86,  85,  84,  83,  82,  81,  80,
    64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
    63,  62,  61,  60,  59,  58,  57,  56,  55,  54,  53,  52,  51,  50,  49,  48,
    32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
    31,  30,  29,  28,  27,  26,  25,  24,  23,  22,  21,  20,  19,  18,  17,  16,
     0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15
  };

  uint8_t i = (y * 16) + x;
  uint8_t j = XYTable[i];
  return j;
}

void Graphic16x16::setBackground(uint32_t const *background){
  int count = 0;
  for(int i = 0; i < 16; i++) {
    for(int j = 0; j < 16; j++){
      BACKGROUND[XY(j, i)] = background ? background[count] : 0;
      MAIN_FRAME[XY(j, i)] = BACKGROUND[XY(j, i)];
      count++;
    }
  }
}

void Graphic16x16::setBackground(uint32_t color){
  int count = 0;
  for(int i = 0; i < 16; i++) {
    for(int j = 0; j < 16; j++){
      BACKGROUND[XY(j, i)] = color ? color : 0;
      MAIN_FRAME[XY(j, i)] = BACKGROUND[XY(j, i)];
      count++;
    }
  }
}

void Graphic16x16::draw(uint32_t const *object, int8_t x, int8_t y){
  int count = 0;
  for(int i = 0; i < 16; i++) {
    for(int j = 0; j < 16; j++){
      if(object[count] && ((j + x) < 16 && (j + x) >= 0) && ((i + y) < 16 && (i + y) >= 0))
        MAIN_FRAME[XY(j + x, i + y)] = object[count];
      count++;
    }
  }
}

void Graphic16x16::draw(uint32_t const *object, uint8_t r, int8_t c, int8_t x, int8_t y){
  int count = 0;
  for(int i = 0; i < r; i++) {
    for(int j = 0; j < c; j++){
      if(object[count] && ((j + x) < 16 && (j + x) >= 0) && ((i + y) < 16 && (i + y) >= 0))
        MAIN_FRAME[XY(j + x, i + y)] = object[count];
      count++;
    }
  }
}

void Graphic16x16::draw(uint32_t color, int8_t x, int8_t y){
  MAIN_FRAME[XY(x, y)] = color;
}

void Graphic16x16::drawWithColor(uint8_t const *object, uint32_t color, uint8_t r, int8_t c, int8_t x, int8_t y){
  int count = 0;
  for(int i = 0; i < r; i++) {
    for(int j = 0; j < c; j++){
      if(object[count])
        MAIN_FRAME[XY(j + x, i + y)] = color;
      count++;
    }
  }
}


void Graphic16x16::clear(){
  int count = 0;
  for(int i = 0; i < 16; i++) {
    for(int j = 0; j < 16; j++){
      MAIN_FRAME[XY(j, i)] = BACKGROUND[XY(j, i)];
      count++;
    }
  }
}

void Graphic16x16::setBrightness(uint8_t bright){
  FastLED.setBrightness(bright);
}