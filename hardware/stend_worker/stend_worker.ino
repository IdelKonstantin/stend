#include <GyverOS.h>

#include "pinSetup.h"
#include "queues.h"
#include "UARTtxWorker.h"
#include "UARTrxWorker.h"
#include "agregator.h"
#include "sensorAsker.h"

GyverOS<4> OS;

cppQueue inMSGQ{sizeof(UART_data::incomingMSG), MSG_QUEUES_SIZE, FIFO};
cppQueue outMSGQ{sizeof(UART_data::outcomingMSG), MSG_QUEUES_SIZE, FIFO};

void setup() {
  
  stend::pinSetup();

  Serial.begin(9600);
  Serial.println("STARTED");

  OS.attach(UART_TX_TASK_NUM, UART::proceedOutcomingMessageTask, UART_TX_TASK_TIME);
  OS.attach(UART_RX_TASK_NUM, UART::proceedIncomingMessageTask, UART_RX_TASK_TIME);
  OS.attach(AGREGATOR_TASK_NUM, agregator::executeIncomingCommandsTask, AGREGATOR_TASK_TIME);
  OS.attach(SENSOR_TASK_NUM, sensors::askSensorsTask, SENSOR_TASK_TIME);
}

void loop() {
  OS.tick();
}