/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   ExtObjLuaHostImpl
*   Author      :   李亚星
*   Create      :   2013-5-21 23:08
*   LastChange  :   2013-5-21 23:08
*   History     :	
*
*   Description :   BOLT扩展对象的lua封装辅助类
*
********************************************************************/ 
#ifndef __EXTOBJLUAHOSTIMPL_H__
#define __EXTOBJLUAHOSTIMPL_H__

#include "./ExtLayoutObjMethodsImpl.h"

namespace Xunlei
{
namespace Bolt
{


// T为扩展对象ObjectClass的lua封装类，比如MagicObject的lua封装类LuaMagicObject
// 子类T需要定义static XLLRTGlobalAPI s_szLuaMemberFuncs[]，并在里面放置相应的lua方法
template<typename T, typename ObjectClass>
class ExtObjLuaHostImpl
{
public:

	typedef ExtObjLuaHostImpl this_class;
	typedef T derived_class;
	typedef ObjectClass obj_class;

public:

	// 其实此类可以为全静态类，但是给每个扩展类的lua封装提供一个实例可能会有用，但是起码不会带来更坏的结果
	ExtObjLuaHostImpl()
	{

	}

	virtual ~ExtObjLuaHostImpl()
	{

	}

	template<typename FinalClass>
	void FillCStruct(ExtObjLuaHost* lpExtLuaHost)
	{
		assert(lpExtLuaHost);

		lpExtLuaHost->size = sizeof(ExtObjLuaHost);
		lpExtLuaHost->userData = this;

		lpExtLuaHost->lpfnGetLuaFunctions = GetLuaFunctionsCallBack;
	}

public:

	// 几个放置对象到lua栈和从lua获取对象的辅助函数

	// 从lua栈获取元对象句柄
	static XLUE_LAYOUTOBJ_HANDLE CheckObject(lua_State* luaState, int index)
	{
		assert(luaState);
		return XLUE_CheckObject(luaState, index);
	}

	// 直接从lua栈的指定位置获取扩展对象类，比如MagicObject
	static obj_class* CheckExtObject(lua_State* luaState, int index)
	{
		XLUE_LAYOUTOBJ_HANDLE hObj = CheckObject(luaState, index);
		if (hObj == NULL)
		{
			return NULL;
		}

		void* lpExtObjHandle = XLUE_GetExtHandle(hObj);
		assert(lpExtObjHandle);
		if (lpExtObjHandle == NULL)
		{
			return NULL;
		}

		return ExtLayoutObjMethodsImpl::ObjectFromExtHandle<obj_class>(lpExtObjHandle);
	}

	// Push类似MagicObject*的对象到栈顶，失败的话会放置一个nil到栈顶
	static bool PushExtObject(lua_State* luaState, obj_class* lpExtObject)
	{
		assert(luaState);
		if (lpExtObject == NULL)
		{
			lua_pushnil(luaState);
			return false;
		}

		XLUE_LAYOUTOBJ_HANDLE hObj = lpExtObject->GetObjectHandle();
		assert(hObj);
		if (lpExtObject == NULL)
		{
			lua_pushnil(luaState);
			return false;
		}

		return !!XLUE_PushObject(luaState, hObj);
	}

private:

	static this_class* ThisFromUserData(void* userData)
	{
		assert(userData);
		return (this_class*)userData;
	}

private:

	BOOL GetLuaFunction(const char* /*className*/, XLLRTGlobalAPI** lplpLuaFunctions, size_t* lpFuncCount)
	{
		assert(lplpLuaFunctions);
		assert(lpFuncCount);

		XLLRTGlobalAPI* lpLuaFuncs = T::s_szLuaMemberFuncs;
		*lplpLuaFunctions = lpLuaFuncs;

		size_t count = 0;
		while(lpLuaFuncs->func != NULL && lpLuaFuncs->name != NULL)
		{
			++lpLuaFuncs;
			++count;
		}

		*lpFuncCount = count;

		return TRUE;
	}

	static BOOL XLUE_STDCALL GetLuaFunctionsCallBack(void* userData, const char* className, XLLRTGlobalAPI** lplpLuaFunctions, size_t* lpFuncCount)
	{
		return ThisFromUserData(userData)->GetLuaFunction(className, lplpLuaFunctions, lpFuncCount);
	}
};

} // Bolt
} // Xunlei

#endif // __EXTOBJLUAHOSTIMPL_H__