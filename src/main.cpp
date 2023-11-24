// GxEPD2_HelloWorld.ino by Jean-Marc Zingg

// see GxEPD2_wiring_examples.h for wiring suggestions and examples
// if you use a different wiring, you need to adapt the constructor parameters!

// uncomment next line to use class GFX of library GFX_Root instead of Adafruit_GFX
//#include <GFX.h>

// #include <GxEPD2_BW.h>
// #include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Arduino.h>
// #include "Images/Jax_Fullbody/black.c"
// #include "Images/Jax_Fullbody/blue.c"
// #include "Images/Jax_Fullbody/green.c"
// #include "Images/Jax_Fullbody/orange.c"
// #include "Images/Jax_Fullbody/red.c"
// #include "Images/Jax_Fullbody/white.c"
// #include "Images/Jax_Fullbody/yellow.c"
#include "Images/Jax_Icon/black.c"
#include "Images/Jax_Icon/blue.c"
#include "Images/Jax_Icon/green.c"
#include "Images/Jax_Icon/orange.c"
#include "Images/Jax_Icon/red.c"
#include "Images/Jax_Icon/white.c"
#include "Images/Jax_Icon/yellow.c"
#include "../.pio/libdeps/esp32doit-devkit-v1/GxEPD2/src/bitmaps/Bitmaps7c800x480.h"


// // select the display class and display driver class in the following file (new style):
// #include "GxEPD2_display_selection_new_style.h"

// // or select the display constructor line in one of the following files (old style):
// #include "GxEPD2_display_selection.h"
// #include "GxEPD2_display_selection_added.h"

// // alternately you can copy the constructor from GxEPD2_display_selection.h or GxEPD2_display_selection_added.h to here
// // e.g. for Wemos D1 mini:
GxEPD2_7C < GxEPD2_565c, GxEPD2_565c::HEIGHT / 2 > display(GxEPD2_565c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // Waveshare 5.65" 7-color

void helloWorld();


void display_image1(){
  display.setRotation(0);
  // display.setFont(&FreeMonoBold9pt7b);
  // display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
  display.firstPage();
  int width = 448;
  int height = 448;
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(0,0);
    display.drawBitmap(0,0, black, width, height, GxEPD_BLACK);
    display.drawBitmap(0,0, blue, width, height, GxEPD_BLUE);
    display.drawBitmap(0,0, green, width, height, GxEPD_GREEN);
    display.drawBitmap(0,0, orange, width, height, GxEPD_ORANGE);
    display.drawBitmap(0,0, red, width, height, GxEPD_RED);
    display.drawBitmap(0,0, white, width, height, GxEPD_WHITE);
    display.drawBitmap(0,0, yellow, width, height, GxEPD_YELLOW);
  }
  while (display.nextPage());
}

void setup()
{
  display.init(115200); // default 10ms reset pulse, e.g. for bare panels with DESPI-C02
  Serial.begin(115200);
  display.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
  display_image1();
  display.hibernate();
}

const char HelloWorld[] = "Hello World!";

void loop() {};
