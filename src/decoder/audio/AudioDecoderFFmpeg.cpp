#include "AudioDecoder.h"
#include "Factory.h"
#include "AVDecoder_p.h"
#include "commpeg.h"
#include "DPTR.h"
#include "util.h"
#include "AVLog.h"
#include "mkid.h"

namespace Puff {

class AudioDecoderFFmpegPrivate: public AudioDecoderPrivate
{
public:
    AudioDecoderFFmpegPrivate():
        frame(NULL)
    {
        frame = av_frame_alloc();
    }
    ~AudioDecoderFFmpegPrivate()
    {
        if (frame) {
            av_frame_free(&frame);
            frame = NULL;
        }
    }

    AVFrame *frame;
};

class AudioDecoderFFmpeg: public AudioDecoder
{
public:
    AudioDecoderFFmpeg();
    ~AudioDecoderFFmpeg();

    AudioDecoderId id() const;
    std::string description() const;

    AudioFrame frame();

    bool decode(const Packet& packet);
};

AudioDecoderId AudioDecoderId_FFmpeg = mkid::id32base36_6<'F', 'F', 'm', 'p', 'e', 'g'>::value;
FACTORY_REGISTER(AudioDecoder, FFmpeg, "FFmpeg")

AudioDecoderFFmpeg::AudioDecoderFFmpeg()
{

}

AudioDecoderFFmpeg::~AudioDecoderFFmpeg()
{

}

AudioDecoderId AudioDecoderFFmpeg::id() const
{
    return AudioDecoderId_FFmpeg;
}

std::string AudioDecoderFFmpeg::description() const
{
    const int patch = PUFF_VERSION_PATCH((int)avcodec_version());
    return format("%s avcodec %d.%d.%d",
            (patch >= 100 ? "FFmpeg" : "Libav"),
            (PUFF_VERSION_MAJOR((int)avcodec_version())),
            (PUFF_VERSION_MINOR((int)avcodec_version())),
            patch);
}

AudioFrame AudioDecoderFFmpeg::frame()
{
    DPTR_D(AudioDecoderFFmpeg);

    AudioFormat format;
    format.setSampleFormatFFmpeg(d.frame->format);
    format.setChannelLayoutFFmpeg(d.frame->channel_layout);
    format.setSampleRate(d.frame->sample_rate);
    if (!format.isValid())
        return AudioFrame();
    AudioFrame f(format);
    f.setBits(d.frame->extended_data);
    f.setBytesPerLine(d.frame->linesize[0], 0);
    f.setSamplePerChannel(d.frame->nb_samples);
    f.setTimestamp((double)d.frame->pkt_pts / 1000.0);
//    f.setAudioResampler(d.resampler);
    return f;
}

bool AudioDecoderFFmpeg::decode(const Packet &pkt)
{
    DPTR_D(AudioDecoderFFmpeg);
    if (!isAvailable())
        return false;
    int got_frame = 0, ret = 0;

    if (pkt.isEOF()) {
        AVPacket eofpkt;
        av_init_packet(&eofpkt);
        eofpkt.data = NULL;
        eofpkt.size = 0;
        ret = avcodec_decode_audio4(d.codec_ctx, d.frame, &got_frame, &eofpkt);
    } else {
        ret = avcodec_decode_audio4(d.codec_ctx, d.frame, &got_frame, pkt.asAVPacket());
    }
    d.undecoded_size = puMin(pkt.data.size() - ret, pkt.data.size());
    if (ret == AVERROR(EAGAIN))
        return false;
    if (ret < 0) {
        avwarnning("audio decode error: %s", averror2str(ret));
        return false;
    }
    if (!got_frame) {
        avdebug("no frame could be decompressed: %s %d/%d", averror2str(ret), d.undecoded_size, pkt.data.size());
        return !pkt.isEOF();
    }
    return true;
}

}
