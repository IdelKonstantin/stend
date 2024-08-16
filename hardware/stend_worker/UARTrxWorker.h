#ifndef UART_RX_WORKER_H
#define UART_RX_WORKER_H

#define UART_RX_TASK_NUM 1
#define UART_RX_TASK_TIME 10

namespace UART {
  void proceedIncomingMessageTask();
};

#endif /* UART_RX_WORKER_H */ 