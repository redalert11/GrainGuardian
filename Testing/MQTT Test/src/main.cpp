#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "Wifi Name";
const char* password = "Wifi Password";

// MQTT Broker details
const char* mqtt_server = "Server_IP";
const int mqtt_port = 1883;
const char* mqtt_topic = "Topic_name";

// MQTT Client
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void publishDiagnostics() {
  // Create a JSON object to hold diagnostic data
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["uptime"] = millis() / 1000;
  jsonDoc["rssi"] = WiFi.RSSI();
  jsonDoc["free_heap"] = ESP.getFreeHeap();
  jsonDoc["ip_address"] = WiFi.localIP().toString();
  jsonDoc["connected"] = WiFi.isConnected();

  // Serialize JSON object to a string
  char buffer[200];
  serializeJson(jsonDoc, buffer, sizeof(buffer));

  // Publish JSON string to MQTT
  client.publish(mqtt_topic, buffer);
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Publish diagnostics every 10 seconds
  static unsigned long lastMsg = 0;
  unsigned long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    publishDiagnostics();
  }
}
