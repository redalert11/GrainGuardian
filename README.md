# GrainGuardian
Grain Monitoring tool to read DS18B20 sensors. 

# Roadmap
## 1. ESP32 Setup:
* Set up ESP32 with the SX1276 LoRa module and DS18B20 temperature sensor.
* Write code to read temperature readings from the DS18B20 sensor.
* Configure LoRa communication on the ESP32 using PlatformIO.
## 2. LoRa Communication:
* Define a protocol for transmitting data between nodes. This could include the format of the data packet (e.g., temperature value, node ID, etc.).
* Write code to package the temperature readings into LoRa packets and transmit them at regular intervals.
## 3. Concentrator Setup:
* Set up another ESP32 with an SX1276 LoRa module, which will act as the concentrator.
* Write code to receive LoRa packets from the ESP32 nodes.
* Process received data packets, extract temperature readings, and possibly other relevant information.
## 4. Web Server Setup:
* Set up a web server on the concentrator ESP32. You can use libraries like ESPAsyncWebServer or the standard ESP32 web server library.
* Write code to handle incoming HTTP requests and serve a webpage.
## 5. Displaying Data:
* Create a webpage (HTML, CSS, JavaScript) to display the received temperature readings.
* Use JavaScript to periodically fetch data from the concentrator's web server and update the webpage dynamically.


# Visual Studio Code settings
1. Download board settings from here - https://github.com/Xinyuan-LilyGO/LilyGo-LoRa-Series/blob/master/boards/t3_s3_v1_x.json
1. in Visual Studio Code got the platformio and install the "eEspressif 32" platform
1. copy the file you downloaded to this location - C:\Users\adria\.platformio\platforms\espressif32\boards
1. run this comand from within Visual Studio Code in a terminal - pio boards


# Parts
* NiCad 18650 
* [solar panel](https://www.aliexpress.com/item/32905500852.html)
