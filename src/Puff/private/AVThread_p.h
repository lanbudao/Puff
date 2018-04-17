#ifndef PUFF_AVTHREAD_P_H
#define PUFF_AVTHREAD_P_H

#include "DPTR.h"
#include "PacketQueue.h"
#include "AVDecoder.h"

namespace Puff {

class AVThreadPrivate : public DptrPrivate<AVThread> {
public:
    AVThreadPrivate() :
        decoder(NULL),
        stopped(false)
    {
        packets.clear();
    }

    virtual ~AVThreadPrivate() {}

    AVDecoder *decoder;
    PacketQueue packets;

    /*Must be volatile*/
    volatile bool stopped;
};

}
#endif //PUFF_AVTHREAD_P_H
