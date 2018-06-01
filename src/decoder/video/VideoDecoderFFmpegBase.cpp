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
    int got_picture = 0, ret = 0;

    if (pkt.isEOF()) {
        AVPacket eofpkt;
        av_init_packet(&eofpkt);
        eofpkt.data = NULL;
        eofpkt.size = 0;
        ret = avcodec_decode_video2(d->codec_ctx, d->frame, &got_picture, &eofpkt);
    } else {
        ret = avcodec_decode_video2(d->codec_ctx, d->frame, &got_picture, pkt.asAVPacket());
    }
    d->undecoded_size = puMin(pkt.data.size() - ret, pkt.data.size());
    if (ret < 0) {
        avwarnning("video decode error: %s", averror2str(ret));
        return false;
    }
    if (!got_picture) {
        avdebug("no frame could be decompressed: %s %d/%d", averror2str(ret), d->undecoded_size, pkt.data.size());
        return !pkt.isEOF();
    }
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
