#include "AVPlayer_p.h"

namespace Puff {

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
    if (!d->demuxer->isSeekable())
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

void AVPlayer::setAudioTrack(int track)
{
    DPTR_D(AVPlayer);
    d->audio_track = track;
}

void AVPlayer::setVideoTrack(int track)
{
    DPTR_D(AVPlayer);
    d->video_track = track;
}

void AVPlayer::setSubtitleTrack(int track)
{
    DPTR_D(AVPlayer);
    d->sub_track = track;
}

void AVPlayer::setClockType(ClockType type)
{
    DPTR_D(AVPlayer);
    d->clock_type = type;
}

void AVPlayer::addVideoRenderer(VideoRenderer *renderer)
{
    DPTR_D(AVPlayer);
    renderer->setStatistics(&d->statistics);
    d->video_output_set.addOutput((AVOutput*)renderer);
}

void AVPlayer::installFilter(AudioFilter *filter)
{

}

void AVPlayer::installFilter(VideoFilter *filter)
{

}

std::map<string, string> AVPlayer::internalSubtitles() const
{
    return std::map<string, string>();
}

Subtitle *AVPlayer::internalSubtitle()
{
    DPTR_D(AVPlayer);
    return &d->internal_subtitle;
}

void AVPlayer::enableExternalSubtitle(Subtitle *sub)
{
    DPTR_D(AVPlayer);
    std::list<Subtitle*>::iterator itor = d->external_subtitles.begin();
    for (; itor != d->external_subtitles.end(); itor++) {
        (*itor)->setEnabled(sub == (*itor));
    }
}

Subtitle *AVPlayer::addExternalSubtitle(const string &fileName, bool enabled)
{
    DPTR_D(AVPlayer);
    Subtitle *sub = new Subtitle;
    sub->setEnabled(enabled);
    sub->setFile(fileName);
    d->external_subtitles.push_back(sub);
    return sub;
}

void AVPlayer::removeExternalSubtitle(Subtitle *sub)
{
    DPTR_D(AVPlayer);
    std::list<Subtitle*>::iterator itor = d->external_subtitles.begin();
    for (; itor != d->external_subtitles.end(); itor++) {
        if (sub == NULL || sub == (*itor)) {
            delete (*itor);
            d->external_subtitles.remove(*itor);
            break;
        }
    }
}

std::list<Subtitle *> AVPlayer::externalSubtitles()
{
    DPTR_D(AVPlayer);
    return d->external_subtitles;
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

    /*Set Clock Type*/
    if (d->clock_type == SyncToAudio) {
        if (d->audio_thread) {
            d->clock_type = SyncToAudio;
        } else {
            d->clock_type = SyncToExternalClock;
        }
    } else if (d->clock_type == SyncToVideo) {
        if (d->video_thread) {
            d->clock_type = SyncToVideo;
        } else {
            d->clock_type = SyncToAudio;
        }
    }
    d->clock.setClockType(d->clock_type);

    d->demux_thread->start();
}

}
