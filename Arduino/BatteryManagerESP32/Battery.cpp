#include <Filters.h>
#include "PINs.h"
#include "Battery.h"
#include "Params.h"

FilterOnePole voltageFilter(LOWPASS, VOLTAGE_FILTER_FREQ);
FilterOnePole currentFilter(LOWPASS, CURRENT_FILTER_FREQ);

Battery::Battery()
{  

  
}

float Battery::getCurrentValue()
{
 
 
}

float Battery::getVoltageValue()
{
  

}