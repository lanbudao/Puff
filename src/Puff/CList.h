#ifndef PUFF_CLIST_H
#define PUFF_CLIST_H

#include "AVGlobal.h"

namespace Puff {

template<typename T>
class CList: public std::list<T>
{
public:
    T& at(int i);
    T& operator [] (int i);
};

template<typename T>
T& CList<T>::at(int i)
{
    assert(i >= 0 && i < this->size());
    typename std::list<T>::iterator it;
    it = this->begin();
    std::advance(it, i);
    return *it;
}

template<typename T>
T &CList<T>::operator[](int i) {
    return at(i);
}

}
#endif //PUFF_CLIST_H
