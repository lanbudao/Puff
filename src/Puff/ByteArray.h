#ifndef PUFF_BYTEARRAY_H
#define PUFF_BYTEARRAY_H

#include "AVGlobal.h"
#include <string.h>
#include "DPTR.h"

namespace Puff {

class ByteArrayPrivate;
class PU_AV_EXPORT ByteArray
{
    DPTR_DECLARE_PRIVATE(ByteArray)
public:
    ByteArray();
    ByteArray(const char* data, size_t size);
    ByteArray& operator = (const ByteArray &other);
    ~ByteArray();

    static ByteArray fromRawData(const char *, size_t size);
    char *data();
    const char *constData() const;
    size_t size() const;

    void resize(size_t size);

    bool isEmpty() const;

    void setData(const char* data, size_t size);

private:
    DPTR_DECLARE(ByteArray)
};
}

#endif //PUFF_BYTEARRAY_H
