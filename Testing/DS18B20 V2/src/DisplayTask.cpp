#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "DisplayTask.h"

// Display temperature task
void displayTemperature(void *parameter) {
  for(;;) {
    // Clear the display buffer
    display.clearDisplay();
    display.setCursor(0, 0);

    // Read from the buffer
    xSemaphoreTake(bufferMutex, portMAX_DELAY);
    String localTemperatureReadings = temperatureReadings;
    xSemaphoreGive(bufferMutex);

    // Print the number of DS18B20 sensors
    int deviceCount = sensors.getDeviceCount();
    display.print("DS18B20 Sensors: ");
    display.println(deviceCount);
    
    // Iterate through each sensor and print its temperature
    int startIndex = 0;
    for (int i = 0; i < deviceCount; i++) {
      int endIndex = localTemperatureReadings.indexOf("<br>", startIndex);
      String line = localTemperatureReadings.substring(startIndex, endIndex);
      int separatorIndex = line.indexOf(": ");
      String temperatureString = line.substring(separatorIndex + 2);
      display.print("Sensor ");
      display.print(i + 1);
      display.print(": ");
      display.println(temperatureString);
      startIndex = endIndex + 4;
    }

    // Display the temperature readings
    display.display();
    
    // Wait 2 seconds before the next update
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void createDisplayTask() {
  // Create the temperature display task
  xTaskCreate(
    displayTemperature, // Task function
    "Display Temp",     // Name of the task
    4000,               // Stack size in words
    NULL,               // Task input parameter
    1,                  // Priority of the task
    NULL                // Task handle
  );
}
