#ifndef _PWM_WORKER_H_
#define _PWM_WORKER_H_

#include <stdint.h>

class pwmWorker {

public:
    pwmWorker();

    void init() const;
    void setIntencity(uint16_t intencity) const;
};

#endif /* _PWM_WORKER_H_ */