////////////////////////////
// USE AN ESP32 / ESP8266 //
//     FOR THIS SKETCH    //
////////////////////////////

#if defined(ESP32)
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#define DEVICE "ESP8266"
#endif 
#include <InfluxDbClient.h>
// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
// Conversion factor for micro seconds to seconds
#define uS_TO_S_FACTOR 1000000  
// Time ESP32 will go to sleep (in seconds)
#define TIME_TO_SLEEP  30  
// WiFi AP SSID
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
//ESP Board MAC Address:  34:94:54:25:85:4C
// InfluxDB  server url. Don't use localhost, always server name or ip address.
#define INFLUXDB_URL "YOUR_INFLUX_DB_ADDRESS"
// InfluxDB 2 server or cloud API authentication token (Use: InfluxDB UI -> Load Data -> Tokens -> <select token>)
#define INFLUXDB_TOKEN "YOUR_INFLUX_DB_TOKEN"
// InfluxDB 2 organization id (Use: InfluxDB UI -> Settings -> Profile -> <name under tile> )
#define INFLUXDB_ORG "GrainGuardian"
// InfluxDB 2 bucket name (Use: InfluxDB UI -> Load Data -> Buckets)
#define INFLUXDB_BUCKET "influx_Init"
// InfluxDB client instance
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
// Data point for influxDB
Point sensor("Tom_temps");
// Data wire is plugged into port 4 on the Arduino
#define ONE_WIRE_BUS 4
#define TEMPERATURE_PRECISION 11 // Higher resolution
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
 // Number of temperature devices found
int numberOfDevices;
//DeviceAddress sensorAddr;
DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address

void setup(void) {
  // start serial port
  Serial.begin(9600);
  delay(1000);
  // Sets deep sleep wakeup timer - set at 30 seconds
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  // Connect WiFi
  Serial.println("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    }
  Serial.println();
  // Set insecure connection to skip server certificate validation 
  client.setInsecure();
  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
    } 
  else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
    }
  // Start up the Dallas library
  sensors.begin();
  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress) {
  //variable for showing the sensor temperatures
  float tempC = sensors.getTempC(deviceAddress);
  // clears Fields to repopulate before sending to InfluxDB
  sensor.clearFields();
  Serial.print(": ");
  Serial.println(tempC);
  // adds a Field to send info to InfluxDB
  sensor.addField("Temperature", tempC);
}

void loop(void) { 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // Loop through each device, print out temperature data
  for(int i=0;i<numberOfDevices; i++) {
      // Search the wire for address
      if(sensors.getAddress(tempDeviceAddress, i)) {
          //Clears Tag info to repopulate for each device being read
          sensor.clearTags();
          // Output the Cable Serial Number (CableSN) and append it to the Tags sent to InfluxDB
          uint16_t cableID = sensors.getUserData(tempDeviceAddress);
          char cableSN[16];
          sprintf(cableSN,"%d", cableID);
          sensor.addTag("CableSN", cableSN);
          // Output the Sensor #
          Serial.print("Temp for device ");
          Serial.print(i,DEC);
          String devNumber = String(i);
          sensor.addTag("Sensor", devNumber);
          //print out the temperature values
          printTemperature(tempDeviceAddress);
          //print out the device addresses          
          printAddress(tempDeviceAddress);
          // Print what are we exactly writing to InfluxDB
          Serial.print("Writing: ");
          Serial.println(client.pointToLineProtocol(sensor));
          // If no Wifi signal, try to reconnect it
          if (wifiMulti.run() != WL_CONNECTED) {
            Serial.println("Wifi connection lost");
            }
          // Write point to influxDB
          if (!client.writePoint(sensor)) {
            Serial.print("InfluxDB write failed: ");
            Serial.println(client.getLastErrorMessage());
            }
        delay(500);
        } 
     }
  // Puts ESP into deep sleep mode
  esp_deep_sleep_start();
  //delay(20000);
}

// function to print a device address and converts it to a String
void printAddress(DeviceAddress deviceAddress) {
   String sensorAddressString = "";
    for (uint8_t i = 0; i < 8; i++) {
        if (deviceAddress[i] < 16) sensorAddressString += "0";
        sensorAddressString += String(deviceAddress[i], HEX);
        }
   // Add a Tag with the Sensor Address to send to InfluxDB
   sensor.addTag("Address", sensorAddressString);
}