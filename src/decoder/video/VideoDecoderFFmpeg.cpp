#include "VideoDecoderFFmpegBase.h"
#include "Factory.h"
#include "AVDecoder_p.h"

namespace Puff {

class VideoDecoderFFmpegPrivate;
class VideoDecoderFFmpeg: public VideoDecoderFFmpegBase
{
public:
    enum StrictType {
        Very = FF_COMPLIANCE_VERY_STRICT,
        Strict = FF_COMPLIANCE_STRICT,
        Normal = FF_COMPLIANCE_NORMAL, //default
        Unofficial = FF_COMPLIANCE_UNOFFICIAL,
        Experimental = FF_COMPLIANCE_EXPERIMENTAL
    };

    enum DiscardType {
        None = AVDISCARD_NONE,
        Default = AVDISCARD_DEFAULT, //default
        NoRef = AVDISCARD_NONREF,
        Bidir = AVDISCARD_BIDIR,
        NoKey = AVDISCARD_NONKEY,
        All = AVDISCARD_ALL
    };

    enum ThreadFlag {
        DefaultType = FF_THREAD_SLICE | FF_THREAD_FRAME,//default
        Slice = FF_THREAD_SLICE,
        Frame = FF_THREAD_FRAME
    };
    // flags. visualize motion vectors (MVs)
    enum MotionVectorVisFlag {
        No = 0, //default
        PF = FF_DEBUG_VIS_MV_P_FOR,
        BF = FF_DEBUG_VIS_MV_B_FOR,
        BB = FF_DEBUG_VIS_MV_B_BACK
    };
    enum BugFlag {
        autodetect = FF_BUG_AUTODETECT, //default
        xvid_ilace = FF_BUG_XVID_ILACE,
        ump4 = FF_BUG_UMP4,
        no_padding = FF_BUG_NO_PADDING,
        amv = FF_BUG_AMV,
        qpel_chroma = FF_BUG_QPEL_CHROMA,
        std_qpel = FF_BUG_QPEL_CHROMA2,
        direct_blocksize = FF_BUG_DIRECT_BLOCKSIZE,
        edge = FF_BUG_EDGE,
        hpel_chroma = FF_BUG_HPEL_CHROMA,
        dc_clip = FF_BUG_DC_CLIP,
        ms = FF_BUG_MS,
        trunc = FF_BUG_TRUNCATED
    };

    DPTR_DECLARE_PRIVATE(VideoDecoderFFmpeg)
    VideoDecoderFFmpeg()
        :VideoDecoderFFmpegBase()
    {

    }
    ~VideoDecoderFFmpeg()
    {

    }

    static void registerHWA()
    {

    }

    VideoDecoderId id() const PU_DECL_OVERRIDE;
    String description() const PU_DECL_OVERRIDE;
};

extern VideoDecoderId VideoDecoderId_FFmpeg;

FACTORY_REGISTER(VideoDecoder, FFmpeg, "FFmpeg")

void RegisterFFmpegHWA_Man()
{
    VideoDecoderFFmpeg::registerHWA();
}
namespace {
    static const struct factory_register_FFmpegHWA {
        inline factory_register_FFmpegHWA() {
            VideoDecoderFFmpeg::registerHWA();
        }
    } sInit_FFmpegHWA;
}

class VideoDecoderFFmpegPrivate: public VideoDecoderFFmpegBasePrivate
{
public:
    VideoDecoderFFmpegPrivate()
            : skip_loop_filter(VideoDecoderFFmpeg::Default)
            , skip_idct(VideoDecoderFFmpeg::Default)
            , strict(VideoDecoderFFmpeg::Normal)
            , skip_frame(VideoDecoderFFmpeg::Default)
            , thread_type(VideoDecoderFFmpeg::DefaultType)
            , threads(0)
            , debug_mv(VideoDecoderFFmpeg::No)
            , bug(VideoDecoderFFmpeg::autodetect)
    {

    }
    ~VideoDecoderFFmpegPrivate()
    {

    }
    bool open() PU_DECL_OVERRIDE {
        av_opt_set_int(codec_ctx, "skip_loop_filter", (int64_t) skip_loop_filter, 0);
        av_opt_set_int(codec_ctx, "skip_idct", (int64_t) skip_idct, 0);
        av_opt_set_int(codec_ctx, "strict", (int64_t) strict, 0);
        av_opt_set_int(codec_ctx, "skip_frame", (int64_t) skip_frame, 0);
        av_opt_set_int(codec_ctx, "threads", (int64_t) threads, 0);
        av_opt_set_int(codec_ctx, "thread_type", (int64_t) thread_type, 0);
        av_opt_set_int(codec_ctx, "vismv", (int64_t) debug_mv, 0);
        av_opt_set_int(codec_ctx, "bug", (int64_t) bug, 0);
        return true;
    }

    int skip_loop_filter;
    int skip_idct;
    int strict;
    int skip_frame;
    int thread_type;
    int threads;
    int debug_mv;
    int bug;
    String hwa;
};

VideoDecoderId VideoDecoderFFmpeg::id() const
{
    return VideoDecoderId_FFmpeg;
}

String VideoDecoderFFmpeg::description() const
{
    return String("FFmpeg Codec");
}

}
