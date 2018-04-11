#include "AudioDecoder.h"
#include "AVDecoder_p.h"

namespace MSAV {

class AudioDecoderPrivate: public AVDecoderPrivate
{
public:
    AudioDecoderPrivate() {}
    ~AudioDecoderPrivate() {}
};

AudioDecoder::AudioDecoder()
{

}

AudioDecoder::~AudioDecoder()
{

}

StringList AudioDecoder::supportedCodecs()
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

}