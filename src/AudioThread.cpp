#include "AudioThread.h"

namespace MSAV {

class AudioThreadPrivate: public DptrPrivate<AudioThread>
{
public:
    AudioThreadPrivate() {}
    ~AudioThreadPrivate() {}
};

AudioThread::AudioThread() {

}

AudioThread::~AudioThread() {

}

void AudioThread::run()
{
    while (true) {

    }
}

}