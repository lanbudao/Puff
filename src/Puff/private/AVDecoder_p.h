#ifndef PUFF_AVDECODER_P_H
#define PUFF_AVDECODER_P_H

#include "AVGlobal.h"
#include "DPTR.h"
#include "VideoFrame.h"
#include "commpeg.h"

namespace Puff {

class AVDecoderPrivate
{
public:
    AVDecoderPrivate():
        codec_ctx(NULL),
        dict(NULL),
        opened(false),
        undecoded_size(0),
        frame(NULL)
    {
        avcodec_register_all();
        codec_ctx = avcodec_alloc_context3(NULL);
        frame = av_frame_alloc();
    }
    ~AVDecoderPrivate()
    {
        if (frame) {
            av_frame_free(&frame);
            frame = NULL;
        }
        if (dict)
            av_dict_free(&dict);
        avcodec_free_context(&codec_ctx);
    }

    /*Internal Open/Close*/
    virtual bool open() {return true;}
    virtual bool close() {return true;}

    void flush();
    void applyOptionsForDict();

    bool opened;
    AVCodecContext *codec_ctx;
    std::string codec_name;
    std::string hwaccel;
    std::hash<std::string> options;
    AVDictionary *dict;
    int undecoded_size;
    AVFrame *frame;
};

class VideoDecoderPrivate: public AVDecoderPrivate
{
public:
    VideoDecoderPrivate() {}
    ~VideoDecoderPrivate() {}
};

extern ColorSpace colorSpaceFromFFmpeg(AVColorSpace space);
extern ColorRange colorRangeFromFFmpeg(AVColorRange range);

class VideoDecoderFFmpegBasePrivate: public VideoDecoderPrivate
{
public:
    VideoDecoderFFmpegBasePrivate()
    {
    }
    ~VideoDecoderFFmpegBasePrivate()
    {
    }

    void setColorDetails(VideoFrame &f)
    {
        if (f.pixelFormat() == frame->format) {
            setColorDetailFFmpeg(f);
            return;
        }
        // hw decoder output frame may have a different format, e.g. gl interop frame may have rgb format for rendering(stored as yuv)
        bool isRGB = f.format().isRGB();
        if (isRGB) {
            f.setColorSpace(f.format().isPlanar() ? ColorSpace_GBR : ColorSpace_RGB);
            f.setColorRange(ColorRange_Full);
            return;
        }
        uint8_t flags = av_pix_fmt_desc_get((AVPixelFormat)f.pixelFormatFFmpeg())->flags;
        bool isRgbCoded = ((flags & AV_PIX_FMT_FLAG_RGB) == AV_PIX_FMT_FLAG_RGB);
        if (isRgbCoded) {
            if (f.width() >= 1280 && f.height() >= 576) {
                f.setColorSpace(ColorSpace_BT709);
            } else {
                f.setColorSpace(ColorSpace_BT601);
            }
        } else {
            setColorDetailFFmpeg(f);
        }
    }

    void setColorDetailFFmpeg(VideoFrame &f)
    {
        ColorSpace space = colorSpaceFromFFmpeg(av_frame_get_colorspace(frame));
        if (space == ColorSpace_Unknown)
            space = colorSpaceFromFFmpeg(codec_ctx->colorspace);
        f.setColorSpace(space);

        ColorRange range = colorRangeFromFFmpeg(av_frame_get_color_range(frame));
        if (range == ColorRange_Unknown) {
            AVPixelFormat pixfmt_ff = (AVPixelFormat)f.pixelFormatFFmpeg();
            switch (pixfmt_ff) {
                case AV_PIX_FMT_YUVJ420P:
                case AV_PIX_FMT_YUVJ422P:
                case AV_PIX_FMT_YUVJ440P:
                case AV_PIX_FMT_YUVJ444P:
                    range = ColorRange_Full;
                    break;
                default:
                    break;
            }
        }
        if (range == ColorRange_Unknown) {
            range = colorRangeFromFFmpeg(codec_ctx->color_range);
            if (range == ColorRange_Unknown) {
                if (f.format().isXYZ()) {
                    range = ColorRange_Full;
                } else if (!f.format().isRGB()) {
                    range = ColorRange_Limited;
                }
            }
        }
        f.setColorRange(range);
    }

    float getDisplayAspectRatio(AVFrame *f)
    {
        // lavf 54.5.100 av_guess_sample_aspect_ratio: stream.sar > frame.sar
        float dar = 0;
        if (f->height > 0)
            dar = (float)f->width / (float)f->height;
        // prefer sar from AVFrame if sar != 1/1
        if (f->sample_aspect_ratio.num > 1)
            dar *= av_q2d(f->sample_aspect_ratio);
        else if (codec_ctx && codec_ctx->sample_aspect_ratio.num > 1) // skip 1/1
            dar *= av_q2d(codec_ctx->sample_aspect_ratio);
        return dar;
    }

};

class AudioDecoderPrivate: public AVDecoderPrivate
{
public:
    AudioDecoderPrivate()
    {
    }
    ~AudioDecoderPrivate()
    {
    }
};

}
#endif //PUFF_AVDECODER_P_H
