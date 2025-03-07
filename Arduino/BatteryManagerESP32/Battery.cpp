#include <Filters.h>
#include "PINs.h"
#include "Battery.h"
#include "Params.h"
#include "Settings.h"

FilterOnePole voltageFilter(LOWPASS, VOLTAGE_FILTER_FREQ);
FilterOnePole currentFilter(LOWPASS, CURRENT_FILTER_FREQ);

Battery* Battery::instance = nullptr;

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
  return 1.1F;
}

float Battery::getVoltageValue()
{
  return 12.5F;
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
    // Da implementare la routine di controllo batteria
}

