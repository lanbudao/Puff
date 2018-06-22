#ifndef PUFF_STATISTICS_H
#define PUFF_STATISTICS_H

#include "AVGlobal.h"
#include "DPTR.h"

namespace Puff {

class PU_AV_EXPORT Statistics
{
public:
    Statistics();
    ~Statistics();

    void reset();

    std::string url;
    int bit_rate;
    uint64_t start_time, duration; /*second*/
    class Common {
    public:
        Common();
        bool avaliable;
        std::string codec, codec_long;
        uint64_t current_time, start_time, duration;  /*ms*/
        int bit_rate;
        uint64_t frames;
        double frame_rate;
    } audio, video;

    class AudioOnly {
    public:
        AudioOnly();
        int sample_rate;
        int channels;
        int frame_size;
        std::string sample_fmt;
        std::string channel_layout;
    } audio_only;

    class VideoOnly {
    public:
        VideoOnly();
        int width, height;
        int coded_width, coded_height;
        int rotate;
        double pts;
        float aspect_ratio;
    } video_only;

private:

};

}
#endif //PUFF_STATISTICS_H
