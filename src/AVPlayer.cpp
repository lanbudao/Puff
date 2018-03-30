
#include "AVPlayer.h"

#ifdef BUILD_MSAV_LIB
#define XXXX 1
#else
#define XXXX 0
#endif

namespace MSAV {

class AVPlayer::Private
{
public:
    explicit Private()
    {

    }
    ~Private()
    {

    }
    String fileName;
    hash<String> format_dict;
};

AVPlayer::AVPlayer():
    d(new Private())
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