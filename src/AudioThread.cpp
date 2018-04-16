#include "AudioThread.h"

namespace Puff {

class AudioThreadPrivate: public DptrPrivate<AudioThread>
{
public:
    AudioThreadPrivate() {}
    ~AudioThreadPrivate() {}
};

AudioThread::AudioThread()//:AVThread()
{

}

AudioThread::~AudioThread() {

}

void AudioThread::run()
{
    while (true) {

    }
}

}