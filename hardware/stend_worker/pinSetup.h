#ifndef STEND_PIN_SETUP_H
#define STEND_PIN_SETUP_H

#define A4T_VENT_PIN    PC4
#define A4T_HOT_PIN     PB5
#define ZERO_SENSOR_PIN PE4
#define CALIBRATION_PIN PE2

namespace stend {
  void pinSetup();
};

#endif /* STEND_PIN_SETUP_H */