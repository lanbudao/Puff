#ifndef PUFF_AVCLOCK_H
#define PUFF_AVCLOCK_H

#include "AVGlobal.h"
#include "DPTR.h"

namespace Puff {

class AVClockPrivate;
class AVClock
{
    DPTR_DECLARE_PRIVATE(AVClock)
public:
    AVClock();
    ~AVClock();

    double value() const;
    void updateValue(double pts);

    ClockType clockType() const;
    void setClockType(ClockType type);

private:
    DPTR_DECLARE(AVClock)
};

}
#endif //PUFF_AVCLOCK_H
