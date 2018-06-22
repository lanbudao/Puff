#include "Statistics.h"

namespace Puff {

Statistics::Statistics()
{
    url.clear();
    bit_rate = 0;
    start_time = duration = 0;
}

Statistics::~Statistics()
{

}

void Statistics::reset()
{
    url.clear();
    audio = Common();
    video = Common();
    audio_only = AudioOnly();
    video_only = VideoOnly();
}

Statistics::Common::Common()
{
    avaliable = false;
    codec = codec_long = "";
    current_time = start_time = duration = 0;
    bit_rate = 0;
    frames = 0;
    frame_rate = 0;
}

Statistics::AudioOnly::AudioOnly()
{
    sample_rate = 0;
    channels = 0;
    frame_size = 0;
    sample_fmt.clear();
    channel_layout.clear();
}

Statistics::VideoOnly::VideoOnly()
{
    width = height = 0;
    coded_width = coded_height = 0;
    rotate = 0;
    pts = 0.0;
    aspect_ratio = 0.0;
}

}
