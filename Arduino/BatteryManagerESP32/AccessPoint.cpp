#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include "AccessPoint.h"
#include "Params.h"
#include "WebPages.h"

AsyncWebServer server(80);

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1); // Indirizzo IP dell'AP
DNSServer dnsServer;


void AccessPoint::activate()
{

  uint64_t chipId = ESP.getEfuseMac();
  // Converti il chipId in una stringa esadecimale
  String chipIdStr = String((uint32_t)(chipId >> 32), HEX) + String((uint32_t)chipId, HEX);

  // Crea l'SSID con il chipId
  String ssid = "BM_"+ chipIdStr;

  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); // Configura l'AP 
  WiFi.softAP(ssid, "12345678");
  Serial.println("Access Point creato");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());  // Stampa l'IP dell'ESP32 (di solito 192.168.4.1)
  dnsServer.start(DNS_PORT, "*", apIP); // Avvia il server DNS

}


void AccessPoint::startServer(){

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


  server.on("/save_battery_param", HTTP_GET, [](AsyncWebServerRequest *request) {
      
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
        } 
        else 
        {
          request->send(400, "text/plain", "Bad Request: Parametro mancante.");
        }
  });
  
  //server.on("/save_battery_param", HTTP_GET, handleSaveBatteryParam);

  server.begin();

}

