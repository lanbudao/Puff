#include <stdio.h>
#include <iostream>
#include "AVPlayer.h"
#include "CObject.h"
#include "CThread.h"
#include "AVLog.h"
#include "signals2/signal_type.hpp"
#include "renderer/SDLRenderer.h"

using namespace Puff;

class MyThread: public CThread
{
public:

protected:
    void run() PU_DECL_OVERRIDE
    {
        int i = 100;
        while (i-- > 0) {
            printf("child thread1: %d\n", i);
            msleep(400);
        }

        CThread::run();
    }
};
class MyThread2: public CThread
{
public:

protected:
    void run() PU_DECL_OVERRIDE
    {
        int i = 100;
        while (i-- > 0) {
            printf("child thread2: %d\n", i);
            PU_EMIT Changed(i);
            msleep(400);
        }

        CThread::run();
    }
PU_SIGNALS:
    boost::signals2::signal<void(int)> Changed;
};

class MyReceive {
public:
    void func()
    {
        printf("thread is exit!\n");
    }
    void func2(int x)
    {
        printf("func2: %d!\n", x);
    }
};

static void func1()
{
    printf("thread is exit111!\n");
}

int main(int argc, char *argv[])
{
    logger.setLogFile("F:/log.txt");

    AVPlayer *player = new AVPlayer();
    SDLRenderer *renderer = new SDLRenderer();
    renderer->init(400, 300);

    player->play("F:/work_file/video/M.I.A.OfficialVideo.mp4");

    renderer->show();
    avdebug("hello world\n");
    avdebug("hello world\n");
    avdebug("hello world\n");
//    int value = 12;
//    const int a = value;
//    const int *ap = &a;
//    int* tmp = const_cast<int*>(ap);
//    *tmp = 11;
//    cout << value << endl;
//    cout << a << endl;
//    cout << *tmp << endl;
//    CThread *t1 = new MyThread();
//    MyThread2 *t2 = new MyThread2();
//    MyReceive *receive = new MyReceive;
//    Connect(t1->finished, receive, &MyReceive::func);
//    Connect(t1->finished, func1);
//    Connect1(t2->Changed, receive, &MyReceive::func2);
////    t2->Changed.connect(boost::bind(&MyReceive::func2, receive, _1));
////    t->finished.connect(func1);
//    t1->start();
//    t2->start();

    return 0;
}
//
//template<class T1>
//class MS_AV_EXPORT SlotBase {
//public:
//    virtual void Exec(T1 param1) = 0;
//
//    virtual ~SlotBase() {}
//};
//
//template<class T, class T1>
//class MS_AV_EXPORT SlotImpl : public SlotBase<T1>
//{
//public:
//    SlotImpl(T *pObj, void (T::*func)(T1)) {
//        obj = pObj;
//        func = func;
//    }
//
//    void Exec(T1 param1) {
//        (obj->*func)(param1);
//    }
//
//private:
//    T *obj;
//
//    void (T::*func)(T1);
//};
//
//
//template<class T1>
//class MS_AV_EXPORT Signal {
//public:
//    template<class T>
//    void Bind(T *obj, void (T::*func)(T1)) {
//        slotSet.push_back(new SlotImpl<T, T1>(obj, func));
//    }
//
//    ~Signal() {
//        for (int i = 0; i < (int) slotSet.size(); i++) {
//            delete slotSet[i];
//        }
//    }
//
//    void operator()(T1 param1) {
//        for (int i = 0; i < (int) slotSet.size(); i++) {
//            slotSet[i]->Exec(param1);
//        }
//    }
//
//private:
//    vector<SlotBase<T1> *> slotSet;
//};
//
//#define Connect(sender, signal, receiver, method) ( (sender)->signal.Bind(receiver, method) )
//
//class A
//{
//public:
//    void FuncOfA(int param)
//    {
//        printf("A::FuncOfA(%d)\n", param);
//    }
//};
//class B
//{
//public:
//    void FuncOfB(int param)
//    {
//        printf("B::FuncOfB(%d)\n", param);
//    }
//};
//class C
//{
//public:
//    C()
//    {
//        m_Value = 0;
//    }
//    void SetValue(int value)
//    {
//        if(m_Value != value)
//        {
//            m_Value = value;
//            ValueChanged(m_Value);
//        }
//    }
//public:
//    Signal<int> ValueChanged;
//private:
//    int m_Value;
//};
//
//int main(int argc, char *argv[])
//{
////    printf("What is that!");
////    getchar();
//
//    int a = 0;
//    int b = a;
//    printf("%d, %d\n", a, b);
////    return 0;
//    A* pA = new A;
//    B* pB = new B;
//
//    C* pC = new C;
//    Connect(pC, ValueChanged, pA, &A::FuncOfA);
//    Connect(pC, ValueChanged, pB, &B::FuncOfB);
//    pC->SetValue(10);
////    pC->SetValue(5);
////    pC->SetValue(5);
////    delete pC;
////    delete pB;
////    delete pA;
////    scanf("%*s");
//
//    return 0;
//}
