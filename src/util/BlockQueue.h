#ifndef PUFF_BLOCKQUEUE_H
#define PUFF_BLOCKQUEUE_H

#include <queue>
#include "CMutex.h"

namespace Puff {

template <typename T>
class BlockQueue
{
public:
    BlockQueue();
    virtual ~BlockQueue() {}

    /**
     * enqueue
     * @param timeout wait time out(ms)
     */
    void enqueue(const T &t, unsigned long timeout = ULONG_MAX);
    T dequeue(bool *isValid = NULL, unsigned long timeout = ULONG_MAX);

    void clear();
    void setCapacity(int cap);
    void setThreshold(int thr);
    void setBlock(bool block);
    bool isFull() const;
    bool isEmpty() const;
    bool isEnough() const;
    void blockEmpty(bool block);
    void blockFull(bool block);

protected:
    virtual bool checkFull() const;
    virtual bool checkEmpty() const;
    virtual bool checkEnough() const;

    virtual void onEnqueue(const T &t) {}
    virtual void onDequeue(const T &t) {}

protected:
    std::queue<T> q;

    /*Must be mutable*/
    mutable CMutex mutex, lock_change_mutex;
    CCondition empty_cond, full_cond;
    int capacity, threshold;
    bool block_full, block_empty;
};

template<typename T>
BlockQueue<T>::BlockQueue():
        block_full(true),
        block_empty(true),
        capacity(48),
        threshold(32)
{

}

template<typename T>
void BlockQueue<T>::clear()
{
    WriteLock lock(&mutex);
    (void)lock;
    full_cond.notifyAll();

    /*Clear the queue*/
    std::queue<T> null;
    std::swap(q, null);
    onDequeue(T());
}

template<typename T>
void BlockQueue<T>::enqueue(const T &t, unsigned long timeout)
{
    WriteLock lock(&mutex);
    (void)lock;
    if (q.size() >= (unsigned int)capacity) {
        if (block_full) {
            full_cond.timeWait(&mutex, timeout);
        }
    }
    q.push(t);
    onEnqueue(t);

    if (q.size() >= (unsigned int)threshold && !q.empty()) {
        empty_cond.notifyOne();
    }
}

template<typename T>
T BlockQueue<T>::dequeue(bool *isValid, unsigned long timeout)
{
    if (isValid)
        *isValid = false;

    WriteLock lock(&mutex);
    (void)lock;
    if (q.empty()) {
        if (block_empty)
            empty_cond.timeWait(&mutex, timeout);
    }
    if (q.empty())
        return T();

    T t = q.front();
    q.pop();
    full_cond.notifyOne();
    onDequeue(t);

    if (isValid)
        *isValid = true;
    return t;
}

template<typename T>
void BlockQueue<T>::setBlock(bool block)
{
    WriteLock lock(&mutex);
    (void)lock;
    block_full = block_empty = block;
    if (!block) {
        full_cond.notifyAll();
        empty_cond.notifyAll();
    }
}

template<typename T>
void BlockQueue<T>::setCapacity(int cap) {
    capacity = cap;
}

template<typename T>
void BlockQueue<T>::setThreshold(int thr) {
    threshold = thr;
}

template<typename T>
bool BlockQueue<T>::isFull() const {
    ReadLock lock(&mutex);
    (void)lock;
    return q.size() >= capacity;
}

template<typename T>
bool BlockQueue<T>::isEmpty() const {
    ReadLock lock(&mutex);
    (void)lock;
    return q.empty();
}

template<typename T>
bool BlockQueue<T>::isEnough() const {
    ReadLock lock(&mutex);
    (void)lock;
    return q.size() >= (unsigned int)threshold;
}

template<typename T>
bool BlockQueue<T>::checkFull() const {
    return q.size() >= (unsigned int)capacity;
}

template<typename T>
bool BlockQueue<T>::checkEmpty() const {
    return q.empty();
}

template<typename T>
bool BlockQueue<T>::checkEnough() const {
    return q.size() >= (unsigned int)threshold && !q.empty();
}

template<typename T>
void BlockQueue<T>::blockEmpty(bool block) {
    if (!block)
        empty_cond.notifyAll();
    WriteLock lock(lock_change_mutex);
    (void)lock;
    block_empty = block;
}

template<typename T>
void BlockQueue<T>::blockFull(bool block) {
    if (!block)
        full_cond.notifyAll();
    WriteLock lock(&lock_change_mutex);
    (void)lock;
    block_full = block;
}

}
#endif //PUFF_BLOCKQUEUE_H
