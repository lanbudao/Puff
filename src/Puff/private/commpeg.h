#ifndef PUFF_COMMPEG_H
#define PUFF_COMMPEG_H

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#ifdef __cplusplus
}
#endif

#define AV_RUN_CHECK(FUNC, RETURN, ...) do { \
    int ret = FUNC; \
    if (ret < 0) { \
        char str[AV_ERROR_MAX_STRING_SIZE]; \
        memset(str, 0, sizeof(str)); \
        av_strerror(ret, str, sizeof(str)); \
        av_log(NULL, AV_LOG_WARNING, "Error " #FUNC " @%d " __FILE__ ": (%#x) %s\n", __LINE__, ret, str); \
        RETURN __VA_ARGS__; \
     } } while(0)

#endif //PUFF_COMMPEG_H
