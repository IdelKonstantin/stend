#ifndef SENSOR_ASKER_H
#define SENSOR_ASKER_H

#include <Arduino.h>

namespace sensors {
  
  struct sensorsData {

    uint16_t intencity{0};
    uint16_t backTemp{0};
    uint16_t ABBTemp{0};
    bool zeroPosFlag{false}; 
  };

  void askSensors();
}

#endif /* SENSOR_ASKER_H */ 