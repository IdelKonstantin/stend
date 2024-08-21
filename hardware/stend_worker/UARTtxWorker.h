#ifndef UART_TX_WORKER_H
#define UART_TX_WORKER_H

#include <Arduino.h>

namespace UART {

  void proceedOutcomingMessage(const String& msg);
};

#endif /* UART_TX_WORKER_H */ 