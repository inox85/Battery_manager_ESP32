from microdot import Microdot, Response, send_file


def start_server(config_manager, battery_manager):
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
            "actual_voltage": round(battery_manager.get_actual_voltage, 2),
            "actual_current": round(battery_manager.get_actual_current, 2),
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
