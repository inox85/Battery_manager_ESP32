#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

class Battery 
{
  private:

    static Battery* instance;
    Battery();

    float nominalVoltage = 0.0F;

    float currentVoltage = 0.0F;
    float currentCurrent = 0.0F;

    float currentLimit = 0.0F;
    float voltageLimit = 0.0F;

  public:

    static Battery* getInstance();

    float getCurrentValue();
    float getVoltageValue();

    void setCurrentLimit(float value);
    void setVoltageLimit(float value);
    float getCurrentLimit();
    float getVoltageLimit();

    void setNominalVoltage(float value);
    float getNominalVoltage();

    void monitoringRoutine();
    
  };

#endif 