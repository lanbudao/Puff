
#ifndef MSAV_AVPLAYER_H
#define MSAV_AVPLAYER_H

#include "MSAVGlobal.h"

/**
 *
 */
namespace MSAV {

class MS_AV_EXPORT AVPlayer
{
public:
    AVPlayer();
    ~AVPlayer();

    void setFile(const String &fileName);
    void play(const String fileName);

private:
    MS_DECL_PRIVATE
};

};
#endif //MSAV_AVPLAYER_H
