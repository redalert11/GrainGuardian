These arent the only files that will exist on the ESP board
You'll also need to get the WebRepl_cfg.py file as well as wifi.dat and wifimgr.py files set up
however, those are specific to your board and your network, as the WebRepl_cfg just has the password you set for your device
wifi.dat just has the ssid / pswrd for your wifi, and wifimgr sets the IP/Subnet/gateway for your board on your network

Aside from that, assuming you're trying to read on IO Pin 4, the rest of the files here should just work with the webrepl local page (webrepl.html)

All the actual "work" is done in the sensor.py file, which sends a http request to the php file that then populates the database