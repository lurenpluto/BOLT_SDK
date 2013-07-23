/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   ExtObjParserImpl
*   Author      :   李亚星
*   Create      :   2013-5-19 22:40
*   LastChange  :   2013-5-19 22:40
*   History     :	
*
*   Description :   扩展元对象的XML解析辅助类
*
********************************************************************/ 
#ifndef __EXTOBJPARSERIMPL_H__
#define __EXTOBJPARSERIMPL_H__

#include <XLUE.h>
#include <assert.h>

namespace Xunlei
{
namespace Bolt
{

template<typename ObjectClass>
class ExtObjParserImpl
{
public:

	typedef ExtObjParserImpl this_class;
	typedef ObjectClass obj_class;

public:
	ExtObjParserImpl()
	{

	}

	virtual ~ExtObjParserImpl()
	{

	}

	template<typename FinalClass>
	void FillCStruct(ExtObjParser* lpExtParser)
	{
		assert(lpExtParser);

		typedef FinalClass final_class;

		lpExtParser->size = sizeof(ExtObjParser);
		lpExtParser->userData = this;

		AssignIfOverride(this_class, final_class, ParserAttribute, lpExtParser);
		AssignIfOverride(this_class, final_class, ParserEvent, lpExtParser);
	}

public:

	// 元对象的xml属性列表解析事件
	virtual bool ParserAttribute(obj_class* /*lpObj*/, const char* /*key*/, const char* /*value*/)
	{
		return true;
	}

	// 元对象的xml事件列表解析事件，不响应该事件表示使用引擎内部的默认事件解析策略，扩展对象绝大多数情况下不需要响应该事件！
	// 如果扩展对象需要定制的事件解析方式，那么需要响应该事件并自己管理解析结果
	virtual bool ParserEvent(obj_class* /*lpObj*/, const char* /*eventName*/, XL_LRT_CHUNK_HANDLE /*hCodeChunk*/, XL_LRT_RUNTIME_HANDLE /*hRunTime*/)
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

	static BOOL XLUE_STDCALL ParserAttributeCallBack(void* userData, void* objHandle, const char* key, const char* value)
	{
		obj_class* lpObj = ExtLayoutObjMethodsImpl::ObjectFromExtHandle<obj_class>(objHandle);
		assert(lpObj);

		return ThisFromUserData(userData)->ParserAttribute(lpObj, key, value)? TRUE : FALSE;
	}

	static BOOL XLUE_STDCALL ParserEventCallBack(void* userData, void* objHandle, const char* eventName, XL_LRT_CHUNK_HANDLE hCodeChunk, XL_LRT_RUNTIME_HANDLE hRunTime)
	{
		obj_class* lpObj = ExtLayoutObjMethodsImpl::ObjectFromExtHandle<obj_class>(objHandle);
		assert(lpObj);

		return ThisFromUserData(userData)->ParserEvent(lpObj, eventName, hCodeChunk, hRunTime)? TRUE : FALSE;
	}
};

} // Bolt
} // Xunlei

#endif // __EXTOBJPARSERIMPL_H__