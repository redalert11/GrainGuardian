# GrainGuardian
Grain Monitoring tool to read DS18B20 sensors. 

### Roadmap
* Read Sensor
* Sort sensors by position. this value is stored in the internal memory
* setup wifi and use [webrepl](https://micropython.org/webrepl/)
* email a report
* text on alarm(text via email)


### Resources

* [mycropython](https://docs.micropython.org/en/latest/index.html) 
* [Flash micropython firmware on esp8266](https://docs.micropython.org/en/latest/esp8266/tutorial/intro.html?highlight=install)
* [micropython in pycharm](https://medium.com/@andymule/micropython-in-pycharms-basic-setup-9169b497ec8a) 

### Visual Studio Code settings
1. Download board settings from here - https://github.com/Xinyuan-LilyGO/LilyGo-LoRa-Series/blob/master/boards/t3_s3_v1_x.json
1. in Visual Studio Code got the platformio and install the "eEspressif 32" platform
1. copy the file you downloaded to this location - C:\Users\adria\.platformio\platforms\espressif32\boards
1. run this comand from within Visual Studio Code in a terminal - pio boards


### Parts
* NiCad 18650 
* [solar panel](https://www.aliexpress.com/item/32905500852.html)
