#include <stdio.h>
#include <iostream>
#include "AVPlayer.h"
#include "AVObject.h"
#include "signals2/signal_type.hpp"

using namespace MSAV;

//int main(int argc, char *argv[])
//{
//    AVPlayer *player = new AVPlayer();
//
//    printf("hello world");
//
//    return 0;
//}

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

class A
{
public:
    void FuncOfA(int param)
    {
        printf("A::FuncOfA(%d)\n", param);
    }
};
class B
{
public:
    void FuncOfB(int param)
    {
        printf("B::FuncOfB(%d)\n", param);
    }
};
class C
{
public:
    C()
    {
        m_Value = 0;
    }
    void SetValue(int value)
    {
        if(m_Value != value)
        {
            m_Value = value;
            ValueChanged(m_Value);
        }
    }
public:
    Signal<int> ValueChanged;
private:
    int m_Value;
};

int main(int argc, char *argv[])
{
//    printf("What is that!");
//    getchar();

    int a = 0;
    int b = a;
    printf("%d, %d\n", a, b);
//    return 0;
    A* pA = new A;
    B* pB = new B;

    C* pC = new C;
    Connect(pC, ValueChanged, pA, &A::FuncOfA);
    Connect(pC, ValueChanged, pB, &B::FuncOfB);
    pC->SetValue(10);
//    pC->SetValue(5);
//    pC->SetValue(5);
//    delete pC;
//    delete pB;
//    delete pA;
//    scanf("%*s");

    return 0;
}