#include "pinSetup.h"
#include <Arduino.h>

void stend::pinSetup() {

  pinMode(A4T_VENT_PIN, OUTPUT);
  digitalWrite(A4T_VENT_PIN, LOW);

  pinMode(A4T_HOT_PIN, OUTPUT);
  digitalWrite(A4T_HOT_PIN, LOW);

  pinMode(ZERO_SENSOR_PIN, INPUT);
  pinMode(CALIBRATION_PIN, INPUT);

  //TODO....
}