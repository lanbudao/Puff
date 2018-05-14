#include <stdio.h>
#include <iostream>
#include "AVPlayer.h"
#include "CObject.h"
#include "AVLog.h"
#include "renderer/SDLRenderer.h"

using namespace Puff;

int main(int argc, char *argv[])
{
    logger.setLogFile("F:/log.txt");

    AVPlayer *player = new AVPlayer();
    SDLRenderer *renderer = new SDLRenderer();
    renderer->init(400, 300);

    player->play("F:/work_file/video/M.I.A.OfficialVideo.mp4");

    renderer->show();
    avdebug("hello world\n");
    avdebug("hello world\n");
    avdebug("hello world\n");
}
