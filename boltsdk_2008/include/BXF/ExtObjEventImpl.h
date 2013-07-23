/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   ExtObjEventImpl
*   Author      :   李亚星
*   Create      :   2013-6-9 
*   LastChange  :   2013-6-9
*   History     :	
*
*   Description :   BOLT扩展对象的lua事件封装辅助类
*
********************************************************************/ 
#ifndef __EXTOBJEVENTIMPL_H__
#define __EXTOBJEVENTIMPL_H__

#include "./ExtLayoutObjMethodsImpl.h"

namespace Xunlei
{
namespace Bolt
{

// 该类应该是每个ExtObj对象都应该有一个，子类需要实现ExtEventObjFromObject方法！
template<typename T, typename ObjectClass>
class ExtObjEventImpl
{
public:

	typedef ExtObjEventImpl this_class;
	typedef ObjectClass obj_class;
	typedef T derived_class;

public:
	ExtObjEventImpl()
	{

	}

	virtual ~ExtObjEventImpl()
	{

	}

	template<typename FinalClass>
	static void FillCStruct(ExtObjEvent* lpExtEvent)
	{
		assert(lpExtEvent);

		typedef FinalClass final_class;

		lpExtEvent->size = sizeof(ExtObjEvent);
		lpExtEvent->userData = NULL;

		lpExtEvent->lpfnEventExists = EventExistsCallBack;

		bool ret = false;
		IsFunctionOverride(this_class, final_class, AttachListener, ret);
		IsFunctionOverride(this_class, final_class, RemoveListener, ret);
		if (ret)
		{
			lpExtEvent->lpfnAttachListener = AttachListenerCallBack;
			lpExtEvent->lpfnRemoveListener = RemoveListenerCallBack;
		}

		AssignIfOverride(this_class, final_class, OnAttachListener, lpExtEvent);
		AssignIfOverride(this_class, final_class, OnRemoveListener, lpExtEvent);
	}

protected:

	// 判断指定的内置元对象事件是否存在
	virtual bool EventExists(const char* eventName) = 0;

	/*------------------------------非托管模式---------------------------------*/
	// 挂接指定的事件，luaFuncRef指定了该事件的luafunction，isPushBack指定该事件是放到事件队列前还是末尾(使用者可以根据具体情况来使用)
	// 如果挂接成功，那么返回TRUE，并且lpEventCookie指定一个大于0的值；否则返回FALSE
	virtual bool AttachListener(const char* /*eventName*/, lua_State* /*luaState*/, long /*luaFuncRef*/, 
		BOOL /*isPushBack*/, unsigned long* /*lpEventCookie*/)
	{
		assert(false);
		return false;
	}

	virtual bool RemoveListener(const char* /*eventName*/, unsigned long /*eventCookie*/)
	{
		assert(false);
		return false;
	}

	/*------------------------------托管模式-----------------------------------*/
	// 该模式下事件由引擎内部管理，挂接和移除一个事件会有OnAttachEvent和OnRemoveEvent通知(如果需要关注的话)
	virtual void OnAttachListener(const char* /*eventName*/, unsigned long /*eventCookie*/)
	{

	}

	virtual void OnRemoveListener(const char* /*eventName*/, unsigned long /*eventCookie*/)
	{

	}

private:

	static this_class* ThisFromObjHandle(void* objHandle)
	{
		obj_class* lpObj = ExtLayoutObjMethodsImpl::ObjectFromExtHandle<obj_class>(objHandle);
		assert(lpObj);

		this_class* lpThis = derived_class::ExtEventObjFromObject(lpObj);
		assert(lpThis);

		return lpThis;
	}

	// 由于ExtEvent是应该是每个ExtObj有一个，所以子类需要实现从ExtObjClass对象到ExtEvent对象的推导函数
	static this_class* ExtEventObjFromObject(obj_class* lpObj)
	{
		assert(false && L"class inherit from ExtObjEventImpl should impl ExtEventObjFromObject method!");
		return NULL;
	}

private:

	static BOOL XLUE_STDCALL EventExistsCallBack(void* /*userData*/, void* objHandle, const char* eventName)
	{
		this_class* lpThis = ThisFromObjHandle(objHandle);
		assert(lpThis);

		return lpThis->EventExists(eventName)? TRUE : FALSE;
	}

