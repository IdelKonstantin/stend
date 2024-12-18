#include "pid_regulator.h"
#include "motor_regulator.h"
//#include "light_regulator.h"

#define DELTAS_SIZE 10
#define START_THERMAL_DELTA 0
#define DEBOUNCE_DELAY delay(100);
#define RESTART_DELAY delay(5000);

namespace stend {

  String UARTbuffer;
  String UARTMessage;
  uint8_t setedDelta;
  uint16_t setedMiraSteps;

  const uint8_t firstIndex = 3;
  uint8_t lastIndex;

  void(* resetFunc) (void) = 0;
};

bool deltaIsSet{false};

pidRegulator pid;
motorRouter motor;
//lightregulator light;

void setup() {
  
  Serial.begin(9600);

  pid.init();
  pid.keepThermalDelta(START_THERMAL_DELTA);
  //light.init();
  motor.init();
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

      deltaIsSet = true;
  
      stend::lastIndex = stend::UARTbuffer.length();
      stend::setedDelta = static_cast<uint8_t>(stend::UARTbuffer.substring(stend::firstIndex, stend::lastIndex).toInt());

      stend::UARTMessage = "STDOK";
      stend::UARTMessage += String{(uint16_t)stend::setedDelta};
      Serial.println(stend::UARTMessage);
      
      pid.keepThermalDelta(stend::setedDelta);
      DEBOUNCE_DELAY;
      continue;
    }
     
//    /* ИЗЛУЧАТЕЛИ - ВКЛ */
//    if(stend::UARTbuffer == "LT1ON") {
//      Serial.println("LT1ONOK");
//      light.on(FIRST_EMMITER);
//      DEBOUNCE_DELAY;
//      continue;
//    }
//    if(stend::UARTbuffer == "LT2ON") {
//      Serial.println("LT1ONOK");
//      light.on(SECOND_EMMITER);
//      DEBOUNCE_DELAY;
//      continue;
//    }
//    /* ИЗЛУЧАТЕЛИ - ВЫКЛ */
//    if(stend::UARTbuffer == "LT1OFF") {
//      Serial.println("LT1FFOK");
//      light.off(FIRST_EMMITER);
//      DEBOUNCE_DELAY;
//      continue;
//    }
//    if(stend::UARTbuffer == "LT2OFF") {
//      Serial.println("LT2FFOK");
//      light.off(SECOND_EMMITER);
//      DEBOUNCE_DELAY;
//      continue;
//    }
//    /* ИЗЛУЧАТЕЛИ - увеличить интенсивность */
//    if(stend::UARTbuffer == "LT1U") {
//      Serial.println("LT1UOK");
//      light.riseUp(FIRST_EMMITER);
//      DEBOUNCE_DELAY;
//      continue;
//    }
//    if(stend::UARTbuffer == "LT2U") {
//      Serial.println("LT2UOK");
//      light.riseUp(SECOND_EMMITER);
//      DEBOUNCE_DELAY;
//      continue;
//    }
//    /* ИЗЛУЧАТЕЛИ - снизить интенсивность */
//    if(stend::UARTbuffer == "LT1D") {
//      Serial.println("LT1DOK");
//      light.slowDown(FIRST_EMMITER);
//      DEBOUNCE_DELAY;
//      continue;
//    }
//    if(stend::UARTbuffer == "LT2D") {
//      Serial.println("LT2DOK");
//      light.slowDown(SECOND_EMMITER);
//      DEBOUNCE_DELAY;
//      continue;
//    }
    /* АЧТ ВКЛ */
    if(stend::UARTbuffer.startsWith("BBON")) {
      Serial.println("BBONOK");
      pid.on();
      DEBOUNCE_DELAY;
      continue;
    }
    /* АЧТ ВЫКЛ */
    if(stend::UARTbuffer.startsWith("BBOFF")) {
      Serial.println("BBOFFOK");
      pid.off();
      DEBOUNCE_DELAY;
      continue;
    }
    /* Вентилятор ВКЛ */
    if(stend::UARTbuffer.startsWith("VON")) {
      Serial.println("VONOK");
      pid.turnONVentilator();
      DEBOUNCE_DELAY;
      continue;
    }
    /* Вентилятор ВЫКЛ */
    if(stend::UARTbuffer.startsWith("VOFF")) {
      Serial.println("VOFFOK");
      pid.turnOFFVentilator();
      DEBOUNCE_DELAY;
      continue;
    }
    /* Перезагрузка устройства */
    if(stend::UARTbuffer.startsWith("RST")) {
      Serial.println("RSTOK");
      RESTART_DELAY;
      stend::resetFunc();
    }
  
    /* Сбросить "нулевое" положение миры */
    if(stend::UARTbuffer.startsWith("MRZ")) {
      Serial.println("MRZOK");
      motor.eraseStartPosition();
      DEBOUNCE_DELAY;
      continue;
    }  
    /* Передвинуть миру вперед на один шаг */
    if(stend::UARTbuffer.startsWith("MFS")) {
      motor.miraJustOneForwardStep();
      DEBOUNCE_DELAY;
      continue;
    }
    /* Передвинуть миру назад на один шаг */
    if(stend::UARTbuffer.startsWith("MBS")) {
      motor.miraJustOneBackwardStep();
      DEBOUNCE_DELAY;
      continue;
    }
  
    /* Установить "нулевое" положение миры */
    if(stend::UARTbuffer.startsWith("MSZ")) {
      Serial.println("MSZOK");
      motor.savecurrentPositionAsStart();
      DEBOUNCE_DELAY;
      continue;
    }
    /* Передвинуть миру вперед (+) или назад (-) */
    if(stend::UARTbuffer.startsWith("MMF")) {
            
      stend::lastIndex = stend::UARTbuffer.length();
      stend::setedMiraSteps = static_cast<uint16_t>(stend::UARTbuffer.substring(stend::firstIndex, stend::lastIndex).toInt());
      motor.goForward(stend::setedMiraSteps);
      DEBOUNCE_DELAY;
      continue;
    }
    if(stend::UARTbuffer.startsWith("MMB")) {
      
      stend::lastIndex = stend::UARTbuffer.length();
      stend::setedMiraSteps = static_cast<uint16_t>(stend::UARTbuffer.substring(stend::firstIndex, stend::lastIndex).toInt());
      motor.goBackward(stend::setedMiraSteps);
      DEBOUNCE_DELAY;
      continue;
    }
  } 
}
