/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   ExtResImpl
*   Author      :   李亚星
*   Create      :   2013-7-23 
*   LastChange  :   2013-7-23
*   History     :	
*
*   Description :   外部扩展资源的辅助类
*
********************************************************************/ 
#ifndef __EXTRESOURCEIMPL_H__
#define __EXTRESOURCEIMPL_H__

#include <XLLuaRuntime.h>
#include <XLUE.h>
#include <assert.h>
#include "./ExtObjDefine.h"
#include "./ExtObjRegisterHelper.h"

namespace Xunlei
{
namespace Bolt
{

class ResourceWrapper
{
public:

	XLUE_RESOURCE_HANDLE m_hResHandle;

public:

	long AddRef()
	{
		assert(m_hResHandle);
		return XLUE_AddRefResource(m_hResHandle);
	}

	long Release()
	{
		assert(m_hResHandle);
		return XLUE_ReleaseResource(m_hResHandle);
	}

	XLUE_RESOURCE_HANDLE GetHandleWithoutRef()
	{
		assert(m_hResHandle);
		return m_hResHandle;
	}

	XLUE_RESOURCE_HANDLE GetHandleWithRef()
	{
		assert(m_hResHandle);
		AddRef();

		return m_hResHandle;
	}

	const char* GetType()
	{
		assert(m_hResHandle);
		return XLUE_GetResType(m_hResHandle);
	}

	const char* GetID()
	{
		assert(m_hResHandle);
		return XLUE_GetResType(m_hResHandle);
	}

	void* GetExtHandle()
	{
		assert(m_hResHandle);
		return XLUE_GetResExtHandle(m_hResHandle);
	}
};


class ExtResourceMethodsImpl
	: public ResourceWrapper
{
public:

	typedef ExtResourceMethodsImpl this_class;
	typedef ResourceWrapper base_class;

public:
	ExtResourceMethodsImpl(XLUE_RESOURCE_HANDLE hResHandle)
	{
		assert(hResHandle);
		m_hResHandle = hResHandle;
		
		AddRef();
	}

	virtual ~ExtResourceMethodsImpl()
	{
		assert(m_hResHandle);
		Release();
		m_hResHandle = NULL;
	}

	// 获取当前扩展资源类型的外部handle
	void* GetResourceExtHandle()
	{
		return this;
	}

	// 从扩展资源的外部handle，转换到具体对应的扩展资源实现对象
	template<typename TargetClass>
	static TargetClass* ResourceObjFromExtHandle(void* resHandle)
	{
		this_class* lpBase = (this_class*)resHandle;
		return dynamic_cast<TargetClass*>(lpBase);
	}

	template<typename FinalClass>
	static void FillCStruct(ExtResourceMethods* lpExtMethods)
	{
		typedef FinalClass final_class;

		assert(lpExtMethods);

		lpExtMethods->size = sizeof(ExtResourceMethods);
		lpExtMethods->userData = NULL;
		
		lpExtMethods->lpfnGetRealHandle = GetRealHandleCallBack;
		lpExtMethods->lpfnAddRefRealHandle = AddRefRealHandleCallBack;
		lpExtMethods->lpfnReleaseRealHandle = ReleaseRealHandleCallBack;

		AssignIfOverride(this_class, final_class, LoadRes, lpExtMethods);
		AssignIfOverride(this_class, final_class, FreeRes, lpExtMethods);
	}

public:

	// 真正加载该资源，主要用以实现按需加载
	virtual bool LoadRes(const wchar_t* /*lpResFolder*/)
	{
		return true;
	}

	// 释放该资源，主要用以闲时的垃圾回收
	virtual bool FreeRes()
	{
		return true;
	}

	// 获取对应的真正资源句柄，比如XL_BITMAP_HANDLE，XLGP_ICON_HANDLE等
	// 返回值不需增加引用计数！！
	virtual void* GetRealHandle() = NULL;

	// 真正资源句柄的生命周期基于引用计数管理
	virtual long AddRefRealHandle(void* /*lpRealHandle*/) = NULL;
	virtual long ReleaseRealHandle(void* /*lpRealHandle*/) = NULL;

private:

	static this_class* ThisFromResHandle(void* resHandle)
	{
		assert(resHandle);

		return (this_class*)resHandle;
	}

private:

	static BOOL XLUE_STDCALL LoadResCallBack(void* /*userData*/, void* lpResHandle, const wchar_t* lpResFolder)
	{
		return ThisFromResHandle(lpResHandle)->LoadRes(lpResFolder)? TRUE : FALSE;
	}

	static BOOL XLUE_STDCALL FreeResCallBack(void* /*userData*/, void* lpResHandle)
	{
		return ThisFromResHandle(lpResHandle)->FreeRes()? TRUE : FALSE;
	}

	static void* XLUE_STDCALL GetRealHandleCallBack(void* /*userData*/, void* lpResHandle)
	{
		return ThisFromResHandle(lpResHandle)->GetRealHandle();
	}

	static long XLUE_STDCALL AddRefRealHandleCallBack(void* /*userData*/, void* lpResHandle, void* lpResRealHandle)
	{
		return ThisFromResHandle(lpResHandle)->AddRefRealHandle(lpResRealHandle);
	}

	static long XLUE_STDCALL ReleaseRealHandleCallBack(void* /*userData*/, void* lpResHandle, void* lpResRealHandle)
	{
		return ThisFromResHandle(lpResHandle)->ReleaseRealHandle(lpResRealHandle);
	}
};

template<typename ResourceClass>
class ExtResourceCreatorImpl
{
public:

