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

class A: public CThread
{
public:
    void run()
    {
        VideoFormat format(VideoFormat::Format_BGRA32);

        int i = 10;
        while (i-- > 0) {
//            VideoFormat f1, f5;

//            f1 = format;
//            f5 = format;

//            VideoFormat f2 = f1;
//            VideoFormat f3 = f2;
//            f3.isRGB();
            VideoFrame frame(100, 100, VideoFormat());

            msleep(1000);
        }
    }
};
class B: public CThread
{
public:
    void run()
    {
        VideoFormat format(VideoFormat::Format_BGRA32);

        int i = 10;
        while (i-- > 0) {
            VideoFormat f1, f5;

            f1 = format;
            f5 = format;

            VideoFormat f2 = f1;
            VideoFormat f3 = f2;
            f3.isRGB();
            msleep(1000);
        }
    }
};

int main(int argc, char *argv[])
{
    logger.setLogFile("F:/log.txt");

    AVPlayer *player = new AVPlayer();
    SDLRenderer *renderer = new SDLRenderer();
    renderer->init(400, 300);

    player->addVideoRenderer(renderer);
//    player->play("F:/work_file/video/M.I.A.OfficialVideo.mp4");
    player->play("F:/work_file/video/3D First Person Mario.mp4");

    renderer->show();
    avdebug("player uninitialize\n");
    return 0;
}
