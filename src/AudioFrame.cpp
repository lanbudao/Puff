#include "AudioFrame.h"
#include "Frame_p.h"

namespace Puff {

class AudioFramePrivate : public FramePrivate {
public:
    AudioFramePrivate() {

    }
};

AudioFrame::AudioFrame():
    Frame()
{

}

AudioFrame::~AudioFrame()
{

}

}
