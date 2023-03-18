import wifimgr

wlan = wifimgr.get_connection()
if wlan is None:
    print("Could not initialize the network connection.")
    while True:
        pass  # you shall not pass :D

# connect to webRepl, then just run:
#         exec(open("sensor.py").read())
# this command will execute importing,
# reading, running, and reloading sensor.py