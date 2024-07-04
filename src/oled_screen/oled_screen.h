#ifndef oled_screen_h
#define oled_screen_h
#include <Arduino.h>
#include <Adafruit_SSD1331.h>
#include <Adafruit_GFX.h>

class Oled_Screen{
    public:
        Oled_Screen() : oled_display(7, 18, 15, 16, 17) {}
        Oled_Screen(int8_t pins[5]) : oled_display(pins[0], pins[1], pins[2], pins[3], pins[4]) {}
        void setup();
        void display_text(String text);
        void display_text(String text, int x, int y);
        
    private:
        Adafruit_SSD1331 oled_display;
};


#endif