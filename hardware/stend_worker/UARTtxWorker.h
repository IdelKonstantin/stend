#ifndef UART_TX_WORKER_H
#define UART_TX_WORKER_H

#define UART_TX_TASK_NUM 0
#define UART_TX_TASK_TIME 10

namespace UART {
  void proceedOutcomingMessageTask();
};

#endif /* UART_TX_WORKER_H */ 