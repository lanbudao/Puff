#include "renderer/SDLRenderer.h"
#include "private/AVOutput_p.h"
#include "SDL.h"
#include "AVLog.h"

namespace Puff {

class SDLRendererPrivate: public VideoRendererPrivate
{
public:
    SDLRendererPrivate():
        window(NULL),
        renderer(NULL),
        texture(NULL),
        winId(0)
    {

    }

    ~SDLRendererPrivate()
    {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
        SDL_Quit();
    }

    void init(int id = 0)
    {
        winId = id;
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
            avwarnning("Can not initialize SDL.");
            return;
        }
        if (!winId) {
            window = SDL_CreateWindow("SDL Renderer",
                                      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                      renderer_width, renderer_height,
                                      SDL_WINDOW_OPENGL);
        } else {
            window = SDL_CreateWindowFrom((void*)winId);
            if (window) {
                SDL_GetWindowSize(window, &renderer_width, &renderer_height);
            }
        }
        if (!window) {
            avwarnning("Can't create SDL window, error: %s.", SDL_GetError());
            return;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
                                    renderer_width, renderer_height);
    }

    void resizeWindow(int w, int h)
    {
        if (window) {
            SDL_SetWindowSize(window, w, h);
        }
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect rect;
    int winId;
};

SDLRenderer::SDLRenderer():
    VideoRenderer(new SDLRendererPrivate)
{
}

void SDLRenderer::init(int w, int h) {
    d_func()->init();
    resizeRenderer(w, h);
}

void SDLRenderer::init(int winId) {
    d_func()->init(winId);
}

SDLRenderer::~SDLRenderer()
{

}

void SDLRenderer::onResizeRenderer(int width, int height)
{
    d_func()->resizeWindow(width, height);
}

void SDLRenderer::show() {
    if (d_func()->winId)
        return;
    SDL_Event event;
    while (1) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
    }
}

}
