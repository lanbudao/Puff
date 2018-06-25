#include "AudioThread.h"
#include "AVThread_p.h"
#include "AudioFrame.h"
#include "AudioDecoder.h"
#include "AudioOutput.h"
#include "OutputSet.h"
#include "AVLog.h"
#include "AVClock.h"

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
    AudioOutput *ao = static_cast<AudioOutput*>(d->output->outputs().front());
    AVClock *clock = d->clock;
    Packet pkt;
    d->stopped = false;

    while (true) {
        if (d->stopped)
            break;
        if (d->paused) {
            msleep(1);
            continue;
        }
        pkt = d->packets.dequeue();
        if (pkt.isEOF()) {
            break;
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
        bool has_ao = ao && ao->isAvaliable();
        if (has_ao) {
            frame.setAudioResampler(dec->audioResample());
            frame = frame.to(ao->audioFormat());
        }
        //Write data to audio device
        const ByteArray &decoded = frame.data();
        int decodedSize = decoded.size();
        int decodedPos = 0;
        double delay = 0;
        const double byte_rate = frame.format().bytesPerSecond();
        double pts = frame.timestamp();
        clock->updateValue(pts);
        avdebug("frame samples: %d timestemp: %.3f duration: %lld\n", frame.samplePerChannel()*frame.channelCount(), frame.timestamp(), frame.duration()/1000LL);
        while (decodedSize > 0) {
            if (d->stopped) {
                avdebug("Audio thread stopped after decode\n");
                break;
            }
            const int chunk = puMin(decodedSize, ao->bufferSize());
            const double chunk_delay = (double)chunk / byte_rate;
            if (has_ao && ao->isOpen()) {
                char *decodedChunk = (char *)malloc(chunk);
                memcpy(decodedChunk, decoded.constData() + decodedPos, chunk);
                //qDebug("ao.timestamp: %.3f, pts: %.3f, pktpts: %.3f", ao->timestamp(), pts, pkt.pts);
                ao->write(decodedChunk, chunk, pts);
//                if (!is_external_clock && ao->timestamp() > 0) {//TODO: clear ao buffer
//                    d.clock->updateValue(ao->timestamp());
//                }
                free(decodedChunk);
            } else {
//                d.clock->updateDelay(delay += chunk_delay);
                /*
                 * why need this even if we add delay? and usleep sounds weird
                 * the advantage is if no audio device, the play speed is ok too
                 * So is portaudio blocking the thread when playing?
                 */
                //TODO: avoid acummulative error. External clock?
                msleep((unsigned long)(chunk_delay * 1000.0));
            }
            decodedPos += chunk;
            decodedSize -= chunk;
            pts += chunk_delay;
            pkt.pts += chunk_delay; // packet not fully decoded, use new pts in the next decoding
            pkt.dts += chunk_delay;
        }

        msleep(1);
    }
    d->packets.clear();
    CThread::run();
}

}
