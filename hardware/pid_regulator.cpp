#include "pid_regulator.h"
#include <avr/io.h>
#include <string.h>

#include <Arduino.h>

pidRegulator::pidRegulator() {};

void pidRegulator::init() const {

  m_pwm.init();
  m_pidADC.init();  
}

void pidRegulator::turnONVentilator() const {
  digitalWrite(A4T_VENT_PIN, HIGH);
}

void pidRegulator::turnOFFVentilator() const {
  digitalWrite(A4T_VENT_PIN, LOW);
}

void pidRegulator::tickTackToe() {

  m_currentTime = millis();

  if(m_currentTime - m_previousTime >= PID_MEASURE_INTERVAL_MSEC) {

    if(m_warmerData.pwm == 0) {
      Serial.println("CTD0");
      m_pwm.setIntencity(0);

      m_previousTime = millis();
      return;
    }

    m_ADCdiffRead = readCurrentADCDiff();
    
    Serial.print("CTD");
    Serial.println((uint16_t)readCurrentThermalDiff());

    if(m_warmerData.warmDirection == PID_COOL_DOWN) {

      if(m_ADCdiffRead > m_warmerData.adc - PID_THRESHOLD) {
          turnONVentilator();
          m_pwm.setIntencity(1);
      }
      else {
        turnOFFVentilator();

        if(m_ADCdiffRead > m_warmerData.adc) {
          m_pwm.setIntencity(m_warmerData.pwm - PID_STEP);
        }
        else {
          m_pwm.setIntencity(m_warmerData.pwm);
        }
      }
    }
    else if(m_warmerData.warmDirection == PID_WARM_UP) {

      if(m_ADCdiffRead > m_warmerData.adc) {
        m_pwm.setIntencity(m_warmerData.pwm - PID_STEP);
      }
      else {    
        m_pwm.setIntencity(m_warmerData.pwm);
      }
    }

    m_previousTime = millis();
  }
}

void pidRegulator::keepThermalDelta(uint8_t thermalDelta) {

  m_warmerData = m_pidParams.dataForThermalDelta(thermalDelta);
}

void pidRegulator::on() {
  keepThermalDelta(PID_ON_THERMAL_DELTA);
}

void pidRegulator::off() {
  keepThermalDelta(0);
}

stend::ADC_diff_t pidRegulator::readCurrentADCDiff() const {

  if(m_warmerData.resolution == ADC_RESOLUTION_x1) {

    m_pidADC.setResolutionX1();
    return m_pidADC.readBufferized(PID_DIFFERENTIAL_READ);
  }
  
  m_pidADC.setResolutionX10();
  return m_pidADC.readBufferized(PID_DIFFERENTIAL_READ);
}

stend::ADC_resolution_t pidRegulator::readCurrentADCResolution() const {

  return m_warmerData.resolution;
}

stend::ADC_t pidRegulator::readCurrentADC0() const {

   m_pidADC.setADC0read();
   return m_pidADC.readBufferized(ADC_PID_REGULAR_READ);
}

uint8_t pidRegulator::readCurrentThermalDiff() {

  m_ADCdiffRead = readCurrentADCDiff();
  return m_pidParams.currentThermalDelta(m_ADCdiffRead, m_warmerData.resolution);
}

stend::PWM_t pidRegulator::readCurrentPWM() const {

  return m_warmerData.pwm; 
}

/*******************************************************************************************************/

adcPIDWorker::adcPIDWorker() {};

int16_t adcPIDWorker::read() {

  ADCSRA |= (1 << ADSC);
  while (!(ADCSRA & (1 << ADSC)));

  m_resRaw = (uint16_t)(ADCL | ADCH << 8);

  return m_resRaw;
}

void adcPIDWorker::resetAndRereadBuffer() {

  memset(m_buff, 0, sizeof(int16_t) * ADC_PID_BUFF_SIZE);

  for(uint8_t i = 0; i < ADC_PID_BUFF_SIZE; i++) {
    m_buff[i] = read();
  }
}

