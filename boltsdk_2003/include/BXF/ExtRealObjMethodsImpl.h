/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   ExtRealObjMethodsImpl
*   Author      :   李亚星
*   Create      :   2013-5-22 11:08
*   LastChange  :   2013-5-22 11:08
*   History     :	
*
*   Description :   扩展元对象的RealObj层事件辅助类
*
********************************************************************/ 
#ifndef __EXTREALOBJMETHODSIMPL_H__
#define __EXTREALOBJMETHODSIMPL_H__

#include "./ExtLayoutObjMethodsImpl.h"

namespace Xunlei
{
namespace Bolt
{

class ExtRealObjMethodsImpl
	: public ExtLayoutObjMethodsImpl
{
public:
	typedef ExtRealObjMethodsImpl this_class;
	typedef ExtLayoutObjMethodsImpl base_class;

public:

	ExtRealObjMethodsImpl(XLUE_LAYOUTOBJ_HANDLE hObj)
		:ExtLayoutObjMethodsImpl(hObj)
	{
	}

	virtual ~ExtRealObjMethodsImpl()
	{
	}

	template<typename FinalClass>
	static void FillCStruct(ExtRealObjMethods* lpExtMethods)
	{
		assert(lpExtMethods);

		lpExtMethods->size = sizeof(ExtRealObjMethods);
		lpExtMethods->userData = NULL;

		base_class::FillCVector<FinalClass>(&lpExtMethods->layoutObjMethodVector);
		FillCVector<FinalClass>(&lpExtMethods->realObjMethodVector);
	}

	template<typename FinalClass>
	static void FillCVector(ExtRealObjMethodsVector* lpExtMethodsVector)
	{
		assert(lpExtMethodsVector);
		
		typedef FinalClass final_class;
		lpExtMethodsVector->size = sizeof(ExtRealObjMethodsVector);

		AssignIfOverride(this_class, final_class, OnCreateRealWindow, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnSetRealFocus, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnGetRenderWindow, lpExtMethodsVector);
	}

public:

	// Real类型元对象的事件，指示父窗口已经创建/将要销毁，外部扩展对象需要在这个里面创建/销毁自己的系统窗口，
	// 并调用XLUE_SetRealObjectWindow设置到内置RealObject上或者移除
	virtual void OnCreateRealWindow(BOOL /*bCreate*/, OS_HOSTWND_HANDLE /*hWnd*/)
	{

	}

	// Real类型元对象的事件，指示设置焦点到自己的系统窗口上,，或者移除焦点
	virtual void OnSetRealFocus(BOOL /*focus*/)
	{

	}

	// Real类型元对象的事件，获取当前对象显示的真正窗口句柄
	virtual OS_HOSTWND_HANDLE OnGetRenderWindow()
	{
		assert(false);
		return NULL;
	}

protected:

	static this_class* ThisFromObjHandle(void* objHandle)
	{
		assert(objHandle);

		base_class* lpBase = base_class::ThisFromObjHandle(objHandle);

		return dynamic_cast<this_class*>(lpBase);
	}

private:

	static void XLUE_STDCALL OnCreateRealWindowCallBack(void* /*userData*/, void* objHandle, BOOL bCreate, OS_HOSTWND_HANDLE hWnd)
	{
		return ThisFromObjHandle(objHandle)->OnCreateRealWindow(bCreate, hWnd);
	}

	static void XLUE_STDCALL OnSetRealFocusCallBack(void* /*userData*/, void* objHandle, BOOL focus)
	{
		return ThisFromObjHandle(objHandle)->OnSetRealFocus(focus);
	}

	static OS_HOSTWND_HANDLE XLUE_STDCALL OnGetRenderWindowCallBack(void* /*userData*/, void* objHandle)
	{
		return ThisFromObjHandle(objHandle)->OnGetRenderWindow();
	}
};

} // Bolt
} // Xunlei

#endif // __EXTREALOBJMETHODSIMPL_H__