#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>
#include "Battery.h"
#include "AccessPoint.h"

AccessPoint accessPoint;

void setup() 
{

  Serial.begin(115200);
  
  accessPoint.activate();

  accessPoint.startServer();

}


void loop() 
{
  

}

