#ifndef PUFF_PACKET_H
#define PUFF_PACKET_H

#include "Frame.h"
#include "Frame_p.h"

namespace Puff {

Frame::Frame()
{

}

Frame::~Frame()
{

}

Frame &Frame::operator=(const Frame &other) {
    dptr_d = other.dptr_d;
    return *this;
}

int Frame::planeCount() const {
    DPTR_D(const Frame);
    return d.planes.size();
}

int Frame::channelCount() const {
    return planeCount();
}

int Frame::bytesPerLine(int plane) const {
    DPTR_D(const Frame);
    if (plane < 0 || plane >= planeCount())
        return 0;
    return d.line_sizes[plane];
}

ByteArray Frame::frameData() {
    DPTR_D(Frame);
    return d.data;
}

ByteArray Frame::data(int plane) const {
    DPTR_D(const Frame);
    if (plane < 0 || plane >= planeCount())
        return ByteArray();
    return ByteArray((char*)d.planes[plane], bytesPerLine(plane));
}

uchar *Frame::bits(int plane) {
    DPTR_D(Frame);
    if (plane < 0 || plane >= planeCount())
        return NULL;
    return d.planes[plane];
}

const uchar* Frame::constBits(int plane) const {
    DPTR_D(const Frame);
    if (plane < 0 || plane >= planeCount())
        return NULL;
    return d.planes[plane];
}

void Frame::setBits(uchar *b, int plane) {
    DPTR_D(Frame);
    if (plane < 0 || plane >= planeCount())
        return;
    d.planes[plane] = b;
}

void Frame::setBits(const vector<uchar *> &bits) {
    DPTR_D(Frame);
    const int nb_plane = planeCount();
    d.planes = bits;
    if (d.planes.size() > nb_plane) {
        d.planes.reserve(nb_plane);
        d.planes.resize(nb_plane);
    }
}

void Frame::setBytesPerLine(int lineSize, int plane) {
    DPTR_D(Frame);
    if (plane < 0 || plane >= planeCount())
        return;
    d.line_sizes[plane] = lineSize;
}

double Frame::timestamp() const {
    DPTR_D(const Frame);
    return d.timestamp;
}

void Frame::setTimestamp(double t) {
    DPTR_D(Frame);
    d.timestamp = t;
}

}
#endif //PUFF_PACKET_H
