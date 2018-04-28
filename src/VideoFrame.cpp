#include "VideoFrame.h"
#include "Frame_p.h"

namespace Puff {

class VideoFramePrivate : public FramePrivate {
public:
    VideoFramePrivate() {

    }
};

VideoFrame::VideoFrame():
    Frame()
{

}

VideoFrame::~VideoFrame()
{

}

Size VideoFrame::size() {
    return Size();
}

}
