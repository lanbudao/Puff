#ifndef MSAV_AVLOG_H
#define MSAV_AVLOG_H

#include "MSAVGlobal.h"
#include "DPTR.h"

namespace MSAV {

class AVLogPrivate;
class MS_AV_EXPORT AVLog
{
    DPTR_DECLARE_PRIVATE(AVLog);
public:
    AVLog *instance();

    void log(LogLevel level, const char *fmt, ...);

    void setLevel(LogLevel level);

    LogLevel level();

private:
    explicit AVLog();
    ~AVLog();
};

}
#endif //MSAV_AVLOG_H
