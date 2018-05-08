#include "AVDemuxer.h"
#include "AVError.h"
#include "Packet.h"
#include "commpeg.h"
#include "AVLog.h"

namespace Puff {

class InterruptHandler
{
public:
    InterruptHandler(AVDemuxer *demuxer):
        mCurAction(None),
        mDemuxer(demuxer),
        interruptCB(new AVIOInterruptCB())
    {
        interruptCB->callback = handleInterrupt;
        interruptCB->opaque = this;
    }
    enum Action
    {
        None = 0,
        OpenStream,
        FindStream,
        ReadStream
    };
    ~InterruptHandler()
    {
        delete interruptCB;
    }
    AVIOInterruptCB *handler() {return interruptCB;}
    Action action() const {return mCurAction;}
    void begin(Action action)
    {

    }
    void end()
    {

    }

    void setInterruptTimeout(int64_t timeout) { mTimeout = timeout;}

    static int handleInterrupt(void *obj)
    {
        InterruptHandler *handler = static_cast<InterruptHandler *>(obj);
        if (!handler)
            return 0;
        switch (handler->action()) {
            case OpenStream:
                break;
            case FindStream:
                break;
            case ReadStream:
                break;
            default:
                break;
        }
    }

private:
    int64_t mTimeout;
    Action mCurAction;
    AVIOInterruptCB *interruptCB;
    AVDemuxer *mDemuxer;
};

class AVDemuxerPrivate: public DptrPrivate<AVDemuxer>
{
public:
    AVDemuxerPrivate():
        format_ctx(NULL),
        input_format(NULL),
        format_opts(NULL),
        interruptHandler(NULL),
        isEOF(true),
        stream(-1)
    {
        av_register_all();
    }
    ~AVDemuxerPrivate() PU_NO_COPY
    {
        delete interruptHandler;
        if (format_opts) {
            av_dict_free(&format_opts);
            format_opts = NULL;
        }
    }

    void resetStreams()
    {
        video_stream_info.clear();
        audio_stream_info.clear();
        subtitle_stream_info.clear();
    }

    bool findStreams()
    {
        if (!format_ctx)
            return false;
        resetStreams();
        for (unsigned int i = 0; i < format_ctx->nb_streams; ++i) {
            AVMediaType type = format_ctx->streams[i]->codec->codec_type;
            if (type == AVMEDIA_TYPE_VIDEO) {
                video_stream_info.streams.push_back(i);
                video_stream_info.codec_ctx = format_ctx->streams[i]->codec;
            } else if (type == AVMEDIA_TYPE_AUDIO) {
                audio_stream_info.streams.push_back(i);
                audio_stream_info.codec_ctx = format_ctx->streams[i]->codec;
            } else if (type == AVMEDIA_TYPE_SUBTITLE) {
                subtitle_stream_info.streams.push_back(i);
                subtitle_stream_info.codec_ctx = format_ctx->streams[i]->codec;
            }
        }
        if (video_stream_info.streams.empty() &&
                audio_stream_info.streams.empty() &&
                subtitle_stream_info.streams.empty())
            return false;

        setMediaStream(AVDemuxer::Stream_Video, -1);
        setMediaStream(AVDemuxer::Stream_Audio, -1);
        setMediaStream(AVDemuxer::Stream_Subtitle, -1);

        return true;
    }

    bool setMediaStream(AVDemuxer::StreamType type, int value)
    {
        StreamInfo *info = NULL;
        AVMediaType media_type = AVMEDIA_TYPE_UNKNOWN;

        if (type == AVDemuxer::Stream_Video) {
            info = &video_stream_info;
            media_type = AVMEDIA_TYPE_VIDEO;
        } else if (type == AVDemuxer::Stream_Audio) {
            info = &audio_stream_info;
            media_type = AVMEDIA_TYPE_AUDIO;
        } else if (type == AVDemuxer::Stream_Subtitle) {
            info = &subtitle_stream_info;
            media_type = AVMEDIA_TYPE_SUBTITLE;
        }

        if (!info)
            return false;
        int s = AVERROR_STREAM_NOT_FOUND;
        if (info->wanted_index >= 0 && info->wanted_index < info->streams.size()) {
            s = info->streams.at(info->wanted_index);
        } else {
            s = av_find_best_stream(format_ctx, media_type, value, -1, NULL, 0);
        }
        if (s == AVERROR_STREAM_NOT_FOUND) {
            return false;
        }
        info->stream = s;
        info->wanted_stream = value;
        info->codec_ctx = format_ctx->streams[s]->codec;
        return true;
    }

    String fileName;
    AVFormatContext *format_ctx;
    AVInputFormat *input_format;
    AVDictionary *format_opts;
    std::hash<String> format_dict;
    InterruptHandler *interruptHandler;

    struct StreamInfo {
        std::vector<int> streams;
        int stream, wanted_stream;
        int index, wanted_index;
        AVCodecContext *codec_ctx;
        StreamInfo() { clear(); }
        void clear() {
            streams.clear();
            index = wanted_index = -1;
            stream = wanted_stream = -1;
            codec_ctx = NULL;
        }
    } video_stream_info, audio_stream_info, subtitle_stream_info;

