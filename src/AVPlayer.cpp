#include "AVPlayer.h"
#include "AVDemuxer.h"
#include "AVDemuxThread.h"
#include "VideoThread.h"
#include "AudioThread.h"
#include "PacketQueue.h"
#include "VideoDecoder.h"
#include "AudioDecoder.h"
#include "AudioOutput.h"
#include "OutputSet.h"
#include "commpeg.h"
#include "AVLog.h"
#include "AVClock.h"
#include "Statistics.h"
#include "VideoRenderer.h"
#include "util.h"

namespace Puff {

class AVPlayerPrivate
{
public:
    AVPlayerPrivate():
        loaded(false),
        paused(false),
        seeking(false),
        seek_type(AccurateSeek),
        demuxer(NULL),
        demux_thread(NULL),
        video_thread(NULL),
        audio_thread(NULL),
        video_track(0),
        audio_track(0),
        sub_track(-1),   /*default do not use subtitle*/
        video_dec(NULL),
        audio_dec(NULL),
        ao(new AudioOutput)
    {
        demuxer = new AVDemuxer();
        demuxer->setStatistics(&statistics);
        demux_thread = new AVDemuxThread();
        demux_thread->setDemuxer(demuxer);
        demux_thread->setClock(&clock);
        puconnect(demux_thread, seekFinished, this, &AVPlayerPrivate::onSeekFinished);

        video_dec_ids = VideoDecoder::registered();
    }
    ~AVPlayerPrivate()
    {
        delete demuxer;
        if (video_thread) {
            video_thread->wait();
            delete video_thread;
        }
        if (audio_thread) {
            audio_thread->wait();
            delete audio_thread;
        }
        if (demux_thread) {
            demux_thread->wait();
            delete demux_thread;
        }
        delete video_dec;
        delete audio_dec;
        delete ao;
    }

    bool setupAudioThread()
    {
        demuxer->setStreamIndex(AVDemuxer::Stream_Audio, audio_track);
        if (audio_thread) {
            audio_thread->packets()->clear();
            audio_thread->setDecoder(NULL);
        }
        AVCodecContext *ctx = demuxer->audioCodecCtx();
        if (!ctx)
            return false;
        if (audio_dec) {
            delete audio_dec;
            audio_dec = NULL;
        }

        AudioDecoder *dec = AudioDecoder::create();
        if (!dec)
            return false;
        dec->setCodecCtx(ctx);
        if (!dec->open()) {
            return false;
        }
        audio_dec = dec;
        if (!audio_dec) {
            avdebug("Can not found audio decoder.\n");
            return false;
        }

        AudioFormat af;
        af.setSampleRate(ctx->sample_rate);
        af.setChannelLayoutFFmpeg(ctx->channel_layout);
        af.setSampleFormatFFmpeg(ctx->sample_fmt);
        af.setChannels(ctx->channels);
        if (!af.isValid()) {
            avwarnning("Invalid audio format, disable audio!");
            return false;
        }

        ao->setAudioFormat(af);
        ao->close();
        if (!ao->open()) {
            return false;
        }

        if (!audio_thread) {
            audio_thread = new AudioThread();
            audio_output_set.addOutput(ao);
            audio_thread->setOutputSet(&audio_output_set);
            audio_thread->setClock(&clock);
            demux_thread->setAudioThread(audio_thread);
        }
        audio_thread->setDecoder(audio_dec);
        demuxer->initAudioStatistics();

        return true;
    }

    bool setupVideoThread()
    {
        demuxer->setStreamIndex(AVDemuxer::Stream_Video, video_track);
        if (video_thread) {
            video_thread->packets()->clear();
            video_thread->setDecoder(NULL);
        }
        AVCodecContext *ctx = demuxer->videoCodecCtx();
        if (!ctx)
            return false;
        if (video_dec) {
            delete video_dec;
            video_dec = NULL;
        }

        for (size_t i = 0; i < video_dec_ids.size(); ++i) {
            VideoDecoder *dec = VideoDecoder::create(video_dec_ids.at(i));
            if (!dec)
                continue;
            dec->setCodecCtx(ctx);
            if (dec->open()) {
                video_dec = dec;
                break;
            }
            delete dec;
        }
        if (!video_dec) {
            avdebug("Can not found video decoder.\n");
            return false;
        }
        if (!video_thread) {
            video_thread = new VideoThread();
            video_thread->setOutputSet(&video_output_set);
            video_thread->setClock(&clock);
            demux_thread->setVideoThread(video_thread);
        }
        video_thread->setDecoder(video_dec);
        demuxer->initVideoStatistics();
        return true;
    }

