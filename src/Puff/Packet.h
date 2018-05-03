#ifndef PUFF_PACKET_H
#define PUFF_PACKET_H

#include "AVGlobal.h"
#include "DPTR.h"
#include "ByteArray.h"

struct AVPacket;

namespace Puff {

class PacketPrivate;
class PU_AV_EXPORT Packet
{
    DPTR_DECLARE_PRIVATE(Packet)
public:
    Packet();
    ~Packet();

    const AVPacket *asAVPacket() const;

    static Packet fromAVPacket(const AVPacket *packet, double time_base);

    bool isEOF() const;
    inline bool isValid() const;

    bool containKeyFrame, isCorrupted;
    double pts, dts, duration;
    int64_t pos;
    ByteArray data;
    int size;

private:
    DPTR_DECLARE(Packet)
};

bool Packet::isValid() const
{
    return !isCorrupted && size > 0 && pts >= 0 && duration >= 0;
}

}
#endif //PUFF_PACKET_H
