#include <Arduino.h>
#include "oled_screen.h"

Oled_Screen::Oled_Screen(){
    this->oled_display = Adafruit_SSD1331(7, 18, 15, 16, 17);
}

Oled_Screen::Oled_Screen(int cs, int dc, int rst, int sclk, int mosi){
    this->oled_display = Adafruit_SSD1331(cs, dc, rst, sclk, mosi);
}



void Oled_Screen::display_text(String text){
  this->oled_display.fillScreen(BLACK);
  this->oled_display.setCursor(0,0);
  this->oled_display.printf(text.c_str());
}