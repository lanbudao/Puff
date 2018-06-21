#include "AVClock.h"

namespace Puff {

class AVClockPrivate
{
public:
    AVClockPrivate():
        pts(0.0)
    {

    }
    ~AVClockPrivate()
    {
    }

    double pts;
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

}
