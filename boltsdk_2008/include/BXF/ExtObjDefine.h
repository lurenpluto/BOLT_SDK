/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   ExtObjDefine
*   Author      :   李亚星
*   Create      :   2013-5-21 22:59
*   LastChange  :   2013-5-21 22:59
*   History     :	
*
*   Description :   BOLT元对象扩展框架的相关辅助定义
*
********************************************************************/ 
#ifndef __EXTOBJDEFINE_H__
#define __EXTOBJDEFINE_H__

#define GetFunctionAddress(T, M, var) __asm\
	{\
	__asm mov eax, T::M \
	__asm mov var, eax \
	}

#define IsFunctionOverride(B, D, M, var) \
	for(;;) \
	{ \
		void *bm, *dm; \
		GetFunctionAddress(B, M, bm) \
		GetFunctionAddress(D, M, dm) \
		var = (var || (bm != dm)); \
		break; \
	}

#define AssignIfOverride(B, D, M, dest) \
	for(;;) \
	{ \
		bool ret = false; \
		IsFunctionOverride(B, D, M, ret); \
		if (ret) \
		{ \
			dest->lpfn##M = &M##CallBack; \
		} \
		break; \
	}

#endif // __EXTOBJDEFINE_H__