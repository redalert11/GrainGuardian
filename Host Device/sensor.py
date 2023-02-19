import machine, onewire, ds18x20, time

ds_pin = machine.Pin(4)  # sets IO pin 4 to read sensors from
ds_sensor = ds18x20.DS18X20(onewire.OneWire(ds_pin))
# count=0

roms = ds_sensor.scan()
print('Found DS devices: ', roms)  # scans for one-wire devices and shows their address
# for rom in (roms):

# f = open("cableAddress001", "ab")
# f.write(romDec)
# f.close()


while True:
    ds_sensor.convert_temp()
    time.sleep_ms(750)
    for rom in (roms):
        # print(bytes(rom))
        print(ds_sensor.read_temp(rom), "*Celsius")
        # print(ds_sensor.read_scratch(rom))
        # if count in range(0,10):
        #     print("10 sensors")
        print("trying to write to my_file.bin")
        f = open("my_file.bin", "ab")
        f.write(rom)
        f.write("\n")
        f.write(ds_sensor.read_scratch(rom))
        f.write("\n")
        f.close()

    break;

