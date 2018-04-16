#ifndef PUFF_PACKETQUEUE_H
#define PUFF_PACKETQUEUE_H

#include "Packet.h"
#include "util/BlockQueue.h"

namespace Puff {

class PacketQueue: public BlockQueue<Packet>
{
public:
    PacketQueue();
    ~PacketQueue() PU_DECL_OVERRIDE;

protected:
    bool checkFull() const PU_DECL_OVERRIDE;
    bool checkEmpty() const PU_DECL_OVERRIDE;
    bool checkEnough() const PU_DECL_OVERRIDE;

};

}
#endif //PUFF_PACKETQUEUE_H
