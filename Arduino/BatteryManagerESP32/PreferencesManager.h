#ifndef PREFERENCES_MANAGER_H
#define PREFERENCES_MANAGER_H

#include <Arduino.h>
#include <Preferences.h>

extern Preferences globalPrefs; // Dichiarazione dell'oggetto globale
extern bool prefsInitialized;

void initPreferences(bool writeProtect);
void endPreferences();

void saveString(const char* key, const char* value);
String getString(const char* key, const String& defaultValue = ""); // Aggiunto valore di default

#endif