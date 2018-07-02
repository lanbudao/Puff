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
        clock(NULL),
        seeking(false),
        seek_requested(false)
    {
        packets.clear();
    }

    virtual ~AVThreadPrivate() {}

    void addTask(Runnable *task) {
        tasks.push_back(task);
    }

    AVDecoder *decoder;
    PacketQueue packets;

    OutputSet *output;

    /*Must be volatile*/
    volatile bool stopped;
    volatile bool paused;

    AVClock *clock;
    bool seek_requested;

    std::list<Runnable*> tasks;

    bool seeking = false;
};

}
#endif //PUFF_AVTHREAD_P_H
