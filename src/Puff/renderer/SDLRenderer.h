#ifndef PUFF_SDLRENDERER_H
#define PUFF_SDLRENDERER_H

#include "AVGlobal.h"
#include "VideoRenderer.h"

class SDL_Window;

namespace Puff {

class SDLRendererPrivate;
class PU_AV_EXPORT SDLRenderer: public VideoRenderer
{
    DPTR_DECLARE_PRIVATE(SDLRenderer)
public:
    SDLRenderer();
    ~SDLRenderer();

    void init(int w, int h);
    void init(int winId);
    void setBackgroundColor(int r, int g, int b);
    SDL_Window *window();

protected:
    bool receiveFrame(const VideoFrame& frame);
    void onResizeWindow(int width, int height) PU_DECL_OVERRIDE;
};

}
#endif //PUFF_SDLRENDERER_H
