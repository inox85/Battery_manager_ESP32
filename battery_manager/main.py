import network
import socket
import ujson
import time
from machine import Pin, SPI
import _thread
from config_manager import ConfigManager
from microdot import Microdot, Response, send_file
import random

config = ConfigManager('config.json')

SSID = config.get_value("wifi", "ssid")
PASSWORD = config.get_value("wifi", "password")

ap = network.WLAN(network.AP_IF)
ap.active(False)
time.sleep(1)
ap.active(True)
ap.config(essid=SSID, password=PASSWORD)

while not ap.active():
    time.sleep(1)

print("Access Point attivo! IP:", ap.ifconfig()[0])

# Variabili globali
voltage = 12.0
current = 0.5
min_voltage = config.get_value("battery", "voltage_limit")
max_current = config.get_value("battery", "current_limit")
# Funzione per aggiornare la voltageeratura (thread separato)


def aggiorna_dati():
    global voltage
    global current
    while True:
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
        "nominal_voltage": config.get_value("battery", "nominal_voltage"),
        "actual_voltage": round(voltage, 2),
        "actual_current": round(current, 2),
        "voltage_limit": config.get_value("battery", "voltage_limit"),
        "current_limit": config.get_value("battery", "current_limit")
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
           config.update_config("battery", key, float(value))
           print("OK")
           return f"Impostazione {key} avvenuta correttamente", 400

    except Exception as e:
         return f"Errore salva valore: {e}", 400    


server.run(port=80)