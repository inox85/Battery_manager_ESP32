#include "Settings.h"

Settings* Settings::instance = nullptr;

Settings::Settings() {
  preferences.begin("my_settings"); // Namespace per le impostazioni
}

Settings* Settings::getInstance() {
  if (!instance) {
    instance = new Settings();
  }
  return instance;
}

void Settings::writeFloat(const char* key, float value) {
  preferences.putFloat(key, value);
}

float Settings::readFloat(const char* key, float defaultValue) {
  return preferences.getFloat(key, defaultValue);
}

void Settings::writeString(const char* key, const String& value) {
  preferences.putString(key, value);
}

String Settings::readString(const char* key, const String& defaultValue) {
  return preferences.getString(key, defaultValue);
}