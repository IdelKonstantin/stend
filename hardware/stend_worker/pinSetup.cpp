#include "pinSetup.h"
#include <Arduino.h>

extern bool kalibrationAlowed;

void stend::pinSetup() {

  pinMode(A4T_VENT_PIN, OUTPUT);
  digitalWrite(A4T_VENT_PIN, LOW);

  pinMode(A4T_HOT_PIN, OUTPUT);
  analogWrite(A4T_HOT_PIN, 127); //todo read from EEPROM

  pinMode(ZERO_SENSOR_PIN, INPUT);
  pinMode(CALIBRATION_PIN, INPUT);

  pinMode(S1_PIN, INPUT);
  pinMode(S2_PIN, INPUT);

  pinMode(DAC2_SHDN_PIN, OUTPUT);
  digitalWrite(A4T_HOT_PIN, LOW);

  pinMode(MOTOR_A_MIN_PIN, OUTPUT);
  digitalWrite(A4T_VENT_PIN, LOW);

  pinMode(MOTOR_A_PLUS_PIN, OUTPUT);
  digitalWrite(A4T_VENT_PIN, LOW);

  pinMode(MOTOR_B_MIN_PIN, OUTPUT);
  digitalWrite(A4T_VENT_PIN, LOW);

  pinMode(MOTOR_B_PLUS_PIN, OUTPUT);
  digitalWrite(A4T_VENT_PIN, LOW);

  stend::checkKalibrationAllowance();
}

void stend::checkKalibrationAllowance() {
  kalibrationAlowed = digitalRead(CALIBRATION_PIN) == LOW;
}