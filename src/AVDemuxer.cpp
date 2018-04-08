#include "AVDemuxer.h"
#include "AVError.h"
#include "Packet.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavcodec/avcodec.h"
#ifdef __cplusplus
}
#endif

namespace MSAV {

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
        interruptHandler(NULL)
    {
        av_register_all();
    }
    ~AVDemuxerPrivate() MS_NO_COPY
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

        setMediaStream(AVMEDIA_TYPE_VIDEO, -1);
        setMediaStream(AVMEDIA_TYPE_AUDIO, -1);
        setMediaStream(AVMEDIA_TYPE_SUBTITLE, -1);

        return true;
    }

    bool setMediaStream(AVMediaType type, int value)
    {
        StreamInfo *info = NULL;

        if (type == AVMEDIA_TYPE_VIDEO) {
            info = &video_stream_info;
        } else if (type == AVMEDIA_TYPE_AUDIO) {
            info = &audio_stream_info;
        } else if (type == AVMEDIA_TYPE_SUBTITLE) {
            info = &subtitle_stream_info;
        }

        if (!info)
            return false;
        int s = AVERROR_STREAM_NOT_FOUND;
        if (info->wanted_index >= 0 && info->wanted_index < info->streams.size()) {
            s = info->streams.at(info->wanted_index);
        } else {
            s = av_find_best_stream(format_ctx, type, value, -1, NULL, 0);
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
    hash<String> format_dict;
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

    Packet curPkt;
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

bool AVDemuxer::readFrame()
{
    DPTR_D(AVDemuxer);

    int ret = 0;
    int stream_index = -1;
    AVPacket avpkt;

    d.curPkt = Packet();
    av_init_packet(&avpkt);

    d.interruptHandler->begin(InterruptHandler::ReadStream);
    ret = av_read_frame(d.format_ctx, &avpkt);
    d.interruptHandler->end();

    if (ret < 0) {
        return false;
    }
    stream_index = avpkt.stream_index;
    if (stream_index != videoStream() && stream_index != audioStream() && stream_index != subtitleStream()) {
        av_packet_unref(&avpkt);
        return false;
    }

    d.curPkt = Packet::fromAVPacket(&avpkt, av_q2d(d.format_ctx->streams[stream_index]->time_base));
    av_packet_unref(&avpkt);

    return true;
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

}