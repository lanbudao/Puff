#ifndef MSAV_PACKET_H
#define MSAV_PACKET_H

#include "MSAVGlobal.h"
#include "DPTR.h"

struct AVPacket;

namespace MSAV {

class PacketPrivate;
class MS_AV_EXPORT Packet
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
};

}
#endif //MSAV_PACKET_H
