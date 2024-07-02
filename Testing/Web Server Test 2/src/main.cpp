#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <esp_partition.h>

const char* ssid = "Nacho Internet";
const char* password = "redalert";

AsyncWebServer server(80);

String getPartitionInfo() {
  String info = "";
  esp_partition_iterator_t it;

  it = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
  while (it != NULL) {
    const esp_partition_t* partition = esp_partition_get(it);
    info += String(partition->label) + ": " + 
            String(partition->size) + " bytes, " +
            String((partition->size * 100.0) / (ESP.getFlashChipSize()), 1) + "% of total\n";
    it = esp_partition_next(it);
  }
  esp_partition_iterator_release(it);

  return info;
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>ESP32 Partition Info</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial, sans-serif; margin: 0; padding: 20px; }
    h1 { color: #0066cc; }
    #partitionInfo { white-space: pre-wrap; }
  </style>
</head>
<body>
  <h1>ESP32 Partition Information</h1>
  <div id="partitionInfo">Loading...</div>
  <script>
    function loadPartitionInfo() {
      fetch('/partitioninfo')
        .then(response => response.text())
        .then(data => {
          document.getElementById('partitionInfo').innerText = data;
        });
    }
    setInterval(loadPartitionInfo, 5000);
    loadPartitionInfo();
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/partitioninfo", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", getPartitionInfo());
  });

  server.begin();
}

void loop() {
  // The AsyncWebServer runs in the background, so we don't need anything in the loop
}