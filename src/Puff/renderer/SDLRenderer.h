#ifndef PUFF_SDLRENDERER_H
#define PUFF_SDLRENDERER_H

#include "AVGlobal.h"
#include "VideoRenderer.h"

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
    void show();

protected:
    bool receiveFrame(const VideoFrame& frame);
    void onResizeRenderer(int width, int height) PU_DECL_OVERRIDE;
};

}
#endif //PUFF_SDLRENDERER_H
