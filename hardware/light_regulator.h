#ifndef _LIGHTREGULATOR_WORKER_H_
#define _LIGHTREGULATOR_WORKER_H_
#include <SPI.h>

#define DAC2_CS_PIN      43 // PA1 - DAC2
#define DAC2_LDAC_PIN    41 // PA3 - LDAC2
#define DAC2_SHDN_PIN    40 // PA4 - SHDN-DAC2

#define LIGHT_INTENS_SIZE 13
#define FIRST_EMMITER 1
#define SECOND_EMMITER 2

class lightregulator {

private:
  int8_t m_inten1Index{0};
  int8_t m_inten2Index{0};

  const uint8_t m_intens_1[LIGHT_INTENS_SIZE] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 6y  , 100, 500, 4096};
  const uint8_t m_intens_2[LIGHT_INTENS_SIZE] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 500, 4096};

  void setintencity(uint8_t emmiterNum);
  void mcp4922(uint16_t value, uint8_t channel);

public:
  lightregulator();

  void init();
  void on(uint8_t emmiterNum);
  void off(uint8_t emmiterNum);
  void riseUp(uint8_t emmiterNum);
  void slowDown(uint8_t emmiterNum);
};

lightregulator::lightregulator() {};

void lightregulator::mcp4922(uint16_t value, uint8_t channel) {
  
  uint16_t data = 0x3000 | value;
  if (channel == 1) data |= 0x8000;
  digitalWrite(DAC2_CS_PIN, LOW);
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  SPI.transfer((uint8_t)(data >> 8));
  SPI.transfer((uint8_t)(data & 0xFF));
  SPI.endTransaction();
  digitalWrite(DAC2_CS_PIN, HIGH);
}

void lightregulator::setintencity(uint8_t emmiterNum) {

  if(emmiterNum == FIRST_EMMITER) {
    mcp4922(m_intens_1[m_inten1Index], 0);
  }
  else {
    mcp4922(m_intens_1[m_inten1Index], 1);
  }
}

void lightregulator::init() {
    
  pinMode(DAC2_CS_PIN, OUTPUT);
  digitalWrite(DAC2_CS_PIN, HIGH);  

  SPI.begin();

  off(FIRST_EMMITER);
  off(SECOND_EMMITER);
}

void lightregulator::on(uint8_t emmiterNum) {

  if(emmiterNum == FIRST_EMMITER) {

    m_inten1Index = LIGHT_INTENS_SIZE - 2;
    setintencity(FIRST_EMMITER);
  }
  else {
    m_inten2Index = LIGHT_INTENS_SIZE - 2;
    setintencity(SECOND_EMMITER);
  }
}

void lightregulator::off(uint8_t emmiterNum) {
  
  if(emmiterNum == FIRST_EMMITER) {

    m_inten1Index = 0;
    setintencity(FIRST_EMMITER);
  }
  else {
    m_inten2Index = 0;
    setintencity(SECOND_EMMITER);
  }
}

void lightregulator::riseUp(uint8_t emmiterNum) {

  if(emmiterNum == FIRST_EMMITER) {

    m_inten1Index++;
    if(m_inten1Index > LIGHT_INTENS_SIZE - 1) {
      m_inten1Index = LIGHT_INTENS_SIZE - 1;
    }

    setintencity(FIRST_EMMITER);
  }
  else {
    m_inten2Index++;
    if(m_inten2Index > LIGHT_INTENS_SIZE - 1) {
      m_inten2Index = LIGHT_INTENS_SIZE - 1;
    }
    setintencity(SECOND_EMMITER);
  }
}

void lightregulator::slowDown(uint8_t emmiterNum) {

  if(emmiterNum == FIRST_EMMITER) {

    m_inten1Index--;
    if(m_inten1Index < 0) {
      m_inten1Index = 0;
    }

    setintencity(FIRST_EMMITER);
  }
  else {
    m_inten2Index--;
    if(m_inten2Index < 0) {
      m_inten2Index = 0;
    }
    setintencity(SECOND_EMMITER);
  }
}

#endif /* _LIGHTREGULATOR_WORKER_H_ */
