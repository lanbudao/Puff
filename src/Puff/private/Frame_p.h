#ifndef PUFF_FRAME_P_H
#define PUFF_FRAME_P_H

#include "DPTR.h"
#include "AVGlobal.h"
#include "ByteArray.h"
#include <vector>

namespace Puff {

class FramePrivate : public DptrPrivate<Frame>
{
public:
    FramePrivate()
        : timestamp(0)
    {

    }

    virtual ~FramePrivate()
    {

    }

    std::vector<uchar *> planes; //slice
    std::vector<int> line_sizes; //stride
//    QVariantMap metadata;
    ByteArray data;
    double timestamp;
};

}
#endif //PUFF_FRAME_P_H
