#ifndef buttons_h
#define buttons_h
#include <Arduino.h>
class Buttons {
    public:
        Buttons();
        Buttons(int button_0, int button_1, int button_2, int button_3);
        void button_0();
        void button_1();
        void button_2();
        void button_3();
        void neg_change_img();
        void pos_change_img();
    private:
        int buttons[4];
        unsigned long button_time;  
        unsigned long last_button_time; 
        bool display_image_flag;
        bool changed_image_flag;
}
// code goes here
#endif