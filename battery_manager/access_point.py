from machine import Pin, SPI
import time
import network

class AccessPoint:
    def __init__(self, config_manager):
        print("AP activation..")
        SSID = config_manager.get_value("wifi", "ssid")
        PASSWORD = config_manager.get_value("wifi", "password")
        self.ap = network.WLAN(network.AP_IF)
        self.ap.active(False)
        time.sleep(1)
        self.ap.active(True)
        self.ap.config(essid=SSID, password=PASSWORD)
        
        while not self.ap.active():
            time.sleep(1)
     
        print("AP activation done! ->", "SSID:", SSID, "PASSWORD:", PASSWORD, "IP:", self.ap.ifconfig()[0])
