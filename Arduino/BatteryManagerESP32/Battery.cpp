#include <Filters.h>
#include "PINs.h"
#include "Battery.h"

FilterOnePole voltageFilter(LOWPASS, 1.0F);
FilterOnePole currentFilter(LOWPASS, 1.0F);

Battery::Battery()
{  

  
}

float Battery::getCurrentValue()
{
 
 
}

float Battery::getVoltageValue()
{
  

}