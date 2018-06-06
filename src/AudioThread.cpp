#include "AudioThread.h"
#include "AVThread_p.h"
#include "AudioFrame.h"
#include "AudioDecoder.h"

namespace Puff {

class AudioThreadPrivate: public AVThreadPrivate
{
public:
    AudioThreadPrivate() {}
    virtual ~AudioThreadPrivate() {}
};

AudioThread::AudioThread():
    AVThread(new AudioThreadPrivate)
{

}

AudioThread::~AudioThread() {

}

void AudioThread::run()
{
    DPTR_D(AudioThread);
    auto *dec = dynamic_cast<AudioDecoder *>(d->decoder);

    Packet pkt;
    while (!d->stopped) {
        pkt = d->packets.dequeue();

        if (pkt.isEOF()) {

        } else {
            if (!pkt.isValid()) {
                continue;
            }
        }

        if (!dec->decode(pkt)) {
            pkt = Packet();
            continue;
        }
        AudioFrame frame = dec->frame();
        if (!frame.isValid()) {
            continue;
        }

//        while ()

        msleep(10);
    }
}

}
