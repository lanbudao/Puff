#include "ByteArray.h"

namespace Puff {

ByteArray::ByteArray(): mData(nullptr), mSize(0)
{

}

ByteArray::ByteArray(const char *data, size_t size):
    mData(nullptr),
    mSize(size)
{
    if (size > 0) {
        mData = (char *) malloc(size);
        memcpy(mData, data, size);
    }
}

ByteArray::~ByteArray()
{
    if (mData) {
        delete mData;
        mData = nullptr;
    }
    mSize = 0;
}

ByteArray ByteArray::fromRawData(const char *p, size_t size)
{
    ByteArray data(p, size);
    return data;
}

ByteArray &ByteArray::operator =(const ByteArray &other)
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

void ByteArray::setData(const char *data, size_t size)
{
    if (size > 0) {
        if (mData)
            free(mData);
        mData = (char *) malloc(size);
        memcpy(mData, data, size);
        mSize = size;
    }
}

}
