#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to GPIO 23
#define ONE_WIRE_BUS 23

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(void)
{
  // Start serial communication
  Serial.begin(9600);
  Serial.println("Dallas Temperature Sensors Test");

  // Initialize the DS18B20 sensors
  sensors.begin();
}

void loop(void)
{
  // Request temperature readings from all sensors
  sensors.requestTemperatures();

  // Get the number of sensors found
  int numberOfDevices = sensors.getDeviceCount();
  Serial.print("Found ");
  Serial.print(numberOfDevices);
  Serial.println(" devices.");

  // Loop through each sensor and read its temperature
  for (int i = 0; i < numberOfDevices; i++)
  {
    // Fetch temperature in Celsius from the sensor
    float tempC = sensors.getTempCByIndex(i);

    // Check if reading is valid
    if (tempC != DEVICE_DISCONNECTED_C)
    {
      Serial.print("Temperature for device ");
      Serial.print(i);
      Serial.print(" : ");
      Serial.print(tempC);
      Serial.println(" °C");
    }
    else
    {
      Serial.print("Error: Could not read temperature data for device ");
      Serial.println(i);
    }
  }

  // Wait 1 second before taking another reading
  delay(1000);
}
