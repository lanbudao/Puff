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
    ~Signal() {
        for(int i = 0; i < (int)slotSet.size();i++){
            delete slotSet[i];
        }
    }
    void operator()(Args ...args) {
        for(int i = 0; i < (int)slotSet.size();i++){
            slotSet[i]->Exec(args...);
        }
    }
private:
    std::vector<SlotBase<Args...>*> slotSet;
};

#define puconnect(sender, signal, receiver, method) ((sender)->signal.Bind(receiver, method))

}
#endif //PUFF_SIGNALSLOT_H
