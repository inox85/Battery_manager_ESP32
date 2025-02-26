import time
from machine import Pin, SPI
import _thread
from config_manager import ConfigManager
from access_point import AccessPoint
import random
import network
from battery_manager import BatteryManager
from microdot import Microdot, Response, send_file

config_manager = ConfigManager('config.json')

access_point = AccessPoint(config_manager)


voltage = 12.0
current = 0.5
min_voltage = config_manager.get_value("battery", "voltage_limit")
max_current = config_manager.get_value("battery", "current_limit")

def aggiorna_dati():
    global voltage
    global current
    while True:
        #print(ap.status('stations'))
        voltage = round(random.uniform(11.9, 12.1), 2)
        current = round(random.uniform(0.45, 0.55), 2)  # 0.5-3A
        time.sleep_ms(10)

_thread.start_new_thread(aggiorna_dati, ())

server = Microdot()

@server.route('/')
def index(request):
    return send_file('static/index.html', content_type='text/html')

@server.route('/graph')
def index(request):
    return send_file('static/index_graph.html', content_type='text/html')

@server.route('/data')
def get_data(request):
    data = {
        "nominal_voltage": config_manager.get_value("battery", "nominal_voltage"),
        "actual_voltage": round(voltage, 2),
        "actual_current": round(current, 2),
        "voltage_limit": config_manager.get_value("battery", "voltage_limit"),
        "current_limit": config_manager.get_value("battery", "current_limit")
    }
    return data, 200, {'Content-Type': 'application/json'}


@server.post('/save_battery_param')
def save_battery_param(request):
    print(request.body.decode())
    json_data = ujson.loads(request.body.decode())
    print(json_data)
    
    try:       
        for key, value in json_data.items():
           print(f"{key}: {value}")
           print("Salvataggio limite di tensione...")
           config_manager.update_config("battery", key, float(value))
           print("OK")
           return f"Impostazione {key} avvenuta correttamente", 400

    except Exception as e:
         return f"Errore salva valore: {e}", 400    


server.run(port=80)
