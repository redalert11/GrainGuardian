# GrainGuardian
Grain Monitoring tool to read DS18B20 sensors. 

###Roadmap
* Read Sensor
* Sort sensors by position. this value is stored in the internal memory
* setup wifi and use [webrepl](https://micropython.org/webrepl/)


###Resources

* [mycropython](https://docs.micropython.org/en/latest/index.html) 
* [Flash micropython firmware on esp8266](https://docs.micropython.org/en/latest/esp8266/tutorial/intro.html?highlight=install)
* [micropython in pycharm](https://medium.com/@andymule/micropython-in-pycharms-basic-setup-9169b497ec8a) 

###Pycham settings
close any project that is open. then do the following
* get from VCS
* version control = git and url = https://github.com/redalert11/GrainGuardian then click clone
* file -> settings 
    * Project: GrainGuardian
        * Python Interpreter - select your interpreter
        * Project Structure - right click .idea and click exclude
        * plugins -> marketplace - search for micropython and install
        * languages & frameworks -> MicroPython
            * enable MicroPython support
            * set device type
            * set device path example com3