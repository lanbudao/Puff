#include "AVDecoder.h"
#include "AVDecoder_p.h"
#include <boost/format.hpp>

namespace Puff {

static AVCodec *findCodec(const String &name, const String &hwaccel, AVCodecID id)
{
    String fullName(name);
    if (fullName.empty()) {
        if (hwaccel.empty())
            return avcodec_find_decoder(id);
        fullName = boost::str(boost::format("%s_%s") % avcodec_get_name(id) % hwaccel);
    }
    AVCodec *codec = avcodec_find_decoder_by_name(fullName.c_str());
    if (!codec)
        return NULL;
    const AVCodecDescriptor *des = avcodec_descriptor_get_by_name(fullName.c_str());
    if (des)
        return avcodec_find_decoder(des->id);
    return NULL;
}

AVDecoder::AVDecoder()
{
    avcodec_register_all();
}

AVDecoder::~AVDecoder()
{

}

AVCodecContext * AVDecoder::codecCtx()
{
    DPTR_D(AVDecoder);
    return d.codec_ctx;
}

bool AVDecoder::open()
{
    DPTR_D(AVDecoder);

    d.opened = false;
    if (!d.codec_ctx) {
        return false;
    }
    AVCodec *codec = findCodec(d.codec_name, d.hwaccel, d.codec_ctx->codec_id);
    if (!codec) {
        //TODO error signal
        return false;
    }
    if (!d.open()) {
        d.close();
        return false;
    }
    d.opened = true;
    AV_RUN_CHECK(avcodec_open2(d.codec_ctx, codec, &d.dict), false);
    return true;
}

bool AVDecoder::close()
{
    DPTR_D(AVDecoder);
    if (!d.opened)
        return true;
    flush();
    d.close();
    if (d.codec_ctx)
        AV_RUN_CHECK(avcodec_close(d.codec_ctx), false);
    return true;
}

bool AVDecoder::isOpen()
{
    DPTR_D(const AVDecoder);
    return d.opened;
}

void AVDecoder::flush()
{
    DPTR_D(AVDecoder);
    d.flush();
}

void AVDecoder::setCodecName(const String &name)
{
    DPTR_D(AVDecoder);

    if (d.codec_name == name)
        return;
    d.codec_name = name;
}

String AVDecoder::codecName() const
{
    DPTR_D(const AVDecoder);
    return d.codec_name;
}

void AVDecoderPrivate::flush()
{
    if (!opened)
        return;
    if (codec_ctx)
        avcodec_flush_buffers(codec_ctx);
}

void AVDecoderPrivate::applyOptionsForDict()
{
    //TODO
}

}
