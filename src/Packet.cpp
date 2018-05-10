#include "commpeg.h"
#include "Packet.h"

namespace Puff {

class PacketPrivate: public DptrPrivate<Packet>
{
public:
    PacketPrivate()
    {
        av_init_packet(&avpkt);
    }
    ~PacketPrivate()
    {
        av_free_packet(&avpkt);
    }

    mutable AVPacket avpkt;
};

Packet::Packet()
{

}

Packet::~Packet()
{

}

Packet::Packet(const Packet &other)
{
    dptr_d = other.dptr_d;
    containKeyFrame = other.containKeyFrame;
    isCorrupted = other.isCorrupted;
    pts = other.pts;
    duration = other.duration;
    dts = other.dts;
    pos = other.pos;
    data = other.data;
}

Packet &Packet::operator =(const Packet &other)
{
    if (this == &other)
        return *this;
    dptr_d = other.dptr_d;
    containKeyFrame = other.containKeyFrame;
    isCorrupted = other.isCorrupted;
    pts = other.pts;
    duration = other.duration;
    dts = other.dts;
    pos = other.pos;
    data = other.data;
    return *this;
}

Packet Packet::fromAVPacket(const AVPacket *packet, double time_base)
{
    Packet pkt;

    pkt.pos = packet->pos;
    pkt.containKeyFrame = !!(packet->flags & AV_PKT_FLAG_KEY);
    pkt.isCorrupted = !!(packet->flags & AV_PKT_FLAG_CORRUPT);

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

    AVPacket *p = pkt.avpacket();
    av_packet_ref(p, packet);  //properties are copied internally

    pkt.size = packet->size;
    pkt.data = ByteArray((char*)packet->data, packet->size);

    return pkt;
}

bool Packet::isEOF() const
{
    return !memcmp(data.constData(), "eof", data.size()) && pts < 0.0 && dts < 0.0;
}

AVPacket *Packet::avpacket()
{
    DPTR_D(Packet);
    return &d.avpkt;
}

const AVPacket *Packet::asAVPacket() const
{
    DPTR_D(const Packet);
    AVPacket *p = &d.avpkt;
    p->pts = int64_t(pts * 1000.0);
    p->dts = int64_t(dts * 1000.0);
    p->duration = int(duration * 1000.0);
    p->pos = pos;
    if (isCorrupted)
        p->flags |= AV_PKT_FLAG_CORRUPT;
    if (containKeyFrame)
        p->flags |= AV_PKT_FLAG_KEY;
    if (!data.isEmpty()) {
        p->data = (uint8_t *)data.constData();
        p->size = data.size();
    }
    return p;
}

}