	typedef ExtResourceCreatorImpl this_class;
	typedef ResourceClass resource_class;

public:
	ExtResourceCreatorImpl()
	{

	}

	virtual ~ExtResourceCreatorImpl()
	{

	}

	template<typename FinalClass>
	void FillCStruct(ExtResourceCreator* lpExtCreator)
	{
		assert(lpExtCreator);

		lpExtCreator->size = sizeof(ExtResourceCreator);
		lpExtCreator->userData = this;

		lpExtCreator->lpfnCreateRes = CreateResourceCallBack;
		lpExtCreator->lpfnDestroyRes = DestroyResourceCallBack;
	}

public:

	// 创建扩展资源类型的外部实例，如果成功则返回一个handle用来唯一标识
	virtual resource_class* CreateResource(const char* lpResType, XLUE_RESOURCE_HANDLE hResHandle) = 0;

	// 销毁扩展资源类型的外部实例，lpResObj为创建时候返回的handle
	virtual void DestroyResource(resource_class* lpResObj) = 0;

private:

	static this_class* ThisFromUserData(void* userData)
	{
		assert(userData);
		return (this_class*)userData;
	}

private:
	
	static void* XLUE_STDCALL CreateResourceCallBack(void* userData, const char* lpResType, XLUE_RESOURCE_HANDLE hResHandle)
	{
		resource_class* lpResObj = ThisFromUserData(userData)->CreateResource(lpResType, hResHandle);

		return lpResObj->GetResourceExtHandle();
	}

	static void XLUE_STDCALL DestroyResourceCallBack(void* userData, void* lpResHandle)
	{
		resource_class* lpResObj = ExtLayoutObjMethodsImpl::ObjectFromExtHandle<resource_class>(lpResHandle);
		assert(lpResObj);

		return ThisFromUserData(userData)->DestroyResource(lpResObj);
	}
};

template<typename ResourceClass>
class ExtResourceParserImpl
{
public:

	typedef ExtResourceParserImpl this_class;
	typedef ResourceClass resource_class;

public:
	ExtResourceParserImpl()
	{

	}

	virtual ~ExtResourceParserImpl()
	{

	}

	template<typename FinalClass>
	void FillCStruct(ExtResourceParser* lpExtParser)
	{
		assert(lpExtParser);

		typedef FinalClass final_class;

		lpExtParser->size = sizeof(ExtResourceParser);
		lpExtParser->userData = this;

		AssignIfOverride(this_class, final_class, ParseFromXML, lpExtParser);
		AssignIfOverride(this_class, final_class, ParseFromLua, lpExtParser);
	}

public:

