#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <freertos/semphr.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "LEDTask.h"
#include "TempReadTask.h"
#include "DisplayTask.h"
#include "WebServerTask.h"
#include "OTATask.h"

// WiFi credentials
const char* ssid = "Nacho Internet";
const char* password = "redalert";

// Global variables to store temperature readings
String temperatureReadings;
String sensorAddresses;

// Semaphore to protect the buffer
SemaphoreHandle_t bufferMutex;

// Create SSD1306 display instance
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Create OneWire instance
OneWire oneWire(ONE_WIRE_BUS);

// Pass OneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

/*
 * Note: To upload the files to SPIFFS, run the following command:
 * pio run --target uploadfs
 * This command uploads the contents of the 'data' folder to the SPIFFS file system on the ESP32.
 */

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize I2C with specified pins
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize SSD1306 display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Initialize the DS18B20 sensors
  sensors.begin();

  // Initialize the green LED pin as output
  pinMode(GREEN_LED_PIN, OUTPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Initialize OTA
  setupOTA();

  // Create a semaphore to protect the buffer
  bufferMutex = xSemaphoreCreateMutex();

  // Create the tasks
  //createLEDTask();
  createTempReadTask();
  createDisplayTask();
  createWebServerTask();
}

void loop() {
  handleOTA(); // Handle OTA updates
  // Nothing to do here, everything is handled by FreeRTOS tasks
}
