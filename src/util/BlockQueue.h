#ifndef PUFF_BLOCKQUEUE_H
#define PUFF_BLOCKQUEUE_H

//#include "boost/thread/locks.hpp"
//#include "boost/thread/shared_mutex.hpp"
//#include "boost/thread/condition.hpp"

#include "SDL_mutex.h"
#include <queue>

//typedef boost::shared_mutex Mutex;
//typedef boost::unique_lock<Mutex> WriteLock;
//typedef boost::shared_lock<Mutex> ReadLock;
//typedef boost::condition Condition;

typedef SDL_mutex Mutex;
typedef SDL_LockMutex<Mutex> WriteLock;
typedef boost::shared_lock<Mutex> ReadLock;
typedef boost::condition Condition;

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
    mutable Mutex mutex, lock_change_mutex;
    Condition empty_cond, full_cond;
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
    WriteLock lock(mutex);
    full_cond.notify_all();

    /*Clear the queue*/
    std::queue<T> null;
    std::swap(q, null);
    onDequeue(T());
}

template<typename T>
void BlockQueue<T>::enqueue(const T &t, unsigned long timeout)
{
    WriteLock lock(mutex);
    if (q.size() >= capacity) {
        if (block_full) {
            printf("is full\n");
            full_cond.timed_wait(mutex, boost::get_system_time() + boost::posix_time::milliseconds(timeout));
        }
    }
    q.push(t);
    onEnqueue(t);

    if (q.size() >= threshold && !q.empty()) {
        empty_cond.notify_one();
    }
}

template<typename T>
T BlockQueue<T>::dequeue(bool *isValid, unsigned long timeout)
{
    if (isValid)
        *isValid = false;

    WriteLock lock(mutex);
    if (q.empty()) {
        if (block_empty)
            empty_cond.timed_wait(mutex, boost::get_system_time() + boost::posix_time::milliseconds(timeout));
    }
    if (q.empty())
        return T();

    T t = q.front();
    q.pop();
    full_cond.notify_one();
    onDequeue(t);

    if (isValid)
        *isValid = true;
    return t;
}

template<typename T>
void BlockQueue<T>::setBlock(bool block)
{
    WriteLock lock(mutex);
    block_full = block_empty = block;
    if (!block) {
        full_cond.notify_all();
        empty_cond.notify_all();
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
    ReadLock lock(mutex);
    return q.size() >= capacity;
}

template<typename T>
bool BlockQueue<T>::isEmpty() const {
    ReadLock lock(mutex);
    return q.empty();
}

template<typename T>
bool BlockQueue<T>::isEnough() const {
    ReadLock lock(mutex);
    return q.size() >= threshold;
}

template<typename T>
bool BlockQueue<T>::checkFull() const {
    return q.size() >= capacity;
}

template<typename T>
bool BlockQueue<T>::checkEmpty() const {
    return q.empty();
}

template<typename T>
bool BlockQueue<T>::checkEnough() const {
    return q.size() >= threshold && !q.empty();
}

template<typename T>
void BlockQueue<T>::blockEmpty(bool block) {
    if (!block)
        empty_cond.notify_all();
    WriteLock lock(lock_change_mutex);
    block_empty = block;
}

template<typename T>
void BlockQueue<T>::blockFull(bool block) {
    if (!block)
        full_cond.notify_all();
    WriteLock lock(lock_change_mutex);
    block_full = block;
}

}
#endif //PUFF_BLOCKQUEUE_H
