#include "AVClock.h"

namespace Puff {

class AVClockPrivate
{
public:
    AVClockPrivate():
        pts(0.0),
        sync_type(SyncToAudio)
    {

    }
    ~AVClockPrivate()
    {
    }

    double pts;
    SyncType sync_type;
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

SyncType AVClock::syncType() const
{
    return d_func()->sync_type;
}

void AVClock::setSyncType(SyncType type)
{
    DPTR_D(AVClock);
    d->sync_type = type;
}

}
