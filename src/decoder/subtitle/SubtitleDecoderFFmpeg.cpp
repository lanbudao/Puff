#include "SubtitleDecoder.h"
#include "Factory.h"
#include "AVDecoder_p.h"
#include "commpeg.h"
#include "DPTR.h"
#include "util.h"
#include "AVLog.h"
#include "mkid.h"

namespace Puff {

class SubtitleDecoderFFmpegPrivate: public SubtitleDecoderPrivate
{
public:
    SubtitleDecoderFFmpegPrivate():
        frame(nullptr)
    {
        frame = av_frame_alloc();
    }
    virtual ~SubtitleDecoderFFmpegPrivate()
    {
        if (frame) {
            av_frame_free(&frame);
            frame = nullptr;
        }
    }

    AVFrame *frame;
};

class SubtitleDecoderFFmpeg: public SubtitleDecoder
{
    DPTR_DECLARE_PRIVATE(SubtitleDecoderFFmpeg)
public:
    SubtitleDecoderFFmpeg();
    ~SubtitleDecoderFFmpeg();

    SubtitleDecoderId id() const;
    std::string description() const;

    bool decode(const Packet& packet);
};

SubtitleDecoderId SubtitleDecoderId_FFmpeg = mkid::id32base36_6<'F', 'F', 'm', 'p', 'e', 'g'>::value;
FACTORY_REGISTER(SubtitleDecoder, FFmpeg, "FFmpeg")

SubtitleDecoderFFmpeg::SubtitleDecoderFFmpeg():
    SubtitleDecoder(new SubtitleDecoderFFmpegPrivate)
{

}

SubtitleDecoderFFmpeg::~SubtitleDecoderFFmpeg()
{

}

SubtitleDecoderId SubtitleDecoderFFmpeg::id() const
{
    return SubtitleDecoderId_FFmpeg;
}

std::string SubtitleDecoderFFmpeg::description() const
{
    const int patch = PUFF_VERSION_PATCH((int)avcodec_version());
    return sformat("%s avcodec %d.%d.%d",
            (patch >= 100 ? "FFmpeg" : "Libav"),
            (PUFF_VERSION_MAJOR((int)avcodec_version())),
            (PUFF_VERSION_MINOR((int)avcodec_version())),
            patch);
}

bool SubtitleDecoderFFmpeg::decode(const Packet &pkt)
{

    return true;
}

}
