#include <Arduino.h>
#include "LEDTask.h"

// LED blinking task
void blinkLED(void *parameter) {
  const TickType_t xDelay = 500 / portTICK_PERIOD_MS; // 500ms delay
  for(;;) {
    digitalWrite(GREEN_LED_PIN, HIGH); // Turn the LED on
    vTaskDelay(xDelay);                // Wait for 500ms
    digitalWrite(GREEN_LED_PIN, LOW);  // Turn the LED off
    vTaskDelay(xDelay);                // Wait for 500ms
  }
}

void createLEDTask() {
  // Create the LED blinking task
  xTaskCreate(
    blinkLED,      // Task function
    "Blink LED",   // Name of the task
    1000,          // Stack size in words
    NULL,          // Task input parameter
    1,             // Priority of the task
    NULL           // Task handle
  );
}