	// 从资源xml配置里面解析
	virtual bool ParseFromXML(resource_class* /*lpResObj*/, XLUE_XML_HANDLE /*hResXML*/)
	{
		assert(false);
		return false;
	}

	// 从lua table动态解析资源，资源属性在lua table里面定义和对应的xml定义结构须保持一致
	virtual bool ParseFromLua(resource_class* /*lpResObj*/, lua_State* /*luaState*/, int /*index*/)
	{
		assert(false);
		return false;
	}

private:

	static this_class* ThisFromUserData(void* userData)
	{
		assert(userData);
		return (this_class*)userData;
	}

private:

	static BOOL XLUE_STDCALL ParseFromXMLCallBack(void* userData, void* lpResHandle, XLUE_XML_HANDLE hResXML)
	{
		resource_class* lpResObj = ExtResourceMethodsImpl::ResourceObjFromExtHandle<resource_class>(lpResHandle);
		assert(lpResObj);

		return ThisFromUserData(userData)->ParseFromXML(lpResObj, hResXML);
	}
	
	static BOOL XLUE_STDCALL ParseFromLuaCallBack(void* userData, void* lpResHandle, lua_State* luaState, int index)
	{
		resource_class* lpResObj = ExtResourceMethodsImpl::ResourceObjFromExtHandle<resource_class>(lpResHandle);
		assert(lpResObj);

		return ThisFromUserData(userData)->ParseFromLua(lpResObj, luaState, index);
	}
};


// 如果资源在xml里面配置的是简单模式，也就是只有属性列表，没有嵌套子节点
// 那么可以使用该辅助类
template<typename ResourceClass>
class ExtResourceParserImplEx
	: public ExtResourceParserImpl<ResourceClass>
{
public:

	typedef ExtResourceParserImplEx this_class;
	typedef ExtResourceParserImpl<ResourceClass> base_class;
	typedef ResourceClass resource_class;

public:
	ExtResourceParserImplEx()
	{

	}

	virtual ~ExtResourceParserImplEx()
	{

	}

public:

	// 从xml里面解析属性
	virtual bool ParseAttributeFromXML(resource_class* /*lpResObj*/, const char* /*lpName*/, const char* /*lpValue*/)
	{
		assert(false);
		return false;
	}

	// 从lua里面解析属性
	virtual bool ParseAttributeFromLua(resource_class* /*lpResObj*/, const char* /*lpName*/, lua_State* /*luaState*/, int /*index*/)
	{
		assert(false);
		return false;
	}

private:

	// ExtResourceParserImpl methods
	virtual bool ParseFromXML(resource_class* lpResObj, XLUE_XML_HANDLE hResXML)
	{
		assert(lpResObj);
		assert(hResXML);

		if (!XLUE_XML_BeginGetAttribute(hResXML))
		{
			assert(false);
			return false;
		}

		const char* lpName = NULL, *lpValue = NULL;
		while(XLUE_XML_GetNextAttribute(hResXML, &lpName, &lpValue))
		{
			assert(lpName);
			assert(lpValue);
			if (lpName != NULL && lpValue != NULL)
			{
				ParseAttributeFromXML(lpResObj, lpName, lpValue);
			}
		}

		return true;
	}

	virtual bool ParseFromLua(resource_class* lpResObj, lua_State* luaState, int index)
	{
		assert(lpResObj);
		assert(luaState);
		assert(lua_istable(luaState, index));

		lua_pushvalue(luaState, index);
		lua_pushnil(luaState);
		while (lua_next(luaState, -2) != 0)
		{
			const char* key = luaL_checkstring(luaState, -2);
			assert(key);
			if (key != NULL)
			{
				ParseAttributeFromLua(lpResObj, key, luaState, -1);
			}

			lua_pop(luaState, 1);
		}

		lua_pop(luaState, 1);

		return true;
	}
};

// T为扩展对象ObjectClass的lua封装类，比如MagicObject的lua封装类LuaMagicObject
// 子类T需要实现GetLuaFunction并在里面返回需要注册的API
template<typename T, typename ResourceClass>
class ExtResourceLuaHostImpl
{
public:

