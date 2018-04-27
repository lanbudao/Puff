#ifndef PUFF_BYTEARRAY_H
#define PUFF_BYTEARRAY_H

#include "AVGlobal.h"

namespace Puff {

class PU_AV_EXPORT ByteArray
{
public:
    ByteArray(): mData(NULL), mSize(0)
    {

    }
    ByteArray(const char* data, size_t size):
        mData(NULL),
        mSize(size)
    {
        if (size > 0) {
            mData = (char *) malloc(size);
            memcpy(mData, data, size);
        }
    }
    ~ByteArray()
    {
        if (mData)
            free(mData);
    }

    const char *data() const {return mData;}
    size_t size() const {return mSize;}

    ByteArray& operator = (const ByteArray &other)
    {
        if (other.size() == 0) {
            return *this;
        }
        if (mSize != other.size()) {
            if (mData)
                free(mData);
            mData = (char *)malloc(other.size());
        }
        memcpy(mData, other.data(), other.size());
        return *this;
    }

private:
    char* mData;
    size_t mSize;
};
}

#endif //PUFF_BYTEARRAY_H
