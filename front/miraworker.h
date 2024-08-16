#ifndef MIRAWORKER_H
#define MIRAWORKER_H

#include <cstdint>

class MiraWorker {

private:

    //TODO: read from config
    const uint16_t m_miraStep{1};
    const double m_miraScaleFactor{1};
    double m_miraMaxPosition{300};
    double m_miraMinPosition{10};

    double m_currentPosition{0};
    double m_zadPosition{0};

    bool m_saveZeroFlag{false};

public:

    MiraWorker();

    void clickSaveZero();
    bool getSaveZerolag() const;

    void setCurrentPosition(double currentPosition);
    double getCurrentPosition() const;

    void setZadPosition(double zadPosition);
    double getZadPosition() const;

    void plusZadPosition();
    void minusZadPosition();
};

#endif // MIRAWORKER_H
