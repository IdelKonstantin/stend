#ifndef AGREGATOR_H
#define AGREGATOR_H

#include <Arduino.h>

namespace agregator {

  const String NOKmsg{"FAIL"};

  String executeIncomingCommand(const String& incMsg);
}

#endif /* AGREGATOR_H */