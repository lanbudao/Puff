
#ifndef PUFF_AVPLAYER_H
#define PUFF_AVPLAYER_H

#include "AVGlobal.h"
#include "DPTR.h"

/**
 *
 */
namespace Puff {

class AudioFilter;
class VideoFilter;
class VideoRenderer;
class AVPlayerPrivate;
class PU_AV_EXPORT AVPlayer
{
    DPTR_DECLARE_PRIVATE(AVPlayer)
public:
    AVPlayer();
    ~AVPlayer();

    void setFile(const std::string &fileName);
    void play(const std::string& fileName);
    bool load();
    bool isLoaded() const;
    void pause(bool p = true);
    bool isPaused() const;
    void stop();

    bool isPlaying();
    void seek(uint64_t ms);
    void seekForward();
    void seekBackward();
    uint64_t position();
    uint64_t duration();

    /**
     * @brief setAudio, Video and Subtitle's Track
     * Should be called before video is playing
     * @param track
     */
    void setAudioTrack(int track);
    void setVideoTrack(int track);
    void setSubtitleTrack(int track);

    /**
     * @brief default is SyncToAudio if not set and audio stream is not null
     * Note: must call it before play() is called
     * @param type
     */
    void setClockType(ClockType type);

    void addVideoRenderer(VideoRenderer *renderer);

    void installFilter(AudioFilter *filter);
    void installFilter(VideoFilter *filter);

private:
    void loadInternal();
    void playInternal();

    DPTR_DECLARE(AVPlayer)
};

}
#endif //PUFF_AVPLAYER_H
