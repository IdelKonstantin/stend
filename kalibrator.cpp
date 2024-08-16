#include "kalibrator.h"

Kalibrator::Kalibrator() {}

void Kalibrator::clickTurnOnDevs() {

    m_turnOnDevices = !m_turnOnDevices;
}

bool Kalibrator::getTurnOnDevs() const {

    return m_turnOnDevices;
}


double Kalibrator::getCurrentPositionDev_1() const {

    return m_currentPositionDev_1;
}

double Kalibrator::getCurrentPositionDev_2() const {

    return m_currentPositionDev_2;
}

double Kalibrator::getCurrentPositionDevX_3() const {

    return m_currentPositionDevX_3;
}

double Kalibrator::getCurrentPositionDevY_3() const {

    return m_currentPositionDevY_3;
}

double Kalibrator::getCurrentPositionDevZ_3() const {

    return m_currentPositionDevZ_3;
}

void Kalibrator::clickSaveZeroDev_1() {

    m_saveZeroFlagDev_1 = !m_saveZeroFlagDev_1;
}

void Kalibrator::clickSaveZeroDev_2() {

    m_saveZeroFlagDev_2 = !m_saveZeroFlagDev_2;
}

void Kalibrator::clickSaveZeroDev_3() {

    m_saveZeroFlagDev_3 = !m_saveZeroFlagDev_3;
}

bool Kalibrator::getSaveZeroFlagDev_1() const {

    return m_saveZeroFlagDev_1;
}

bool Kalibrator::getSaveZeroFlagDev_2() const {

    return m_saveZeroFlagDev_2;
}

bool Kalibrator::getSaveZeroFlagDev_3() const {

    return m_saveZeroFlagDev_3;
}

void Kalibrator::setZadPositionDev_1(double zadPosition) {

    m_zadPositionDev_1 = zadPosition;
}

void Kalibrator::setZadPositionDev_2(double zadPosition) {

    m_zadPositionDev_2 = zadPosition;
}

double Kalibrator::getZadPositionDev_1() const {

    return m_zadPositionDev_1;
}

double Kalibrator::getZadPositionDev_2() const {

    return m_zadPositionDev_2;
}

void Kalibrator::plusZadPositionDev_1() {

    auto currentZadPosition = getZadPositionDev_1();
    currentZadPosition += m_stepDev_1 * m_scaleFactorDev_1;

    if(currentZadPosition >= m_maxPositionDev_1) {
        currentZadPosition = m_maxPositionDev_1;
    }

    setZadPositionDev_1(currentZadPosition);
}

void Kalibrator::minusZadPositionDev_1() {

    auto currentZadPosition = getZadPositionDev_1();
    currentZadPosition -= m_stepDev_1 * m_scaleFactorDev_1;

    if(currentZadPosition <= m_minPositionDev_1) {
        currentZadPosition = m_minPositionDev_1;
    }

    setZadPositionDev_1(currentZadPosition);
}

void Kalibrator::plusZadPositionDev_2() {

    auto currentZadPosition = getZadPositionDev_2();
    currentZadPosition += m_stepDev_2 * m_scaleFactorDev_2;

    if(currentZadPosition >= m_maxPositionDev_2) {
        currentZadPosition = m_maxPositionDev_2;
    }

    setZadPositionDev_2(currentZadPosition);
}

void Kalibrator::minusZadPositionDev_2() {

    auto currentZadPosition = getZadPositionDev_2();
    currentZadPosition -= m_stepDev_2 * m_scaleFactorDev_2;

    if(currentZadPosition <= m_minPositionDev_2) {
        currentZadPosition = m_minPositionDev_2;
    }

    setZadPositionDev_2(currentZadPosition);
}

void Kalibrator::plusZadPositionDevX_3() {

    auto currentZadPosition = getCurrentPositionDevX_3();
    currentZadPosition += m_stepDevX_3 * m_scaleFactorDevX_3;

    if(currentZadPosition >= m_maxPositionDevX_3) {
        currentZadPosition = m_maxPositionDevX_3;
    }

    setCurrentPositionDevX_3(currentZadPosition);
}

void Kalibrator::minusZadPositionDevX_3() {

    auto currentZadPosition = getCurrentPositionDevX_3();
    currentZadPosition -= m_stepDevX_3 * m_scaleFactorDevX_3;

    if(currentZadPosition <= m_minPositionDevX_3) {
        currentZadPosition = m_minPositionDevX_3;
    }

    setCurrentPositionDevX_3(currentZadPosition);
}

void Kalibrator::plusZadPositionDevY_3() {

    auto currentZadPosition = getCurrentPositionDevY_3();
    currentZadPosition += m_stepDevY_3 * m_scaleFactorDevY_3;

    if(currentZadPosition >= m_maxPositionDevY_3) {
        currentZadPosition = m_maxPositionDevY_3;
    }

    setCurrentPositionDevY_3(currentZadPosition);
}

void Kalibrator::minusZadPositionDevY_3() {

    auto currentZadPosition = getCurrentPositionDevY_3();
    currentZadPosition -= m_stepDevY_3 * m_scaleFactorDevY_3;

    if(currentZadPosition <= m_minPositionDevY_3) {
        currentZadPosition = m_minPositionDevY_3;
    }

    setCurrentPositionDevY_3(currentZadPosition);
}

void Kalibrator::plusZadPositionDevZ_3() {

    auto currentZadPosition = getCurrentPositionDevZ_3();
    currentZadPosition += m_stepDevZ_3 * m_scaleFactorDevZ_3;

    if(currentZadPosition >= m_maxPositionDevZ_3) {
        currentZadPosition = m_maxPositionDevZ_3;
    }

    setCurrentPositionDevZ_3(currentZadPosition);
}

void Kalibrator::minusZadPositionDevZ_3() {

    auto currentZadPosition = getCurrentPositionDevZ_3();
    currentZadPosition -= m_stepDevZ_3 * m_scaleFactorDevZ_3;

    if(currentZadPosition <= m_minPositionDevZ_3) {
        currentZadPosition = m_minPositionDevZ_3;
    }

    setCurrentPositionDevZ_3(currentZadPosition);
}

void Kalibrator::setCurrentPositionDevX_3(double currentPosition) {

    m_currentPositionDevX_3 = currentPosition;
}

void Kalibrator::setCurrentPositionDevY_3(double currentPosition) {

    m_currentPositionDevY_3 = currentPosition;
}

void Kalibrator::setCurrentPositionDevZ_3(double currentPosition) {

    m_currentPositionDevZ_3 = currentPosition;
}
