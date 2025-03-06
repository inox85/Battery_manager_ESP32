#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>
#include "Battery.h"
#include "AccessPoint.h"
#include "PreferencesManager.h"

AccessPoint accessPoint;

void setup() 
{

  Serial.begin(115200);
  
  accessPoint.activate();

  accessPoint.startServer();

  Serial.println("Qui");

  initPreferences(false);

  saveString("Saluto", "Ciao");

  endPreferences();

  String text = getString("Saluto", "None");

  Serial.println(text);
  Serial.println("Ciaone");
  
}


void loop() 
{


}

