#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <DallasTemperature.h>
#include "WebServerTask.h"
#include <SPIFFS.h>


// Web server instance
AsyncWebServer server(80);

// Function to get the uptime
String getUptime() {
  uint64_t uptimeMillis = esp_timer_get_time() / 1000;
  uint32_t seconds = (uptimeMillis / 1000) % 60;
  uint32_t minutes = (uptimeMillis / (1000 * 60)) % 60;
  uint32_t hours = (uptimeMillis / (1000 * 60 * 60)) % 24;
  uint32_t days = uptimeMillis / (1000 * 60 * 60 * 24);
  return String(days) + "d " + String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s";
}

// Function to get sensor address as string
String getAddressString(const uint8_t* deviceAddress) {
  String address = "";
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) address += "0";
    address += String(deviceAddress[i], HEX);
  }
  return address;
}

// Web server task
void webServerTask(void *parameter) {
  // Serve the index.html file from SPIFFS
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // API endpoint to get sensor data
  server.on("/api/data", HTTP_GET, [](AsyncWebServerRequest *request){
    xSemaphoreTake(bufferMutex, portMAX_DELAY);
    String localSensorAddresses = sensorAddresses;
    String localTemperatureReadings = temperatureReadings;
    int deviceCount = sensors.getDeviceCount();
    xSemaphoreGive(bufferMutex);

    // Get usage statistics
    String uptime = getUptime();
    uint32_t freeHeap = ESP.getFreeHeap();
    uint32_t maxAllocHeap = ESP.getMaxAllocHeap();
    uint32_t minFreeHeap = ESP.getMinFreeHeap();
    uint32_t psramFree = ESP.getFreePsram();
    uint32_t psramSize = ESP.getPsramSize();
    uint32_t psramUsed = psramSize - psramFree;
    uint32_t flashSize = ESP.getFlashChipSize();
    uint32_t flashUsed = flashSize - ESP.getFreeSketchSpace();

    // Create a JSON document
    DynamicJsonDocument jsonDoc(2048); // Increase size for larger JSON
    jsonDoc["uptime"] = uptime;
    jsonDoc["freeHeap"] = freeHeap;
    jsonDoc["maxAllocHeap"] = maxAllocHeap;
    jsonDoc["minFreeHeap"] = minFreeHeap;
    jsonDoc["psramUsed"] = psramUsed;
    jsonDoc["psramFree"] = psramFree;
    jsonDoc["psramSize"] = psramSize;
    jsonDoc["flashUsed"] = flashUsed;
    jsonDoc["flashFree"] = ESP.getFreeSketchSpace();
    jsonDoc["flashSize"] = flashSize;

    JsonArray sensorsArray = jsonDoc.createNestedArray("sensors");
    for (int i = 0; i < deviceCount; i++) {
      DeviceAddress deviceAddress;
      sensors.getAddress(deviceAddress, i);
      String addressString = getAddressString(deviceAddress);
      float temperature = sensors.getTempC(deviceAddress);
      
      JsonObject sensor = sensorsArray.createNestedObject();
      sensor["address"] = addressString;
      sensor["temperature"] = String(temperature);
    }

    String jsonString;
    serializeJson(jsonDoc, jsonString);

    request->send(200, "application/json", jsonString);
  });

  // Start server
  server.begin();

  for(;;) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay to keep the task alive
  }
}


void createWebServerTask() {
  // Create the web server task
  xTaskCreate(
    webServerTask,    // Task function
    "Web Server",     // Name of the task
    4000,             // Stack size in words
    NULL,             // Task input parameter
    1,                // Priority of the task
    NULL              // Task handle
  );
}
