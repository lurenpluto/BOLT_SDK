/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2009                =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   XLMath.h
*   Author      :   xlue group(xlue@xunlei.com)
*   Create      :   2009-9-3
*   LastChange  :   
*   History     :	
*
*   Description :   图形库的数学相关函数的头文件
*
********************************************************************/ 
#ifndef __XUNLEI_MATH_H__
#define __XUNLEI_MATH_H__

#ifndef XLMATH_EXTERN_C
	#ifdef __cplusplus	
		#define XLMATH_EXTERN_C extern "C"
	#else
		#define XLMATH_EXTERN_C 
	#endif // __cplusplus
#endif //XLMATH_EXTERN_C

#ifndef XLUE_STDCALL
	#if defined(_MSC_VER)
		#define XLUE_STDCALL __stdcall
	#elif defined(__GNUC__)
		#define XLUE_STDCALL __attribute__((__stdcall__))
	#endif
#endif //XLUE_STDCALL

#if defined(_MSC_VER)
	#if defined(XLUE_UNIONLIB)
			#define XLMATH_API(x) XLMATH_EXTERN_C  x __stdcall 
	#elif defined(XLGRAPHIC_EXPORTS)
			#define XLMATH_API(x) XLMATH_EXTERN_C __declspec(dllexport) x __stdcall 
	#elif defined (XLUE_UNION)
			#define XLMATH_API(x) XLMATH_EXTERN_C  x __stdcall 
	#else // XLGRAPHIC_EXPORTS
			#define XLMATH_API(x) XLMATH_EXTERN_C __declspec(dllimport) x __stdcall 
	#endif // XLGRAPHIC_EXPORTS
#elif defined(__GNUC__)
	#if defined(XLUE_UNIONLIB)
			#define XLMATH_API(x) XLMATH_EXTERN_C  __attribute__((__stdcall__)) x
	#elif defined(XLGRAPHIC_EXPORTS)
			#define XLMATH_API(x) XLMATH_EXTERN_C __attribute__((__visibility__("default"), __stdcall__)) x
	#elif defined (XLUE_UNION)
			#define XLMATH_API(x) XLMATH_EXTERN_C  __attribute__((__stdcall__)) x
	#else // XLGRAPHIC_EXPORTS
			#define XLMATH_API(x) XLMATH_EXTERN_C __attribute__((__visibility__("default"), __stdcall__)) x 
	#endif // XLGRAPHIC_EXPORTS
#endif

#if !defined(WIN32) && !defined(XLUE_WIN32)
#include <XLUESysPreDefine.h>
#endif // WIN32 && XLUE_WIN32

#define XL_RECT_NOT_INTERSECT 0
#define XL_RECT_INTERSET      1
#define XL_RECT_INCLUDE_RECT  2
#define XL_RECT_IN_RECT       3
//********************* 矩形相关算法 ****************************
//***矩形相交
XLMATH_API(BOOL) XL_SetRect(LPRECT lprc,int xLeft,int yTop,int xRight,int yBottom);
XLMATH_API(BOOL) XL_OffsetRect(LPRECT lprc,int dx,int dy);
XLMATH_API(BOOL) XL_InflateRect(LPRECT lprc, int cx, int cy);
XLMATH_API(BOOL) XL_SetRectEmpty(LPRECT lprc);
XLMATH_API(BOOL) XL_EqualRect(const RECT* lprc1,const RECT* lprc2);
XLMATH_API(BOOL) XL_CopyRect(LPRECT lprcDst,const RECT* lprsSrc);
XLMATH_API(BOOL) XL_IsRectEmpty(const RECT* lprc);
XLMATH_API(BOOL) XL_PtInRect(const RECT *lprc,POINT pt);
XLMATH_API(BOOL) XL_SubtractRect(LPRECT lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2);

XLMATH_API(long) XL_IsRectIntersect(const RECT* pRect1,const RECT* pRect2);
XLMATH_API(BOOL) XL_IntersectRect(RECT* pResult,const RECT* pSrc1,const RECT* pSrc2);
//对一组矩形求交
XLMATH_API(BOOL) XL_IntersectRectEx(RECT* pResult,const RECT* pRectList,size_t listSize);

