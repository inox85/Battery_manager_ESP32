import random
import _thread
import time

class BatteryManager:
    def __init__(self, config_manager):
        self.voltage = 12.0
        self.current = 0.5
        self.min_voltage = config_manager.get_value("battery", "voltage_limit")
        self.max_current = config_manager.get_value("battery", "current_limit")
    
    def aggiorna_dati(self):
        while True:
            #print(ap.status('stations'))
            self.voltage = round(random.uniform(11.9, 12.1), 2)
            self.current = round(random.uniform(0.45, 0.55), 2)  # 0.5-3A
            time.sleep_ms(10)

    def get_actual_voltage(self):
        return self.voltage
    
    def get_actual_current(self):
        return self.current