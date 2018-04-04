
#ifndef MSAV_AVPLAYER_H
#define MSAV_AVPLAYER_H

#include "MSAVGlobal.h"
#include "DPTR.h"

/**
 *
 */
namespace MSAV {

class AVPlayerPrivate;
class MS_AV_EXPORT AVPlayer
{
    DPTR_DECLARE_PRIVATE(AVPlayer);
public:
    AVPlayer();
    ~AVPlayer();

    void setFile(const String &fileName);
    void play(const String fileName);

private:

};

};
#endif //MSAV_AVPLAYER_H
