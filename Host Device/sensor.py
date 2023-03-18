import machine, onewire, ds18x20, time
try:
  import urequests as requests
except:
  import requests

import gc
gc.collect()

ds_pin = machine.Pin(4)  # sets IO pin 4 to read sensors from
ds_sensor = ds18x20.DS18X20(onewire.OneWire(ds_pin))
count=0  # used for displaying "sensor 1, sensor 2 in monitor"
tempReading = (str(ds_sensor.read_temp(rom)))  #convert reading to string for URL

def send_message(tempReading):
  #set your host URL
  url = 'http://192.168.0.142/insert_temperature.php?rom1='+str(rom)+'&temperature='+tempReading

  #make the request
  response = requests.get(url)
  #check if it was successful
  if response.status_code == 200:
    print('Success!')
  else:
    print('Error')
    print(response.text)

roms = ds_sensor.scan()
print('Found DS devices: ', roms)  # scans for one-wire devices and shows their address
    #with open("cableAddress001.txt", "ab") as binary_file:
    # binary_file.write(roms)
     #close()

while True:
    ds_sensor.convert_temp()
    time.sleep_ms(750)
    for rom in (roms):  # for each sensor in the cable, do this
        count+=1  # display which sensor is being read at the moment
        #rom1 = str(rom)
        print("sensor ",count)
        # print(bytes(rom))
        print(ds_sensor.read_temp(rom), "*Celsius")
        send_message(tempReading)   # Send message to database
        time.sleep_ms(250)
        
        
        # print(ds_sensor.read_scratch(rom))
        # if count in range(0,10):
        #     print("10 sensors")
#         print("trying to write to my_file.txt")
#         f = open("my_file.txt", "a")
#         f.write("Sensor ")
#         f.write(str(count))
        #f.write(rom)
#         f.write("\n")
#         f.write(str(ds_sensor.read_temp(rom)))
#         f.write("\n")
        #f.write(output)
#         f.write("\n")
#         f.close()
#         with open("addresses.bin", "wb") as binary_file:
#             binary_file.write(rom)
#         print("wrote to addresses.bin")
        
    break;

