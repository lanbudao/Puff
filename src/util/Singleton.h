#ifndef PUFF_SINGLETON_H
#define PUFF_SINGLETON_H

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "AVGlobal.h"

/**
 * Internal class
 */

template <typename T>
class Singleton
{
    DISABLE_COPY(Singleton)
public:
    typedef T objectType;
    static T& instance();

protected:
    Singleton() {}
    virtual ~Singleton() {}

private:
    static void destroy();
    static T* m_instance;
    static bool m_destroyed;
};

template<typename T>
T* Singleton<T>::m_instance = NULL;

template<typename T>
bool Singleton<T>::m_destroyed = false;

template<typename T>
T &Singleton<T>::instance()
{
    if (!m_instance) {
        if (m_destroyed) {
            m_destroyed = false;
            printf("Dead Reference Detected\n");
            exit(1);
        }
        m_instance = new T();
        printf("Singleton %p created...\n", m_instance);
        std::atexit(&destroy);
    }
    return *m_instance;
}

template<typename T>
void Singleton<T>::destroy()
{
    printf("DestroySingleton...\n");
    assert(!m_destroyed);
    delete m_instance;
    m_instance = NULL;
    m_destroyed = true;
}

#endif //PUFF_SINGLETON_H
