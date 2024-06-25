#ifndef WEBSERVERTASK_H
#define WEBSERVERTASK_H

#include <freertos/semphr.h>
#include <DallasTemperature.h>

extern String temperatureReadings;
extern String sensorAddresses;
extern SemaphoreHandle_t bufferMutex;
extern DallasTemperature sensors;  // Declare the sensors variable as extern

void createWebServerTask();

#endif