void adcPIDWorker::init() const {
  
  ADCSRA |= (1 << ADEN);
  ADCSRA &= ~(1 << ADPS2) & ~ (1 << ADPS1) & ~(1 << ADPS0);
  ADMUX |= (1 << REFS0);
}

void adcPIDWorker::setResolutionX1() const {
  
  //ADC3 - ABC2 x 1 (MUX[4:0] = 0b11011)
  ADMUX &= ~(1 << MUX2);
  ADMUX |= (1 << MUX0) | (1 << MUX1) | (1 << MUX3) | (1 << MUX4);
}

void adcPIDWorker::setResolutionX10() const {
  
  //ADC3 - ABC2 x 10 (MUX[4:0] = 0b01101)
  ADMUX &= ~(1 << MUX1) & ~(1 << MUX4);
  ADMUX |= (1 << MUX0) | (1 << MUX2) | (1 << MUX3);
}

void adcPIDWorker::setADC0read() const {

  //ADC0 as regular read (MUX[4:0] = 0b0000)
  ADMUX &= ~(1 << MUX0) & ~(1 << MUX1) & ~(1 << MUX2) & ~(1 << MUX3) & ~(1 << MUX4);
}

int16_t adcPIDWorker::readBufferized(bool diffRead) {
  
  resetAndRereadBuffer();
  resetAndRereadBuffer();
  resetAndRereadBuffer();

  m_sum = 0;
  m_bufferizedValue = 0;

  for(uint8_t i = 2; i < ADC_PID_BUFF_SIZE; i++) {
    m_sum += m_buff[i];
  }

  m_bufferizedValue = (m_sum >> 3);

  if(diffRead == ADC_PID_REGULAR_READ) {
    return m_bufferizedValue;    
  }

  if(m_bufferizedValue > 512) {
    return 512 - m_bufferizedValue; 
  }

  return m_bufferizedValue;
}

pwmWorker::pwmWorker() {};

void pwmWorker::init() const {

    DDRB |= (1 << PB5);
    PORTB &= ~(1 << PB5);
    OCR1A = 0;

    //Fast PWM with downcounting
    TCCR1A |= (1 << WGM11) | (1 << WGM10);
    TCCR1B |= (1 << WGM12);

    TCCR1A |= (1 << COM1A1);

    //Pre-scailer 256
    TCCR1B |= (1 << CS12);
    TCCR1B &= ~(1 << CS11) & ~(1 << CS10);
}

void pwmWorker::setIntencity(uint16_t intencity) const {

    if(intencity > 1023) {
        intencity = 1023;
    }

    OCR1A = intencity;
}

const stend::warmer_data_t& warmerData::dataForThermalDelta(uint8_t thermalDelta) {

  if(thermalDelta == 0) {
    
    m_warmerData.pwm = 0;
    m_warmerData.adc = 0;
    m_warmerData.resolution = ADC_RESOLUTION_x1;
    return m_warmerData;   
  }

  if(thermalDelta == m_warmerData.prevDelta) {
    return m_warmerData;
  }
  
  m_warmerData.warmDirection = (thermalDelta > m_warmerData.prevDelta) ? PID_WARM_UP : PID_COOL_DOWN;
  m_warmerData.prevDelta = thermalDelta;

  switch(thermalDelta) {
      case 10:
        m_warmerData.adc = 63;
        break;
      case 20:
        m_warmerData.adc = 87;
        break;
      case 30:
        m_warmerData.adc = 112;
        break;
      case 40:
        m_warmerData.adc = 121;
        break;
      case 50:
        m_warmerData.adc = 124;
        break;
      case 60:
        m_warmerData.adc = 127;
        break;
      case 70:
        m_warmerData.adc = 133;
        break;
      case 80:
        m_warmerData.adc = 141;
        break;
      case 90:
        m_warmerData.adc = 151;
        break;
      case 100:
        m_warmerData.adc = 161;
        break;
  }

  switch(thermalDelta) {
      case 10:
        m_warmerData.pwm = 100;  
        break;
      case 20:
        m_warmerData.pwm = 135;  
        break;
      case 30:
        m_warmerData.pwm = 183;  
        break;
      case 40:
        m_warmerData.pwm = 212;  
        break;
      case 50:
        m_warmerData.pwm = 229;  
        break;
      case 60:
        m_warmerData.pwm = 246;  
        break;
      case 70:
        m_warmerData.pwm = 270;  
        break;
      case 80:
        m_warmerData.pwm = 295;  
        break;
      case 90:
        m_warmerData.pwm = 350;  
        break;
      case 100:
        m_warmerData.pwm = 411;  
        break;
  }

  m_warmerData.resolution = ADC_RESOLUTION_x1;

  return m_warmerData;
}

