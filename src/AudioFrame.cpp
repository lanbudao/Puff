#include "AudioFrame.h"
#include "Frame_p.h"

namespace Puff {

class AudioFramePrivate : public FramePrivate {
public:
    AudioFramePrivate():
        samples_per_channel(0)
    {

    }

    AudioFormat format;
    int samples_per_channel;
};

AudioFrame::AudioFrame():
    Frame()
{

}

AudioFrame::~AudioFrame()
{

}

bool AudioFrame::isValid() const {
    DPTR_D(const AudioFrame);
    return d.format.isValid() && d.samples_per_channel > 0;
}

int AudioFrame::channelCount() const {
    DPTR_D(const AudioFrame);
    if (d.format.isValid())
        return d.format.channels();
    return 0;
}

int64_t AudioFrame::duration() {
    DPTR_D(AudioFrame);
    return d.format.durationForBytes(d.data.size());
}

AudioFormat AudioFrame::format() const {
    DPTR_D(const AudioFrame);
    return d.format;
}

int AudioFrame::samplePerChannel() const {
    DPTR_D(const AudioFrame);
    return d.samples_per_channel;
}

void AudioFrame::setSamplePerChannel(int channel) {
    DPTR_D(AudioFrame);
    if (!d.format.isValid())
        return;
    d.samples_per_channel = channel;
    int nb_planes = d.format.planeCount();
    int bytesPerLine = 0;

    if (d.line_sizes[0] > 0) {
        bytesPerLine = d.line_sizes[0];
    } else {
        bytesPerLine = d.samples_per_channel *
                       d.format.bytesPerSample() *
                       d.format.isPlanar() ? 1 : d.format.channels();
    }
    for (int i = 0; i < nb_planes; ++i) {
        setBytesPerLine(bytesPerLine, i);
    }
    if (d.data.isEmpty())
        return;
    if (!constBits(0))
        setBits((uchar *)d.data.constData(), 0);
    for (int i = 1; i < nb_planes; ++i) {
        if (!constBits(i)) {
            setBits((uchar *)constBits(i - 1) + bytesPerLine, i);
        }
    }
}

}
