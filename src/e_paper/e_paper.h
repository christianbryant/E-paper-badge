#ifndef e_paper_h
#define e_paper_h
#include <Arduino.h>
#include <GxEPD2_7C.h>
#include "../.pio/libdeps/esp32-s3-devkitm-1/GxEPD2/src/bitmaps/Bitmaps7c800x480.h"

class E_Paper{
    public:
        E_Paper() : epaper_display(GxEPD2_565c(/*CS=5*/ 10, /*DC=*/ 13, /*RST=*/ 3, /*BUSY=*/ 14)) {}
        void setup();
        void display_image();
        uint16_t getColor(int index);
        
    private:
        GxEPD2_7C < GxEPD2_565c, GxEPD2_565c::HEIGHT / 2 > epaper_display; // Waveshare 5.65" 7-color
};


#endif