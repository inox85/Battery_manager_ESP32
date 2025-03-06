#include "PreferencesManager.h" // Includi il file header
#include <Preferences.h>
#include <Arduino.h>

Preferences globalPrefs; // Definizione dell'oggetto globale
bool prefsInitialized = false;


void initPreferences(bool writeProtect) {
  if (!prefsInitialized) {
    globalPrefs.begin("settings", writeProtect);
    prefsInitialized = true;
  }
}

void endPreferences() {
  globalPrefs.end();
  prefsInitialized = false;
}

void saveString(const char* key, const char* value) {
  initPreferences(false);
  globalPrefs.putString(key, value);
  endPreferences();
}

String getString(const char* key, const String& defaultValue) {
  initPreferences(true);
  String val = globalPrefs.getString(key, defaultValue);
  endPreferences();
  return val;
}

// String getStringSetting(String settingName)
// {

// }

// String saveStringSetting(String settingName)
// {

// }



// String getFloatSetting(String settingName)
// {

// }

// String saveFloatSetting(String settingName)
// {

// }