#ifndef oled_screen_h
#define oled_screen_h
#include <Arduino.h>
#include <Adafruit_SSD1331.h>

class Oled_Screen {
    public:
        Oled_Screen();
        void setup();
        void display_text(String text);
    private:
        Adafruit_SSD1331 oled_display; // Oled Screen
};

#endif