#include "renderer/SDLRenderer.h"
#include "private/AVOutput_p.h"
#include "SDL.h"
#include "AVLog.h"
#include "commpeg.h"

namespace Puff {

class SDLRendererPrivate: public VideoRendererPrivate
{
public:
    SDLRendererPrivate():
        window(NULL),
        renderer(NULL),
        texture(NULL),
        winId(0),
        img_convert_ctx(NULL),
        frame_yuv(NULL)
    {

    }

    ~SDLRendererPrivate()
    {
        if (frame_yuv) {
            av_frame_free(&frame_yuv);
            frame_yuv = NULL;
        }
        if (img_convert_ctx) {
            sws_freeContext(img_convert_ctx);
            img_convert_ctx = NULL;
        }
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
    SwsContext *img_convert_ctx;
    AVFrame *frame_yuv;
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

bool SDLRenderer::receiveFrame(const VideoFrame &frame)
{
    DPTR_D(SDLRenderer);
    if (!d->frame_yuv) {
        d->frame_yuv = av_frame_alloc();
        int size = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, frame.width(), frame.height(), 1);
        unsigned char *buffer = (unsigned char *)av_malloc(size);
        av_image_fill_arrays(d->frame_yuv->data, d->frame_yuv->linesize, buffer, AV_PIX_FMT_YUV420P, frame.width(), frame.height(), 1);
    }
    if (!d->img_convert_ctx) {
        d->img_convert_ctx = sws_getContext(frame.width(), frame.height(), AVPixelFormat(frame.pixelFormatFFmpeg()),
                                         d->renderer_width, d->renderer_height, AV_PIX_FMT_YUV420P,
                                         SWS_BICUBIC, NULL, NULL, NULL);
    }
    d->rect.x = 0;
    d->rect.y = 0;
    d->rect.w = d->renderer_width;
    d->rect.h = d->renderer_height;
    sws_scale(d->img_convert_ctx,
              (const unsigned char *const*)frame.data().constData(), frame.lineSize(),
              0, frame.height(),
              d->frame_yuv->data, d->frame_yuv->linesize);
    SDL_UpdateYUVTexture(d->texture, &d->rect,
                         d->frame_yuv->data[0], d->frame_yuv->linesize[0],
                         d->frame_yuv->data[1], d->frame_yuv->linesize[1],
                         d->frame_yuv->data[2], d->frame_yuv->linesize[2]);
    SDL_RenderClear(d->renderer);
    SDL_RenderCopy(d->renderer, d->texture, NULL, &d->rect);
    SDL_RenderPresent(d->renderer);
    return true;
}

}