	static BOOL XLUE_STDCALL AttachListenerCallBack(void* /*userData*/, void* objHandle, const char* eventName, 
		lua_State* luaState, long luaFuncRef, BOOL isPushBack, unsigned long* lpEventCookie)
	{
		this_class* lpThis = ThisFromObjHandle(objHandle);
		assert(lpThis);

		return lpThis->AttachListener(eventName, luaState, luaFuncRef, isPushBack, lpEventCookie)? TRUE : FALSE;
	}

	static BOOL XLUE_STDCALL RemoveListenerCallBack(void* /*userData*/, void* objHandle, const char* eventName, 
		unsigned long eventCookie)
	{
		this_class* lpThis = ThisFromObjHandle(objHandle);
		assert(lpThis);

		return lpThis->RemoveListener(eventName, eventCookie)? TRUE : FALSE;
	}

	static void XLUE_STDCALL OnAttachListenerCallBack(void* /*userData*/, void* objHandle, const char* eventName, 
		unsigned long eventCookie)
	{
		this_class* lpThis = ThisFromObjHandle(objHandle);
		assert(lpThis);

		return lpThis->OnAttachListener(eventName, eventCookie);
	}

	static void XLUE_STDCALL OnRemoveListenerCallBack(void* /*userData*/, void* objHandle, const char* eventName, 
		unsigned long eventCookie)
	{
		this_class* lpThis = ThisFromObjHandle(objHandle);
		assert(lpThis);

		return lpThis->OnRemoveListener(eventName, eventCookie);
	}
};

struct ExtObjEventItem
{
	const char* name;	
};

struct ExtObjEventItems
{
	const ExtObjEventItems* m_lpBaseEventItems;
	const ExtObjEventItem* m_lpEventItems;
};

#define DECLEAR_EXT_EVENT_VECTOR() \
	static const Xunlei::Bolt::ExtObjEventItems* s_lpBaseEventItems; \
	static const Xunlei::Bolt::ExtObjEventItem s_vtEventItems[]; \
	static const Xunlei::Bolt::ExtObjEventItems s_eventItems;

#define BEGIN_EXT_EVENT_VECTOR(classname) \
	const Xunlei::Bolt::ExtObjEventItems classname::s_eventItems = {classname::s_lpBaseEventItems,  classname::s_vtEventItems}; \
	const Xunlei::Bolt::ExtObjEventItems* classname::s_lpBaseEventItems = NULL; \
	const Xunlei::Bolt::ExtObjEventItem classname::s_vtEventItems[] = {
	
#define BEGIN_EXT_EVENT_VECTOR_EX(classname, baseclass) \
	const Xunlei::Bolt::ExtObjEventItems classname::s_eventItems = {classname::s_lpBaseEventItems,  classname::s_vtEventItems}; \
	const Xunlei::Bolt::ExtObjEventItems* classname::s_lpBaseEventItems = &baseclass::s_eventItems; \
	const Xunlei::Bolt::ExtObjEventItem classname::s_vtEventItems[] = {

#define EXT_EVENT_ITEM(eventname) { s_##eventname },
#define END_EXT_EVENT_VECTOR() { NULL } };

#define DECLEAR_EXT_EVENT(eventname) static const char s_##eventname[];
#define IMPL_EXT_EVENT(classname, eventname) const char classname::s_##eventname[] = #eventname;

template<typename T, typename ObjectClass>
class ExtObjEventImplEx
	: public ExtObjEventImpl<T, ObjectClass>
{
public:

	// ExtObjEventImpl method
	virtual bool EventExists(const char* eventName)
	{
		return EventExistsImpl(eventName, &T::s_eventItems);
	}

private:

	bool EventExistsImpl(const char* eventName, const ExtObjEventItems* lpEventItems) const
	{
		assert(eventName);
		assert(lpEventItems);

		bool ret = false;
		if (lpEventItems->m_lpBaseEventItems != NULL)
		{
			ret = EventExistsImpl(eventName, lpEventItems->m_lpBaseEventItems);
		}

		if (ret)
		{
			return true;
		}

		return EventExistsFromItems(eventName, lpEventItems->m_lpEventItems);
	}

	bool EventExistsFromItems(const char* eventName, const ExtObjEventItem* lpEventVector) const
	{
		assert(eventName);
		assert(lpEventVector);
		
		const ExtObjEventItem* lpItrator = lpEventVector;
		while(lpItrator->name != NULL && ::strcmp(lpItrator->name, eventName) != 0)
		{
			++lpItrator;
		}

		if (lpItrator->name != NULL)
		{
			return true;
		}

		return false;
	}
};

} // Bolt
} // Xunlei

#endif // __EXTOBJEVENTIMPL_H__