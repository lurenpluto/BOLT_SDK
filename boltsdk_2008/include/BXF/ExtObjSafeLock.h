/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   ExtObjSafeLock
*   Author      :   李亚星
*   Create      :   2013-10-16 
*   LastChange  :   2013-10-16
*   History     :	
*
*   Description :   扩展元对象的事件锁，用以实现安全的同步事件机制
*
********************************************************************/ 
#ifndef __EXTOBJSAFELOCK_H__
#define __EXTOBJSAFELOCK_H__

namespace Xunlei
{
namespace Bolt
{

class ExtObjImplSafeLock
{
public:
	ExtObjImplSafeLock()
		:m_lockCount(0)
		,m_del(false)
	{

	}

	virtual ~ExtObjImplSafeLock()
	{
		assert(m_del);
		assert(m_lockCount <= 0);
	}

	void Lock()
	{
		assert(m_lockCount >= 0);
		++m_lockCount;
	}

	void Unlock()
	{
		assert(m_lockCount >= 1);
		--m_lockCount;
		if (m_lockCount <= 0)
		{
			if (m_del)
			{
				DeleteImpl();
			}
		}
	}

	void SafeDelete()
	{
		if (!m_del)
		{
			m_del = true;

			if (m_lockCount <= 0)
			{
				DeleteImpl();
			}
		}
	}

	bool IsDelete() const
	{
		return m_del;
	}

	bool IsLocked() const
	{
		return (m_lockCount > 0);
	}

protected:

	virtual void DeleteImpl()
	{
		delete this;
	}

private:

	long m_lockCount;
	bool m_del;
};

template<typename T>
class ExtObjImplAutoSafeLockT
{
public:
	ExtObjImplAutoSafeLockT(T* lpObj)
		:m_lpObj(lpObj)
	{
		assert(m_lpObj);
		lpObj->Lock();
	}

	~ExtObjImplAutoSafeLockT()
	{
		assert(m_lpObj);
		m_lpObj->Unlock();
	}

private:

	T*	m_lpObj;
};

} // Bolt
} // Xunlei

#endif // __EXTOBJSAFELOCK_H__