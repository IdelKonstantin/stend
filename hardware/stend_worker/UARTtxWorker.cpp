#include "UARTtxWorker.h"

#include "queues.h"

extern cppQueue outMSGQ;

void UART::proceedOutcomingMessageTask() {
  
  UART_data::outcomingMSG outcomingData;

  while(true) {
    
    if (!outMSGQ.isEmpty()) {

      outMSGQ.pop(&outcomingData);
      Serial.println(outcomingData.msg);
    }
  }
}
