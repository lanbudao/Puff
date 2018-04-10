#include "AVThread.h"
#include "PacketQueue.h"

namespace MSAV {

class AVThreadPrivate: public DptrPrivate<AVThread>
{
public:
    AVThreadPrivate()
    {

    }
    ~AVThreadPrivate() {}

    PacketQueue packets;
};

AVThread::AVThread() {

}

AVThread::~AVThread() {

}

PacketQueue * AVThread::packets()
{
    DPTR_D(AVThread);
    return &d.packets;
}

}