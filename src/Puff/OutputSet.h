#ifndef PUFF_OUTPUTSET_H
#define PUFF_OUTPUTSET_H

#include "AVGlobal.h"
#include "VideoFrame.h"
#include "DPTR.h"
#include "CList.h"

namespace Puff {

class AVOutput;
class OutputSetPrivate;
class PU_AV_EXPORT OutputSet
{
    DPTR_DECLARE_PRIVATE(OutputSet)
public:
    OutputSet();
    ~OutputSet();

    CList<AVOutput *> outputs();

    void lock();
    void unlock();

    void addOutput(AVOutput *output);
    void removeOutput(AVOutput *output);
    void clearOutput();

    void sendVideoFrame(const VideoFrame &frame);

private:
    DPTR_DECLARE(OutputSet)
};

}
#endif //PUFF_OUTPUTSET_H
