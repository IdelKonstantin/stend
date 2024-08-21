#ifndef STEND_PIN_SETUP_H
#define STEND_PIN_SETUP_H

#define A4T_VENT_PIN      32 //PC4 - VENT
#define A4T_HOT_PIN       13 //PB5 - HOT
#define ZERO_SENSOR_PIN   4 //PE4 - DAT"0"
#define CALIBRATION_PIN   2 //PE2 - CALIBR
#define ILLUM_SENSOR_PIN  A4 //(49) PF4 - FD

#define BCGR_TEMP_PIN     A0 //(45) or A2(46) PF0 || PF2 - DT1
#define A4T_TEMP_PIN      A1 //(47) or A3(48) PF1 || PF3 - DT2

#define S1_PIN            6 // PE6 LS-L
#define S2_PIN            7 // PE7 LS-R

#define SPI_INV_SS_PIN    8 // PB0 - ~SS
#define SPI_SCK_PIN       9 // PB1 - SCK
#define SPI_MOSI_PIN      10 // PB2 - MOSI
#define SPI_MISO_PIN      11 // PB3 - MISO
#define DAC1_CS_PIN      44 // PA0 - DAC1
#define DAC1_LDAC_PIN    43 // PA2 - LDAC1
#define DAC2_CS_PIN      42 // PA1 - DAC2
#define DAC2_LDAC_PIN    41 // PA3 - LDAC2
#define DAC2_SHDN_PIN    40 // PA4 - SHDN-DAC2

#define MOTOR_A_MIN_PIN  29 // PC1 - UA-
#define MOTOR_A_PLUS_PIN 31 // PC3 - UA+
#define MOTOR_B_MIN_PIN  28 // PC0 - UB-
#define MOTOR_B_PLUS_PIN 30 // PC2 - UB+

namespace stend {
  
  void pinSetup();
  void checkKalibrationAllowance();
};

#endif /* STEND_PIN_SETUP_H */