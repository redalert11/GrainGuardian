#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "TempReadTask.h"

// Function to convert device address to string
String getAddressString(DeviceAddress deviceAddress) {
  String address = "";
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) address += "0";
    address += String(deviceAddress[i], HEX);
  }
  return address;
}

// Temperature reading task
void readTemperature(void *parameter) {
  for(;;) {
    // Request temperatures from all sensors
    sensors.requestTemperatures();
    vTaskDelay(100 / portTICK_PERIOD_MS); // Delay to ensure accurate readings

    // Update buffer with new temperature readings
    xSemaphoreTake(bufferMutex, portMAX_DELAY);
    temperatureReadings = "";
    sensorAddresses = "";
    int deviceCount = sensors.getDeviceCount();
    for (int i = 0; i < deviceCount; i++) {
      DeviceAddress deviceAddress;
      sensors.getAddress(deviceAddress, i);
      String addressString = getAddressString(deviceAddress);
      float temperature = sensors.getTempC(deviceAddress);
      sensorAddresses += "Sensor " + addressString + "<br>";
      if (temperature == DEVICE_DISCONNECTED_C) {
        temperatureReadings += "Sensor " + addressString + ": Disconnected<br>";
      } else {
        temperatureReadings += "Sensor " + addressString + ": " + String(temperature) + " C<br>";
      }
    }
    xSemaphoreGive(bufferMutex);

    vTaskDelay(2000 / portTICK_PERIOD_MS);  // Update every 2 seconds
  }
}

void createTempReadTask() {
  // Create the temperature reading task
  xTaskCreate(
    readTemperature, // Task function
    "Read Temp",     // Name of the task
    4000,            // Stack size in words
    NULL,            // Task input parameter
    1,               // Priority of the task
    NULL             // Task handle
  );
}
