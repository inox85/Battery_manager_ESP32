import time
from machine import Pin, SPI
import _thread
from config_manager import ConfigManager
from access_point import AccessPoint
import random
import network
from battery_manager import BatteryManager
from microdot import Microdot, Response, send_file
import ujson

config_manager = ConfigManager('config.json')

access_point = AccessPoint(config_manager)

battery_manager = BatteryManager(config_manager)

_thread.start_new_thread(battery_manager.aggiorna_dati, ())

server = Microdot()

@server.route('/')
def index(request):
    return send_file('static/index.html', content_type='text/html')

@server.route('/graph')
def index(request):
    return send_file('static/index_graph.html', content_type='text/html')

@server.route('/data')
def get_data(request):
    try:
        data = {
            "nominal_voltage": config_manager.get_value("battery", "nominal_voltage"),
            "actual_voltage": round(battery_manager.current_voltage, 2),
            "actual_current": round(battery_manager.current_current, 2),
            "voltage_limit": config_manager.get_value("battery", "voltage_limit"),
            "current_limit": config_manager.get_value("battery", "current_limit")
        }
        return data, 200, {'Content-Type': 'application/json'}
    except Exception as e:
        return f"Errore recupero valori: {e}", 400    
    


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
