#include "pid_regulator.h"
#include "light_regulator.h"
#include "motor_regulator.h"

#define DELTAS_SIZE 10
#define START_THERMAL_DELTA 0

namespace stend {

  String UARTbuffer;
  uint8_t setedDelta;
  uint16_t setedMiraSteps;

  const uint8_t firstIndex = 3;
  uint8_t lastIndex;

  void(* resetFunc) (void) = 0;
};

pidRegulator pid;
lightregulator light;
motorRouter motor;

void setup() {
  
  Serial.begin(9600);

  pid.init();
  pid.keepThermalDelta(START_THERMAL_DELTA);

  light.init();
  motor.init();

  Serial.println("STROK");
}

void loop() {

  pid.tickTackToe();

  while (Serial.available() > 0) {
    
    stend::UARTbuffer = Serial.readStringUntil('\n');

    if(stend::UARTbuffer.length() == 0) {
      continue;
    }
    
    /* Установка температурной дельты - STD30 - 3 градуса и т.д.*/
    if(stend::UARTbuffer.startsWith("STD")) { 

      stend::lastIndex = stend::UARTbuffer.length();
      stend::setedDelta = static_cast<uint8_t>(stend::UARTbuffer.substring(stend::firstIndex, stend::lastIndex).toInt());

      Serial.print("STDOK");
      Serial.println(stend::setedDelta);
      pid.keepThermalDelta(stend::setedDelta);
      continue;
    }
    /* ИЗЛУЧАТЕЛИ - ВКЛ */
    if(stend::UARTbuffer == "LT1ON") {
      Serial.println("LT1ONOK");
      light.on(FIRST_EMMITER);
      continue;
    }
    if(stend::UARTbuffer == "LT2ON") {
      Serial.println("LT1ONOK");
      light.on(SECOND_EMMITER);
      continue;
    }
    /* ИЗЛУЧАТЕЛИ - ВЫКЛ */
    if(stend::UARTbuffer == "LT1OFF") {
      Serial.println("LTOFFOK");
      light.off(FIRST_EMMITER);
      continue;
    }
    if(stend::UARTbuffer == "LT2OFF") {
      Serial.println("LTOFFOK");
      light.off(SECOND_EMMITER);
      continue;
    }
    /* ИЗЛУЧАТЕЛИ - увеличить интенсивность */
    if(stend::UARTbuffer == "LT1U") {
      Serial.println("LT1UOK");
      light.riseUp(FIRST_EMMITER);
      continue;
    }
    if(stend::UARTbuffer == "LT2U") {
      Serial.println("LT2UOK");
      light.riseUp(SECOND_EMMITER);
      continue;
    }
    /* ИЗЛУЧАТЕЛИ - снизить интенсивность */
    if(stend::UARTbuffer == "LT1D") {
      Serial.println("LT1DOK");
      light.slowDown(FIRST_EMMITER);
      continue;
    }
    if(stend::UARTbuffer == "LT2D") {
      Serial.println("LT2DOK");
      light.slowDown(SECOND_EMMITER);
      continue;
    }
    /* АЧТ ВКЛ */
    if(stend::UARTbuffer == "BBON") {
      Serial.println("BBONOK");
      pid.on();
      continue;
    }
    /* АЧТ ВЫКЛ */
    if(stend::UARTbuffer == "BBOFF") {
      Serial.println("BBOFFOK");
      pid.off();
      continue;
    }
    /* Вентилятор ВКЛ */
    if(stend::UARTbuffer == "VON") {
      Serial.println("VONOK");
      pid.turnONVentilator();
      continue;
    }
    /* Вентилятор ВЫКЛ */
    if(stend::UARTbuffer == "VOFF") {
      Serial.println("VOFFOK");
      pid.turnOFFVentilator();
      continue;
    }
    /* Перезагрузка устройства */
    if(stend::UARTbuffer == "RST") {
      Serial.println("RSTOK");
      delay(100);
      stend::resetFunc();
    }
    /* Передвинуть миру вперед на один шаг */
    if(stend::UARTbuffer == "MFS") {
      Serial.println("MFSOK");
      motor.makeStepForward();
      motor.stop();
      continue;
    }
    /* Передвинуть миру назад на один шаг */
    if(stend::UARTbuffer == "MBS") {
      Serial.println("MBSOK");
      motor.makeStepBackward();
      motor.stop();
      continue;
    }

    /* Установить "нулевое" положение миры */
    if(stend::UARTbuffer == "MSZ") {
      Serial.println("MSZOK");
      motor.savecurrentPositionAsStart();
      continue;
    }
    
    /* Сбросить "нулевое" положение миры */
    if(stend::UARTbuffer == "MRZ") {
      Serial.println("MRZOK");
      motor.eraseStartPosition();
      continue;
    }

    /* Передвинуть миру вперед (+) или назад (-) */
    if(stend::UARTbuffer.startsWith("MMF")) {
            
      stend::lastIndex = stend::UARTbuffer.length();
      stend::setedMiraSteps = static_cast<uint16_t>(stend::UARTbuffer.substring(stend::firstIndex, stend::lastIndex).toInt());
      
      Serial.print("MMFOK");
      Serial.println(stend::setedMiraSteps);
      motor.goForward(stend::setedMiraSteps);
      continue;
    }
    if(stend::UARTbuffer.startsWith("MMB")) {
      
      stend::lastIndex = stend::UARTbuffer.length();
      stend::setedMiraSteps = static_cast<uint16_t>(stend::UARTbuffer.substring(stend::firstIndex, stend::lastIndex).toInt());
      
      Serial.print("MMBOK");
      Serial.println(stend::setedMiraSteps);
      motor.goBackward(stend::setedMiraSteps);
      continue;
    }
  }
}