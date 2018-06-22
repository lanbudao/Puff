#include <stdio.h>
#include <iostream>
#include "AVPlayer.h"
#include "CObject.h"
#include "AVLog.h"
#include "renderer/SDLRenderer.h"
#include "VideoFormat.h"
#include "AudioFormat.h"
#include "VideoFrame.h"
#include "AudioFrame.h"
#include "CThread.h"

using namespace Puff;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        avdebug("Use Puff.exe fileName\n");
        return 0;
    }
    logger.setLogFile("F:/log.txt");
    std::string fileName = argv[1];
    avdebug("The name of file to play: %s\n", fileName.c_str());

    AVPlayer *player = new AVPlayer();
    SDLRenderer *renderer = new SDLRenderer();
    renderer->init(800, 600);

    player->addVideoRenderer(renderer);
    player->play(fileName);

    renderer->show();
    avdebug("player uninitialize\n");
    return 0;
}
