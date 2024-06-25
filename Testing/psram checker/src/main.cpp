#include <Arduino.h>

void setup() {
  Serial.begin(9600);

  // Check heap memory
  Serial.printf("Heap size: %d\n", ESP.getHeapSize());
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
  Serial.printf("Min free heap: %d\n", ESP.getMinFreeHeap());
  Serial.printf("Max alloc heap: %d\n", ESP.getMaxAllocHeap());

  // Check PSRAM if available
  if (psramFound()) {
    Serial.printf("PSRAM size: %d\n", ESP.getPsramSize());
    Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());
    Serial.printf("Min free PSRAM: %d\n", ESP.getMinFreePsram());
    Serial.printf("Max alloc PSRAM: %d\n", ESP.getMaxAllocPsram());
  } else {
    Serial.println("PSRAM not found");
  }
}

void loop() {
  // Your main code here
}
