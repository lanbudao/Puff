#include "AudioResample.h"
#include "AudioResample_p.h"
#include "Factory.h"

namespace Puff {

FACTORY_DEFINE(AudioResample)

void AudioResample_RegisterAll()
{
    ONLY_RUN_ONES;
    if (!AudioResampleFactory::instance().registeredIds().empty()) {
        return;
    }
    extern bool RegisterAudioResampleFFmpeg_Man();
    RegisterAudioResampleFFmpeg_Man();
}

AudioResample::AudioResample():
    d_ptr(new AudioResamplePrivate)
{

}

AudioResample::~AudioResample()
{

}

}
