#include <Arduino.h>

#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pin configuration for SX1276
#define SCK     5
#define MISO    3
#define MOSI    6
#define SS      7
#define RST     8
#define DI0     9  // DI0 (Interrupt pin)

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// I2C pins for the SSD1306
#define SDA_PIN 18
#define SCL_PIN 17

int messageCount = 0;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  delay(2000);  // Wait for 2 seconds instead of waiting for serial connection

  // Initialize I2C for OLED
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Check for 0x3D if 0x3C doesn't work
    Serial.println(F("SSD1306 allocation failed"));
    while (1); // Stay in loop
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("LoRa Receiver");
  display.display();
  delay(2000);  // Wait for 2 seconds to show the initial message

  // Setup LoRa transceiver module
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);

  if (!LoRa.begin(915E6)) {  // Initialize LoRa at 915 MHz
    Serial.println("Starting LoRa failed!");
    display.clearDisplay();
    display.setCursor(0, 10);
    display.print("LoRa Init Failed!");
    display.display();
    while (1); // Stay in loop
  }

  Serial.println("LoRa Receiver Initialized");
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("LoRa Init OK");
  display.display();
}

void loop() {
  // Try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    messageCount++;
    // Read packet
    String receivedText = "";
    while (LoRa.available()) {
      receivedText += (char)LoRa.read();
    }

    // Print received packet
    Serial.print("Received packet: ");
    Serial.println(receivedText);

    // Display received message on OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Received packet:");
    display.setCursor(0, 10);
    display.print(receivedText);
    display.setCursor(0, 20);
    display.print("Count: " + String(messageCount));
    display.display();
  }
}







