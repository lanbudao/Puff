#include "OutputSet.h"
#include "VideoRenderer.h"
#include "CMutex.h"

namespace Puff {

class OutputSetPrivate
{
public:
    OutputSetPrivate()
    {

    }

    ~OutputSetPrivate()
    {

    }

    std::list<AVOutput *> outputs;
    CMutex mutex;
};

OutputSet::OutputSet():
    d_ptr(new OutputSetPrivate)
{

}

OutputSet::~OutputSet()
{

}

std::list<AVOutput *> OutputSet::outputs()
{
    DPTR_D(const OutputSet);
    return d->outputs;
}

void OutputSet::lock()
{
    DPTR_D(OutputSet);
    d->mutex.writeLock();
}

void OutputSet::unlock()
{
    DPTR_D(OutputSet);
    d->mutex.writeUnlock();
}

void OutputSet::addOutput(AVOutput *output)
{
    DPTR_D(OutputSet);
    ReadLock lock(&d->mutex);
    PU_UNUSED(lock);
    d->outputs.push_back(output);
}

void OutputSet::removeOutput(AVOutput *output)
{
    DPTR_D(OutputSet);
    ReadLock lock(&d->mutex);
    PU_UNUSED(lock);
    d->outputs.remove(output);
}

void OutputSet::clearOutput()
{
    DPTR_D(OutputSet);
    ReadLock lock(&d->mutex);
    PU_UNUSED(lock);
    d->outputs.clear();
}

void OutputSet::sendVideoFrame(const VideoFrame &frame)
{
    DPTR_D(OutputSet);
    std::list<AVOutput *>::iterator itor;
    for (itor = d->outputs.begin(); itor != d->outputs.end(); itor++) {
        VideoRenderer *renderer = static_cast<VideoRenderer *>(*itor);
        if (!renderer->isAvaliable())
            continue;
        renderer->receive(frame);
    }
}

}
