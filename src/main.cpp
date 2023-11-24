#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <SPI.h>

int batteryPercentage;

struct timeval lastBatteryUpdate;

float const R1 = 30000.0;
float const R2 = 7500.0;
float const MAX_BATTERY_VOLTAGE = 4.2;
float const MIN_BATTERY_VOLTAGE = 3.3;

// Data bus definition
Arduino_DataBus *bus = new Arduino_ESP32SPI(12 /* DC */, 15 /* CS */, 14 /* SCK */, 13 /* MOSI */, GFX_NOT_DEFINED /* MISO */, HSPI);

// Graphics handle init
Arduino_GFX *gfx = new Arduino_ST7789(bus, 19 /* RST */, 1 /* rotation */, true /* IPS */);

// put function declarations here:

void drawParticleCircle(int percentage) {
  int centerX = 320 / 2;
  int centerY = 172 / 2;
  int radius = 50;
  
  // Calculate the angle based on the percentage
  float angle = map(percentage, 0, 100, 0, 360);
  angle = angle; 
  
  Serial.printf("Angle: %f\n", angle);
  // Draw the empty circle
  gfx->drawCircle(centerX, centerY, radius+5, WHITE);
  gfx->fillCircle(centerX, centerY, radius+2, BLACK);
  // Draw the filled circle
  // gfx->fillArc(centerX, centerY, radius, 0, 30, 330, GREEN);
  if (percentage == 100){
    gfx->fillCircle(centerX, centerY, radius, GREEN);
  } else if (percentage >= 75 && percentage < 100){
    gfx->fillArc(centerX, centerY, radius, 0, 10, angle, GREEN);
  } else if (percentage >= 50 && percentage < 75){
    gfx->fillArc(centerX, centerY, radius, 0, 10, angle, YELLOW);
  } else if (percentage >= 25 && percentage < 50){
    gfx->fillArc(centerX, centerY, radius, 0, 10, angle, ORANGE);
  } else if (percentage > 0 && percentage < 25){
    gfx->fillArc(centerX, centerY, radius, 0, 10, angle, RED);
  } else if (percentage == 0){
    gfx->fillCircle(centerX, centerY, radius, RED);
  }
}

float battery_voltage() {
  int raw = analogRead(35);
  float voltage = raw * ((float)(R1 + R2)/R2);
  return voltage;
}

int battery_percentage() {
  float voltage = battery_voltage();
  int percentage = map(voltage, MAX_BATTERY_VOLTAGE, MIN_BATTERY_VOLTAGE, 0, 100);
  return percentage;
}

bool is_charging() {
  float prev = battery_voltage();
  delay(1000);
  float curr = battery_voltage();
  return curr > prev;
}

void update_battery(){
  int percentage = battery_percentage();
  if (percentage != batteryPercentage) {
    batteryPercentage = percentage;
    drawParticleCircle(batteryPercentage);
  }
  gettimeofday(&lastBatteryUpdate, NULL);
}

void setup() {
  Serial.begin(115200);
  gfx->begin();
  gfx->setRotation(1);
  gfx->fillScreen(BLACK);
  gfx->fillRect(0, 0, 50, 50, RED);
  Serial.println("Hello World!");
  delay(5000);
  Serial.println("Leaving setup");
  batteryPercentage = 100;
  
  
}

void loop() {
  Serial.printf("Starting the loop of Battery positon, given in percentage: %d\n" , batteryPercentage);

  delay(5000);
}