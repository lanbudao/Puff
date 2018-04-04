
#include "AVLog.h"

namespace MSAV {

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

AVLog *AVLog::instance() {
    static AVLog log;
    return &log;
}

void AVLog::log(LogLevel level, const char *fmt, ...)
{

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