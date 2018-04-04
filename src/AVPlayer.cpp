
#include "AVPlayer.h"

#ifdef BUILD_MSAV_LIB
#define XXXX 1
#else
#define XXXX 0
#endif

namespace MSAV {

class AVPlayerPrivate: public DptrPrivate<AVPlayer>
{
public:
    AVPlayerPrivate()
    {

    }
    ~AVPlayerPrivate()
    {

    }
    String fileName;
    hash<String> format_dict;
};

AVPlayer::AVPlayer()
{
    printf("AVPlayer Initialize...");
    printf("BUILD_MSAV_LIB: %d", XXXX);
}

AVPlayer::~AVPlayer()
{

}

void AVPlayer::setFile(const String &fileName)
{

}

}