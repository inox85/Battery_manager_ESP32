#include "PINs.h"
#include "Battery.h"
#include "Params.h"
#include "Settings.h"

Battery* Battery::instance = nullptr;

esp_adc_cal_characteristics_t *adc_chars = nullptr;

RunningMedian AverageFilterVoltage(DIM_BUFFER_VOLTAGE);
RunningMedian AverageFilterCurrent(DIM_BUFFER_CURRENT);
RunningMedian AverageFilterExtVref(DIM_BUFFER_EXT_VREF);

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

float Battery::getExtVrefValue()
{
  return  currentVref;
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
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN, ADC_WIDTH, 1100, adc_chars);

  Vref = adc_chars->vref;

  switch (val_type) {
    case ESP_ADC_CAL_VAL_EFUSE_VREF:
     Serial.printf("eFuse, Vref: %u\n", Vref);
    break;
    case ESP_ADC_CAL_VAL_EFUSE_TP:
     Serial.printf("Two Point, Vref: %u\n", Vref);
    break;
    default: 
       Serial.printf("ADC not calibrated, Vref: %u\n", Vref);
      break;
  }

  // Configurazione dell'ADC
  adc1_config_width(ADC_WIDTH);
  adc1_config_channel_atten(BATTERY_VOLTAGE_PIN, ADC_ATTEN);
  adc1_config_channel_atten(BATTERY_CURRENT_PIN, ADC_ATTEN);
  adc1_config_channel_atten(EXT_VREF_PIN, ADC_ATTEN);
}

uint32_t Battery::getVref()
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

void Battery::analogMonitoring(bool voltage, bool current, bool extvref) 
{
   if(voltage) analog_Read(AverageFilterVoltage, BATTERY_VOLTAGE_PIN, currentVoltage, DISCARDS_VOLTAGE_VALUE);
   if(current) analog_Read(AverageFilterCurrent, BATTERY_CURRENT_PIN, currentCurrent, DISCARDS_CURRENT_VALUE);
   if(extvref) analog_Read(AverageFilterExtVref, EXT_VREF_PIN, currentVref, DISCARDS_EXT_VREF_VALUE);
}

void Battery::analog_Read(RunningMedian& filter, adc1_channel_t pin, float& output, int discardCount) 
{
    filter.add(adc1_get_raw(pin));
    if (filter.isFull()) {
        float median = filter.getMedianAverage(discardCount);
        output = esp_adc_cal_raw_to_voltage(median, adc_chars);
    }
}


