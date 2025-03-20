#include "Battery.h"
#include "AccessPoint.h"

AccessPoint accessPoint;

void setup() 
{

  Serial.begin(115200);

  Battery::getInstance()->setParamsADC();
  
  accessPoint.setActive();

}

void loop() 
{
  
  accessPoint.checkClientConnected();
  
  Battery::getInstance()->monitoringRoutine();

}
