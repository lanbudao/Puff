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
    Packet(const Packet& other);
    Packet &operator = (const Packet &other);

    const AVPacket *asAVPacket() const;

    static Packet fromAVPacket(const AVPacket *packet, double time_base);

    bool isEOF() const;
    static Packet createEOF();
    inline bool isValid() const;

    bool containKeyFrame, isCorrupted;
    double pts, dts, duration;
    int64_t pos;
    ByteArray data;

    AVPacket *avpacket();

private:
    DPTR_DECLARE(Packet)
};

bool Packet::isValid() const
{
    if (data.isEmpty())
        return false;
    return !isCorrupted && data.size() > 0 && pts >= 0 && duration >= 0;
}

}
#endif //PUFF_PACKET_H
