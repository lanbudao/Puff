#ifndef MSAV_PACKETQUEUE_H
#define MSAV_PACKETQUEUE_H

#include "Packet.h"
#include "util/BlockQueue.h"

namespace MSAV {

class PacketQueue: public BlockQueue<Packet>
{
public:
    PacketQueue();
    ~PacketQueue() MS_DECL_OVERRIDE;

protected:
    bool checkFull() const MS_DECL_OVERRIDE;
    bool checkEmpty() const MS_DECL_OVERRIDE;
    bool checkEnough() const MS_DECL_OVERRIDE;

};

}
#endif //MSAV_PACKETQUEUE_H
