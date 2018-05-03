#include "OutputSet.h"
#include "VideoRenderer.h"

#include "boost/thread/locks.hpp"
#include "boost/thread/shared_mutex.hpp"
#include "boost/thread/condition.hpp"

namespace Puff {

typedef boost::shared_mutex Mutex;
typedef boost::unique_lock<Mutex> WriteLock;
typedef boost::shared_lock<Mutex> ReadLock;
typedef boost::condition Condition;

class OutputSetPrivate: public DptrPrivate<OutputSet>
{
public:
    OutputSetPrivate()
    {

    }

    ~OutputSetPrivate()
    {

    }

    CList<AVOutput *> outputs;
    Mutex mutex;
    Condition condition;
};

OutputSet::OutputSet()
{

}

OutputSet::~OutputSet()
{

}

CList<AVOutput *> OutputSet::outputs()
{
    DPTR_D(const OutputSet);
    return d.outputs;
}

void OutputSet::lock()
{
    DPTR_D(OutputSet);
    d.mutex.lock();
}

void OutputSet::unlock()
{
    DPTR_D(OutputSet);
    d.mutex.unlock();
}

void OutputSet::addOutput(AVOutput *output)
{
    DPTR_D(OutputSet);
    ReadLock lock(d.mutex);
    PU_UNUSED(lock);
    d.outputs.push_back(output);
}

void OutputSet::removeOutput(AVOutput *output)
{
    DPTR_D(OutputSet);
    ReadLock lock(d.mutex);
    PU_UNUSED(lock);
    d.outputs.remove(output);
}

void OutputSet::clearOutput()
{
    DPTR_D(OutputSet);
    ReadLock lock(d.mutex);
    PU_UNUSED(lock);
    d.outputs.clear();
}

void OutputSet::sendVideoFrame(const VideoFrame &frame)
{
    DPTR_D(OutputSet);
    for (int i = 0; i < d.outputs.size(); ++i) {
        VideoRenderer *renderer = static_cast<VideoRenderer *>(d.outputs.at(i));
        if (!renderer->isAvaliable())
            continue;
        renderer->receiveFrame(frame);
    }
}

}