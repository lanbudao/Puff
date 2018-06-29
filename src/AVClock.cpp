#include "AVClock.h"

namespace Puff {

class AVClockPrivate
{
public:
    AVClockPrivate():
        pts(0.0),
        clock_type(SyncToAudio)
    {

    }
    ~AVClockPrivate()
    {
    }

    double pts;
    ClockType clock_type;
};

AVClock::AVClock():
    d_ptr(new AVClockPrivate)
{

}

AVClock::~AVClock() {

}

double AVClock::value() const
{
    DPTR_D(const AVClock);
    return d->pts;
}

void AVClock::updateValue(double pts)
{
    DPTR_D(AVClock);
    d->pts = pts;
}

ClockType AVClock::clockType() const
{
    return d_func()->clock_type;
}

void AVClock::setClockType(ClockType type)
{
    DPTR_D(AVClock);
    d->clock_type = type;
}

}
