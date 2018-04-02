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

using namespace boost::signals2;

class AVObject
{
public:
    AVObject()
    {
        typedef signal_type<void()> SignalV;
    }
    virtual ~AVObject()
    {

    }
//    typedef signal_type<void()> Signal0;
//    template <typename T1>
//    typedef signal_type<void(T1)> Signal1;
//    template <typename T1, typename T2>
//    typedef signal_type<void(T1, T2)> Signal2;
};

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