#ifndef PUFF_AUDIOFRAME_H
#define PUFF_AUDIOFRAME_H

#include "Frame.h"

namespace Puff {

class AudioFramePrivate;
class PU_AV_EXPORT AudioFrame: public Frame
{
    DPTR_DECLARE_PRIVATE(AudioFrame)
public:
    AudioFrame();
    virtual ~AudioFrame();

private:

};


}
#endif //PUFF_AUDIOFRAME_H
