#include "agregator.h"

#include "queues.h"
#include "pinSetup.h"

extern cppQueue inMSGQ;
extern cppQueue outMSGQ;

void agregator::executeIncomingCommandsTask() {

  UART_data::incomingMSG incomingData;
  UART_data::outcomingMSG outcomingData;

  argegatorCommander aglc;

  while(true) {

    if(inMSGQ.isEmpty()) {
      continue;
    }

    inMSGQ.pop(&incomingData);
    outcomingData.msg = aglc.executeCommand(incomingData.msg);
    outMSGQ.push(&outcomingData);
  }
}

argegatorCommander::argegatorCommander() {};

String argegatorCommander::executeCommand(const String& incMsg) {

  if(incMsg == "GO") {
    //Отправить в ответе упакованное сообщение с калибровочными коэффициентами и положением выключателя "КАЛИБРОВКА"
  }
  else if(incMsg == "TON_ABB") {

    digitalWrite(A4T_HOT_PIN, HIGH);
    return m_OKmsg;
  }
  else if(incMsg == "TOF_ABB") {
    
    digitalWrite(A4T_HOT_PIN, LOW);
    return m_OKmsg;
  }
  else if(incMsg == "TON_VNT") {
    
    digitalWrite(A4T_VENT_PIN, HIGH);
    return m_OKmsg;
  }
  else if(incMsg == "TOF_VNT") {
    
    digitalWrite(A4T_VENT_PIN, LOW);
    return m_OKmsg;
  }
  else if(incMsg == "TON_EM1") {
    
    //TODO
    return m_OKmsg;
  }
  else if(incMsg == "TON_EM1") {
    
    //TODO
    return m_OKmsg;
  }
  else if(incMsg == "TON_EM2") {
    
    //TODO
    return m_OKmsg;
  }
  else if(incMsg == "TON_EM3") {
    
    //TODO
    return m_OKmsg;
  }
  else if(incMsg == "TOF_EM1") {
    
    //TODO
    return m_OKmsg;
  }
  else if(incMsg == "TOF_EM2") {
    
    //TODO
    return m_OKmsg;
  }
  else if(incMsg == "TOF_EM3") {
    
    //TODO
    return m_OKmsg;
  }
  else if(incMsg == "SETZ_MR") {
    
    //TODO
    return m_OKmsg;
  }
  else if(incMsg == "TON_ALD") {
    
    //TODO
    return m_OKmsg;
  }
  else if(incMsg == "TOF_ALD") {
    
    //TODO
    return m_OKmsg;
  }
  else if(incMsg == "SETZ_D1") {
    
    //TODO
    return m_OKmsg;
  }
  else if(incMsg == "SETZ_D2") {
    
    //TODO
    return m_OKmsg;
  }
  else if(incMsg == "SETZ_D3") {
    
    //TODO
    return m_OKmsg;
  }

/* TODO:
return QString{"DEF_TDEL_"} + payload;
return QString{"DEF_E1IN_"} + payload;
return QString{"DEF_E2IN_"} + payload;
return QString{"DEF_E3IN_"} + payload;
return QString{"DEF_MPOS_"} + payload;
return QString{"DEF_D1PX_"} + payload;
return QString{"DEF_D2PX_"} + payload;
return QString{"DEF_D3PX_"} + payload;
return QString{"DEF_D3PY_"} + payload;
return QString{"DEF_D3PZ_"} + payload;

*/
  else {
    return m_NOKmsg;
  }

  return "OK";
}

