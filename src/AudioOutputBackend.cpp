#include "AudioOutputBackend.h"
#include "AudioOutputBackend_p.h"
#include "Factory.h"

namespace Puff {

FACTORY_DEFINE(AudioOutputBackend)

void AudioOutput_RegisterAll()
{
    ONLY_RUN_ONES;
    if (!AudioOutputBackendFactory::instance().registeredIds().empty()) {
        return;
    }
    extern bool RegisterAudioOutputBackendPortAudio_Man();
    RegisterAudioOutputBackendPortAudio_Man();
}

AudioOutputBackend::AudioOutputBackend():
    d_ptr(new AudioOutputBackendPrivate)
{

}

AudioOutputBackend::~AudioOutputBackend()
{

}

bool AudioOutputBackend::avaliable() const
{
    DPTR_D(const AudioOutputBackend);
    return d->avaliable;
}

void AudioOutputBackend::setAvaliable(bool b)
{
    DPTR_D(AudioOutputBackend);
    d->avaliable = b;
}

const AudioFormat *AudioOutputBackend::format()
{
    DPTR_D(const AudioOutputBackend);
    return &d->format;
}

}
