#ifndef WORKINHG_QUEUES_H
#define WORKINHG_QUEUES_H

#include <Arduino.h>
#include <cppQueue.h>

#define MSG_QUEUES_SIZE 10

namespace UART_data {
  
  struct incomingMSG {
    String msg;
  };

  struct outcomingMSG {
    String msg;
  };
}

#endif /* WORKINHG_QUEUES_H */