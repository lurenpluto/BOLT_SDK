/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   RectHelper
*   Author      :   李亚星
*   Create      :   2013-5-26 9:16
*   LastChange  :   2013-5-26 9:16
*   History     :	
*
*   Description :   BOLT的矩形计算辅助函数
*
********************************************************************/ 
#ifndef __RECTHELPER_H__
#define __RECTHELPER_H__

#include <XLMath.h>

namespace Xunlei
{
namespace Bolt
{

class RectHelper
{
public:

	static bool SetRect(LPRECT lprc, int left, int top, int right, int bottom)
	{
		return !!XL_SetRect(lprc, left, top, right, bottom);
	}

	static bool IsRectEmpty(const RECT *lprc)
	{
		return !!XL_IsRectEmpty(lprc);
	}

	static bool EqualRect(const RECT *lprc1, const RECT *lprc2)
	{
		return !!XL_EqualRect(lprc1, lprc2);
	}

	static bool PtInRect(const RECT *lprc, POINT  pt)
	{
		return !!XL_PtInRect(lprc, pt);
	}

	static bool OffsetRect(LPRECT lprc, int cx, int cy)
	{
		return !!XL_OffsetRect(lprc, cx, cy);
	}

	static bool InflateRect(LPRECT lprc, int cx, int cy)
	{
		return !!XL_InflateRect(lprc, cx, cy);
	}

	static bool IntersectRect(LPRECT lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2)
	{
		return !!XL_IntersectRect(lprcDst, lprcSrc1, lprcSrc2);
	}

	static bool CopyRect(LPRECT lprcDst, const RECT *lprcSrc)
	{
		return !!XL_CopyRect(lprcDst, lprcSrc);
	}

	static bool SetRectEmpty(LPRECT lprc)
	{
		return !!XL_SetRectEmpty(lprc);
	}

	static bool SubtractRect(LPRECT lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2)
	{
		return !!XL_SubtractRect(lprcDst, lprcSrc1, lprcSrc2);
	}

	static bool UnionRect(LPRECT lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2)
	{
		return !!XL_UnionRect(lprcDst, lprcSrc1, lprcSrc2);
	}

	static bool IsRectIntersect(const RECT *lprcSrc1, const RECT *lprcSrc2)
	{
		return !!XL_IsRectIntersect(lprcSrc1, lprcSrc2);
	}
};

} // Bolt
} // Xunlei

#endif // __RECTHELPER_H__