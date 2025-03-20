#ifndef ACCESSPOINT_H
#define ACCESSPOINT_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

class AccessPoint
{
  public:

    void activate();

    void startServer();

    void shutDown();

    void setActive();

    void setInactive();

    void checkClientConnected();

    
  private:

    unsigned long lastCheckTime = 0;
    unsigned long noClientSince = 0;
    const unsigned long checkInterval = 5000;  // ogni quanto controllare (5 sec)
    const unsigned long timeout = 60000;       // timeout di 1 minuto senza client
    bool wifiActive = true;
   
    
    //void handleSaveBatteryParam(AsyncWebServerRequest *request);

    static String processor(const String& var);

};

#endif 
