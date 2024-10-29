#include "miraworker.h"

MiraWorker::MiraWorker() {}

void MiraWorker::clickSaveZero() {

    m_saveZeroFlag = !m_saveZeroFlag;
}

void MiraWorker::clickResetZero() {

    m_resetZeroFlag = !m_resetZeroFlag;
}

bool MiraWorker::getResetZerolag() const {

    return m_resetZeroFlag;
}

bool MiraWorker::getSaveZerolag() const {

    return m_saveZeroFlag;
}

void MiraWorker::setCurrentPosition(double currentPosition) {

    m_currentPosition = currentPosition;
}

double MiraWorker::getCurrentPosition() const {

    return m_currentPosition;
}

void MiraWorker::setZadPosition(double zadPosition) {

    m_zadPosition = zadPosition;
}

double MiraWorker::getZadPosition() const {

    return m_zadPosition;
}

void MiraWorker::plusZadPosition() {

    auto currentZadPosition = getZadPosition();
    currentZadPosition += m_miraStep * m_miraScaleFactor;

    if(currentZadPosition >= m_miraMaxPosition) {
        currentZadPosition = m_miraMaxPosition;
    }

    setZadPosition(currentZadPosition);
}

void MiraWorker::minusZadPosition() {

    auto currentZadPosition = getZadPosition();
    currentZadPosition -= m_miraStep * m_miraScaleFactor;

    if(currentZadPosition <= m_miraMinPosition) {
        currentZadPosition = m_miraMinPosition;
    }

    setZadPosition(currentZadPosition);
}
