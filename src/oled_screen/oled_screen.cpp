#include <Arduino.h>
#include "oled_screen.h"
#include "..\src\definitions.h"

void Oled_Screen::setup(){
    oled_display.begin();
    oled_display.fillScreen(BLACK);
    oled_display.setCursor(0,0);
    oled_display.println("System Starting!");
    delay(3000);
}

void Oled_Screen::display_text(String text){
    oled_display.fillScreen(BLACK);
    oled_display.setCursor(0,0);
    oled_display.println(text.c_str());
}

void Oled_Screen::display_text(String text, int x, int y){
    oled_display.fillScreen(BLACK);
    oled_display.setCursor(x,y);
    oled_display.println(text.c_str());
}