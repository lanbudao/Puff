#include "renderer/SDLRenderer.h"
#include "private/AVOutput_p.h"
#include "SDL.h"
#include "AVLog.h"
#include "commpeg.h"

#define SDL_REFRESH_EVENT (SDL_USEREVENT + 1)

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
    }

    void resizeWindow(int w, int h)
    {
        if (window) {
            SDL_SetWindowSize(window, w, h);
        }
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = NULL;
        }
    }

    void renderYUV(const VideoFrame &frame)
    {
        int height = 224;
        if (!texture) {
            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
                                        renderer_width, height);
        }
        if (!frame_yuv) {
            frame_yuv = av_frame_alloc();
            int size = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, renderer_width, height, 1);
            unsigned char *buffer = (unsigned char *)av_malloc(size);
            av_image_fill_arrays(frame_yuv->data, frame_yuv->linesize, buffer, AV_PIX_FMT_YUV420P, renderer_width, height, 1);
        }
        if (!img_convert_ctx) {
            img_convert_ctx = sws_getContext(frame.width(), frame.height(), AVPixelFormat(frame.pixelFormatFFmpeg()),
                                             renderer_width, height, AV_PIX_FMT_YUV420P,
                                             SWS_BICUBIC, NULL, NULL, NULL);
        }
        const unsigned char *const* data = frame.datas();
        sws_scale(img_convert_ctx,
                  data, frame.lineSize(),
                  0, frame.height(),
                  frame_yuv->data, frame_yuv->linesize);
        rect.x = 0; rect.y = 0; rect.w = renderer_width; rect.h = height;
        SDL_Rect update_rect;
        update_rect.x = 0; update_rect.y = (renderer_height - height) / 2;
        update_rect.w = renderer_width;
        update_rect.h = height;
        SDL_UpdateYUVTexture(texture, &rect,
                             frame_yuv->data[0], frame_yuv->linesize[0],
                             frame_yuv->data[1], frame_yuv->linesize[1],
                             frame_yuv->data[2], frame_yuv->linesize[2]);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &update_rect);
        SDL_RenderPresent(renderer);
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect rect;
    SwsContext *img_convert_ctx;
    AVFrame *frame_yuv;
    int winId;
    VideoFrame current_frame;
};

SDLRenderer::SDLRenderer():
    VideoRenderer(new SDLRendererPrivate)
{
}

void SDLRenderer::init(int w, int h) {
    d_func()->init();
    resizeRenderer(w, h);
    setBackgroundColor(0, 0, 0);
}

void SDLRenderer::init(int winId) {
    d_func()->init(winId);
    setBackgroundColor(0, 0, 0);
}

SDLRenderer::~SDLRenderer()
{

}

void SDLRenderer::onResizeRenderer(int width, int height)
{
    d_func()->resizeWindow(width, height);
}

void SDLRenderer::show() {
    DPTR_D(SDLRenderer);
    if (d->winId)
        return;
    SDL_Event event;
    while (1) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_REFRESH_EVENT) {
            d->renderYUV(d->current_frame);
        }
        else if (event.type == SDL_QUIT) {
            break;
        }
        SDL_Delay(10);
    }
}

void SDLRenderer::setBackgroundColor(int r, int g, int b)
{
    DPTR_D(SDLRenderer);
    SDL_SetRenderDrawColor(d->renderer, r, g, b, 255);
}

bool SDLRenderer::receiveFrame(const VideoFrame &frame)
{
    DPTR_D(SDLRenderer);
    d->current_frame = frame;
    SDL_Event event;
    event.type = SDL_REFRESH_EVENT;
    SDL_PushEvent(&event);
    return true;
}

}
