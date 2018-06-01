#include "AudioThread.h"
#include "AVThread_p.h"

namespace Puff {

class AudioThreadPrivate: public AVThreadPrivate
{
public:
    AudioThreadPrivate() {}
    ~AudioThreadPrivate() {}
};

AudioThread::AudioThread():
    AVThread(new AudioThreadPrivate)
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
