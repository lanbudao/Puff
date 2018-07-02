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
#include "SDL.h"

using namespace Puff;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        avdebug("Use Puff.exe fileName\n");
        getchar();
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

    /*Set window icon*/
    SDL_Surface *icon = SDL_LoadBMP("app.bmp");
    if (icon) {
        SDL_SetColorKey(icon, SDL_TRUE, SDL_MapRGB(icon->format, 255, 0, 255));
        SDL_SetWindowIcon(renderer->window(), icon);
        SDL_FreeSurface(icon);
    }

    SDL_Event event;
    int winID = SDL_GetWindowID(renderer->window());
    SDL_SetWindowTitle(renderer->window(), "Puff (A Simple Player)");
    while (1) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_WINDOWEVENT) {
            if (event.window.windowID == winID) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    int width, height;
                    SDL_GetWindowSize(renderer->window(), &width, &height);
                    renderer->resizeWindow(width, height);
                }
            }
        }
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE) {
                player->pause(!player->isPaused());
            } else if (event.key.keysym.sym == SDLK_RIGHT) {
                player->seekForward();
            } else if (event.key.keysym.sym == SDLK_LEFT) {
                player->seekBackward();
            }
        }
        else if (event.type == SDL_QUIT) {
            player->stop();
            SDL_Delay(10);
            break;
        }
        SDL_Delay(10);
    }
    avdebug("player uninitialize\n");
    return 0;
}
