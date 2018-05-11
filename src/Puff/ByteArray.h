#ifndef PUFF_BYTEARRAY_H
#define PUFF_BYTEARRAY_H

#include "AVGlobal.h"
#include <string.h>
#include <boost/array.hpp>

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

    char *data()  {return mData;}
    const char *constData() const {return mData;}
    size_t size() const {return mSize;}

    ByteArray& operator = (const ByteArray &other)
    {
        if (other.size() == 0) {
            mSize = 0;
            return *this;
        }
        if (mSize != other.size()) {
            mSize = other.size();
            if (mData)
                free(mData);
            mData = (char *)malloc(other.size());
        }
        memcpy(mData, other.constData(), other.size());
        return *this;
    }
    inline bool isEmpty() const {return mSize <= 0;}

    void setData(const char* data, size_t size)
    {
        if (size > 0) {
            if (mData)
                free(mData);
            mData = (char *) malloc(size);
            memcpy(mData, data, size);
            mSize = size;
        }
    }

private:
    char* mData;
    size_t mSize;
};
}

#endif //PUFF_BYTEARRAY_H
