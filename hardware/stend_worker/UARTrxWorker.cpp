#include "UARTrxWorker.h"
#include "queues.h"

extern cppQueue inMSGQ;

void UART::proceedIncomingMessageTask() {

  String incomingMSG;

  while(true) {
    
    if (Serial.available()) {
      
      incomingMSG = Serial.readString();

      if(incomingMSG.length() != 0) {
        inMSGQ.push(&UART_data::incomingMSG{incomingMSG});
      }
    }
  }
}
