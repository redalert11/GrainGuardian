#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "WebServerTask.h"
#include <SPIFFS.h>
#include <esp_partition.h>

// Web server instance
AsyncWebServer server(80);

// Function to get partition information
String getPartitionInfo() {
  String info = "";
  esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
  while (it != NULL) {
    const esp_partition_t* partition = esp_partition_get(it);
    info += String(partition->label) + ":" + String(partition->size) + ":" +
            String((partition->size * 100.0) / (ESP.getFlashChipSize()), 1) + ",";
    it = esp_partition_next(it);
  }
  esp_partition_iterator_release(it);
  return info;
}

// Function to get SPIFFS file information
void getSPIFFSInfo(JsonArray& filesArray) {
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while (file) {
    JsonObject fileObj = filesArray.createNestedObject();
    fileObj["name"] = String(file.name());
    fileObj["size"] = file.size();
    file = root.openNextFile();
  }
}

// Web server task
void webServerTask(void *parameter) {
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Serve the index.html file from SPIFFS
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Serve the CSS files
  server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap.min.css", "text/css");
  });

  server.on("/dashboard.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/dashboard.css", "text/css");
  });

  server.on("/bootstrap-icons.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap-icons.min.css", "text/css");
  });

  // Serve the JavaScript files
  server.on("/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap.bundle.min.js", "application/javascript");
  });

  server.on("/chart.umd.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/chart.umd.js", "application/javascript");
  });

  server.on("/dashboard.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/dashboard.js", "application/javascript");
  });

  server.on("/color-modes.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/color-modes.js", "application/javascript");
  });

  // API endpoint to get partition and SPIFFS info
  server.on("/api/system-info", HTTP_GET, [](AsyncWebServerRequest *request){
    // Get partition information
    String partitionInfo = getPartitionInfo();

    // Create a JSON document
    DynamicJsonDocument jsonDoc(4096);

    // Add partition information
    JsonArray partitionsArray = jsonDoc.createNestedArray("partitions");
    int startIndex = 0;
    int endIndex = partitionInfo.indexOf(',');
    while (endIndex != -1) {
      String partitionData = partitionInfo.substring(startIndex, endIndex);
      int separatorIndex1 = partitionData.indexOf(':');
      int separatorIndex2 = partitionData.lastIndexOf(':');
      if (separatorIndex1 != -1 && separatorIndex2 != -1 && separatorIndex1 != separatorIndex2) {
        JsonObject partition = partitionsArray.createNestedObject();
        partition["label"] = partitionData.substring(0, separatorIndex1);
        partition["size"] = partitionData.substring(separatorIndex1 + 1, separatorIndex2).toInt();
        partition["percentage"] = partitionData.substring(separatorIndex2 + 1).toFloat();
      }
      startIndex = endIndex + 1;
      endIndex = partitionInfo.indexOf(',', startIndex);
    }

    // Add SPIFFS information
    JsonArray filesArray = jsonDoc.createNestedArray("spiffs_files");
    getSPIFFSInfo(filesArray);

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
