
#include "AVLog.h"

namespace Puff {

class AVLogPrivate: public DptrPrivate<AVLog> {
public:
    AVLogPrivate() :
        level(LogDebug)
    {

    }

    ~AVLogPrivate() {

    }

    LogLevel level;
};

AVLog::AVLog()
{

}

AVLog::~AVLog() {

}

void AVLog::log(LogLevel level, const char *fmt, ...)
{
    DPTR_D(AVLog);

//    if (level < d.level)
//        printf(fmt, __VA_ARGS__);
}

void AVLog::setLevel(LogLevel level)
{
    DPTR_D(AVLog);

    d.level = level;
}

LogLevel AVLog::level() const
{
    DPTR_D(const AVLog);

    return d.level;
}

}
