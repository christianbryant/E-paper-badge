#ifndef buttons_h
#define buttons_h
#include <Arduino.h>
class Buttons {
    public:
        Buttons();
        Buttons(int button_0, int button_1, int button_2, int button_3);

        unsigned long get_last_sleep_time();
        void set_last_sleep_time(unsigned long time);

        bool get_display_image_flag();
        void set_display_image_flag(bool flag);
        
        bool get_changed_image_flag();
        void set_changed_image_flag(bool flag);

        int *get_buttons();
    private:
        int buttons[4];
        unsigned long button_time;  
        unsigned long last_button_time; 
        bool display_image_flag;
        bool changed_image_flag;

};

void setup_buttons();
void button_0();
void button_1();
void button_2();
void button_3();
void neg_change_img();
void pos_change_img();
#endif