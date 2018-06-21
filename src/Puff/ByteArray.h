#ifndef PUFF_BYTEARRAY_H
#define PUFF_BYTEARRAY_H

#include "AVGlobal.h"
#include <string.h>

namespace Puff {

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
}

#endif //PUFF_BYTEARRAY_H
