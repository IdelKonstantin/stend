#include "izluchworker.h"

IzluchWorker::IzluchWorker() {}

void IzluchWorker::clickTurnOnIzluch1() {

    m_turnOnIzluch1 = !m_turnOnIzluch1;
}

void IzluchWorker::clickTurnOnIzluch2() {

    m_turnOnIzluch2 = !m_turnOnIzluch2;
}

void IzluchWorker::clickTurnOnIzluch3() {

    m_turnOnIzluch3 = !m_turnOnIzluch3;
}

bool IzluchWorker::getTurnOnIzluch1Flag() {

    return m_turnOnIzluch1;
}

bool IzluchWorker::getTurnOnIzluch2Flag() {

    return m_turnOnIzluch2;
}

bool IzluchWorker::getTurnOnIzluch3Flag() {

    return m_turnOnIzluch3;
}

double IzluchWorker::getIzluch1Intesity() const {

    return m_izluch1Intensity;
}

double IzluchWorker::getIzluch2Intesity() const {

    return m_izluch2Intensity;
}

double IzluchWorker::getIzluch3Intesity() const {

    return m_izluch3Intensity;
}

void IzluchWorker::setIzluch1Intesity(uint16_t intensityGrade) {

    m_izluch1Intensity = intensityGrade * m_izluch1IntensityScaleFactor;
}

void IzluchWorker::setIzluch1Intesity(double izluchIntesity) {

    m_izluch1Intensity = izluchIntesity;
}

void IzluchWorker::setIzluch2Intesity(uint16_t intensityGrade) {

    m_izluch2Intensity = intensityGrade * m_izluch2IntensityScaleFactor;
}

void IzluchWorker::setIzluch2Intesity(double izluchIntesity) {

    m_izluch2Intensity = izluchIntesity;
}


void IzluchWorker::setIzluch3Intesity(uint16_t intensityGrade) {

    m_izluch3Intensity = intensityGrade * m_izluch3IntensityScaleFactor;
}

void IzluchWorker::setIzluch3Intesity(double izluchIntesity) {

    m_izluch3Intensity = izluchIntesity;
}

double IzluchWorker::izluch1IntesityMin() const {

    return m_izluch1IntensityMin;
}

double IzluchWorker::izluch1IntesityMax() const {

    return m_izluch1IntensityMax;
}

double IzluchWorker::izluch2IntesityMin() const {

    return m_izluch2IntensityMin;
}

double IzluchWorker::izluch2IntesityMax() const {

    return m_izluch2IntensityMax;
}

double IzluchWorker::izluch3IntesityMin() const {

    return m_izluch3IntensityMin;
}

double IzluchWorker::izluch3IntesityMax() const {

    return m_izluch3IntensityMax;
}
