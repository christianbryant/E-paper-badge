#ifndef BUTTONS_H
#define BUTTONS_H
#include <Arduino.h>
#include "..\src\definitions.h"

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

#endif