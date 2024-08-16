#include "achtworker.h"

ACHTWorket::ACHTWorket() {}

void ACHTWorket::clickTurnOnACHT() {

    m_turnOnACHT = !m_turnOnACHT;
}

void ACHTWorket::clickTurnOnFan() {

    m_turnOnFan = !m_turnOnFan;
}

bool ACHTWorket::getTurnOnACHTFlag() const {

    return m_turnOnACHT;
}

bool ACHTWorket::getTurnOnFanFlag() const {

    return m_turnOnFan;
}

double ACHTWorket::getbackTemperature() const {

    return m_backTemperature;
}

double ACHTWorket::getThermalDelta() const {

    return m_thermalDelta;
}

void ACHTWorket::setThermalDelta(uint16_t deltaGrade) {

    m_thermalDelta = deltaGrade * m_thermalDeltaScaleFactor;
}

void ACHTWorket::setThermalDelta(double thermalDelta) {

    m_thermalDelta = thermalDelta;
}

double ACHTWorket::thermalDeltaMin() const {

    return m_thermalDeltaMin;
}

double ACHTWorket::thermalDeltaMax() const {

    return m_thermalDeltaMax;
}

