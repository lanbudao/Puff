#include "AVOutput.h"
#include "AVOutput_p.h"

namespace Puff {

AVOutput::AVOutput()
{

}

AVOutput::~AVOutput()
{

}

bool AVOutput::isAvaliable() const
{
    DPTR_D(const AVOutput);
    return d.avaliable;
}

void AVOutput::pause(bool p)
{
    DPTR_D(AVOutput);
    d.pause = p;
}

bool AVOutput::isPaused() const
{
    DPTR_D(const AVOutput);
    return d.pause;
}

}