uint8_t warmerData::currentThermalDelta(stend::ADC_diff_t adcDiff, stend::ADC_resolution_t res) {

  if(res == ADC_RESOLUTION_x1) {
    
    if(adcDiff < 63) {
      return MIN_POSSIBLE_VALUE;
    }

    if(adcDiff >= 185) {
      return MAX_POSSIBLE_VALUE;
    }

    switch(adcDiff) {
      case 63:
        return 10;
      case 87:
        return 20;
      case 112:
        return 30;
      case 121:
        return 40;
      case 124:
        return 50;
      case 127:
        return 60;
      case 133:
        return 70;
      case 141:
        return 80;
      case 151:
        return 90;
      case 161:
        return 100;
    }

    stend::ADC_diff_t startADC, endADC;
    uint8_t startDelta, endDelta;

    if(adcDiff > 63 && adcDiff < 87) {

      startADC = 63;
      endADC = 87;
      startDelta = 10;
      endDelta = 20;
      goto CALCULATE;
    }
    else if(adcDiff > 87 && adcDiff < 112) {
      startADC = 87;
      endADC = 112;
      startDelta = 20;
      endDelta = 30;
      goto CALCULATE;      
    }
    else if(adcDiff > 112 && adcDiff < 121) {
      startADC = 112;
      endADC = 121;
      startDelta = 30;
      endDelta = 40;
      goto CALCULATE;      
    }
    else if(adcDiff > 121 && adcDiff < 124) {
      startADC = 121;
      endADC = 124;
      startDelta = 40;
      endDelta = 50;
      goto CALCULATE;      
    }
    else if(adcDiff > 124 && adcDiff < 127) {
      startADC = 124;
      endADC = 127;
      startDelta = 50;
      endDelta = 60;
      goto CALCULATE;      
    }
    else if(adcDiff > 127 && adcDiff < 133) {
      startADC = 127;
      endADC = 133;
      startDelta = 60;
      endDelta = 70;
      goto CALCULATE;      
    }
    else if(adcDiff > 133 && adcDiff < 141) {
      startADC = 133;
      endADC = 141;
      startDelta = 70;
      endDelta = 80;
      goto CALCULATE;      
    }
    else if(adcDiff > 141 && adcDiff < 151) {
      startADC = 141;
      endADC = 151;
      startDelta = 80;
      endDelta = 90;
      goto CALCULATE;      
    }
    else if(adcDiff > 151 && adcDiff < 161) {
      startADC = 151;
      endADC = 161;
      startDelta = 90;
      endDelta = 100;
      goto CALCULATE;      
    }
    else if(adcDiff > 161 && adcDiff < 175) {
      startADC = 161;
      endADC = 175;
      startDelta = 100;
      endDelta = 110;
      goto CALCULATE;      
    }
    else if(adcDiff > 175 && adcDiff < 185) {
      startADC = 175;
      endADC = 185;
      startDelta = 110;
      endDelta = 120;
      goto CALCULATE;      
    }

CALCULATE:
    float currentDelta = startDelta + (endDelta - startDelta) * 1.0 * (adcDiff - startADC)/(endADC - startADC); 
    return currentDelta;
  }
  else {
    return MAX_POSSIBLE_VALUE;
  }
}