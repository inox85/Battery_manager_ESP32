#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>
#include <RunningMedian.h>

class Battery 
{
  private:

    static Battery* instance;
    Battery();

    float nominalVoltage = 0.0F;

    float currentVoltage = 0.0F;
    float currentCurrent = 0.0F;
    float currentVref = 0.0F;

    float currentLimit = 0.0F;
    float voltageLimit = 0.0F;

    uint32_t Vref = 0;

  public:

    static Battery* getInstance();

    float getCurrentValue();
    float getVoltageValue();
    float getExtVrefValue();

    void setCurrentLimit(float value);
    void setVoltageLimit(float value);
    float getCurrentLimit();
    float getVoltageLimit();

    void setNominalVoltage(float value);
    float getNominalVoltage();

    void analogMonitoring(bool voltage, bool current, bool extVref);
    void analog_Read(RunningMedian& filter, adc1_channel_t pin, float& output, int discardCount);

    void setParamsADC();
    uint32_t getVref();
  };

#endif 