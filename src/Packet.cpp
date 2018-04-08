#include <libavcodec/avcodec.h>
#include "Packet.h"

namespace MSAV {

class PacketPrivate: public DptrPrivate<Packet>
{
public:
    PacketPrivate()
    {

    }
    ~PacketPrivate()
    {

    }
};

Packet::Packet()
{

}

Packet::~Packet()
{

}

Packet Packet::fromAVPacket(const AVPacket *packet, double time_base)
{
    Packet pkt;

    pkt.pos = packet->pos;
    pkt.containKeyFrame = (bool)(packet->flags & AV_PKT_FLAG_KEY);
    pkt.isCorrupted = (bool)(packet->flags & AV_PKT_FLAG_CORRUPT);

    /*Set pts*/
    if (packet->pts != AV_NOPTS_VALUE) {
        pkt.pts = packet->pts * time_base;
    } else if (packet->dts != AV_NOPTS_VALUE) {
        pkt.pts = packet->dts * time_base;
    } else {
        pkt.pts = 0;
    }

    /*Set dts*/
    if (packet->dts != AV_NOPTS_VALUE) {
        pkt.dts = packet->dts * time_base;
    } else {
        pkt.dts = pkt.pts;
    }

    if (pkt.pts < 0)
        pkt.pts = 0;
    if (pkt.dts < 0)
        pkt.dts = 0;

    /*duration*/
    pkt.duration = packet->duration * time_base;
    if (pkt.duration < 0)
        pkt.duration = 0;

    pkt.data = (char *)malloc(packet->size);
    memcpy(pkt.data, packet->data, packet->size);

    return pkt;
}

bool Packet::isEOF()
{
    return !memcmp(data, "eof", strlen(data)) && pts < 0.0 && dts < 0.0;
}

}