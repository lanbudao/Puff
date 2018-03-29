
#include "AVPlayer.h"

#ifdef BUILD_MSAV_LIB
#define XXXX 1
#else
#define XXXX 0
#endif

namespace MSAV {

AVPlayer::AVPlayer() {
    printf("AVPlayer Initialize...");
    printf("BUILD_MSAV_LIB: %d", XXXX);
}

AVPlayer::~AVPlayer() {

}
}