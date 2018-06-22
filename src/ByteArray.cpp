#include "ByteArray.h"
#include "commpeg.h"

namespace Puff {

#if USE_FIFO
class ByteArrayPrivate
{
public:
    ByteArrayPrivate(int s = 0):
        data(NULL)
    {
        size = s;
        if (size == 0)
            return;
        data = av_fifo_alloc(s);
        av_fifo_reset(data);
    }
    ~ByteArrayPrivate()
    {
        av_fifo_freep(&data);
    }
    AVFifoBuffer *data;
    size_t size;
};

ByteArray::ByteArray():
    d_ptr(new ByteArrayPrivate())
{

}

ByteArray::ByteArray(const char *data, size_t size):
    d_ptr(new ByteArrayPrivate(size))
{
    DPTR_D(ByteArray);
    if (!data && size == 0) {
        size = sizeof(data);
    }
    if (size > 0) {
        av_fifo_generic_write(d->data, (void*)data, size, NULL);
    }
}

ByteArray::~ByteArray()
{

}

ByteArray ByteArray::fromRawData(const char *p, size_t size)
{
    ByteArray data(p, size);
    return data;
}

char *ByteArray::data()
{
    DPTR_D(ByteArray);
    return (char *)d->data->buffer;
}

const char *ByteArray::constData() const
{
    DPTR_D(const ByteArray);
    if (!d->data)
        return NULL;
    return (const char *)d->data->buffer;
}

size_t ByteArray::size() const
{
    DPTR_D(const ByteArray);
    return d->size;
}

void ByteArray::resize(size_t size)
{
    DPTR_D(ByteArray);
    if (size == 0 || size == d->size) {
        return;
    }
    if (!d->data) {
        d->data = av_fifo_alloc(size);
        av_fifo_reset(d->data);
    } else {
        av_fifo_realloc2(d->data, size);
    }
    d->size = size;
}

bool ByteArray::isEmpty() const
{
    DPTR_D(const ByteArray);
    return !d->data || d->size == 0;
}

ByteArray &ByteArray::operator =(const ByteArray &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

void ByteArray::setData(const char *data, size_t size)
{
    DPTR_D(ByteArray);
    if (size > 0) {
        if (size != d->size) {
            if (!d->data) {
                d->data = av_fifo_alloc(size);
                av_fifo_reset(d->data);
            } else {
                av_fifo_realloc2(d->data, size);
            }
        }
        d->size = size;
        av_fifo_generic_write(d->data, (void*)data, size, NULL);
    }
}
#else

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

void ByteArray::resize(size_t size)
{
    if (mData)
        free(mData);
    mData = (char *)malloc(size);
    memset(mData, 0, size);
    mSize = size;
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
#endif
}
