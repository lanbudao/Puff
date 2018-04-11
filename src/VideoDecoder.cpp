#include "VideoDecoder.h"
#include "AVDecoder_p.h"

namespace MSAV {

class VideoDecoderPrivate: public AVDecoderPrivate
{
public:
    VideoDecoderPrivate() {}
    ~VideoDecoderPrivate() {}
};

VideoDecoder::VideoDecoder()
{

}

VideoDecoder::~VideoDecoder()
{

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

}