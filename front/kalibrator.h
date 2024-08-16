#ifndef KALIBRATOR_H
#define KALIBRATOR_H

#include <cstdint>

class Kalibrator {

private:

    //TODO: Read from Config
    const uint16_t m_stepDev_1{1};
    const double m_scaleFactorDev_1{1};
    double m_maxPositionDev_1{300};
    double m_minPositionDev_1{10};

    const uint16_t m_stepDev_2{1};
    const double m_scaleFactorDev_2{1};
    double m_maxPositionDev_2{300};
    double m_minPositionDev_2{10};

    const uint16_t m_stepDevX_3{1};
    const double m_scaleFactorDevX_3{1};
    double m_maxPositionDevX_3{300};
    double m_minPositionDevX_3{10};

    const uint16_t m_stepDevY_3{1};
    const double m_scaleFactorDevY_3{1};
    double m_maxPositionDevY_3{300};
    double m_minPositionDevY_3{10};

    const uint16_t m_stepDevZ_3{1};
    const double m_scaleFactorDevZ_3{1};
    double m_maxPositionDevZ_3{300};
    double m_minPositionDevZ_3{10};

////////////////////////////////////////////

    bool m_turnOnDevices{false};

    double m_currentPositionDev_1{0};
    double m_zadPositionDev_1{0};
    bool m_saveZeroFlagDev_1{false};

    double m_currentPositionDev_2{0};
    double m_zadPositionDev_2{0};
    bool m_saveZeroFlagDev_2{false};

    double m_currentPositionDevX_3{0};
    double m_currentPositionDevY_3{0};
    double m_currentPositionDevZ_3{0};
    bool m_saveZeroFlagDev_3{false};

public:
    Kalibrator();

    void clickTurnOnDevs();
    bool getTurnOnDevs() const;

    //Dev 1
    void clickSaveZeroDev_1();
    bool getSaveZeroFlagDev_1() const;
    void setCurrentPositionDev_1(double currentPosition);
    double getCurrentPositionDev_1() const;
    void setZadPositionDev_1(double zadPosition);
    double getZadPositionDev_1() const;
    void plusZadPositionDev_1();
    void minusZadPositionDev_1();

    //Dev 2
    void clickSaveZeroDev_2();
    bool getSaveZeroFlagDev_2() const;
    void setCurrentPositionDev_2(double currentPosition);
    double getCurrentPositionDev_2() const;
    void setZadPositionDev_2(double zadPosition);
    double getZadPositionDev_2() const;
    void plusZadPositionDev_2();
    void minusZadPositionDev_2();

    //Dev 3
    void clickSaveZeroDev_3();
    bool getSaveZeroFlagDev_3() const;
    void setCurrentPositionDevX_3(double currentPosition);
    double getCurrentPositionDevX_3() const;
    void setCurrentPositionDevY_3(double currentPosition);
    double getCurrentPositionDevY_3() const;
    void setCurrentPositionDevZ_3(double currentPosition);
    double getCurrentPositionDevZ_3() const;

    void plusZadPositionDevX_3();
    void minusZadPositionDevX_3();
    void plusZadPositionDevY_3();
    void minusZadPositionDevY_3();
    void plusZadPositionDevZ_3();
    void minusZadPositionDevZ_3();
};

#endif // KALIBRATOR_
