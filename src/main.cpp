
#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Arduino.h>
#include <Adafruit_SSD1331.h>
#include <Adafruit_GFX.h>
#include <Adafruit_INA219.h>
#include <SPI.h>
#include <SD_MMC.h>
#include <FS.h>

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
RTC_DATA_ATTR int curr_image;
RTC_DATA_ATTR unsigned long last_sleep_time;
RTC_DATA_ATTR int total_images;
RTC_DATA_ATTR String image_names[50];

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
  oled_display.println("System Starting!");
  delay(3000);
}


void display_image(){
  display.setRotation(0);
  display.setFullWindow();
  display.firstPage();
  int width = 600;
  int height = 448;
  is_displayed = false;
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(0,0);
    for (int i = 0; i < 7; i++){
      display.drawBitmap(0,0, color_arrays[i], width, height, getColor(i));
    }
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

void process_file(File image_file){
  String file_content = "";
  int curr_array = 0;
  int arrayIndex = 0;
  int arr_item = 0;
  bool end_of_array = false;
  while (image_file.available() && curr_array < 7) {
    file_content = image_file.readStringUntil('\n');
    end_of_array = false;
    if(file_content.indexOf("const unsigned char") != -1){
      arrayIndex = 0;
      while(end_of_array == false){
        file_content = image_file.readStringUntil('\n');
        if(file_content.indexOf("}") != -1){
          curr_array++;
          end_of_array = true;
        } else {
          arr_item = 0;
          char* arrayValue = strtok((char*)file_content.c_str(), ", ");
          if (strstr(arrayValue, "0x") != NULL){
            while (arrayValue != NULL && arr_item < 16) {
              if (arrayValue[0] == ' '){
                arrayValue = arrayValue + 1;
              }
              char arrVal[3] = {arrayValue[2], arrayValue[3], '\0'};
              color_arrays[curr_array][arrayIndex++] = (unsigned char)strtol(arrVal, NULL, 16);
              arr_item++;
              arrayValue = strtok(NULL, ",");
            }
          }
        }
      }
    }
  }
}

void setup_color_arrays(fs::FS &fs){
  if (total_images == 0){
    Serial.println("No images found!");
    return;
  }
  File image_file = fs.open("/Images/" + image_names[curr_image] + "/" + image_names[curr_image] + ".c", FILE_READ);
  if(!image_file){
    Serial.println("Failed to open file for reading");
    return;
  }
  process_file(image_file);
  image_file.close();
}

void sd_setup(){
  int sd_card_pins[6] = {38, 40, 4, 5, 1, 2};
  if(!SD_MMC.setPins(38, 40, 4, 5, 1, 2)) {
    Serial.println("Error setting SD_MMC pins");
    return;
  }
  if(!SD_MMC.begin("/sdcard", false, false, 20000, 5)){
    oled_display.fillScreen(BLACK);
    oled_display.setCursor(0,0);
    oled_display.printf("Card Mount Failed\n");
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

void setup()
{
  display.init(115200); // default 10ms reset pulse, e.g. for bare panels with DESPI-C02
  oled_setup();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_6, 0);
  Serial.begin(115200);
  setup_buttons();
  display.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
  is_displayed = false;
  if(digitalRead(39) == HIGH){
    sd_setup();
  } else {
    oled_display.fillScreen(BLACK);
    oled_display.setCursor(0,0);
    oled_display.printf("No SD Card\n");
    while (digitalRead(39) == LOW){
      delay(1000);
      Serial.println("No SD Card");
    }
    oled_display.fillScreen(BLACK);
    oled_display.setCursor(0,0);
    oled_display.printf("SD Card inserted!\n");
    sd_setup();
  }
  read_image_array(SD_MMC, "/Images/Image_array.txt");
  allocate_color_arrays();
  if (first_boot == true){
    curr_image = 0;
    first_boot = false;
    setup_color_arrays(SD_MMC);
    oled_display.fillScreen(BLACK);
    oled_display.setCursor(0,0);
    oled_display.printf("Changing Image to: %s\n", image_names[curr_image].c_str());
    display_image();
    display.hibernate();
  }
  last_button_time = millis();
  oled_display.fillScreen(BLACK);
  oled_display.setCursor(0,0);
  oled_display.printf("Current Image is: %s\n", image_names[curr_image].c_str());
  last_sleep_time = millis();
}

void loop() {
  unsigned long curr_time = millis();
  if(curr_time - last_sleep_time <= 100000){
    if(changed_image_flag && !display_image_flag){
      oled_display.fillScreen(BLACK);
      oled_display.setCursor(0,0);
      oled_display.printf("Change image to: %s\n", image_names[curr_image].c_str());
      changed_image_flag = false;
      delay(100);
    } 
    else if (!changed_image_flag && display_image_flag){
      oled_display.fillScreen(BLACK);
      oled_display.setCursor(0,0);
      oled_display.printf("Changing Image to: %s\n", image_names[curr_image].c_str());
      setup_color_arrays(SD_MMC);
      display_image();
      display.hibernate();
      oled_display.fillScreen(BLACK);
      oled_display.setCursor(0,0);
      oled_display.printf("Current Image is: %s\n", image_names[curr_image].c_str());
      display_image_flag = false;
      changed_image_flag = false;
    }
  } else {
    oled_display.fillScreen(BLACK);
    oled_display.setCursor(0,0);
    oled_display.println("Going to sleep!");
    delay(5000);
    last_sleep_time = millis();
    esp_deep_sleep_start();
  }
};
