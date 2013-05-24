#pragma once

/*
下面的是一个典型的C++类，有方法和事件，调用Add方法会触发Result Event

*/
#include <vector>

typedef void (*funcResultCallBack) (DWORD dwUserData1,DWORD dwUserData2,int nResult);

struct CallbackNode
{
    funcResultCallBack pfnCallBack;
    DWORD dwUserData1;
    DWORD dwUserData2;
};

class MyClass
{
public:
    MyClass(void);
    ~MyClass(void);

public:
    int Add(int lhs,int rhs);
    int AttachResultListener(DWORD dwUserData1,DWORD dwUserData2,funcResultCallBack pfnCallBack);

protected:
    void FireResultEvent(int nResult);
    std::vector<CallbackNode> m_allCallBack;
};


