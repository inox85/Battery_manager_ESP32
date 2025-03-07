#include "Battery.h"
#include "AccessPoint.h"

AccessPoint accessPoint;

void setup() 
{

  Serial.begin(115200);
  
  accessPoint.activate();

  accessPoint.startServer();

}

void loop() 
{
  Battery::getInstance()->monitoringRoutine();
}

