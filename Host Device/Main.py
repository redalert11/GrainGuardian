import wifimgr, machine, onewire, ds18x20, time

# This section for wifi setup / connection
#wlan = wifimgr.get_connection()
#if wlan is None:
#    print("Could not initialize the network connection.")
#    while True:
#        pass  # you shall not pass :D


# Main Code goes here, wlan is a working network.WLAN(STA_IF) instance.

#This section for ds18b20 comms

ds_pin = machine.Pin(4)
ds_sensor = ds18x20.DS18X20(onewire.OneWire(ds_pin))
count=0

roms = ds_sensor.scan()
print('Found DS devices: ', roms)

while True:
    ds_sensor.convert_temp()
    time.sleep_ms(750)
    for rom in (roms):
        count +=1
        print("For Sensor",count)        
        #print(rom)
        print(ds_sensor.read_temp(rom),"*Celsius")
        #print(ds_sensor.read_scratch(rom))
#        if count in range(0,10):
#             print("10 sensors")
            
    time.sleep(50)

print("ESP OK")

