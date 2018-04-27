#include "VideoDecoder.h"
#include "AVDecoder_p.h"
#include "Factory.h"
#include "boost/functional/hash.hpp"

namespace Puff {

FACTORY_DEFINE(VideoDecoder)

boost::hash<String> hashId;
VideoDecoderId VideoDecoderId_FFmpeg = (int)hashId("FFmpeg");

static void VideoDecoder_RegisterAll()
{
    ONLY_RUN_ONES
    // factory.h does not check whether an id is registered
    if (VideoDecoder::name(VideoDecoderId_FFmpeg)) //registered on load
        return;
//    extern bool RegisterVideoDecoderFFmpeg_Man();
//    RegisterVideoDecoderFFmpeg_Man();
}

VideoDecoder::VideoDecoder()
    :AVDecoder()
{

}

VideoDecoder::~VideoDecoder()
{

}

std::vector<VideoDecoderId> VideoDecoder::registered()
{
    VideoDecoder_RegisterAll();
    return VideoDecoderFactory::instance().registeredIds();
}

StringList VideoDecoder::supportedCodecs()
{
    static StringList codecs;
    if (!codecs.empty())
        return codecs;
    avcodec_register_all();
    AVCodec* c = NULL;
    while ((c=av_codec_next(c))) {
        if (!av_codec_is_decoder(c) || c->type != AVMEDIA_TYPE_VIDEO)
            continue;
        codecs.push_back(c->name);
    }
    return codecs;
}

String VideoDecoder::name() const {
    return String();
}

String VideoDecoder::description() const {
    return String();
}

}
