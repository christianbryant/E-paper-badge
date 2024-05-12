#include <Arduino.h>
#include "buttons.h"

Buttons::Buttons(){
    this->buttons[0] = 6;
    this->buttons[1] = 21;
    this->buttons[2] = 48;
    this->buttons[3] = 47;
    this->button_time = 0;
    this->last_button_time = 0;
    this->display_image_flag = false;
    this->changed_image_flag = false;
    this->setup_buttons();
}
Buttons::Buttons(int button_0, int button_1, int button_2, int button_3, ){
    this->buttons[0] = button_0;
    this->buttons[1] = button_1;
    this->buttons[2] = button_2;
    this->buttons[3] = button_3;
    this->button_time = 0;
    this->last_button_time = 0;
    this->display_image_flag = false;
    this->changed_image_flag = false;
    last_sleep_time = 0;
    this->setup_buttons();
}

void button_0(){
    button_time = millis();
    if (button_time - last_button_time >= 500)
    {
        last_button_time = millis();
        last_sleep_time = millis();
        display_image_flag = true;
    }
}

void button_1(){
  button_time = millis();
  if (button_time - last_button_time >= 500)
    {
      last_button_time = millis();
      last_sleep_time = millis();
      neg_change_img();
    }
}

void button_2(){
  button_time = millis();
    if (button_time - last_button_time >= 500)
    {
      last_button_time = millis();
      last_sleep_time = millis();
      pos_change_img();
    }
}

void button_3(){
  button_time = millis();
  if (button_time - last_button_time >= 500)
    {
      last_button_time = millis();
      last_sleep_time = millis();
      display_image_flag = true;
    }
}

void setup_buttons(){
  for (int i = 0; i < 4; i++){
    pinMode(buttons[i], INPUT_PULLUP);
  }
  attachInterrupt(buttons[0], button_0, FALLING);
  attachInterrupt(buttons[1], button_1, FALLING);
  attachInterrupt(buttons[2], button_2, FALLING);
  attachInterrupt(buttons[3], button_3, FALLING);
}

void Buttons::pos_change_img(){
  this->changed_image_flag = true;
  if(this->curr_image == this->total_images - 1){
    this->curr_image = 0;
  } else {
    this->curr_image += 1;
  }
}

void Buttons::neg_change_img(){
  this->changed_image_flag = true;
  if(this->curr_image == 0){
    this->curr_image = this->total_images - 1;
  } else {
    this->curr_image -= 1;
  }
}

unsigned long get_last_sleep_time(){
    return this->last_sleep_time;
}

void set_last_sleep_time(unsigned long time){
    this->last_sleep_time = time;
}

bool get_display_image_flag(){
    return this->display_image_flag;
}
void set_display_image_flag(bool flag){
    this->display_image_flag = flag;
}

bool get_changed_image_flag(){
    return this->changed_image_flag;
}
void set_changed_image_flag(bool flag){
    this->changed_image_flag = flag;
}

int *get_buttons(){
    return this->buttons;
}