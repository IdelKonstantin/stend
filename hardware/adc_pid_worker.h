#ifndef _ADC_PID_WORKER_H_
#define _ADC_PID_WORKER_H_

#include <stdint.h>

#define ADC_PID_BUFF_SIZE 10
#define ADC_DIFF_SCALE_FACTOR_x1 1
#define ADC_DIFF_SCALE_FACTOR_x10 10

#define PID_DIFFERENTIAL_READ true
#define ADC_PID_REGULAR_READ false

class adcPIDWorker {

private:

  int16_t m_buff[ADC_PID_BUFF_SIZE];
  int32_t m_sum;
  uint16_t m_resRaw;
  int16_t m_bufferizedValue;

  int16_t read();
  void resetAndRereadBuffer();

public:

  adcPIDWorker();

  void init() const;
  void setResolutionX1() const;
  void setResolutionX10() const;
  void setADC0read() const;
  int16_t readBufferized(bool diffRead = ADC_PID_REGULAR_READ);
};

#endif /* _ADC_PID_WORKER_H_ */
