#include <Arduino.h>
#include <SD_MMC.h>
#include "..\oled_screen\oled_screen.h"


void sd_setup(){
  int sd_card_pins[6] = {38, 40, 4, 5, 1, 2};
  Oled_Screen oled;
  if(!SD_MMC.setPins(38, 40, 4, 5, 1, 2)) {
    Serial.println("Error setting SD_MMC pins");
    return;
  }
  if(!SD_MMC.begin("/sdcard", false, false, 20000, 5)){
    oled.display_text("Card Mount Failed\n");
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