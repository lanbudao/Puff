#include "VideoDecoderFFmpegBase.h"
#include "AVDecoder_p.h"

namespace Puff {

extern ColorSpace colorSpaceFromFFmpeg(AVColorSpace space);
extern ColorRange colorRangeFromFFmpeg(AVColorRange range);

class VideoDecoderFFmpegBasePrivate: public VideoDecoderPrivate
{
public:
    VideoDecoderFFmpegBasePrivate():
        frame(NULL),
        undecoded_size(0)
    {
        avcodec_register_all();
        frame = av_frame_alloc();
    }
    ~VideoDecoderFFmpegBasePrivate()
    {
        if (frame) {
            av_frame_free(&frame);
            frame = NULL;
        }
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

    AVFrame *frame;
    int undecoded_size;
};

VideoDecoderFFmpegBase::VideoDecoderFFmpegBase():
    VideoDecoder()
{
}

VideoDecoderFFmpegBase::~VideoDecoderFFmpegBase()
{
}

bool VideoDecoderFFmpegBase::decode(const Packet &pkt) {
    DPTR_D(VideoDecoderFFmpegBase);

    if (!isAvailable())
        return false;
    int got_picture = 0, ret = 0;

    if (pkt.isEOF()) {
        AVPacket eofpkt;
        av_init_packet(&eofpkt);
        eofpkt.data = NULL;
        eofpkt.size = 0;
        ret = avcodec_decode_video2(d.codec_ctx, d.frame, &got_picture, &eofpkt);
    } else {
        ret = avcodec_decode_video2(d.codec_ctx, d.frame, &got_picture, pkt.asAVPacket());
    }
    d.undecoded_size = puMin(pkt.data.size() - ret, pkt.data.size());
    if (ret < 0) {
        return false;
    }
    if (!got_picture) {
        printf("no frame could be decompressed: %s %d/%d", averror2str(ret), d.undecoded_size, pkt.data.size());
        return !pkt.isEOF();
    }
    if (!d.codec_ctx->width || !d.codec_ctx->height)
        return false;
    return true;
}

VideoFrame VideoDecoderFFmpegBase::frame()
{
    DPTR_D(VideoDecoderFFmpegBase);
    if (d.frame->width <= 0 || d.frame->height <= 0 || !d.codec_ctx)
        return VideoFrame();
    VideoFrame frame(d.frame->width, d.frame->height, VideoFormat(d.codec_ctx->pix_fmt));
    frame.setDisplayAspectRatio(d.getDisplayAspectRatio(d.frame));
    frame.setBits((int8_t**)d.frame->data);
    frame.setBytesPerLine(d.frame->linesize);
    frame.setTimestamp((double)d.frame->pts / 1000.0);
    d.setColorDetails(frame);
    return frame;
}


}