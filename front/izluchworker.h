#ifndef IZLUCHWORKER_H
#define IZLUCHWORKER_H

#include <cstdint>

class IzluchWorker {

private:

    //TODO: Read from Config
    double m_izluch1IntensityScaleFactor{1.0};
    double m_izluch2IntensityScaleFactor{1.0};
    double m_izluch3IntensityScaleFactor{1.0};
    const double m_izluch1IntensityMin{0};
    const double m_izluch1IntensityMax{100};
    const double m_izluch2IntensityMin{0};
    const double m_izluch2IntensityMax{100};
    const double m_izluch3IntensityMin{0};
    const double m_izluch3IntensityMax{100};

    bool m_turnOnIzluch1{false};
    bool m_turnOnIzluch2{false};
    bool m_turnOnIzluch3{false};

    double m_izluch1Intensity{0};
    double m_izluch2Intensity{0};
    double m_izluch3Intensity{0};

public:

    IzluchWorker();

    void clickTurnOnIzluch1();
    void clickTurnOnIzluch2();
    void clickTurnOnIzluch3();

    bool getTurnOnIzluch1Flag();
    bool getTurnOnIzluch2Flag();
    bool getTurnOnIzluch3Flag();

    double getIzluch1Intesity() const;
    double getIzluch2Intesity() const;
    double getIzluch3Intesity() const;

    void setIzluch1Intesity(uint16_t intensityGrade);
    void setIzluch1Intesity(double izluchIntesity);

    void setIzluch2Intesity(uint16_t intensityGrade);
    void setIzluch2Intesity(double izluchIntesity);

    void setIzluch3Intesity(uint16_t intensityGrade);
    void setIzluch3Intesity(double izluchIntesity);

    double izluch1IntesityMin() const;
    double izluch1IntesityMax() const;

    double izluch2IntesityMin() const;
    double izluch2IntesityMax() const;

    double izluch3IntesityMin() const;
    double izluch3IntesityMax() const;
};

#endif // IZLUCHWORKER_H
