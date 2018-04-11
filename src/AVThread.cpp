#include "AVThread.h"
#include "AVThread_p.h"

namespace MSAV {
//
//class AVThreadPrivate: public DptrPrivate<AVThread>
//{
//public:
//    AVThreadPrivate():
//        decoder(NULL)
//    {
//        packets.clear();
//    }
//    virtual ~AVThreadPrivate() {}
//
//    AVDecoder *decoder;
//    PacketQueue packets;
//};

AVThread::AVThread() {

}

AVThread::~AVThread() {

}

PacketQueue * AVThread::packets()
{
    DPTR_D(AVThread);
    return &(d.packets);
}

}