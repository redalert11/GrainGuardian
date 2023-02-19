import wifimgr  # machine, onewire, ds18x20, time, wifimgr

wlan = wifimgr.get_connection()
if wlan is None:
    print("Could not initialize the network connection.")
    while True:
        pass  # you shall not pass :D

# NONE of the stuff below works anymore. connect to webRepl, then just run:
#         exec(open("sensor.py").read())
#this command will execute importing, reading, running, and reloading sensor.py
#
# # Main Code goes here, wlan is a working network.WLAN(STA_IF) instance.
# #print("ESP OK")
#
# #ds_pin = machine.Pin(4)
# #ds_sensor = ds18x20.DS18X20(onewire.OneWire(ds_pin))
# #count=0
#
# #roms = ds_sensor.scan()
# #print('Found DS devices: ', roms)
# #for rom in (roms):
#
#     #f = open("cableAddress001", "ab")
#     #f.write(romDec)
#     #f.close()
#
#
# #while True:
# #    ds_sensor.convert_temp()
# #    time.sleep_ms(750)
# #    for rom in (roms):
# #        print(bytes(rom))
# #        print(ds_sensor.read_temp(rom),"*Celsius")
# #        #print(ds_sensor.read_scratch(rom))
# #        if count in range(0,10):
# #             print("10 sensors")
# #
# #    break;
#
# #import sensor
#