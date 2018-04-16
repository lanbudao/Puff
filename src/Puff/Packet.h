#ifndef PUFF_PACKET_H
#define PUFF_PACKET_H

#include "AVGlobal.h"
#include "DPTR.h"

struct AVPacket;

namespace Puff {

class PacketPrivate;
class PU_AV_EXPORT Packet
{
    DPTR_DECLARE_PRIVATE(Packet)
public:
    Packet();
    ~Packet();

    static Packet fromAVPacket(const AVPacket *packet, double time_base);

    bool isEOF();

    bool containKeyFrame, isCorrupted;
    double pts, dts, duration;
    int64_t pos;
    char *data;

private:
    DPTR_DECLARE(Packet)
};

}
#endif //PUFF_PACKET_H
