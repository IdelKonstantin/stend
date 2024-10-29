#ifndef _LIGHTREGULATOR_WORKER_H_
#define _LIGHTREGULATOR_WORKER_H_

#include "MCP_DAC.h"
#include <SPI.h>

#define DAC2_CS_PIN      43 // PA1 - DAC2
#define DAC2_LDAC_PIN    41 // PA3 - LDAC2
#define DAC2_SHDN_PIN    40 // PA4 - SHDN-DAC2

#define LIGHT_INTENS_SIZE 13
#define FIRST_EMMITER 1
#define SECOND_EMMITER 2

class lightregulator {

private:
  MCP4921 m_mcp4922;
  int8_t m_inten1Index{0};
  int8_t m_inten2Index{0};

  const uint8_t m_intens_1[LIGHT_INTENS_SIZE] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120};
  const uint8_t m_intens_2[LIGHT_INTENS_SIZE] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120};

  void setintencity(uint8_t emmiterNum);

public:
  lightregulator();

  void init();
  void on(uint8_t emmiterNum);
  void off(uint8_t emmiterNum);
  void riseUp(uint8_t emmiterNum);
  void slowDown(uint8_t emmiterNum);
};

lightregulator::lightregulator() {};

void lightregulator::setintencity(uint8_t emmiterNum) {

  if(emmiterNum == FIRST_EMMITER) {
    m_mcp4922.fastWriteA(m_intens_1[m_inten1Index]);
  }
  else {
    m_mcp4922.fastWriteB(m_intens_2[m_inten2Index]);
  }
}

void lightregulator::init() {
  
  m_mcp4922.begin(DAC2_CS_PIN);
  m_mcp4922.setLatchPin(DAC2_LDAC_PIN);

  off(FIRST_EMMITER);
  off(SECOND_EMMITER);
}

void lightregulator::on(uint8_t emmiterNum) {

  if(emmiterNum == FIRST_EMMITER) {

    m_inten1Index = 6;
    setintencity(FIRST_EMMITER);
  }
  else {
    m_inten2Index = 6;
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