    void onSeekFinished(void*) { seeking = false; }

    bool loaded;
    bool paused;
    bool seeking;
    SeekType seek_type;
    std::string fileName;
    std::hash<std::string> format_dict;

    /*Demuxer*/
    AVDemuxer *demuxer;
    AVDemuxThread *demux_thread;

    /*AVThread*/
    AVThread *video_thread;
    AVThread *audio_thread;

    int video_track, audio_track, sub_track;

    /*Decoder*/
    VideoDecoder *video_dec;
    AudioDecoder *audio_dec;
    std::vector<VideoDecoderId> video_dec_ids;

    /*OutputSet*/
    OutputSet video_output_set;
    OutputSet audio_output_set;
    AudioOutput *ao;

    AVClock clock;

    Statistics statistics;
};

AVPlayer::AVPlayer():
    d_ptr(new AVPlayerPrivate)
{
}

AVPlayer::~AVPlayer()
{

}

void AVPlayer::setFile(const std::string &fileName)
{
    DPTR_D(AVPlayer);

    d->fileName = fileName;
}

void AVPlayer::play(const std::string& fileName)
{
    DPTR_D(AVPlayer);

    d->fileName = fileName;
    if (fileName.empty())
        return;
    if (!load()) {
        return;
    }
    if (isLoaded()) {
        playInternal();
    }
}

bool AVPlayer::load()
{
    DPTR_D(AVPlayer);
    d->loaded = false;
    loadInternal();
    return d->loaded;
}

bool AVPlayer::isLoaded() const
{
    DPTR_D(const AVPlayer);
    return d->loaded;
}

void AVPlayer::pause(bool p)
{
    DPTR_D(AVPlayer);
    if (d->paused == p)
        return;
    d->paused = p;
    d->demux_thread->pause(p);
    if (d->audio_thread)
        d->audio_thread->pause(p);
    if (d->video_thread)
        d->video_thread->pause(p);
}

bool AVPlayer::isPaused() const
{
    return d_func()->paused;
}

void AVPlayer::stop()
{
    DPTR_D(AVPlayer);
    d->demux_thread->stop();
    d->demuxer->unload();
    d->loaded = false;
}

bool AVPlayer::isPlaying()
{
    DPTR_D(AVPlayer);
    return (d->demux_thread && d->demux_thread->isRunning()) ||
            (d->audio_thread && d->audio_thread->isRunning()) ||
            (d->video_thread && d->video_thread->isRunning());
}

void AVPlayer::seek(uint64_t ms)
{
    DPTR_D(AVPlayer);
    if (!isPlaying())
        return;
    if (d->seeking)
        return;
    d->seeking = true;
    d->clock.updateValue((double)ms / 1000);
    d->demux_thread->seek(ms, d->seek_type);
}

void AVPlayer::seekForward()
{
    uint64_t pos = position() + 10 * 1000;
    if (pos > duration() * 1000)
        pos = duration() * 1000;
    seek(pos);
}

void AVPlayer::seekBackward()
{
    uint64_t pos = position() - 10 * 1000;
    if (pos < 0)
        pos = 0;
    seek(pos);
}

uint64_t AVPlayer::position()
{
    DPTR_D(AVPlayer);
    return uint64_t(d->clock.value() * 1000);
}

uint64_t AVPlayer::duration()
{
    DPTR_D(AVPlayer);
    return d->demuxer->duration();
}

void AVPlayer::addVideoRenderer(VideoRenderer *renderer)
{
    DPTR_D(AVPlayer);
    renderer->setStatistics(&d->statistics);
    d->video_output_set.addOutput((AVOutput*)renderer);
}

void AVPlayer::loadInternal()
{
    DPTR_D(AVPlayer);

    d->demuxer->setMedia(d->fileName);
    d->demuxer->load();
    d->loaded = d->demuxer->isLoaded();
}

void AVPlayer::playInternal()
{
    DPTR_D(AVPlayer);

    if (!d->demuxer->isLoaded())
        return;
    d->demuxer->initBaseStatistics();
    if (!d->setupAudioThread()) {
        d->demux_thread->setAudioThread(NULL);
        if (d->audio_thread) {
            delete d->audio_thread;
            d->audio_thread = NULL;
        }
    }
    if (!d->setupVideoThread()) {
        d->demux_thread->setVideoThread(NULL);
        if (d->video_thread) {
            delete d->video_thread;
            d->video_thread = NULL;
        }
    }

    d->demux_thread->start();
}

}
