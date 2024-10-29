#ifndef _PID_WARMER_H_
#define _PID_WARMER_H_

#include <stdint.h>
#include <math.h>

#define ADC_RESOLUTION_x1 0
#define ADC_RESOLUTION_x10 1

#define PID_WARM_UP true
#define PID_COOL_DOWN false

namespace stend {
  
    using ADC_t = uint16_t;
    using ADC_diff_t = int16_t;
    using PWM_t = uint16_t;
    using ADC_resolution_t = uint8_t;
    
    using warmer_data_t = struct {
        
        PWM_t pwm{0};
        ADC_t adc{0};
        ADC_resolution_t resolution{ADC_RESOLUTION_x1};
        uint8_t prevDelta{0};
        bool warmDirection{PID_COOL_DOWN};
    };
};

class warmerData {

private:
    stend::warmer_data_t m_warmerData;
  
public:
    const stend::warmer_data_t& dataForThermalDelta(uint8_t thermalDelta);
    uint8_t warmerData::currentThermalDelta(stend::ADC_diff_t adcDiff, stend::ADC_resolution_t res);
};

#endif /* _PID_WARMER_H_ */