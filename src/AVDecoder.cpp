#include "AVDecoder.h"
#include "AVDecoder_p.h"
#include "util.h"

namespace Puff {

static AVCodec *findCodec(const std::string &name, const std::string &hwaccel, AVCodecID id)
{
    std::string fullName(name);
    if (fullName.empty()) {
        if (hwaccel.empty())
            return avcodec_find_decoder(id);
        fullName = format("%s_%s", avcodec_get_name(id), hwaccel);
    }
    AVCodec *codec = avcodec_find_decoder_by_name(fullName.c_str());
    if (!codec)
        return NULL;
    const AVCodecDescriptor *des = avcodec_descriptor_get_by_name(fullName.c_str());
    if (des)
        return avcodec_find_decoder(des->id);
    return NULL;
}

AVDecoder::AVDecoder(AVDecoderPrivate *d):
    CObject(),
    d_ptr(d)
{
    avcodec_register_all();
}

AVDecoder::~AVDecoder()
{

}

AVCodecContext * AVDecoder::codecCtx()
{
    DPTR_D(AVDecoder);
    return d->codec_ctx;
}

bool AVDecoder::open()
{
    DPTR_D(AVDecoder);

    d->opened = false;
    if (!d->codec_ctx) {
        return false;
    }
    AVCodec *codec = findCodec(d->codec_name, d->hwaccel, d->codec_ctx->codec_id);
    if (!codec) {
        //TODO error signal
        return false;
    }
    if (!d->open()) {
        d->close();
        return false;
    }
    d->opened = true;
    AV_RUN_CHECK(avcodec_open2(d->codec_ctx, codec, &d->dict), false);
    return true;
}

bool AVDecoder::close()
{
    DPTR_D(AVDecoder);
    if (!d->opened)
        return true;
    flush();
    d->close();
    if (d->codec_ctx)
        AV_RUN_CHECK(avcodec_close(d->codec_ctx), false);
    return true;
}

bool AVDecoder::isOpen()
{
    DPTR_D(const AVDecoder);
    return d->opened;
}

void AVDecoder::flush()
{
    DPTR_D(AVDecoder);
    d->flush();
}

void AVDecoder::setCodecName(const std::string &name)
{
    DPTR_D(AVDecoder);

    if (d->codec_name == name)
        return;
    d->codec_name = name;
}

std::string AVDecoder::codecName() const
{
    DPTR_D(const AVDecoder);
    return d->codec_name;
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

bool AVDecoder::isAvailable() const {
    DPTR_D(const AVDecoder);
    return d->codec_ctx != NULL;
}

void AVDecoder::setCodecCtx(void *ctx)
{
    DPTR_D(AVDecoder);
    AVCodecContext *codec_ctx = (AVCodecContext *)ctx;
    if (codec_ctx == d->codec_ctx)
        return;
    if (isOpen())
        close();
    if (!codec_ctx) {
        avcodec_free_context(&d->codec_ctx);
        d->codec_ctx = NULL;
        return;
    }
    if (!d->codec_ctx)
        d->codec_ctx = avcodec_alloc_context3(codec_ctx->codec);
    if (!d->codec_ctx)
        return;
    AV_ENSURE_OK(avcodec_copy_context(d->codec_ctx, codec_ctx));
}

}
