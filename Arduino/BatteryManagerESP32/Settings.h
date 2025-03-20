#ifndef SETTINGS_H
#define SETTINGS_H

#include <Preferences.h>
#include <Arduino.h>

class Settings {
  private:
    static Settings* instance;
    Preferences preferences;
    Settings();
    
    unsigned long lastCheckTime = 0;
    unsigned long noClientSince = 0;
    const unsigned long checkInterval = 5000;  // ogni quanto controllare (5 sec)
    const unsigned long timeout = 60000;       // timeout di 1 minuto senza client

  public:
    static Settings* getInstance();

    float writeFloat(const char* key, float value);
    float readFloat(const char* key, float defaultValue = 0.0f);

    String writeString(const char* key, const String& value);
    String readString(const char* key, const String& defaultValue = "");

    int writeInt(const char* key, int value);
    int readInt(const char* key, int defaultValue = 0);
    
};

#endif