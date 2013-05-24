/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   ExtImageObjMethodsImpl
*   Author      :   李亚星
*   Create      :   2013-5-22 11:19
*   LastChange  :   2013-5-22 11:19
*   History     :	
*
*   Description :   扩展元对象的ImageObj层事件辅助类
*
********************************************************************/ 
#ifndef __EXTIMAGEOBJMETHODSIMPL_H__
#define __EXTIMAGEOBJMETHODSIMPL_H__

#include "./ExtLayoutObjMethodsImpl.h"

namespace Xunlei
{
namespace Bolt
{

class ExtImageObjMethodsImpl
	: public ExtLayoutObjMethodsImpl
{
public:
	typedef ExtImageObjMethodsImpl this_class;
	typedef ExtLayoutObjMethodsImpl base_class;

public:

	ExtImageObjMethodsImpl(XLUE_LAYOUTOBJ_HANDLE hObj)
		:ExtLayoutObjMethodsImpl(hObj)
	{
	}

	virtual ~ExtImageObjMethodsImpl()
	{
	}

	template<typename FinalClass>
	static void FillCStruct(ExtImageObjMethods* lpExtMethods)
	{
		assert(lpExtMethods);

		lpExtMethods->size = sizeof(ExtImageObjMethods);
		lpExtMethods->userData = NULL;

		base_class::FillCVector<FinalClass>(&lpExtMethods->layoutObjMethodVector);
		FillCVector<FinalClass>(&lpExtMethods->imageObjMethodVector);
	}

	template<typename FinalClass>
	static void FillCVector(ExtImageObjMethodsVector* lpExtMethodsVector)
	{
		assert(lpExtMethodsVector);
		
		typedef FinalClass final_class;
		lpExtMethodsVector->size = sizeof(ExtImageObjMethodsVector);
	}

public:

protected:

	static this_class* ThisFromObjHandle(void* objHandle)
	{
		assert(objHandle);

		base_class* lpBase = base_class::ThisFromObjHandle(objHandle);

		return dynamic_cast<this_class*>(lpBase);
	}

private:

};

} // Bolt
} // Xunlei

#endif // __EXTIMAGEOBJMETHODSIMPL_H__