	typedef ExtResourceLuaHostImpl this_class;
	typedef T derived_class;
	typedef ResourceClass resource_class;

public:

	ExtResourceLuaHostImpl()
	{

	}

	virtual ~ExtResourceLuaHostImpl()
	{

	}

	template<typename FinalClass>
	void FillCStruct(ExtResourceLuaHost* lpExtLuaHost)
	{
		typedef FinalClass final_class;

		assert(lpExtLuaHost);

		lpExtLuaHost->size = sizeof(ExtResourceLuaHost);
		lpExtLuaHost->userData = this;

		lpExtLuaHost->lpfnGetLuaFunctions = GetLuaFunctionsCallBack;
		AssignIfOverride(this_class, final_class, RegisterAuxClass, lpExtLuaHost);
	}

public:

	// 几个放置对象到lua栈和从lua获取对象的辅助函数

	// 从lua栈获取资源句柄，返回值如果不为空，那么释放
	static XLUE_RESOURCE_HANDLE CheckRes(lua_State* luaState, int index, const char* lpResType = NULL)
	{
		assert(luaState);
		XLUE_RESOURCE_HANDLE hResHandle = NULL;

		if (lpResType == NULL)
		{
			XLUE_CheckRes(luaState, index, &hResHandle);
		}
		else
		{
			XLUE_CheckResEx(luaState, index, lpResType, &hResHandle);
		}

		return hResHandle;
	}

	// 直接从lua栈的指定位置获取扩展资源类，比如IconRes
	static resource_class* CheckExtResObj(lua_State* luaState, int index, const char* lpResType = NULL)
	{
		XLUE_RESOURCE_HANDLE hResHandle = CheckRes(luaState, index, lpResType);
		if (hResHandle == NULL)
		{
			return NULL;
		}

		void* lpExtResHandle = XLUE_GetResExtHandle(hResHandle);
		XLUE_ReleaseResource(hResHandle);

		assert(lpExtResHandle);
		if (lpExtResHandle == NULL)
		{
			return NULL;
		}

		return ExtResourceMethodsImpl::ResourceObjFromExtHandle<resource_class>(lpExtResHandle);
	}

	template<typename TargetClass>
	static TargetClass* CheckExtResObjEx(lua_State* luaState, int index, const char* lpResType = NULL)
	{
		XLUE_RESOURCE_HANDLE hResHandle = CheckRes(luaState, index, lpResType);
		if (hResHandle == NULL)
		{
			return NULL;
		}

		void* lpExtResHandle = XLUE_GetResExtHandle(hResHandle);
		XLUE_ReleaseResource(hResHandle);

		assert(lpExtResHandle);
		if (lpExtResHandle == NULL)
		{
			return NULL;
		}

		return ExtResourceMethodsImpl::ResourceObjFromExtHandle<TargetClass>(lpExtResHandle);
	}

	// Push扩展资源类似IconRes的对象到栈顶，失败的话会放置一个nil到栈顶
	static bool PushExtResObj(lua_State* luaState, resource_class* lpExtResObj)
	{
		assert(luaState);
		if (lpExtResObj == NULL)
		{
			lua_pushnil(luaState);
			return false;
		}

		XLUE_RESOURCE_HANDLE hResHandle = lpExtResObj->GetHandleWithoutRef();
		assert(hResHandle);
		if (hResHandle == NULL)
		{
			lua_pushnil(luaState);
			return false;
		}

		return !!XLUE_PushRes(luaState, hResHandle);
	}

	template<typename TargetClass>
	static bool PushExtObjectEx(lua_State* luaState, TargetClass* lpExtResObj)
	{
		assert(luaState);
		if (lpExtResObj == NULL)
		{
			lua_pushnil(luaState);
			return false;
		}

		XLUE_RESOURCE_HANDLE hResHandle = lpExtResObj->GetHandleWithoutRef();
		assert(hResHandle);
		if (hResHandle == NULL)
		{
			lua_pushnil(luaState);
			return false;
		}

		return !!XLUE_PushRes(luaState, hResHandle);
	}

public:

