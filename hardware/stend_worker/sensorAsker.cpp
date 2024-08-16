#include "sensorAsker.h"

#include "queues.h"

extern cppQueue outMSGQ;

void sensors::askSensorsTask() {
  
  String outcomingMSG;

  while(true) {

    //Проверка температуры 1 и 2
    //Проверка освещенности 
    //Проверка датчика положения
    //Интенсивность излучения 1,2,3 и бомбим серией сообщений
  }
}
