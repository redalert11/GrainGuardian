#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

const char* ssid = "Nacho Internet";
const char* password = "redalert";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Serve the SPIFFS usage information as JSON
  server.on("/spiffsinfo", HTTP_GET, [](AsyncWebServerRequest *request){
    size_t totalBytes = SPIFFS.totalBytes();
    size_t usedBytes = SPIFFS.usedBytes();

    String json = "{\"total\":" + String(totalBytes) + ", \"used\":" + String(usedBytes) + "}";
    request->send(200, "application/json", json);
  });

  // Serve the index.html file
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  // Start the server
  server.begin();
}

void loop() {
  // Your code here
}
