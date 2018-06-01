#include "AVOutput.h"
#include "AVOutput_p.h"

namespace Puff {

AVOutput::AVOutput(AVOutputPrivate *d):
    d_ptr(d)
{

}

AVOutput::~AVOutput()
{

}

bool AVOutput::isAvaliable() const
{
    DPTR_D(const AVOutput);
    return d->avaliable;
}

void AVOutput::setAvaliable(bool avaliable)
{
    DPTR_D(AVOutput);
    d->avaliable = avaliable;
}

void AVOutput::pause(bool p)
{
    DPTR_D(AVOutput);
    d->pause = p;
}

bool AVOutput::isPaused() const
{
    DPTR_D(const AVOutput);
    return d->pause;
}

}
