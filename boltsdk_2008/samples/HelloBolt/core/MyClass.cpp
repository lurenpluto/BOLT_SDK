#include "StdAfx.h"
#include ".\myclass.h"

using namespace std;

MyClass::MyClass(void)
{
}

MyClass::~MyClass(void)
{
    
}

int MyClass::Add(int lhs,int rhs)
{
    int result = lhs + rhs;
    FireResultEvent(result);
    return result;
}

int MyClass::AttachResultListener(DWORD dwUserData1,DWORD dwUserData2,funcResultCallBack pfnCallBack)
{
    CallbackNode newNode;
    newNode.pfnCallBack = pfnCallBack;
    newNode.dwUserData1 = dwUserData1;
    newNode.dwUserData2 = dwUserData2;
    m_allCallBack.push_back(newNode);
    return 0;
}

void MyClass::FireResultEvent(int nResult)
{
    for(size_t i = 0;i < m_allCallBack.size();++ i)
    {
        m_allCallBack[i].pfnCallBack(m_allCallBack[i].dwUserData1,m_allCallBack[i].dwUserData2,nResult);
    }
}
