import ujson

class ConfigManager:

    def __init__(self, json_file):
        self.config = None
        self.json_file = json_file
        self.load_json(self.json_file)
        
    def load_json(self, json_file):
        with open(json_file, 'r') as f:
            self.config = ujson.load(f)
            print(self.config)
    
    def get_value(self, group, param):
        return self.config[group][param]
    
    def update_config(self, gruppo, parametro, valore):
        self.config[gruppo][parametro] = valore

        try:
            with open(self.json_file, 'w') as f:
                ujson.dump(self.config, f)
            print(f"File '{self.json_file}' aggiornato.")
        except OSError as e:
            print(f"Errore durante la scrittura del file: {e}")
            
        self.load_json(self.json_file)
            
            
