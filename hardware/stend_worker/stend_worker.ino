
#include "UARTrxWorker.h"
#include "pinSetup.h"

void setup() {
  
  stend::pinSetup();
  

  Serial.begin(9600);
  Serial.println("Stend_started");
}

void loop() {

  UART::proceedMainCycle();
}