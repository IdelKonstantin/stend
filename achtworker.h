#ifndef ACHTWORKET_H
#define ACHTWORKET_H

#include <cstdint>

class ACHTWorket {

private:

    //TODO: set from Config
    double m_thermalDeltaScaleFactor{1.0};
    const double m_thermalDeltaMin{0};
    const double m_thermalDeltaMax{20.0};

    bool m_turnOnACHT{false};
    bool m_turnOnFan{false};

    double m_backTemperature{0};
    double m_thermalDelta{0};

public:

    ACHTWorket();

    void clickTurnOnACHT();
    void clickTurnOnFan();
    bool getTurnOnACHTFlag() const;
    bool getTurnOnFanFlag() const;

    double getbackTemperature() const;
    double getThermalDelta() const;
    void setThermalDelta(uint16_t deltaGrade);
    void setThermalDelta(double thermalDelta);

    double thermalDeltaMin() const;
    double thermalDeltaMax() const;
};

#endif // ACHTWORKET_H
