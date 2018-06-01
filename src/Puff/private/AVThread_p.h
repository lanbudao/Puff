#ifndef PUFF_AVTHREAD_P_H
#define PUFF_AVTHREAD_P_H

#include "DPTR.h"
#include "PacketQueue.h"
#include "AVDecoder.h"

namespace Puff {

class OutputSet;
class AVThreadPrivate
{
public:
    AVThreadPrivate() :
        decoder(NULL),
        stopped(false),
        output(NULL)
    {
        packets.clear();
    }

    virtual ~AVThreadPrivate() {}

    AVDecoder *decoder;
    PacketQueue packets;

    OutputSet *output;

    /*Must be volatile*/
    volatile bool stopped;
};

}
#endif //PUFF_AVTHREAD_P_H
