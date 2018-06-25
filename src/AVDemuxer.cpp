#include "AVDemuxer_p.h"

namespace Puff {

AVDemuxer::AVDemuxer():
    d_ptr(new AVDemuxerPrivate)
{
    DPTR_D(AVDemuxer);
    d->interruptHandler = new InterruptHandler(this);
}

AVDemuxer::~AVDemuxer()
{

}

void AVDemuxer::setMedia(const std::string &fileName)
{
    DPTR_D(AVDemuxer);
    d->fileName = fileName;
}

bool AVDemuxer::load()
{
    DPTR_D(AVDemuxer);
    unload();

    ReadLock lock(&d->mutex);
    PU_UNUSED(lock);
    int ret = 0;

    if (d->fileName.empty()) {
        return false;
    }
    if (!d->format_ctx) {
        d->format_ctx = avformat_alloc_context();
        d->format_ctx->interrupt_callback = *(d->interruptHandler->handler());
    }

    d->format_ctx->flags |= AVFMT_FLAG_GENPTS;
    /*Open Stream*/
    d->interruptHandler->begin(InterruptHandler::OpenStream);
    ret = avformat_open_input(&d->format_ctx, d->fileName.data(), d->input_format, &d->format_opts);
    d->interruptHandler->end();

    if (ret < 0) {
        AVError::ErrorCode code = AVError::OpenError;
        return false;
    }

    /*Find Stream*/
    d->interruptHandler->begin(InterruptHandler::FindStream);
    ret = avformat_find_stream_info(d->format_ctx, NULL);
    d->interruptHandler->end();

    if (ret < 0) {
        AVError::ErrorCode code = AVError::FindStreamError;
        return false;
    }

    if (!d->findStreams()) {
        return false;
    }
    d->seekable = d->checkSeekable();

    return true;
}

void AVDemuxer::unload()
{
    DPTR_D(AVDemuxer);

    DeclReadLockMutex(&d->mutex);
    d->resetStreams();
    if (d->format_ctx) {
        avformat_close_input(&d->format_ctx);
        d->format_ctx = NULL;
    }
}

bool AVDemuxer::atEnd()
{
    DPTR_D(AVDemuxer);

    if (!d->format_ctx)
        return true;
    return d->isEOF;
}

bool AVDemuxer::isLoaded() const
{
    DPTR_D(const AVDemuxer);
    return d->format_ctx && (d->video_stream_info.codec_ctx || d->audio_stream_info.codec_ctx || d->subtitle_stream_info.codec_ctx);
}

bool AVDemuxer::isSeekable() const
{
    DPTR_D(const AVDemuxer);
    return d->seekable;
}

bool AVDemuxer::seek(uint64_t ms)
{
    DPTR_D(AVDemuxer);
    if (!isSeekable())
        return false;
    if (!isLoaded())
        return false;
    uint64_t upos = ms * 1000LL;
    if (upos > startTimeUs() + durationUs() || upos < 0LL) {
        avwarnning("Invalid seek position %lld %.2f. valid range [%lld, %lld]", upos,
                   double(upos) / double(durationUs()), startTimeUs(), startTimeUs() + durationUs());
        return false;
    }
    bool backward = d->seek_type == AccurateSeek || upos <= (d->curPkt.pts * AV_TIME_BASE);
    int seek_flag = backward ? AVSEEK_FLAG_BACKWARD : 0;
    if (d->seek_type == AccurateSeek) {
        seek_flag = AVSEEK_FLAG_BACKWARD;
    } else if (d->seek_type == AnyFrameSeek) {
        seek_flag == AVSEEK_FLAG_ANY;
    }
    int ret = av_seek_frame(d->format_ctx, -1, upos, seek_flag);
    if (ret < 0 && (seek_flag & AVSEEK_FLAG_BACKWARD)) {
        avwarnning("av_seek_frame error with flag AVSEEK_FLAG_BACKWARD: %s. try to seek without the flag\n", averror2str(ret));
        seek_flag &= ~AVSEEK_FLAG_BACKWARD;
        ret = av_seek_frame(d->format_ctx, -1, upos, seek_flag);
    }
    if (ret < 0) {
        avwarnning("av_seek_frame still error without flag AVSEEK_FLAG_BACKWARD\n");
        return false;
    }
    return true;
}

void AVDemuxer::setSeekType(SeekType type)
{
    DPTR_D(AVDemuxer);
    d->seek_type = type;
}

int AVDemuxer::stream()
{
    DPTR_D(const AVDemuxer);
    return d->stream;
}

const Packet& AVDemuxer::packet() const
{
    DPTR_D(const AVDemuxer);
    return d->curPkt;
}

int AVDemuxer::readFrame()
{
    DPTR_D(AVDemuxer);

    ReadLock lock(&d->mutex);
    PU_UNUSED(lock);

    int ret = -1;
    AVPacket avpkt;

    d->curPkt = Packet();
    av_init_packet(&avpkt);

    d->interruptHandler->begin(InterruptHandler::ReadStream);
    ret = av_read_frame(d->format_ctx, &avpkt);
    d->interruptHandler->end();

    if (ret < 0) {
        if (ret == AVERROR_EOF) {
            if (!d->isEOF) {

                d->isEOF = true;
            }
        }
        return ret;
    }
    d->stream = avpkt.stream_index;
    if (d->stream != videoStream() && d->stream != audioStream() && d->stream != subtitleStream()) {
        av_packet_unref(&avpkt);
        return -1;
    }

    d->curPkt = Packet::fromAVPacket(&avpkt, av_q2d(d->format_ctx->streams[d->stream]->time_base));
    av_packet_unref(&avpkt);
    d->isEOF = false;

    return ret;
}

int AVDemuxer::videoStream() {
    DPTR_D(const AVDemuxer);
    return d->video_stream_info.stream;
}

int AVDemuxer::audioStream() {
    DPTR_D(const AVDemuxer);
    return d->audio_stream_info.stream;
}

int AVDemuxer::subtitleStream() {
    DPTR_D(const AVDemuxer);
    return d->subtitle_stream_info.stream;
}

bool AVDemuxer::setStreamIndex(AVDemuxer::StreamType type, int index)
{
    DPTR_D(AVDemuxer);
    std::vector<int> streams;
    AVDemuxerPrivate::StreamInfo *info = NULL;

    if (type == Stream_Video) {
        streams = d->video_stream_info.streams;
        info = &d->video_stream_info;
    } else if (type == Stream_Audio) {
        streams = d->audio_stream_info.streams;
        info = &d->audio_stream_info;
    } else if (type == Stream_Subtitle) {
        streams = d->subtitle_stream_info.streams;
        info = &d->subtitle_stream_info;
    }
    if (!info)
        return false;
    if (streams.empty())
        return false;
    if (index >= streams.size())
        return false;
    if (index < 0) {
        avdebug("disable %d stream.\n", type);
        info->stream = -1;
        info->wanted_stream = -1;
        info->wanted_index = -1;
        return true;
    }
    if (!d->setMediaStream(type, streams.at(index)))
        return false;
    info->wanted_index = index;
    return true;
}

bool AVDemuxer::hasAttachedPic() const
{
    DPTR_D(const AVDemuxer);
    return d->has_attached_pic;
}

AVCodecContext *AVDemuxer::audioCodecCtx(int stream) const
{
    DPTR_D(const AVDemuxer);
    if (stream < 0)
        return d->audio_stream_info.codec_ctx;
    if (stream >= d->format_ctx->nb_streams)
        return NULL;
    AVCodecContext *ctx = d->format_ctx->streams[stream]->codec;
    if (ctx->codec_type == AVMEDIA_TYPE_AUDIO)
        return ctx;
    return NULL;
}

AVCodecContext *AVDemuxer::videoCodecCtx(int stream) const
{
    DPTR_D(const AVDemuxer);
    if (stream < 0)
        return d->video_stream_info.codec_ctx;
    if (stream >= d->format_ctx->nb_streams)
        return NULL;
    AVCodecContext *ctx = d->format_ctx->streams[stream]->codec;
    if (ctx->codec_type == AVMEDIA_TYPE_VIDEO)
        return ctx;
    return NULL;
}

AVCodecContext *AVDemuxer::subtitleCodecCtx(int stream) const
{
    DPTR_D(const AVDemuxer);
    if (stream < 0)
        return d->subtitle_stream_info.codec_ctx;
    if (stream >= d->format_ctx->nb_streams)
        return NULL;
    AVCodecContext *ctx = d->format_ctx->streams[stream]->codec;
    if (ctx->codec_type == AVMEDIA_TYPE_SUBTITLE)
        return ctx;
    return NULL;
}

void AVDemuxer::setStatistics(Statistics *s)
{
    DPTR_D(AVDemuxer);
    d->statistics = s;
}

void AVDemuxer::initBaseStatistics()
{
    DPTR_D(AVDemuxer);
    d->statistics->url = d->fileName;
    d->statistics->bit_rate = d->format_ctx->bit_rate;
    d->statistics->start_time = d->startTimeBase() / AV_TIME_BASE;
    d->statistics->duration = d->durationBase() / AV_TIME_BASE;
}

void AVDemuxer::initAudioStatistics()
{
    DPTR_D(AVDemuxer);
    AVCodecContext *avctx = audioCodecCtx();
    d->statistics->audio = Statistics::Common();
    d->statistics->audio_only = Statistics::AudioOnly();
    if (!avctx) {
        avwarnning("Invalid codec context!\n");
        return;
    }
    d->statistics->audio.avaliable = true;
    d->initCommonStatistics(&(d->statistics->audio), audioStream(), avctx);
    d->statistics->audio_only.channels = avctx->channels;
    d->statistics->audio_only.sample_rate = avctx->sample_rate;
    d->statistics->audio_only.frame_size = avctx->frame_size;
    d->statistics->audio_only.sample_fmt = av_get_sample_fmt_name(avctx->sample_fmt);
    char buf[128];
    av_get_channel_layout_string(buf, sizeof(buf), avctx->channels, avctx->channel_layout);
    d->statistics->audio_only.channel_layout = std::string(buf);
}

void AVDemuxer::initVideoStatistics()
{
    DPTR_D(AVDemuxer);
    AVCodecContext *avctx = videoCodecCtx();
    int vstream = videoStream();
    d->statistics->video = Statistics::Common();
    d->statistics->video_only = Statistics::VideoOnly();
    if (!avctx) {
        avwarnning("Invalid codec context!\n");
        return;
    }
    d->statistics->video.avaliable = true;
    d->initCommonStatistics(&(d->statistics->video), vstream, avctx);
    d->statistics->video_only.coded_width = avctx->coded_width;
    d->statistics->video_only.coded_height = avctx->coded_height;
    d->statistics->video_only.width = avctx->width;
    d->statistics->video_only.height = avctx->height;
    d->statistics->video_only.rotate = 0;
#if AV_MODULE_CHECK(LIBAVFORMAT, 55, 18, 0, 39, 100)
    if (vstream < 0 || (unsigned int)vstream >= d->format_ctx->nb_streams)
        return;
    AVRational ratio = d->format_ctx->streams[vstream]->display_aspect_ratio;
    d->statistics->video_only.aspect_ratio = 0.0;
    if (ratio.den && ratio.num) {
        d->statistics->video_only.aspect_ratio = ratio.den / ratio.num;
    }
    uint8_t *sd = av_stream_get_side_data(d->format_ctx->streams[vstream], AV_PKT_DATA_DISPLAYMATRIX, NULL);
    if (sd) {
        double r = av_display_rotation_get((int32_t*)sd);
        if (!isnan(r))
            d->statistics->video_only.rotate = ((int)r + 360) % 360;
    }
#endif
}

uint64_t AVDemuxer::startTime()
{
    DPTR_D(AVDemuxer);
    return d->startTimeBase() / AV_TIME_BASE;
}

uint64_t AVDemuxer::duration()
{
    DPTR_D(AVDemuxer);
    return d->durationBase() / AV_TIME_BASE;
}

uint64_t AVDemuxer::startTimeUs()
{
    return startTime() * 1000000L;
}

uint64_t AVDemuxer::durationUs()
{
    return duration() * 1000000L;
}

}
