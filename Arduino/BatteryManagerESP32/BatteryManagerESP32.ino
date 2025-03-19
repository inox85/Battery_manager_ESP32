#include "Battery.h"
#include "AccessPoint.h"

AccessPoint accessPoint;

void setup() 
{

  Serial.begin(115200);

  Battery::getInstance()->setParamsADC();
  
  accessPoint.activate();

  accessPoint.startServer();

}

void loop() 
{
  Battery::getInstance()->monitoringRoutine();
}

