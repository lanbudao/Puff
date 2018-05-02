#ifndef PUFF_AUDIOFRAME_H
#define PUFF_AUDIOFRAME_H

#include "Frame.h"
#include "AudioFormat.h"

namespace Puff {

class AudioFramePrivate;
class PU_AV_EXPORT AudioFrame: public Frame
{
    DPTR_DECLARE_PRIVATE(AudioFrame)
public:
    AudioFrame();
    virtual ~AudioFrame();

    bool isValid() const;

    int channelCount() const;

    int64_t duration();

    AudioFormat format() const;

    int samplePerChannel() const;
    void setSamplePerChannel(int channel);

private:

};


}
#endif //PUFF_AUDIOFRAME_H
