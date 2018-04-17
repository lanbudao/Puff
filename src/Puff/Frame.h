#ifndef PUFF_FRAME_H
#define PUFF_FRAME_H

#include "AVGlobal.h"
#include "DPTR.h"

namespace Puff {

class FramePrivate;
class PU_AV_EXPORT Frame
{
    DPTR_DECLARE_PRIVATE(Frame)
public:
    Frame();
    virtual ~Frame();

private:
    DPTR_DECLARE(Frame)
};


}
#endif //PUFF_FRAME_H
