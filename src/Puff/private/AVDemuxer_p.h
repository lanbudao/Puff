#ifndef PUFF_AVDEMUXER_P_H
#define PUFF_AVDEMUXER_P_H

#include "AVDemuxer.h"
#include "commpeg.h"
#include "Statistics.h"
#include "Packet.h"
#include "AVError.h"
#include "AVLog.h"
#include "CMutex.h"
#include <vector>
#include <math.h>

namespace Puff {

class InterruptHandler
{
public:
    InterruptHandler(AVDemuxer *demuxer):
        mCurAction(None),
        interruptCB(new AVIOInterruptCB()),
        mDemuxer(demuxer)
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

class AVDemuxerPrivate
{
public:
    AVDemuxerPrivate():
        format_ctx(NULL),
        input_format(NULL),
        format_opts(NULL),
        interruptHandler(NULL),
        isEOF(false),
        stream(-1),
        has_attached_pic(false),
        statistics(NULL),
        seekable(false),
        seek_type(AccurateSeek)
    {
        av_register_all();
    }
    ~AVDemuxerPrivate()
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
            } else if (type == AVMEDIA_TYPE_AUDIO) {
                audio_stream_info.streams.push_back(i);
            } else if (type == AVMEDIA_TYPE_SUBTITLE) {
                subtitle_stream_info.streams.push_back(i);
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
        has_attached_pic = !!(format_ctx->streams[s]->disposition & AV_DISPOSITION_ATTACHED_PIC);
        return true;
    }

    void initCommonStatistics(Statistics::Common *common, int stream_index, AVCodecContext *codec_ctx)
    {
        if (stream_index < 0 || stream_index >= format_ctx->nb_streams) {
            avwarnning("No such index of stream: %d\n", stream_index);
            return;
        }
        AVStream *stream = format_ctx->streams[stream_index];
        common->codec = avcodec_get_name(codec_ctx->codec_id);
        common->codec_long = get_codec_long_name(codec_ctx->codec_id);
        common->start_time = stream->start_time == AV_NOPTS_VALUE ? 0 : uint64_t(stream->start_time * av_q2d(stream->time_base) * 1000);
        common->duration = stream->duration == AV_NOPTS_VALUE ? 0 : uint64_t(stream->duration * av_q2d(stream->time_base) * 1000);
        common->bit_rate = codec_ctx->bit_rate;
        common->frames = stream->nb_frames;
        if (stream->avg_frame_rate.den && stream->avg_frame_rate.num) {
            common->frame_rate = av_q2d(stream->avg_frame_rate);
        } else if (stream->r_frame_rate.den && stream->r_frame_rate.num) {
            common->frame_rate = av_q2d(stream->r_frame_rate);
        }
    }

    uint64_t startTimeBase()
    {
        if (!format_ctx || format_ctx->start_time == AV_NOPTS_VALUE)
            return 0;
        return format_ctx->start_time;
    }

    uint64_t durationBase()
    {
        if (!format_ctx || format_ctx->duration == AV_NOPTS_VALUE)
            return 0;
        return format_ctx->duration;
    }

    bool checkSeekable()
    {
        bool flag = false;
        if (!format_ctx)
            return flag;
        if (format_ctx->pb)
            flag |= !!format_ctx->pb->seekable;
        flag |= (format_ctx->iformat->read_seek || format_ctx->iformat->read_seek2);
        return flag;
    }

    std::string fileName;
    AVFormatContext *format_ctx;
    AVInputFormat *input_format;
    AVDictionary *format_opts;
    std::hash<std::string> format_dict;
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
    CMutex mutex;

    bool has_attached_pic;

    Statistics *statistics;

    bool seekable;
    SeekType seek_type;
};

}
#endif //PUFF_AVDEMUXER_P_H
