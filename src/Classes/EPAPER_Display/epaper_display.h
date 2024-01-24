#ifndef epaper_display_h
#define epaper_display_h
#include <Arduino.h>
// code goes here
class Epaper_display {
    public:
        Epaper_display();
        uint16_t getColor(int index);
        void display_image();
        
    private:
        int epaper_display_pins[4];
        unsigned long epaper_display_time;  
        unsigned long last_epaper_display_time; 
        bool display_image_flag;
        bool changed_image_flag;
}
#endif