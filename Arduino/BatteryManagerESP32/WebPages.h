const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="it">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Battery Control System</title>
    <style>
        body {
            font-family: 'Segoe UI', Arial, sans-serif;
            margin: 0;
            padding: 0;
            background-color: #f5f7fa;
            color: #333;
        }
        
        .navbar {
            background: linear-gradient(to right, #2196F3, #00b0ff);
            color: white;
            padding: 15px 0;
            text-align: center;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
        }
        
        .navbar h1 {
            margin: 0;
            font-size: 24px;
        }
        
        .container {
            max-width: 800px;
            margin: 20px auto;
            padding: 0 15px;
        }
        
        .data-container {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(220px, 1fr));
            gap: 15px;
            margin-bottom: 25px;
        }
        
        .data-box {
            background-color: white;
            border-radius: 8px;
            padding: 15px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.05);
            text-align: center;
            transition: transform 0.2s;
        }
        
        .data-box:hover {
            transform: translateY(-3px);
            box-shadow: 0 4px 8px rgba(0,0,0,0.1);
        }
        
        .data-label {
            color: #666;
            font-size: 14px;
            margin-bottom: 8px;
        }
        
        .data-value {
            font-size: 22px;
            font-weight: bold;
            color: #2196F3;
        }
        
        .nominal-voltage { color: #4CAF50; }
        .actual-voltage { color: #2196F3; }
        .actual-current { color: #9C27B0; }
        .voltage-min { color: #FF9800; }
        .current-max { color: #F44336; }
        
        .form-section {
            background-color: white;
            border-radius: 8px;
            padding: 20px;
            margin-bottom: 15px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.05);
        }
        
        .form-title {
            font-size: 16px;
            font-weight: bold;
            margin-bottom: 15px;
            color: #2196F3;
        }
        
        input, select {
            padding: 10px;
            border: 1px solid #ddd;
            border-radius: 6px;
            font-size: 14px;
            width: 70%;
            margin-right: 10px;
        }
        
        input:focus, select:focus {
            outline: none;
            border-color: #2196F3;
            box-shadow: 0 0 0 2px rgba(33, 150, 243, 0.1);
        }
        
        button {
            background: linear-gradient(to right, #2196F3, #00b0ff);
            color: white;
            border: none;
            padding: 10px 15px;
            border-radius: 6px;
            cursor: pointer;
            font-size: 14px;
            transition: background 0.2s, transform 0.1s;
        }
        
        button:hover {
            background: linear-gradient(to right, #0b7dda, #009be5);
            transform: translateY(-2px);
        }
        
        button:active {
            transform: translateY(1px);
        }
        
        .form-row {
            display: flex;
            align-items: center;
            margin-bottom: 15px;
        }
        
        label {
            min-width: 150px;
            display: inline-block;
        }
        
        @media (max-width: 600px) {
            .data-container {
                grid-template-columns: 1fr 1fr;
            }
            
            .form-row {
                flex-direction: column;
                align-items: flex-start;
            }
            
            input, select {
                width: 100%;
                margin-bottom: 10px;
                margin-right: 0;
            }
            
            label {
                margin-bottom: 5px;
            }
            
            button {
                width: 100%;
            }
        }
    </style>
    <script>
        function aggiornaDati() {
            fetch("/data")
                .then(response => response.json())
                .then(data => {
                    document.getElementById("nominal_voltage").innerText = data.nominal_voltage + " V";
                    document.getElementById("voltage").innerText = data.actual_voltage + " V";
                    document.getElementById("current").innerText = data.actual_current + " A";
                    document.getElementById("voltage_min").innerText = data.voltage_limit + " V"|| "-- V";
                    document.getElementById("current_max").innerText = data.current_limit + " A"|| "-- V";
                })
                .catch(error => console.error("Errore nel fetch:", error));
        }

        setInterval(aggiornaDati, 1000);

        function inviaValoreTensione(valore) {
          fetch(`/save_battery_param?voltage_limit=${valore}`) // Usa una query string
            .then(response => response.text())
            .then(data => {
              alert(data);
              document.getElementById("minVoltageValue").value = "";
            })
            .catch(error => console.error("Errore:", error));
        }
        
        function inviaValoreCorrente(valore) {
          fetch(`/save_battery_param?current_limit=${valore}`) // Usa una query string
            .then(response => response.text())
            .then(data => {
              alert(data);
              document.getElementById("minVoltageValue").value = "";
            })
            .catch(error => console.error("Errore:", error));
        }
        
        function inviaValoreTensioneNominale(selectedValue) {
            if (selectedValue) {
                fetch(`/save_battery_param?nominal_voltage=${selectedValue}`) // Usa una query string
                .then(response => response.text())
                .then(data => {
                  alert(data);
                  document.getElementById("nominalVoltage").value = "";
            })
            .catch(error => console.error("Errore:", error));
            } else {
                alert("Seleziona una tensione valida.");
            }
        }
    </script>
</head>
<body>
    <div class="navbar">
        <h1>Battery Control System</h1>
    </div>
    
    <div class="container">
        <div class="data-container">
            <div class="data-box">
                <div class="data-label">Tensione nominale</div>
                <div class="data-value nominal-voltage" id="nominal_voltage">-- V</div>
            </div>
            
            <div class="data-box">
                <div class="data-label">Tensione attuale</div>
                <div class="data-value actual-voltage" id="voltage">-- V</div>
            </div>
            
            <div class="data-box">
                <div class="data-label">Corrente attuale</div>
                <div class="data-value actual-current" id="current">-- A</div>
            </div>
            
            <div class="data-box">
                <div class="data-label">Limite tensione minima</div>
                <div class="data-value voltage-min" id="voltage_min">-- V</div>
            </div>
            
            <div class="data-box">
                <div class="data-label">Limite corrente massima</div>
                <div class="data-value current-max" id="current_max">-- A</div>
            </div>
        </div>
        
        <div class="form-section">
            <div class="form-title">Impostazioni batteria</div>
            
            <form id="voltageForm" onsubmit="event.preventDefault();">
                <div class="form-row">
                    <label for="minVoltageValue">Tensione minima:</label>
                    <input type="text" id="minVoltageValue" placeholder="Inserisci valore">
                    <button type="button" onclick="inviaValoreTensione(document.getElementById('minVoltageValue').value)">Imposta</button>
                </div>
            </form>
            
            <form id="currentForm" onsubmit="event.preventDefault();">
                <div class="form-row">
                    <label for="maxCurrentValue">Corrente massima:</label>
                    <input type="text" id="maxCurrentValue" placeholder="Inserisci valore">
                    <button type="button" onclick="inviaValoreCorrente(document.getElementById('maxCurrentValue').value)">Imposta</button>
                </div>
            </form>
            
            <form id="typeForm" onsubmit="event.preventDefault();">
                <div class="form-row">
                    <label for="nominalVoltage">Tensione batteria:</label>
                    <select id="nominalVoltage">
                        <option value="" selected disabled>-- Seleziona --</option>
                        <option value="6">6V</option>
                        <option value="12">12V</option>
                        <option value="24">24V</option>
                    </select>
                    <button type="button" onclick="inviaValoreTensioneNominale(document.getElementById('nominalVoltage').value)">Imposta</button>
                </div>
            </form>
        </div>
    </div>
</body>
</html>
)rawliteral";
