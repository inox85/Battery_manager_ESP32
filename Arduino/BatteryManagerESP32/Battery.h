#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

class Battery 
{
  private:

  float current_voltage = 0.0F;
  float current_current = 0.0F;

  float current_limit = 0.0F;
  float voltage_limit = 0.0F;

  public:
    Battery();

    float getCurrentValue();
    float getVoltageValue();
  
  };

#endif 