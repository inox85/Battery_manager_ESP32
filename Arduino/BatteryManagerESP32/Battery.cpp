#include <driver/adc.h>
#include <esp_adc_cal.h>
#include "PINs.h"
#include "Battery.h"
#include "Params.h"
#include "Settings.h"

Battery* Battery::instance = nullptr;

esp_adc_cal_characteristics_t *adc_chars;

Battery::Battery() 
{
  // Carico tutti i settings dalla memoria
  this->nominalVoltage = Settings::getInstance()->readFloat("nominalVoltage");
  this->voltageLimit = Settings::getInstance()->readFloat("voltageLimit");
  this->currentLimit = Settings::getInstance()->readFloat("currentLimit");
  
}

Battery* Battery::getInstance() {
  if (!instance) {
    instance = new Battery();
  }
  return instance;
}


float Battery::getCurrentValue()
{
  return currentCurrent;
}

float Battery::getVoltageValue()
{
  return currentVoltage;
}

void Battery::setVoltageLimit(float value)
{
    Settings::getInstance()->writeFloat("voltageLimit", value);
    this->voltageLimit = Settings::getInstance()->readFloat("voltageLimit");
    Serial.print("Limite di tensione impostato: ");
    Serial.println(this->voltageLimit);
}

void Battery::setCurrentLimit(float value)
{ 
    Settings::getInstance()->writeFloat("currentLimit", value);
    this->currentLimit = Settings::getInstance()->readFloat("currentLimit");
    Serial.print("Limite di corrente impostato: ");
    Serial.println(this->currentLimit);
    
}

void Battery::setNominalVoltage(float value)
{
    Settings::getInstance()->writeFloat("nominalVoltage", value);
    this->nominalVoltage = Settings::getInstance()->readFloat("nominalVoltage");
    Serial.print("Tensione nominale impostata: ");
    Serial.println(this->nominalVoltage);
}

void Battery::setParamsADC()
{
  adc_chars = (esp_adc_cal_characteristics_t *) calloc(1, sizeof(esp_adc_cal_characteristics_t));
  
  // Caratterizzazione dell'ADC: controlla se esistono dati calibrati negli eFuse
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN, ADC_WIDTH, ESP_ADC_CAL_VAL_EFUSE_VREF, adc_chars);

  Vref = adc_chars->vref;
  
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
      Serial.print("eFuse Vref: ");
      Serial.println(Vref);
  } 
  else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
      Serial.print("Two Point calibration: ");
      Serial.println(Vref);
  } 
  else {
      Serial.print("Default calibration: ");
      Serial.println(Vref);
  }

  // Configurazione dell'ADC
  adc1_config_width(ADC_WIDTH);
  adc1_config_channel_atten(BATTERY_VOLTAGE_PIN, ADC_ATTEN);
  adc1_config_channel_atten(BATTERY_CURRENT_PIN, ADC_ATTEN);
}

int Battery::getVref()
{
  return this->Vref;
}

float Battery::getNominalVoltage()
{
  return this->nominalVoltage;
}

float Battery::getCurrentLimit()
{
  return this->currentLimit;
}

float Battery::getVoltageLimit()
{
  return this->voltageLimit;
}

void Battery::monitoringRoutine()
{
    uint32_t rawVoltage = adc1_get_raw(BATTERY_VOLTAGE_PIN);
    uint32_t rawCurrent = adc1_get_raw(BATTERY_CURRENT_PIN);

    currentVoltage = esp_adc_cal_raw_to_voltage(rawVoltage, adc_chars);
    currentCurrent = esp_adc_cal_raw_to_voltage(rawCurrent, adc_chars);
}

