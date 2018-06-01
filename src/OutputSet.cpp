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

    CList<AVOutput *> outputs;
    CMutex mutex;
};

OutputSet::OutputSet():
    d_ptr(new OutputSetPrivate)
{

}

OutputSet::~OutputSet()
{

}

CList<AVOutput *> OutputSet::outputs()
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
    for (size_t i = 0; i < d->outputs.size(); ++i) {
        VideoRenderer *renderer = static_cast<VideoRenderer *>(d->outputs.at(i));
        if (!renderer->isAvaliable())
            continue;
        renderer->receive(frame);
    }
}

}