//***矩形合并
//得到一个包含pSrc1,pSrc2的最小矩形
XLMATH_API(BOOL) XL_UnionRect(RECT* pResult,const RECT* pSrc1,const RECT* pSrc2);
//将输入矩形集合合并成结果矩形集合,结果矩形集合包含输入矩形集合，并且结果矩形集合里没有相交的矩形 
XLMATH_API(long) XL_UnionRectEx(const RECT* pInputRectList,size_t listSize,RECT* pOutputRectList,size_t outlistSize);


//***********************表达式相关算法************************************

typedef void* XL_EXP_HANDLE;

typedef LPCRECT (*XL_ExpBindProc)(void* lpObject, const char* lpcstrObject);

XLMATH_API(XL_EXP_HANDLE) XL_CreateExp();
XLMATH_API(BOOL) XL_DeleteExp(XL_EXP_HANDLE hExp);
XLMATH_API(XL_EXP_HANDLE) XL_CloneEXP(XL_EXP_HANDLE hSrcExp);

XLMATH_API(BOOL) XL_SetExp(XL_EXP_HANDLE hExp, const char* lpcstrExp, BOOL* lpbPureNum);
XLMATH_API(BOOL) XL_BindExpObject(XL_EXP_HANDLE hExp, void* lpObject, XL_ExpBindProc lpBindProc);
XLMATH_API(BOOL) XL_BindExpRect(XL_EXP_HANDLE hExp, LPCRECT lpRect);

XLMATH_API(BOOL) XL_CaclExp(XL_EXP_HANDLE hExp);
XLMATH_API(long) XL_GetExpValue(XL_EXP_HANDLE hExp);
XLMATH_API(long) XL_GetExpString(XL_EXP_HANDLE hExp, char* lpBuffer, long len);

// 矩形表达式
typedef DWORD* XL_RECTEXP_HANDLE;

XLMATH_API(XL_RECTEXP_HANDLE) XL_CreateRectExp();
XLMATH_API(BOOL) XL_DeleteRectExp(XL_RECTEXP_HANDLE hExp);

XLMATH_API(BOOL) XL_CaclRectExp(XL_RECTEXP_HANDLE hExp);
XLMATH_API(BOOL) XL_BindRectExpRect(XL_RECTEXP_HANDLE hExp, LPCRECT lpRect);

XLMATH_API(long) XL_GetRectExpLeft(XL_RECTEXP_HANDLE hExp);
XLMATH_API(long) XL_GetRectExpTop(XL_RECTEXP_HANDLE hExp);
XLMATH_API(long) XL_GetRectExpWidth(XL_RECTEXP_HANDLE hExp);
XLMATH_API(long) XL_GetRectExpHeight(XL_RECTEXP_HANDLE hExp);
XLMATH_API(BOOL) XL_GetRectExpRect(XL_RECTEXP_HANDLE hExp, LPRECT lpRect);

XLMATH_API(BOOL) XL_SetRectExpLeft(XL_RECTEXP_HANDLE hExp, const char* lpcstrLeft);
XLMATH_API(BOOL) XL_SetRectExpTop(XL_RECTEXP_HANDLE hExp, const char* lpcstrTop);
XLMATH_API(BOOL) XL_SetRectExpWidth(XL_RECTEXP_HANDLE hExp, const char* lpcstrWidth);
XLMATH_API(BOOL) XL_SetRectExpHeight(XL_RECTEXP_HANDLE hExp, const char* lpcstrHeight);

//*********************曲线拟合的相关算法*******************************************************
//曲线
//typedef DWORD* XL_ANICURVE_HANDLE;


//由于时间是递增的，B样条曲线中的部分点就必须舍弃掉，定义了一个坏点值9999
//若返回值为9999，则该点是坏点，应舍弃

#define XLMATH_BSPLINE_ERROR      9999

#define	XLMATH_BEZIER   0//贝塞尔
#define	XLMATH_BSPLINE  1//B样条

typedef struct ParamControlPoint 
{
	float t;
	float s;
}_ControlPoint_;

//lpControl -- 控制点数据
//nSize -- 控制点数量
//type -- 曲线类型
//t -- 时间值（0~1）

XLMATH_API(float) XL_GetCurvePosition(_ControlPoint_* lpControl, int nSize, int type, float t);

#endif // __XUNLEI_MATH_H__

