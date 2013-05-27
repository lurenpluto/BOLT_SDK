/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   TimerManagerWrapper
*   Author      :   李亚星
*   Create      :   2013-5-21 22:22
*   LastChange  :   2013-5-21 22:22
*   History     :	
*
*   Description :   BOLT引擎的timermanager的C++封装
*
********************************************************************/ 
#ifndef __TIMERMANAGERWRAPPER_H__
#define __TIMERMANAGERWRAPPER_H__

#include <XLUE.h>

namespace Xunlei
{
namespace Bolt
{

class TimerManagerWrapper
{
public:
	TimerManagerWrapper()
	{}

	virtual ~TimerManagerWrapper()
	{}
	
	unsigned long SetTimer(unsigned int elapse)
	{
		return XLUE_SetTimer(&OnTimerCallBack, elapse, this);
	}

	unsigned long SetOnceTimer(unsigned int elapse)
	{
		return XLUE_SetOnceTimer(&OnTimerCallBack, elapse, this);
	}

	unsigned long SetIDTimer(unsigned int timerID, unsigned int elapse)
	{
		return XLUE_SetIDTimer(&OnTimerCallBack, timerID, elapse, this);
	}

	unsigned long SetOnceIDTimer(unsigned int timerID, unsigned int elapse)
	{
		return XLUE_SetOnceIDTimer(&OnTimerCallBack, timerID, elapse, this);
	}

	bool QueryTime(unsigned int timerID)
	{
		return !!XLUE_QueryTimer(timerID);
	}

	bool KillTimer(unsigned int timerID)
	{
		return !!XLUE_KillTimer(timerID, NULL);
	}

protected:

	virtual void OnTimer(unsigned int /*timerID*/)
	{
		
	}

private:

	static void XLUE_STDCALL OnTimerCallBack(void* userData, unsigned int timerID)
	{
		TimerManagerWrapper* lpThis = (TimerManagerWrapper*)userData;
		assert(lpThis);

		lpThis->OnTimer(timerID);
	}
};

template<typename OwnerClass>
class MemberTimerManagerWrapperT
	: public TimerManagerWrapper
{
public:

	typedef OwnerClass owner_class;
	typedef void (OwnerClass::*LPFNOnTimer)(unsigned int);

public:
	MemberTimerManagerWrapperT()
		:m_lpOwner(NULL),
		m_lpTimerProc(NULL)
	{

	}

	MemberTimerManagerWrapperT(owner_class* lpOwner, LPFNOnTimer lpTimerProc)
		:m_lpOwner(lpOwner),
		m_lpTimerProc(lpTimerProc)
	{

	}

	virtual ~MemberTimerManagerWrapperT()
	{
		m_lpOwner = NULL;
		m_lpTimerProc = NULL;
	}

	void SetTimerProc(owner_class* lpOwnerThis, LPFNOnTimer lpTimerProc)
	{
		m_lpOwner = lpOwnerThis;
		m_lpTimerProc = lpTimerProc;
	}

private:

	virtual void OnTimer(unsigned int timerID)
	{
		assert(m_lpOwner);
		assert(m_lpTimerProc);

		(m_lpOwner->*m_lpTimerProc)(timerID);
	}

private:

	owner_class* m_lpOwner;
	LPFNOnTimer m_lpTimerProc;
};

} // Bolt
} // Xunlei

#endif // __TIMERMANAGERWRAPPER_H__