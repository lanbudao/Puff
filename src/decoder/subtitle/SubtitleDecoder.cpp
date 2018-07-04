#include "SubtitleDecoder.h"
#include "AVDecoder_p.h"
#include "Factory.h"

namespace Puff {

FACTORY_DEFINE(SubtitleDecoder)

static void SubtitleDecoder_RegisterAll()
{
    ONLY_RUN_ONES;
    if (SubtitleDecoder::name(SubtitleDecoderId_FFmpeg)) {
        return;
    }
    extern bool RegisterSubtitleDecoderFFmpeg_Man();
    RegisterSubtitleDecoderFFmpeg_Man();
}

SubtitleDecoder::SubtitleDecoder(SubtitleDecoderPrivate *d):
    AVDecoder(d)
{

}

SubtitleDecoder::~SubtitleDecoder()
{

}

StringList SubtitleDecoder::supportedCodecs()
{
    static StringList codecs;
    if (!codecs.empty())
        return codecs;
    avcodec_register_all();
    AVCodec* c = NULL;
    while ((c=av_codec_next(c))) {
        if (!av_codec_is_decoder(c) || c->type != AVMEDIA_TYPE_AUDIO)
            continue;
        codecs.push_back(c->name);
    }
    return codecs;
}

std::string SubtitleDecoder::name() const {
    return std::string();
}

std::string SubtitleDecoder::description() const {
    return std::string();
}

}
