#ifndef PUFF_CMUTEX_H
#define PUFF_CMUTEX_H

#include "AVGlobal.h"
#include "DPTR.h"

namespace Puff {

class CMutexPrivate;
class PU_AV_EXPORT CMutex
{
    DPTR_DECLARE_PRIVATE(CMutex)
public:
    CMutex();
    ~CMutex();

    bool readLock();
    bool readUnlock();
    bool writeLock();
    bool writeUnlock();

private:
    DPTR_DECLARE(CMutex)
};

}
#endif //PUFF_CMUTEX_H
