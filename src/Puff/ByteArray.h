#ifndef PUFF_BYTEARRAY_H
#define PUFF_BYTEARRAY_H

#include "AVGlobal.h"
#include <string.h>
#include "DPTR.h"

#define USE_FIFO 1

namespace Puff {

#if USE_FIFO
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
#else

class PU_AV_EXPORT ByteArray
{
public:
    ByteArray();
    ByteArray(const char* data, size_t size);
    ByteArray& operator = (const ByteArray &other);
    ~ByteArray();

    static ByteArray fromRawData(const char *, size_t size);
    inline char *data()  {return mData;}
    inline const char *constData() const {return mData;}
    inline size_t size() const {return mSize;}

    void resize(size_t size);

    inline bool isEmpty() const {return mSize <= 0;}

    void setData(const char* data, size_t size);

private:
    char* mData;
    size_t mSize;
};
#endif
}

#endif //PUFF_BYTEARRAY_H
