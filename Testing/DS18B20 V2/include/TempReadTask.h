#ifndef TEMPREADTASK_H
#define TEMPREADTASK_H

#include <DallasTemperature.h>
#include <freertos/semphr.h>

#define ONE_WIRE_BUS  16  // Pin for DS18B20 sensors

extern String temperatureReadings;
extern String sensorAddresses;
extern SemaphoreHandle_t bufferMutex;
extern DallasTemperature sensors;  // Declare the sensors variable as extern

void createTempReadTask();

#endif
