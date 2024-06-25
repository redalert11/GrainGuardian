#ifndef DISPLAYTASK_H
#define DISPLAYTASK_H

#include <Adafruit_SSD1306.h>
#include <freertos/semphr.h>
#include <DallasTemperature.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C  // I2C address for SSD1306
#define SDA_PIN 18
#define SCL_PIN 17

extern String temperatureReadings;
extern SemaphoreHandle_t bufferMutex;
extern Adafruit_SSD1306 display;
extern DallasTemperature sensors;  // Declare the sensors variable as extern

void createDisplayTask();

#endif
