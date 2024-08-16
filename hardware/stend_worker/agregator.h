#ifndef AGREGATOR_H
#define AGREGATOR_H

#include <Arduino.h>

#define AGREGATOR_TASK_NUM 2
#define AGREGATOR_TASK_TIME 20

namespace agregator {
  void executeIncomingCommandsTask();
}

class argegatorCommander {

private:

  const String m_OKmsg{"OK"};
  const String m_NOKmsg{"NOK"};

public:

  argegatorCommander();

  String executeCommand(const String& incMsg);
};

#endif /* AGREGATOR_H */