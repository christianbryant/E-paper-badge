#include <Arduino.h>
#include "e_paper.h"
#include "..\src\definitions.h"



void E_Paper::setup(){
  epaper_display.init(115200); // default 10ms reset pulse, e.g. for bare panels with DESPI-C02
  epaper_display.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
  is_displayed = false;
}


void E_Paper::display_image(){
  epaper_display.setRotation(0);
  epaper_display.setFullWindow();
  epaper_display.firstPage();
  int width = 600;
  int height = 448;
  is_displayed = false;
  do
  {
    epaper_display.fillScreen(GxEPD_WHITE);
    epaper_display.setCursor(0,0);
    for (int i = 0; i < 7; i++){
      epaper_display.drawBitmap(0,0, color_arrays[i], width, height, getColor(i));
    }
  }
  while (epaper_display.nextPage());
  is_displayed = true;
  Serial.println("Image has been displayed!");
}

uint16_t E_Paper::getColor(int index) {
  switch (index) {
    case 0: return GxEPD_BLACK;
    case 1: return GxEPD_WHITE;
    case 2: return GxEPD_GREEN;
    case 3: return GxEPD_BLUE;
    case 4: return GxEPD_RED;
    case 5: return GxEPD_YELLOW;
    case 6: return GxEPD_ORANGE;
    default: return GxEPD_BLACK;
  }
}