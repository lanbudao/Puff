
#ifndef PUFF_AVPLAYER_H
#define PUFF_AVPLAYER_H

#include "AVGlobal.h"
#include "DPTR.h"

/**
 *
 */
namespace Puff {
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

    void addVideoRenderer(VideoRenderer *renderer);

private:
    void loadInternal();
    void playInternal();

    DPTR_DECLARE(AVPlayer)
};

}
#endif //PUFF_AVPLAYER_H
