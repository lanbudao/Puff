#ifndef MSAV_AVOBJECT_H
#define MSAV_AVOBJECT_H

#include <algorithm>
#include <iostream>
#include <vector>

#include "MSAVGlobal.h"

namespace MSAV {

template<class T1>
class MS_AV_EXPORT SlotBase {
public:
    virtual void Exec(T1 param1) = 0;

    virtual ~SlotBase() {}
};

template<class T, class T1>
class MS_AV_EXPORT SlotImpl : public SlotBase<T1>
{
public:
    SlotImpl(T *pObj, void (T::*func)(T1)) {
        obj = pObj;
        func = func;
    }

    void Exec(T1 param1) {
        (obj->*func)(param1);
    }

private:
    T *obj;

    void (T::*func)(T1);
};


template<class T1>
class MS_AV_EXPORT Signal {
public:
    template<class T>
    void Bind(T *obj, void (T::*func)(T1)) {
        slotSet.push_back(new SlotImpl<T, T1>(obj, func));
    }

    ~Signal() {
        for (int i = 0; i < (int) slotSet.size(); i++) {
            delete slotSet[i];
        }
    }

    void operator()(T1 param1) {
        for (int i = 0; i < (int) slotSet.size(); i++) {
            slotSet[i]->Exec(param1);
        }
    }

private:
    vector<SlotBase<T1> *> slotSet;
};

#define Connect(sender, signal, receiver, method) ( (sender)->signal.Bind(receiver, method) )
}
#endif //MSAV_AVOBJECT_H
