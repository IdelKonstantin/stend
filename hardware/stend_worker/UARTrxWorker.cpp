#include "UARTrxWorker.h"
#include "UARTtxWorker.h"
#include "sensorAsker.h"
#include "agregator.h"

void UART::proceedMainCycle() {

  String incomingMSG;

  while(true) {
    
    sensors::askSensors();

    if(Serial.available() > 0) {
      
      incomingMSG = Serial.readStringUntil('\n');

      if(incomingMSG.length() != 0) {

        UART::proceedOutcomingMessage(agregator::executeIncomingCommand(incomingMSG));
      }
    }
  }
}
