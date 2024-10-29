#include <EEPROM.h>

#define MOTOR_A_MIN_PIN  29 // PC1 - UA-
#define MOTOR_A_PLUS_PIN 31 // PC3 - UA+
#define MOTOR_B_MIN_PIN  28 // PC0 - UB-
#define MOTOR_B_PLUS_PIN 30 // PC2 - UB+
#define ZERO_SENSOR_PIN   4 //PE4 - DAT"0"

#define TACKT 20

class motorRouter {

private:
  const uint8_t m_forwardStrobes[4] = {
    0b1001,
    0b0011,
    0b0110,
    0b1100,
  };
  const uint8_t m_backwardStrobes[4] = {
    0b0110,
    0b0011,
    0b1001,
    0b1100 
  };
  
  size_t m_forwardIndex{0};
  size_t m_backwardIndex{0};
  int16_t m_stepsCounter{0};
  bool m_canRun{false};

private:

  void setPC(int value) {
    
    PORTC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3));
    PORTC |= (value & 0x0F);
  }
  
  void nextForwardStrobe() {
    m_forwardIndex++;
    
    if(m_forwardIndex > 3) {
      m_forwardIndex = 0;
    }
  }

  void nextBackwardStrobe() {
    
    m_backwardIndex++;

    if(m_backwardIndex > 3) {
      m_backwardIndex = 0;
    }
  }

  void gotoStartPosition() {

    resetCounter();

    int16_t steps;
    steps = EEPROM.get(0, steps);

    if(steps == -1) {
      return;
    }

    for(int i = 0; i < steps; i++) {
      makeStepForward();
    }

    stop();
  }
  
  void findZero() {

    if(digitalRead(ZERO_SENSOR_PIN) == 1) { //Каретка не в нуле, сдаем назад

      while(true) {

        if(digitalRead(ZERO_SENSOR_PIN) == 0) {
          stop();
          break;
        }
        makeStepBackward();
      }
    }
    else { //Если в нуле, сдаем вперед
      while(true) {

        if(digitalRead(ZERO_SENSOR_PIN) == 1) {
          stop();
          break;
        }
        makeStepForward();
      }

      while(true) {
        
        if(digitalRead(ZERO_SENSOR_PIN) == 0) {
          stop();
          break;
        }
        makeStepBackward();
      }
    }
  }

public:
  
  motorRouter() {};

  void resetCounter() {
    m_stepsCounter = 0;
  }

  int16_t getCounter() {
    return m_stepsCounter;
  }

  void stop() {
    
    setPC(0b0000);
    _delay_ms(TACKT);
  }

  void makeStepForward() {

    setPC(m_forwardStrobes[m_forwardIndex]);
    _delay_ms(TACKT);
    nextForwardStrobe();
    m_stepsCounter++;
  }
  void makeStepBackward() {

    setPC(m_backwardStrobes[m_backwardIndex]);
    _delay_ms(TACKT);
    nextBackwardStrobe();
    m_stepsCounter--;
  }

  void init() {
    DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3);
    findZero();
    gotoStartPosition();
  }

  void savecurrentPositionAsStart() const {
    EEPROM.put(0, getCounter());
  }
  
  void eraseStartPosition() const {
    EEPROM.put(0, -1);
  }

  void goForward(uint16_t steps) {
    
    for(uint16_t i = 0; i < steps; i++) {
      makeStepForward();
    }

    stop();
    Serial.print("MMFOK");
    Serial.println(getCounter());
  }

  void goBackward(uint16_t steps) {
    
    for(uint16_t i = 0; i < steps; i++) {
      makeStepBackward();
    }

    stop();

    Serial.print("MMBOK");
    Serial.println(getCounter());
  }
};