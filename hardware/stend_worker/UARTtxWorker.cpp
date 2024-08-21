#include "UARTtxWorker.h"

void UART::proceedOutcomingMessage(const String& msg) {

  Serial.print(msg);
  //Serial.flush();
}
