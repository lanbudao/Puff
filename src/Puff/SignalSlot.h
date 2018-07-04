#ifndef PUFF_SIGNALSLOT_H
#define PUFF_SIGNALSLOT_H

#include "AVGlobal.h"
#include <algorithm>
#include <iostream>
#include <vector>

namespace Puff {

template<typename ... Args>
class PU_AV_EXPORT SlotBase
{
public:
    virtual void Exec(Args... args) = 0;
    virtual ~SlotBase() {}
};

template<class T, typename ... Args>
class PU_AV_EXPORT SlotImpl : public SlotBase<Args...>
{
public:
    SlotImpl(T* pObj, void (T::*func)(Args...)) {
        obj = pObj;
        mfunc = func;
    }
    void Exec(Args... args) {
        (obj->*mfunc)(args...);
    }

private:
    T* obj;
    void (T::*mfunc)(Args...);
};

template<typename ... Args>
class PU_AV_EXPORT Signal
{
public:
    template<class T>
    void Bind(T* pObj, void (T::*func)(Args...)) {
        slotSet.push_back( new SlotImpl<T,Args...>(pObj, func) );
    }
    template<class T>
    void Unbind(T *obj) {
        std::list<SlotBase<Args...>*>::iterator itor;
        for (itor = slotSet.begin(); itor != slotSet.end(); itor++) {
            if ((*itor) == obj) {
                delete obj;
                slotSet.remove(obj);
                return;
            }
        }
    }
    ~Signal() {
        std::list<SlotBase<Args...>*>::iterator itor;
        for (itor = slotSet.begin(); itor != slotSet.end(); itor++) {
            delete (*itor);
        }
        slotSet.clear();
    }
    void operator()(Args ...args) {
        std::list<SlotBase<Args...>*>::iterator itor;
        for (itor = slotSet.begin(); itor != slotSet.end(); itor++) {
            (*itor)->Exec(args...);
        }
    }
private:
    std::list<SlotBase<Args...>*> slotSet;
};

#define PU_LINK(sender, signal, receiver, method) ((sender)->signal.Bind(receiver, method))
#define PU_UNLINK(sender, signal, receiver) ((sender)->signal.Unbind(receiver))

}
#endif //PUFF_SIGNALSLOT_H
