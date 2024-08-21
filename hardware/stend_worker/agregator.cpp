#include "agregator.h"
#include "pinSetup.h"

String agregator::executeIncomingCommand(const String& incMsg) {

  if(incMsg.startsWith("GO")) {
    
    //Отправить в ответе упакованное сообщение с калибровочными коэффициентами и положением выключателя "КАЛИБРОВКА"
    return "GOK";//agregator::OKmsg;
  }
  else if(incMsg.startsWith("TON_ABB")) {

    analogWrite(A4T_HOT_PIN, 127);
    return "TON_ABB_OK";
  }
  else if(incMsg.startsWith("TOF_ABB")) {

    analogWrite(A4T_HOT_PIN, 0);
    return "TOF_ABB_OK";
  }
  else if(incMsg.startsWith("TON_VNT")) {
    
    digitalWrite(A4T_VENT_PIN, HIGH);
    return "TON_VNT_OK";
  }
  else if(incMsg.startsWith("TOF_VNT")) {
    
    digitalWrite(A4T_VENT_PIN, LOW);
    return "TOF_VNT_OK";
  }
  else if(incMsg.startsWith("TON_EM1")) {
    
    //TODO
    return "TON_EM1_OK";
  }
  else if(incMsg.startsWith("TON_EM2")) {
    
    //TODO
    return "TON_EM2_OK";
  }
  else if(incMsg.startsWith("TON_EM3")) {
    
    //TODO
    return "TON_EM3_OK";
  }
  else if(incMsg.startsWith("TOF_EM1")) {
    
    //TODO
    return "TOF_EM1_OK";
  }
  else if(incMsg.startsWith("TOF_EM2")) {
    
    //TODO
    return "TOF_EM2_OK";
  }
  else if(incMsg.startsWith("TOF_EM3")) {
    
    //TODO
    return "TOF_EM3_OK";
  }
  else if(incMsg.startsWith("SETZ_MR")) {
    
    //TODO
    return "SETZ_MR_OK";
  }
  else if(incMsg.startsWith("TON_ALD")) {
    
    //TODO
    return "TON_ALD_OK";
  }
  else if(incMsg.startsWith("TOF_ALD")) {
    
    //TODO
    return "TOF_ALD_OK";
  }
  else if(incMsg.startsWith("SETZ_D1")) {
    
    //TODO
    return "SETZ_D1_OK";
  }
  else if(incMsg.startsWith("SETZ_D2")) {
    
    //TODO
    return "SETZ_D2_OK";
  }
  else if(incMsg.startsWith("SETZ_D3")) {
    
    //TODO
    return "SETZ_D3_OK";
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
  return agregator::NOKmsg;
}