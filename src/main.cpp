
#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Arduino.h>

#include "Images/Image_array.h"

#include "../.pio/libdeps/esp32doit-devkit-v1/GxEPD2/src/bitmaps/Bitmaps7c800x480.h"

// // alternately you can copy the constructor from GxEPD2_display_selection.h or GxEPD2_display_selection_added.h to here
// // e.g. for Wemos D1 mini:
GxEPD2_7C < GxEPD2_565c, GxEPD2_565c::HEIGHT / 2 > display(GxEPD2_565c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // Waveshare 5.65" 7-color

int curr_image;
bool is_displayed;


void display_image(){
  display.setRotation(0);
  display.setFullWindow();
  display.firstPage();
  int width = images[curr_image].width;
  int height = images[curr_image].height;
  is_displayed = false;
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(0,0);
    display.drawBitmap(0,0, images[curr_image].ColorArray[0], width, height, GxEPD_BLACK);
    display.drawBitmap(0,0, images[curr_image].ColorArray[1], width, height, GxEPD_WHITE);
    display.drawBitmap(0,0, images[curr_image].ColorArray[2], width, height, GxEPD_GREEN);
    display.drawBitmap(0,0, images[curr_image].ColorArray[3], width, height, GxEPD_BLUE);
    display.drawBitmap(0,0, images[curr_image].ColorArray[4], width, height, GxEPD_RED);
    display.drawBitmap(0,0, images[curr_image].ColorArray[5], width, height, GxEPD_YELLOW);
    display.drawBitmap(0,0, images[curr_image].ColorArray[6], width, height, GxEPD_ORANGE);
  }
  while (display.nextPage());
  is_displayed = true;
  Serial.println("Image has been displayed!");
}

void testing_buttons(){
  int current_state = digitalRead(32);
  int current_state2 = digitalRead(33);
  int current_state3 = digitalRead(25);
  int current_state4 = digitalRead(26);
  Serial.printf("Current State: %d\n", current_state);
  Serial.printf("Current State2: %d\n", current_state2);
  Serial.printf("Current State3: %d\n", current_state3);
  Serial.printf("Current State4: %d\n", current_state4);

}

void setup()
{
  display.init(115200); // default 10ms reset pulse, e.g. for bare panels with DESPI-C02
  Serial.begin(115200);
  pinMode(32, INPUT_PULLUP);
  pinMode(33, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);
  display.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
  curr_image = 3;
  Serial.printf("Current Image is: %s\n", images[curr_image].name);
  is_displayed = false;
  display_image();
  display.hibernate();
  delay(5000);
}

void loop() {
  Serial.printf("Current Image is: %s\n", images[curr_image].name);
  display_image();
  display.hibernate();
  if(curr_image == 3){
    curr_image = 0;
  } else {
    curr_image += 1;
  }
  delay(2000);
};
