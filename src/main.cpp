
#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Arduino.h>
#include <Adafruit_SSD1331.h>
#include <Adafruit_GFX.h>
#include <Adafruit_INA219.h>
#include <SPI.h>
#include <SD_MMC.h>
#include <FS.h>

#include "Images/Image_array.h"

#include "../.pio/libdeps/esp32doit-devkit-v1/GxEPD2/src/bitmaps/Bitmaps7c800x480.h"

// // alternately you can copy the constructor from GxEPD2_display_selection.h or GxEPD2_display_selection_added.h to here
// // e.g. for Wemos D1 mini:
GxEPD2_7C < GxEPD2_565c, GxEPD2_565c::HEIGHT / 2 > display(GxEPD2_565c(/*CS=5*/ 10, /*DC=*/ 13, /*RST=*/ 3, /*BUSY=*/ 14)); // Waveshare 5.65" 7-color

Adafruit_SSD1331 oled_display = Adafruit_SSD1331(7, 18, 15, 16, 17); // Oled Screen

Adafruit_INA219 ina219; // Current Monitor

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

RTC_DATA_ATTR int curr_image;
RTC_DATA_ATTR unsigned long last_sleep_time;


bool is_displayed;

bool display_image_flag;
bool changed_image_flag;

float busvoltage = 0;
float current_mA = 0;
float power_mW = 0;

unsigned long lastBatteryUpdate;

float const R1 = 30000.0;
float const R2 = 7500.0;
float const MAX_BATTERY_VOLTAGE = 4.2;
float const MIN_BATTERY_VOLTAGE = 3.3;

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

int buttons[4] = {6,21,48,47};

void oled_setup() {
  oled_display.begin();
  oled_display.fillScreen(BLACK);
  oled_display.setCursor(0,0);
  oled_display.println("Screen Started!");
  delay(3000);
}


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

void measurePower(){
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  oled_display.fillScreen(BLACK);
  oled_display.setCursor(0,0);
  oled_display.printf("Voltage: %.4f\n", busvoltage);

  oled_display.printf("Current_mA: %.4f\n", current_mA);
  oled_display.printf("Power(mW): %.4f\n", power_mW);

}

bool is_charging() {
  float prev ;
  delay(1000);
  float curr;
  return curr > prev;
}

void pos_change_img(){
  changed_image_flag = true;
  if(curr_image == 3){
    curr_image = 0;
  } else {
    curr_image += 1;
  }
}

void neg_change_img(){
  changed_image_flag = true;
  if(curr_image == 0){
    curr_image = 3;
  } else {
    curr_image -= 1;
  }
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

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
}

void read_image_array(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }
    int curr_char = 0;
    String curr_line = "";
    Serial.print("Read from file: \n");
    int total_images = 0;
    while(file.available()){
      curr_line = file.readStringUntil('\n');
      if(curr_line.startsWith("#")){
        Serial.printf("Found Image: %s\n", curr_line.c_str());
        total_images += 1;
      }
      if(curr_line.startsWith("const struct Images")){
        Serial.println("Found Image Struct");
        while(!curr_line.endsWith("};\n")){
          Serial.printf("Found Image Struct: %s\n", curr_line.c_str());
          curr_line.concat(file.readStringUntil('\n'));
        }
        Serial.printf("Found Image Struct: %s\n", curr_line.c_str());
      }
    }
    Serial.printf("Total Images: %d\n", total_images);
}

void sd_setup(){
  int sd_card_pins[6] = {38, 40, 4, 5, 1, 2};
  if(!SD_MMC.setPins(38, 40, 4, 5, 1, 2)) {
    Serial.println("Error setting SD_MMC pins");
    return;
  }
  delay(1000);
  if(!SD_MMC.begin("/sdcard", false, false, 20000)){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD_MMC card attached");
    return;
  }

  Serial.print("SD_MMC Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);
  listDir(SD_MMC, "/", 0);
  //readFile(SD_MMC, "/Images/Image_array.h");
  read_image_array(SD_MMC, "/Images/Image_array.h");
}

void setup()
{
  display.init(115200); // default 10ms reset pulse, e.g. for bare panels with DESPI-C02
  oled_setup();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_6, 0);
  Serial.begin(115200);
  setup_buttons();
  display.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
  curr_image = 0;
  is_displayed = false;
  if(digitalRead(39) == HIGH){
    sd_setup();
  } else {
    Serial.println("No SD Card");
  }
  last_button_time = millis();
  oled_display.fillScreen(BLACK);
  oled_display.setCursor(0,0);
  oled_display.printf("Current Image is: %s\n", images[curr_image].name);
  last_sleep_time = millis();
}

void loop() {
  unsigned long curr_time = millis();
  // Serial.println(curr_time - last_sleep_time);
  if(curr_time - last_sleep_time <= 50000){
    if(changed_image_flag && !display_image_flag){
      oled_display.fillScreen(BLACK);
      oled_display.setCursor(0,0);
      oled_display.printf("Change image to: %s\n", images[curr_image].name);
      changed_image_flag = false;
      delay(100);
    } 
    else if (!changed_image_flag && display_image_flag){
      oled_display.fillScreen(BLACK);
      oled_display.setCursor(0,0);
      oled_display.printf("Current Image is: %s\n", images[curr_image].name);
      display_image();
      display.hibernate();
      display_image_flag = false;
      changed_image_flag = false;
    }
    // if(curr_time - lastBatteryUpdate <= 2500){
    //   measurePower();
    //   lastBatteryUpdate = curr_time;
    // }
  } else {
    oled_display.fillScreen(BLACK);
    oled_display.setCursor(0,0);
    oled_display.println("Going to sleep!");
    delay(5000);
    last_sleep_time = millis();
    esp_deep_sleep_start();
  }
};