	// 获取该扩展资源所有的lua扩展api	
	virtual bool GetLuaFunction(const char* className, const XLLRTGlobalAPI** lplpLuaFunctions, size_t* lpFuncCount) = 0;

	// 注册额外的辅助lua类或者全局对象
	virtual bool RegisterAuxClass(const char* /*className*/, XL_LRT_ENV_HANDLE /*hEnv*/)
	{
		return true;
	}

private:

	static this_class* ThisFromUserData(void* userData)
	{
		assert(userData);
		return (this_class*)userData;
	}

private:

	static BOOL XLUE_STDCALL GetLuaFunctionsCallBack(void* userData, const char* className, const XLLRTGlobalAPI** lplpLuaFunctions, size_t* lpFuncCount)
	{
		return ThisFromUserData(userData)->GetLuaFunction(className, lplpLuaFunctions, lpFuncCount)? TRUE : FALSE;
	}

	static BOOL XLUE_STDCALL RegisterAuxClassCallBack(void* userData, const char* className, XL_LRT_ENV_HANDLE hEnv)
	{
		return ThisFromUserData(userData)->RegisterAuxClass(className, hEnv)? TRUE : FALSE;
	}
};

// ExtResourceLuaHostImpl的扩展实现，可以直接从子类读取API数组
// 子类T需要定义static XLLRTGlobalAPI s_szLuaMemberFuncs[]，并在里面放置相应的lua方法
template<typename T, typename ResourceClass>
class ExtResourceLuaHostImplEx
	: public ExtResourceLuaHostImpl<T, ResourceClass>
{
public:
	ExtResourceLuaHostImplEx()
	{

	}

	virtual ~ExtResourceLuaHostImplEx()
	{

	}

public:

	// ExtResourceLuaHostImpl
	virtual bool GetLuaFunction(const char* className, const XLLRTGlobalAPI** lplpLuaFunctions, size_t* lpFuncCount)
	{
		return DefaultGetLuaFunction(className, lplpLuaFunctions, lpFuncCount);
	}

private:

	bool DefaultGetLuaFunction(const char* /*className*/, const XLLRTGlobalAPI** lplpLuaFunctions, size_t* lpFuncCount)
	{
		assert(lplpLuaFunctions);
		assert(lpFuncCount);

		const XLLRTGlobalAPI* lpLuaFuncs = T::s_szLuaMemberFuncs;
		*lplpLuaFunctions = lpLuaFuncs;

		size_t count = 0;
		while(lpLuaFuncs->func != NULL && lpLuaFuncs->name != NULL)
		{
			++lpLuaFuncs;
			++count;
		}

		*lpFuncCount = count;

		return true;
	}
};

// 完全使用BOLT扩展框架辅助类情况下使用的扩展资源注册辅助类，如果某个类没有实现，用NullClass代替
template<typename ResourceClass, typename CreatorClass, 
typename ParserClass = NullClass, typename LuaHostClass = NullClass>
struct ExtResourceRegisterHelper
{
	static bool Register(const char* resType, unsigned long attribute)
	{
		assert(resType);

		ExtResourceRegisterInfo registerInfo = { sizeof(registerInfo) };
		registerInfo.resType = resType;
		registerInfo.attribute = attribute;

		ObjectStructFiller<ExtResourceMethods, ResourceClass> methods;
		registerInfo.lpExtResMethods = methods;

		SingletonStructFiller<ExtResourceCreator, CreatorClass> creator;
		registerInfo.lpExtResCreator = creator;

		SingletonStructFiller<ExtResourceParser, ParserClass> parser;
		registerInfo.lpExtResParser = parser;	

		SingletonStructFiller<ExtResourceLuaHost, LuaHostClass> luaHost;
		registerInfo.lpExtResLuaHost = luaHost;

		return !!XLUE_RegisterExtRes(&registerInfo);
	}
};

} // Bolt
} // Xunlei

#endif // __EXTRESOURCEIMPL_H__