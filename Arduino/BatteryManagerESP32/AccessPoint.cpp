#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include "AccessPoint.h"
#include "Params.h"
#include "WebPages.h"
#include "Battery.h"

AsyncWebServer server(80);

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1); // Indirizzo IP dell'AP
DNSServer dnsServer;


bool isAuthenticated = false;
const char* adminUser = "admin";
const char* adminPass = "password";


String AccessPoint::processor(const String& var) {
    if (var == "STATUS") {
        return isAuthenticated ? "Autenticato" : "Non autenticato";
    }
    return String();
}


void AccessPoint::activate()
{

  uint64_t chipId = ESP.getEfuseMac();
  // Converti il chipId in una stringa esadecimale
  String chipIdStr = String((uint32_t)(chipId >> 32), HEX) + String((uint32_t)chipId, HEX);

  // Crea l'SSID con il chipId
  String ssid = "LDV _"+ chipIdStr;

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
      
      String json = "{ \"nominal_voltage\":" + String(Battery::getInstance()->getNominalVoltage()) + 
                    ", \"actual_voltage\":"  + String(Battery::getInstance()->getVoltageValue()) + 
                    ", \"actual_current\":"  + String(Battery::getInstance()->getCurrentValue()) + 
                    ", \"voltage_limit\":"   + String(Battery::getInstance()->getVoltageLimit()) + 
                    ", \"current_limit\":"   + String(Battery::getInstance()->getCurrentLimit()) + "}"; 
                    
      request->send(200, "application/json", json);
  });


  server.on("/save_battery_param", HTTP_GET, [](AsyncWebServerRequest *request) {
      
    if (request->hasParam("voltage_limit")) 
      {
          String voltageLimitStr = request->getParam("voltage_limit")->value();
          float voltageLimit = voltageLimitStr.toFloat();

          Battery::getInstance() -> setVoltageLimit(voltageLimit);

          request->send(200, "text/plain", "Valore di tensione ricevuto e salvato.");
        }
        else if (request->hasParam("current_limit")) 
        {
          String currentLimitStr = request->getParam("current_limit")->value();
          float currentLimit = currentLimitStr.toFloat();

          Battery::getInstance() -> setCurrentLimit(currentLimit);

          request->send(200, "text/plain", "Valore di corrente ricevuto e salvato.");
        }
        else if (request->hasParam("nominal_voltage")) 
        {
          String nominalVoltageStr = request->getParam("nominal_voltage")->value();
          float nominalVoltage = nominalVoltageStr.toFloat();

          Battery::getInstance() -> setNominalVoltage(nominalVoltage);

          request->send(200, "text/plain", "Valore di tensione nominale ricevuto e salvato.");
        } 
        else 
        {
          request->send(400, "text/plain", "Bad Request: Parametro mancante.");
        }

  });

  server.on("/admin", HTTP_GET, [](AsyncWebServerRequest* request) {
      if (isAuthenticated) {
          request->send_P(200, "text/html", admin_html, processor);
      } else {
          request->redirect("/login");
      }
  });

  server.on("/login", HTTP_GET, [](AsyncWebServerRequest* request) {
      request->send_P(200, "text/html", login_html);
  });

  server.on("/login", HTTP_POST, [](AsyncWebServerRequest* request) {
      if (request->hasParam("username", true) && request->hasParam("password", true)) {
          String user = request->getParam("username", true)->value();
          String pass = request->getParam("password", true)->value();

          if (user == adminUser && pass == adminPass) {
              isAuthenticated = true;
              request->send_P(200, "text/html", admin_html, processor);
          } else {
              request->send(403, "text/plain", "Errore");
          }
      } else {
          request->send(400, "text/plain", "Dati mancanti");
      }
  });

  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest* request) {
      isAuthenticated = false;
      request->redirect("/");
  });
  
  //server.on("/save_battery_param", HTTP_GET, handleSaveBatteryParam);

  server.begin();

}

