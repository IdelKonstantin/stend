#include "sensorAsker.h"
#include "pinSetup.h"

#include <string.h>

static sensors::sensorsData sd;

inline static void sendSensorsDatatoUART();

void sensors::askSensors() {
  
  memset(&sd, 0, sizeof(sensorsData));

  sd.intencity = analogRead(ILLUM_SENSOR_PIN);
  sd.backTemp = analogRead(BCGR_TEMP_PIN);
  sd.ABBTemp = analogRead(A4T_TEMP_PIN);
  sd.zeroPosFlag = digitalRead(ZERO_SENSOR_PIN);

  sendSensorsDatatoUART();
}

inline static void sendSensorsDatatoUART() {

  Serial.print("SNS");
  Serial.print(sd.intencity);
  Serial.print(",");
  Serial.print(sd.backTemp);
  Serial.print(",");
  Serial.print(sd.ABBTemp);
  Serial.print(",");
  Serial.print(sd.zeroPosFlag);
}
