#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <Adafruit_INA219.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>

int batteryPercentage;

Adafruit_INA219 ina219;

float busvoltage = 0;
float current_mA = 0;
float power_mW = 0;

struct timeval lastBatteryUpdate;

float const R1 = 30000.0;
float const R2 = 7500.0;
float const MAX_BATTERY_VOLTAGE = 4.2;
float const MIN_BATTERY_VOLTAGE = 3.3;

Adafruit_SDD1331 display = Adafruit_SSD1331(cs, dc, rst);

// put function declarations here:

void measurePower(){
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();

  Serial.printf("Voltage: %.4f\n", busvoltage);

  Serial.printf("Current_mA: %.4f\n", current_mA);
  Serial.printf("Power(mW): %.4f\n", power_mW);

}

bool is_charging() {
  float prev ;
  delay(1000);
  float curr;
  return curr > prev;
}

void display_image(const char *image_name) {
  
}


void setup() {
  Serial.begin(115200);
  Serial.println("ESP Starting");
  delay(5000);
  Serial.println("Leaving setup");
  batteryPercentage = 100;

  if(!ina219.begin()){
    Serial.println("FAILED TO FIND INA219 MODULE!");
  }
  
  
}

void loop() {
  Serial.printf("Starting to loop using the INA219!\n");
  measurePower();

  delay(5000);
}