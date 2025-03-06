#ifndef ACCESSPOINT_H
#define ACCESSPOINT_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

class AccessPoint
{
  public:

    void activate();

    void startServer();

  private:
    
    void handleSaveBatteryParam(AsyncWebServerRequest *request);

};

#endif 