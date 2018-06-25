#ifndef PUFF_AVTHREAD_P_H
#define PUFF_AVTHREAD_P_H

#include "DPTR.h"
#include "PacketQueue.h"
#include "AVDecoder.h"
#include "AVClock.h"

namespace Puff {

class OutputSet;
class AVThreadPrivate
{
public:
    AVThreadPrivate() :
        decoder(NULL),
        stopped(false),
        paused(false),
        output(NULL),
        clock(NULL)
    {
        packets.clear();
    }

    virtual ~AVThreadPrivate() {}

    AVDecoder *decoder;
    PacketQueue packets;

    OutputSet *output;

    /*Must be volatile*/
    volatile bool stopped;
    volatile bool paused;

    AVClock *clock;
};

}
#endif //PUFF_AVTHREAD_P_H
