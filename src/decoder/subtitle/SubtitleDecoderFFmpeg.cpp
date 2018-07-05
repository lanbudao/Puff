#include "SubtitleDecoder.h"
#include "Factory.h"
#include "AVDecoder_p.h"
#include "commpeg.h"
#include "DPTR.h"
#include "util.h"
#include "AVLog.h"
#include "mkid.h"

namespace Puff {

StringList supported_sub_extensions_ffmpeg()
{
    StringList codecs;
#if AVFORMAT_STATIC_REGISTER
    const AVInputFormat *i = NULL;
    void* it = NULL;
    while ((i = av_demuxer_iterate(&it))) {
#else
    av_register_all();
    AVInputFormat *i = NULL;
    while ((i = av_iformat_next(i))) {
#endif
        if (i->long_name && strstr(i->long_name, "subtitle")) {
            if (i->extensions) {
                Util::push_back(codecs, Util::split(i->extensions, ",", true));
            } else {
                codecs.push_back(std::string(i->name));
            }
        }
    }
    const AVCodec* c = NULL;
#if AVCODEC_STATIC_REGISTER
    it = NULL;
    while ((c = av_codec_iterate(&it))) {
#else
    avcodec_register_all();
    while ((c = av_codec_next(c))) {
#endif
        if (c->type == AVMEDIA_TYPE_SUBTITLE)
            codecs.push_back(std::string(c->name));
    }
    const AVCodecDescriptor *desc = NULL;
    while ((desc = avcodec_descriptor_next(desc))) {
        if (desc->type == AVMEDIA_TYPE_SUBTITLE)
            codecs.push_back(std::string(desc->name));
    }
    codecs.erase(unique(codecs.begin(), codecs.end()), codecs.end());
    return codecs;
}

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

    StringList supportedCodecs();

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

StringList SubtitleDecoderFFmpeg::supportedCodecs()
{
    return supported_sub_extensions_ffmpeg();
}

SubtitleDecoderId SubtitleDecoderFFmpeg::id() const
{
    return SubtitleDecoderId_FFmpeg;
}

std::string SubtitleDecoderFFmpeg::description() const
{
    return "";
}

bool SubtitleDecoderFFmpeg::decode(const Packet &pkt)
{

    return true;
}

}
