#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include "WebPages.h"
#include <ArduinoJson.h>
#include "Battery.h"
#include "AccessPoint.h"

AccessPoint accessPoint;
AsyncWebServer server(80);

void setup() {

  Serial.begin(115200);
  
  accessPoint.activate();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/html", index_html);
  
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
      
      String json = "{ \"nominal_voltage\":" + String(12) + 
                    ", \"actual_voltage\":"  + String(12.5) + 
                    ", \"actual_current\":"  + String(0.9) + 
                    ", \"voltage_limit\":"   + String(11) + 
                    ", \"current_limit\":"   + String(2) + "}"; 
                    
      request->send(200, "application/json", json);
  });

  server.on("/save_battery_param", HTTP_GET, handleSaveBatteryParam);

  server.begin();
  //Serial.println("Server started!");
}


void loop() {
  // put your main code here, to run repeatedly:

}


void handleSaveBatteryParam(AsyncWebServerRequest *request) 
{
  if (request->hasParam("voltage_limit")) 
  {
    String voltageLimitStr = request->getParam("voltage_limit")->value();
    float voltageLimit = voltageLimitStr.toFloat();

    Serial.print("Limite di tensione ricevuto (GET): ");
    Serial.println(voltageLimit);

    // Usa voltageLimit come necessario

    request->send(200, "text/plain", "Valore di tensione ricevuto e salvato.");
  }
  else if (request->hasParam("current_limit")) 
  {
    String currentLimitStr = request->getParam("current_limit")->value();
    float currentLimit = currentLimitStr.toFloat();

    Serial.print("Limite di corrente ricevuto (GET): ");
    Serial.println(currentLimit);

    // Usa voltageLimit come necessario

    request->send(200, "text/plain", "Valore di corrente ricevuto e salvato.");
  }
  else if (request->hasParam("nominal_voltage")) 
  {
    String nominalVoltageStr = request->getParam("nominal_voltage")->value();
    float nominalVoltage = nominalVoltageStr.toFloat();

    Serial.print("Limite di tensione nominale ricevuto (GET): ");
    Serial.println(nominalVoltage);

    // Usa voltageLimit come necessario

    request->send(200, "text/plain", "Valore di tensione nominale ricevuto e salvato.");
  } else {
    request->send(400, "text/plain", "Bad Request: Parametro mancante.");
  }
}
