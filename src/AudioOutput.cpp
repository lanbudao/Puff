#include "AudioOutput.h"
#include "AVOutput_p.h"
#include "AudioOutputBackend.h"

namespace Puff {

class AudioOutputPrivate: public AVOutputPrivate
{
public:
    AudioOutputPrivate():
        backend(NULL)
    {

    }
    ~AudioOutputPrivate()
    {

    }

    AudioFormat format;
    std::vector<std::string> backendNames;
    AudioOutputBackend *backend;
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

void AudioOutput::setBackend(const std::vector<std::string> &names)
{
    DPTR_D(AudioOutput);
    d->backendNames = names;

    if (d->backend) {
        d->backend->close();
        delete d->backend;
        d->backend = NULL;
    }
    int size = d->backendNames.size();
    for (int i = 0; i < size; ++i) {
        d->backend = AudioOutputBackend::create(d->backendNames.at(i).c_str());
        if (!d->backend)
            continue;
        if (!d->backend->avaliable()) {
            delete d->backend;
            d->backend = NULL;
            continue;
        }
    }
    if (d->backend) {

    }
}

}
