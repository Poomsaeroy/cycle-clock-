#ifndef GRAPHIC16X16_H_
#define GRAPHIC16X16_H_
#include "Arduino.h"
class Graphic16x16{
  private:
    uint32_t BACKGROUND[256];
  public:
    uint32_t MAIN_FRAME[256];
    void setup();
    uint8_t XY(uint8_t x, uint8_t y);
    void setBackground(uint32_t const *background);
    void setBackground(uint32_t color);
    void draw(uint32_t const *object, int8_t x, int8_t y);
    void draw(uint32_t const *object, uint8_t r, int8_t c, int8_t x, int8_t y);
    void draw(uint32_t color, int8_t x, int8_t y);
    void drawWithColor(uint8_t const *object, uint32_t color, uint8_t r, int8_t c, int8_t x, int8_t y);
    void display();
    void clear();
    void setBrightness(uint8_t bright);
};

#endif