#include "AudioOutput.h"
#include "AVOutput_p.h"

namespace Puff {

class AudioOutputPrivate: public AVOutputPrivate
{
public:
    AudioOutputPrivate()
    {

    }
    ~AudioOutputPrivate()
    {

    }

    AudioFormat format;
};

AudioOutput::AudioOutput():
    AVOutput(new AudioOutputPrivate)
{

}

AudioOutput::~AudioOutput()
{

}

bool AudioOutput::open()
{

    return true;
}

bool AudioOutput::close()
{

    return true;
}

AudioFormat AudioOutput::setAudioFormat(const AudioFormat &format)
{
    DPTR_D(AudioOutput);
//    if (d->format == format)
        return format;
}

}
