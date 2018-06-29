#include "VideoDecoderFFmpegBase.h"
#include "AVDecoder_p.h"
#include "AVLog.h"

namespace Puff {

VideoDecoderFFmpegBase::VideoDecoderFFmpegBase(VideoDecoderFFmpegBasePrivate *d):
    VideoDecoder(d)
{
}

VideoDecoderFFmpegBase::~VideoDecoderFFmpegBase()
{
}

bool VideoDecoderFFmpegBase::decode(const Packet &pkt) {
    DPTR_D(VideoDecoderFFmpegBase);

    if (!isAvailable())
        return false;
    int ret = 0;
#ifdef VIDEO_DECODER_USE_VIDEO2
    int got_picture = 0;
#endif

    if (pkt.isEOF()) {
        AVPacket eofpkt;
        av_init_packet(&eofpkt);
        eofpkt.data = NULL;
        eofpkt.size = 0;
#ifdef VIDEO_DECODER_USE_VIDEO2
        ret = avcodec_decode_video2(d->codec_ctx, d->frame, &got_picture, &eofpkt);
#else
        ret = avcodec_send_packet(d->codec_ctx, &eofpkt);
#endif
    } else {
#ifdef VIDEO_DECODER_USE_VIDEO2
        ret = avcodec_decode_video2(d->codec_ctx, d->frame, &got_picture, pkt.asAVPacket());
#else
        ret = avcodec_send_packet(d->codec_ctx, pkt.asAVPacket());
#endif
    }
    if (ret < 0) {
        return false;
    }
    int decoder_reorder_pts = -1;
    ret = avcodec_receive_frame(d->codec_ctx, d->frame);
    if (ret >= 0) {
        if (decoder_reorder_pts == -1) {
            d->frame->pts = d->frame->best_effort_timestamp;
        } else if (!decoder_reorder_pts) {
            d->frame->pts = d->frame->pkt_dts;
        }
    }
    if (ret < 0) {
        avwarnning("video decode error: %s\n", averror2str(ret));
        return false;
    }
#ifdef VIDEO_DECODER_USE_VIDEO2
    d->undecoded_size = puMin(pkt.data.size() - ret, pkt.data.size());
    if (!got_picture) {
        avdebug("no frame could be decompressed: %s %d/%d\n", averror2str(ret), d->undecoded_size, pkt.data.size());
        return !pkt.isEOF();
    }
#endif
    if (!d->codec_ctx->width || !d->codec_ctx->height)
        return false;
    return true;
}

VideoFrame VideoDecoderFFmpegBase::frame()
{
    DPTR_D(VideoDecoderFFmpegBase);
    if (d->frame->width <= 0 || d->frame->height <= 0 || !d->codec_ctx)
        return VideoFrame();
    VideoFormat format(d->codec_ctx->pix_fmt);
    VideoFrame frame(d->frame->width, d->frame->height, format);
    frame.setDisplayAspectRatio(d->getDisplayAspectRatio(d->frame));
    frame.setBits(d->frame->data);
    frame.setBytesPerLine(d->frame->linesize);
    frame.setTimestamp((double)d->frame->pts / 1000.0);
    d->setColorDetails(frame);
    return frame;
}


}
