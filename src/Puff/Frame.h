#ifndef PUFF_FRAME_H
#define PUFF_FRAME_H

#include "AVGlobal.h"
#include "ByteArray.h"
#include <vector>
#include "DPTR.h"

namespace Puff {

class FramePrivate;
class PU_AV_EXPORT Frame
{
    DPTR_DECLARE_PRIVATE(Frame)
public:
    Frame();
    virtual ~Frame();

    Frame &operator = (const Frame &other);

    /**
     * @brief A decoded frame can be packed or planar. Packed frame has only one plane but Planar frame
     * may more than one plane.
     * For audio, plane count equals channel count
     * For video, RGB format is 1, YUV420p format is 3
     * @return
     */
    int planeCount() const;

    /**
     * @brief For audio, channel count == plane count
     * For video, channel count >= plane count
     * @return
     */
    virtual int channelCount() const;

    /**
     * @brief For video, it is the size of each picture lien
     * For audio, it is the whole size of plane
     * @param plane
     * @return
     */
    int bytesPerLine(int plane = 0) const;

    ByteArray frameData();

    ByteArray data(int plane = 0) const;

    uchar* bits(int plane);

    const uchar *constBits(int plane) const;

    void setBits(uchar *b, int plane = 0);
    void setBits(const std::vector<uchar *> &bits);

    void setBytesPerLine(int lineSize, int plane);

    double timestamp() const;
    void setTimestamp(double t);

private:
    DPTR_DECLARE(Frame)
};


}
#endif //PUFF_FRAME_H
