/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   ExtObjCreatorImpl
*   Author      :   李亚星
*   Create      :   2013-5-20 0:04
*   LastChange  :   2013-5-20 0:04
*   History     :	
*
*   Description :   外部扩展对象的创建管理器辅助类，每个扩展类别应该对应一个
*
********************************************************************/ 
#ifndef __EXTOBJCREATORIMPL_H__
#define __EXTOBJCREATORIMPL_H__

#include <XLUE.h>
#include <assert.h>
#include "./ExtObjDefine.h"

namespace Xunlei
{
namespace Bolt
{

template<typename ObjectClass>
class ExtObjCreatorImpl
{
public:

	typedef ExtObjCreatorImpl this_class;
	typedef ObjectClass obj_class;

public:
	ExtObjCreatorImpl()
	{

	}

	virtual ~ExtObjCreatorImpl()
	{

	}

	template<typename FinalClass>
	void FillCStruct(ExtObjCreator* lpExtCreator)
	{
		assert(lpExtCreator);

		lpExtCreator->size = sizeof(ExtObjCreator);
		lpExtCreator->userData = this;

		lpExtCreator->lpfnCreateObj = CreateObjCallBack;
		lpExtCreator->lpfnDestroyObj = DestroyObjCallBack;
	}

public:

	// 创建扩展元对象的外部实例，如果成功则返回一个handle用来唯一标识
	virtual obj_class* CreateObj(const char* lpObjClass, XLUE_LAYOUTOBJ_HANDLE hObj) = 0;

	// 销毁扩展元对象的外部实例，lpObjHandle为创建时候返回的handle
	virtual void DestroyObj(obj_class* lpObj) = 0;

private:

	static this_class* ThisFromUserData(void* userData)
	{
		assert(userData);
		return (this_class*)userData;
	}

private:
	
	static void* XLUE_STDCALL CreateObjCallBack(void* userData, const char* lpObjClass, XLUE_LAYOUTOBJ_HANDLE hObj)
	{
		obj_class* lpObj = ThisFromUserData(userData)->CreateObj(lpObjClass, hObj);

		return lpObj->GetObjectExtHandle();
	}

	
	static void XLUE_STDCALL DestroyObjCallBack(void* userData, void* objHandle)
	{
		obj_class* lpObj = ExtLayoutObjMethodsImpl::ObjectFromExtHandle<obj_class>(objHandle);
		assert(lpObj);

		return ThisFromUserData(userData)->DestroyObj(lpObj);
	}
};

} // Bolt
} // Xunlei

#endif // __EXTOBJCREATORIMPL_H__