#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Arduino.h>
#include <Adafruit_SSD1331.h>
#include <Adafruit_GFX.h>
#include <Adafruit_INA219.h>
#include <SPI.h>
#include <SD_MMC.h>
#include <FS.h>

#include "./Classes/Buttons/buttons.h"

#include "../.pio/libdeps/esp32-s3-devkitm-1/GxEPD2/src/bitmaps/Bitmaps7c800x480.h"

// // alternately you can copy the constructor from GxEPD2_display_selection.h or GxEPD2_display_selection_added.h to here
// // e.g. for Wemos D1 mini:
GxEPD2_7C < GxEPD2_565c, GxEPD2_565c::HEIGHT / 2 > epaper_display(GxEPD2_565c(/*CS=5*/ 10, /*DC=*/ 13, /*RST=*/ 3, /*BUSY=*/ 14)); // Waveshare 5.65" 7-color

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

uint16_t getColor(int index) {
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

unsigned char *color_arrays[7];

RTC_DATA_ATTR bool first_boot = true;
RTC_DATA_ATTR String image_names[50];

RTC_DATA_ATTR unsigned long last_sleep_time;
RTC_DATA_ATTR int total_images;
RTC_DATA_ATTR int curr_image;

Buttons buttons;

void allocate_color_arrays(){
  for (int i = 0; i < 7; i++){
    color_arrays[i] = (unsigned char*) ps_malloc(33600);
    for(int j = 0; j < 33600; j++){
      color_arrays[i][j] = 0;
    }
  }
  Serial.println("Memory allocated!");
}

void free_color_arrays(){
  for (int i = 0; i < 7; i++){
    free(color_arrays[i]);
  }
}

bool is_displayed;

float busvoltage = 0;
float current_mA = 0;
float power_mW = 0;

unsigned long lastBatteryUpdate;

float const R1 = 30000.0;
float const R2 = 7500.0;
float const MAX_BATTERY_VOLTAGE = 4.2;
float const MIN_BATTERY_VOLTAGE = 3.3;


void oled_setup() {
  oled_display.begin();
  oled_display.fillScreen(BLACK);
  oled_display.setCursor(0,0);
  oled_display.println("System Starting!");
  delay(3000);
}

void display_text(String text){
  oled_display.fillScreen(BLACK);
  oled_display.setCursor(0,0);
  oled_display.printf(text.c_str());
}

void display_image(){
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

void measurePower(){
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  char buffer[100];
  snprintf(buffer, 100, "Voltage: %.4f\n", busvoltage);
  display_text(buffer);

  oled_display.printf("Current_mA: %.4f\n", current_mA);
  oled_display.printf("Power(mW): %.4f\n", power_mW);

}

bool is_charging() {
  float prev ;
  delay(1000);
  float curr;
  return curr > prev;
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

void read_image_array(fs::FS &fs, const char * path){
    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }
    int curr_char = 0;
    String curr_line = "";
    total_images = 0;
    while(file.available()){
      curr_line = file.readStringUntil(',');
      file.readStringUntil('\n');
      image_names[total_images] = curr_line.c_str();
      total_images += 1;
    }
    Serial.printf("Total Images: %d\n", total_images);
    file.close();
}

void process_file_bin(){
  Serial.println("Processing file!");
  char buffer[200];
  snprintf(buffer, 200, "/sdcard/Images/%s/%s.bin", image_names[curr_image].c_str(), image_names[curr_image].c_str());
  FILE *bin_file = fopen(buffer, "rb");
  if(!bin_file){
    Serial.println("Failed to open file for reading");
    return;
  }
  String file_content = "";
  int curr_array = 0;
  while (curr_array < 7) {
    for(int i = 0; i < 33600; i++){
      if(fread(&color_arrays[curr_array][i], sizeof(unsigned char), 1, bin_file) == 0){
        Serial.println("Error reading file!");
        return;
      }
    }
    curr_array++;
  }
}

void setup_color_arrays_bin(){
  if (total_images == 0){
    Serial.println("No images found!");
    return;
  }
  process_file_bin();
}

void sd_setup(){
  int sd_card_pins[6] = {38, 40, 4, 5, 1, 2};
  if(!SD_MMC.setPins(38, 40, 4, 5, 1, 2)) {
    Serial.println("Error setting SD_MMC pins");
    return;
  }
  if(!SD_MMC.begin("/sdcard", false, false, 20000, 5)){
    display_text("Card Mount Failed\n");
    delay(2000);
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD_MMC card attached");
    return;
  }
}

void first_boot_setup(){
  if (first_boot == true){
    curr_image = 0;
    first_boot = false;
    setup_color_arrays_bin();
    Serial.printf("Before buffer\n");
    char buffer[200];
    snprintf(buffer, 200, "Changing Image to: %s\n", image_names[curr_image].c_str());
    Serial.printf("After buffer\n");
    display_text(buffer);
    display_image();
    epaper_display.hibernate();
  }
}

void setup_button_inturrupts(){
  for (int i = 0; i < 4; i++){
    pinMode(buttons.get_buttons()[i], INPUT_PULLUP);
  }
  attachInterrupt(buttons.get_buttons()[0], buttons.button_0, FALLING);
  attachInterrupt(buttons.get_buttons()[1], buttons.button_0, FALLING);
  attachInterrupt(buttons.get_buttons()[2], buttons.button_0, FALLING);
  attachInterrupt(buttons.get_buttons()[3], buttons.button_0, FALLING);
}

void class_setups(){
  buttons = Buttons();
  setup_button_inturrupts();
}

void ESP_Setup(){
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_6, 0);
  Serial.begin(115200);
}

void epaper_Setup(){
  epaper_display.init(115200); // default 10ms reset pulse, e.g. for bare panels with DESPI-C02
  epaper_display.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
  is_displayed = false;
}

void setup()
{
  oled_setup();
  ESP_Setup();
  class_setups();
  epaper_Setup();
  if(digitalRead(39) == HIGH){
    sd_setup();
  } else {
    display_text("No SD Card dectected!\n");
    while (digitalRead(39) == LOW){
      delay(1000);
      Serial.println("No SD Card");
    }
    display_text("SD Card inserted!\n");
    sd_setup();
  }
  read_image_array(SD_MMC, "/Images/Image_array.txt");
  allocate_color_arrays();
  first_boot_setup();
  char buffer[200];
  snprintf(buffer, 200, "Current Image is: %s\n", image_names[curr_image].c_str());
  display_text(buffer);
}

void loop() {
  unsigned long curr_time = millis();
  char buffer[200];
  if(curr_time - buttons.get_last_sleep_time() <= 100000){
    if(buttons.get_changed_image_flag() && !buttons.get_display_image_flag()){
      snprintf(buffer, 200, "Change image to: %s\n", image_names[curr_image].c_str());
      display_text(buffer);
      buttons.set_changed_image_flag(false);
      delay(100);
    } 
    else if (!buttons.get_changed_image_flag() && buttons.get_display_image_flag()){
      snprintf(buffer, 200, "Changing Image to: %s\n", image_names[curr_image].c_str());
      display_text(buffer);
      setup_color_arrays_bin();
      display_image();
      epaper_display.hibernate();
      snprintf(buffer, 200, "Current Image is: %s\n", image_names[curr_image].c_str());
      display_text(buffer);
      buttons.set_display_image_flag(false);
      buttons.set_changed_image_flag(false);
    }
  } else {
    display_text("Going to sleep!\n");
    free_color_arrays();
    delay(5000);
    buttons.set_last_sleep_time(millis());
    esp_deep_sleep_start();
  }
};
