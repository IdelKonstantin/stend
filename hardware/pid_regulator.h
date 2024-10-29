#ifndef _PIDREGULATOR_WORKER_H_
#define _PIDREGULATOR_WORKER_H_

#include "pwm_worker.h"
#include "adc_pid_worker.h"
#include "warmer_info_worker.h"

#define MIN_POSSIBLE_VALUE 1
#define MAX_POSSIBLE_VALUE 255
#define PID_MEASURE_INTERVAL_MSEC 30000

#define A4T_VENT_PIN 32
#define PID_THRESHOLD 5
#define PID_STEP 2
#define PID_ON_THERMAL_DELTA 50

class pidRegulator {

private:
    stend::warmer_data_t m_warmerData;
    uint64_t m_previousTime{0};
    uint64_t m_currentTime{0};

    stend::ADC_diff_t m_ADCdiffRead{0};
    stend::ADC_t m_ADC0Read{0};

private:
    pwmWorker m_pwm;
    adcPIDWorker m_pidADC;
    warmerData m_pidParams;

public:
    pidRegulator();
    
    void init() const;
    void keepThermalDelta(uint8_t thermalDelta);

    stend::ADC_diff_t readCurrentADCDiff() const;
    stend::ADC_t readCurrentADC0() const;
    uint8_t readCurrentThermalDiff();
    stend::PWM_t readCurrentPWM() const;
    stend::ADC_resolution_t readCurrentADCResolution() const;
    void tickTackToe();
    void turnONVentilator() const;  
    void turnOFFVentilator() const;
    void on();
    void off();
};

#endif /* _PIDREGULATOR_WORKER_H_ */