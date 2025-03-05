#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include "AccessPoint.h"
#include "Params.h"

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
