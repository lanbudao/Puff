
#ifndef PUFF_AVPLAYER_H
#define PUFF_AVPLAYER_H

#include "AVGlobal.h"
#include "DPTR.h"

/**
 *
 */
namespace Puff {

class AVPlayerPrivate;
class PU_AV_EXPORT AVPlayer
{
    DPTR_DECLARE_PRIVATE(AVPlayer);
public:
    AVPlayer();
    ~AVPlayer();

    void setFile(const String &fileName);
    void play(const String& fileName);
    bool load();
    bool isLoaded() const;

private:
    void loadInternal();
    void playInternal();

    DPTR_DECLARE(AVPlayer)
};

};
#endif //PUFF_AVPLAYER_H