    int stream;
    Packet curPkt;
    bool isEOF;
};

AVDemuxer::AVDemuxer()
{
    DPTR_D(AVDemuxer);
    d.interruptHandler = new InterruptHandler(this);
}

AVDemuxer::~AVDemuxer()
{

}

void AVDemuxer::setMedia(const String &fileName)
{
    DPTR_D(AVDemuxer);
    d.fileName = fileName;
}

bool AVDemuxer::load()
{
    DPTR_D(AVDemuxer);
    unload();

    int ret = 0;

    if (d.fileName.empty()) {
        return false;
    }
    if (!d.format_ctx) {
        d.format_ctx = avformat_alloc_context();
    }

    /*Open Stream*/
    d.interruptHandler->begin(InterruptHandler::OpenStream);
    ret = avformat_open_input(&d.format_ctx, d.fileName.data(), d.input_format, &d.format_opts);
    d.interruptHandler->end();

    if (ret < 0) {
        AVError::ErrorCode code = AVError::OpenError;
        return false;
    }

    if (!d.findStreams()) {
        return false;
    }
    return true;
}

void AVDemuxer::unload()
{
    DPTR_D(AVDemuxer);

    d.resetStreams();
    if (d.format_ctx) {
        avformat_close_input(&d.format_ctx);
        d.format_ctx = NULL;
    }
}

bool AVDemuxer::isLoaded() const
{
    DPTR_D(const AVDemuxer);
    return d.format_ctx && (d.video_stream_info.codec_ctx || d.audio_stream_info.codec_ctx || d.subtitle_stream_info.codec_ctx);
}

int AVDemuxer::stream()
{
    DPTR_D(const AVDemuxer);
    return d.stream;
}

Packet AVDemuxer::packet() const
{
    DPTR_D(const AVDemuxer);
    return d.curPkt;
}

int AVDemuxer::readFrame()
{
    DPTR_D(AVDemuxer);

    int ret = -1;
    AVPacket avpkt;

    d.curPkt = Packet();
    av_init_packet(&avpkt);

    d.interruptHandler->begin(InterruptHandler::ReadStream);
    ret = av_read_frame(d.format_ctx, &avpkt);
    d.interruptHandler->end();

    if (ret < 0) {
        if (ret == AVERROR_EOF) {
            if (!d.isEOF) {

                d.isEOF = true;
            }
        }
        return ret;
    }
    d.stream = avpkt.stream_index;
    if (d.stream != videoStream() && d.stream != audioStream() && d.stream != subtitleStream()) {
        av_packet_unref(&avpkt);
        return -1;
    }

    d.curPkt = Packet::fromAVPacket(&avpkt, av_q2d(d.format_ctx->streams[d.stream]->time_base));
    av_packet_unref(&avpkt);
    d.isEOF = false;

    return ret;
}

int AVDemuxer::videoStream() {
    DPTR_D(const AVDemuxer);
    return d.video_stream_info.stream;
}

int AVDemuxer::audioStream() {
    DPTR_D(const AVDemuxer);
    return d.audio_stream_info.stream;
}

int AVDemuxer::subtitleStream() {
    DPTR_D(const AVDemuxer);
    return d.subtitle_stream_info.stream;
}

bool AVDemuxer::setStreamIndex(AVDemuxer::StreamType type, int index)
{
    DPTR_D(AVDemuxer);
    std::vector<int> streams;
    AVDemuxerPrivate::StreamInfo *info = NULL;

    if (type == Stream_Video) {
        streams = d.video_stream_info.streams;
        info = &d.video_stream_info;
    } else if (type == Stream_Audio) {
        streams = d.audio_stream_info.streams;
        info = &d.audio_stream_info;
    } else if (type == Stream_Subtitle) {
        streams = d.subtitle_stream_info.streams;
        info = &d.subtitle_stream_info;
    }
    if (!info)
        return false;
    if (streams.empty())
        return false;
    if (index >= streams.size())
        return false;
    if (index < 0) {
        debug("disable %d stream.\n", type);
        info->stream = -1;
        info->wanted_stream = -1;
        info->wanted_index = -1;
        return true;
    }
    if (!d.setMediaStream(type, streams.at(index)))
        return false;
    info->wanted_index = index;
    return true;
}

AVCodecContext *AVDemuxer::audioCodecCtx(int stream) const
{
    DPTR_D(AVDemuxer);
    if (stream < 0)
        return d.video_stream_info.codec_ctx;
    if (stream >= d.format_ctx->nb_streams)
        return NULL;
    AVCodecContext *ctx = d.format_ctx->streams[stream]->codec;
    if (ctx->codec_type == AVMEDIA_TYPE_VIDEO)
        return ctx;
    return NULL;
}

AVCodecContext *AVDemuxer::videoCodecCtx(int stream) const
{
    DPTR_D(AVDemuxer);
    if (stream < 0)
        return d.audio_stream_info.codec_ctx;
    if (stream >= d.format_ctx->nb_streams)
        return NULL;
    AVCodecContext *ctx = d.format_ctx->streams[stream]->codec;
    if (ctx->codec_type == AVMEDIA_TYPE_AUDIO)
        return ctx;
    return NULL;
}

AVCodecContext *AVDemuxer::subtitleCodecCtx(int stream) const
{
    DPTR_D(AVDemuxer);
    if (stream < 0)
        return d.subtitle_stream_info.codec_ctx;
    if (stream >= d.format_ctx->nb_streams)
        return NULL;
    AVCodecContext *ctx = d.format_ctx->streams[stream]->codec;
    if (ctx->codec_type == AVMEDIA_TYPE_SUBTITLE)
        return ctx;
    return NULL;
}

}
