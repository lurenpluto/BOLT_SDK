/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   ExtLayoutObjMethodsImpl
*   Author      :   李亚星
*   Create      :   2013-5-18 23:52
*   LastChange  :   2013-5-18 23:52
*   History     :	
*
*   Description :   扩展元对象的LayoutObj层事件辅助类
*
********************************************************************/ 
#ifndef __EXTLAYOUTOBJMETHODSIMPL_H__
#define __EXTLAYOUTOBJMETHODSIMPL_H__

#include <XLUE.h>
#include <assert.h>
#include "./ExtObjDefine.h"
#include "./LayoutObjectWrapper.h"

namespace Xunlei
{
namespace Bolt
{

class ExtLayoutObjMethodsImpl
	: public LayoutObjectBaseWrapper
{
public:
	typedef ExtLayoutObjMethodsImpl this_class;
	typedef LayoutObjectBaseWrapper base_class;

public:

	ExtLayoutObjMethodsImpl(XLUE_LAYOUTOBJ_HANDLE hObj)
	{
		m_hObj = hObj;
		assert(m_hObj);
	}

	virtual ~ExtLayoutObjMethodsImpl()
	{
		assert(m_hObj);
		//assert(XLUE_IsObjValid(m_hObj));
		m_hObj = NULL;
	}

	// 获取当前扩展对象的外部handle
	void* GetObjectExtHandle()
	{
		return this;
	}

	XLUE_LAYOUTOBJ_HANDLE GetObjectHandle() const
	{
		return m_hObj;
	}

	// 从当前对象的exthandle获取对应的对象类，比如MagicObject
	template<typename TargetClass>
	static TargetClass* ObjectFromExtHandle(void* objHandle)
	{
		this_class* lpBase = (this_class*)objHandle;
		return dynamic_cast<TargetClass*>(lpBase);
	}

	template<typename FinalClass>
	static void FillCStruct(ExtLayoutObjMethods* lpExtMethods)
	{
		assert(lpExtMethods);

		lpExtMethods->size = sizeof(ExtLayoutObjMethods);
		lpExtMethods->userData = NULL;

		FillCVector<FinalClass>(&lpExtMethods->layoutObjMethods);
	}

	template<typename FinalClass>
	static void FillCVector(ExtLayoutObjMethodsVector* lpExtMethodsVector)
	{
		assert(lpExtMethodsVector);
		
		typedef FinalClass final_class;
		lpExtMethodsVector->size = sizeof(ExtLayoutObjMethodsVector);

		AssignIfOverride(this_class, final_class, IsRenderable, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnBind, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnInitControl, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnDestroy, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnHitTest, lpExtMethodsVector);

		AssignIfOverride(this_class, final_class, OnCtrlHitTest, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, GetCursorID, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, GetCaretLimitRect, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnGetWantKey, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnQueryFocus, lpExtMethodsVector);

		AssignIfOverride(this_class, final_class, OnSetFocus, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnSetControlFocus, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnControlMouseEnter, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnControlMouseLeave, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnControlMouseWheel, lpExtMethodsVector);
		
		AssignIfOverride(this_class, final_class, OnStyleUpdate, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnPaint, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnPaintEx, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnPosChanged, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnAbsPosChanged, lpExtMethodsVector);

		AssignIfOverride(this_class, final_class, OnFatherAbsPosChanged, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnVisibleChange, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnEnableChange, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnCaptureChange, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnZOrderChange, lpExtMethodsVector);

		AssignIfOverride(this_class, final_class, OnAlphaChange, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnCursorIDChange, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnResProviderChange, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnBindLayerChange, lpExtMethodsVector);

		bool ret = false;
		IsFunctionOverride(this_class, final_class, OnDragQuery, ret);
		IsFunctionOverride(this_class, final_class, OnDragEnter, ret);
		IsFunctionOverride(this_class, final_class, OnDragOver, ret);
		IsFunctionOverride(this_class, final_class, OnDragLeave, ret);
		IsFunctionOverride(this_class, final_class, OnDrop, ret);
		if (ret)
		{
			lpExtMethodsVector->lpfnOnDragEvent = OnDragEventCallBack;
		}

		AssignIfOverride(this_class, final_class, CanHandleInput, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, PreInputFilter, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, PostInputFilter, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnBindHostWnd, lpExtMethodsVector);
		AssignIfOverride(this_class, final_class, OnCreateHostWnd, lpExtMethodsVector);
	}

public:

	// 元对象是否可渲染
	virtual bool IsRenderable()
	{
		return false;
	}

	// 元对象绑定到对象树上的事件，不推荐使用
	virtual void OnBind()
	{

	}

	// 元对象初始化事件，可以重写做一些初始化工作
	virtual void OnInitControl()
	{

	}

	// 元对象销毁事件，可以重写做一些清理工作
	virtual void OnDestroy()
	{

	}

	// 元对象的命中测试事件，用来确定一个点是否落在对象区域内
	// 默认实现只考虑visible，alpha，pos和limitrect四个因素，重写此方法可以覆盖掉默认实现
	virtual bool OnHitTest(short x, short y)
	{
		return !!XLUE_ObjHitTest(m_hObj, x, y);
	}

	// 元对象的control命中测试，一般只有控制对象需要响应该方法
	virtual bool OnCtrlHitTest(short /*x*/, short /*y*/, CtrlTestType& /*type*/)
	{
		return false;
	}


	virtual const char* GetCursorID(long /*x*/, long /*y*/, unsigned long /*wParam*/, unsigned long /*lParam*/)
	{
		return NULL;
	}

	// 获取当前对象对光标的限制区域，基于对象树坐标系，绝大多数情况下不需要重写该函数
	virtual void GetCaretLimitRect(RECT* lpLimitRect)
	{
		XLUE_GetObjCaretLimitRect(m_hObj, lpLimitRect);
	}

	// 判断对象自身需要处理哪些按键，主要包括导航键和tab键
	virtual int OnGetWantKey()
	{
		return ObjWantKey_None;
	}

	// 判断对象是否需要设置被动焦点,返回0表示不需要，非0表示不需要
	virtual int OnQueryFocus()
	{
		return XLUE_QueryObjFocus(m_hObj);
	}

	// 对象的焦点状况发生改变
	virtual void OnSetFocus(BOOL /*bFocus*/, XLUE_LAYOUTOBJ_HANDLE /*hOppositeObj*/, FocusReason /*reason*/)
	{

	}

	// 对象的控件焦点状态发生改变
	virtual void OnSetControlFocus(BOOL /*bFocus*/,XLUE_LAYOUTOBJ_HANDLE /*hOppositeObj*/, FocusReason /*reason*/)
	{

	}

	// 对象的控件鼠标事件，包括enter/leave/wheel等
	virtual void OnControlMouseEnter(short /*x*/, short /*y*/, unsigned long /*flags*/)
	{

	}

	virtual void OnControlMouseLeave(short /*x*/, short /*y*/, unsigned long /*flags*/)
	{

	}

	virtual void OnControlMouseWheel(short /*x*/, short /*y*/, unsigned long /*flags*/)
	{

	}

	// 对象的style发生变化，只对control有效
	virtual void OnStyleUpdate()
	{

	}

	// 下面是两个核心绘制函数，OnPaint用以不关心mask的绘制；OnPaintEx是关心mask的绘制，使用起来更复杂些
	// 在定义了OnPaintEx的情况下，优先使用该函数

	// 绘制对象的lpSrcClipRect区域到目标位图，目标位图的大小和lpSrcClipRect大小一致
	// lpSrcClipRect是基于元对象坐标系的，也即相对于该对象的左上角位置
	// hBitmapDest有可能Clip出来的子位图，所以内存不一定是连续的！获取某一行的buffer必须使用XL_GetBitmapBuffer
	virtual void OnPaint(XL_BITMAP_HANDLE /*hBitmapDest*/, const RECT* /*lpDestClipRect*/, const RECT* /*lpSrcClipRect*/, unsigned char /*alpha*/)
	{

	}

	virtual void OnPaintEx(XL_BITMAP_HANDLE /*hBitmapDest*/, const RECT* /*lpDestClipRect*/, const RECT* /*lpSrcClipRect*/, unsigned char /*alpha*/, XLGraphicHint* /*lpHint*/)
	{

	}

	// 对象的位置改变事件，基于对象自身坐标系
	virtual void OnPosChanged(const RECT* /*lpOldPos*/, const RECT* /*lpNewPos*/)
	{

	}

	// 对象的绝对位置改变事件，基于对象树坐标系，只有被绑定到UIObjectTree后该事件才会被触发
	virtual void OnAbsPosChanged(const RECT* /*lpOldAbsPos*/, const RECT* /*lpNewAbsPos*/)
	{

	}

	// 对象的直接或者间接父对象的绝对位置发生改变， 如果子对象依赖父对象的limitrect结果，那么需要响应该事件并处理
	virtual void OnFatherAbsPosChanged()
	{

	}

	// 对象的可见状态发生改变
	virtual void OnVisibleChange(BOOL /*bVisible*/)
	{

	}

	// 对象的enable状态发生改变
	virtual void OnEnableChange(BOOL /*bEnable*/)
	{

	}

	// 对象的capture状态发生改变
	virtual void OnCaptureChange(BOOL /*bCapture*/)
	{

	}

	// 对象的zorder发生改变
	virtual void OnZOrderChange()
	{

	}

	// 对象的alpha发生改变
	virtual void OnAlphaChange(unsigned char /*newAlpha*/, unsigned char /*oldAlpha*/)
	{

	}

	// 对象的cursor发生改变
	virtual void OnCursorIDChange()
	{

	}

	// 对象的resprovider发生改变
	virtual void OnResProviderChange(XLUE_RESPROVIDER_HANDLE /*hResProvider*/)
	{

	}
	
	// 对象绑定的layer发生改变
	virtual void OnBindLayerChange(XLUE_LAYOUTOBJ_HANDLE /*hNewLayerObject*/, XLUE_LAYOUTOBJ_HANDLE /*hOldLayerObj*/)
	{

	}

	// 元对象的drop相关事件
	virtual bool OnDragQuery(void* /*pDataObj*/, DWORD /*grfKeyState*/, POINT /*pt*/, unsigned long* /*lpEffect*/)
	{
		return false;
	}

	virtual bool OnDragEnter(void* /*pDataObj*/, DWORD /*grfKeyState*/, POINT /*pt*/, unsigned long* /*lpEffect*/)
	{
		return false;
	}

	virtual bool OnDragOver(void* /*pDataObj*/, DWORD /*grfKeyState*/, POINT /*pt*/, unsigned long* /*lpEffect*/)
	{
		return false;
	}

	virtual bool OnDragLeave()
	{
		return false;
	}

	virtual bool OnDrop(void* /*pDataObj*/, DWORD /*grfKeyState*/, POINT /*pt*/, unsigned long* /*lpEffect*/)
	{
		return false;
	}

	// 对象是否需要鼠标和键盘输入事件，默认只要重写了PreInputFilter和PostInputFilter两个函数就需要
	// 子类如果重写了该方法，切忌再调用基类的CanHandleInput！！
	virtual bool CanHandleInput()
	{
		assert(false);
		return false;
	}

	// 前置事件处理器，用以处理鼠标和键盘事件，在事件路由的最开始调用，handled=true会拦截事件路由的后续处理
	virtual long PreInputFilter(unsigned long /*actionType*/, unsigned long /*wParam*/, unsigned long /*lParam*/, BOOL* lpHandled)
	{
		assert(lpHandled);
		*lpHandled = FALSE;

		return 0;
	}

	// 后置事件处理器，在事件路由的最后实际调用
	virtual long PostInputFilter(unsigned long /*actionType*/, unsigned long /*wParam*/, unsigned long /*lParam*/, BOOL* lpHandled)
	{
		assert(lpHandled);
		*lpHandled = FALSE;

		return 0;
	}

	// 元对象所在的对象树绑定到hostwnd和从hostwnd解绑的事件
	virtual void OnBindHostWnd(XLUE_OBJTREE_HANDLE /*hTree*/, XLUE_HOSTWND_HANDLE /*hHostWnd*/, BOOL /*bBind*/)
	{

	}

	// 元对象所在的对象树的hostwnd的创建和销毁事件
	virtual void OnCreateHostWnd(XLUE_OBJTREE_HANDLE /*hTree*/, XLUE_HOSTWND_HANDLE /*hHostWnd*/, BOOL /*bCreate*/)
	{

	}

protected:

	static this_class* ThisFromObjHandle(void* objHandle)
	{
		assert(objHandle);

		return (this_class*)objHandle;
	}

private:

	// 在此封装体系下，使用实例相关的objHandle来代替userData
	static BOOL XLUE_STDCALL IsRenderableCallBack(void* /*userData*/, void* objHandle)
	{
		return ThisFromObjHandle(objHandle)->IsRenderable()? TRUE : FALSE;
	}

	static void XLUE_STDCALL OnBindCallBack(void* /*userData*/, void* objHandle)
	{
		return ThisFromObjHandle(objHandle)->OnBind();
	}

	static void XLUE_STDCALL OnInitControlCallBack(void* /*userData*/, void* objHandle)
	{
		return ThisFromObjHandle(objHandle)->OnInitControl();
	}

	static void XLUE_STDCALL OnDestroyCallBack(void* /*userData*/, void* objHandle)
	{
		return ThisFromObjHandle(objHandle)->OnDestroy();
	}

	static BOOL XLUE_STDCALL OnHitTestCallBack(void* /*userData*/, void* objHandle, short x, short y)
	{
		return ThisFromObjHandle(objHandle)->OnHitTest(x, y)? TRUE : FALSE;
	}

	static BOOL XLUE_STDCALL OnCtrlHitTestCallBack(void* /*userData*/, void* objHandle, short x, short y, CtrlTestType& type)
	{
		return ThisFromObjHandle(objHandle)->OnCtrlHitTest(x, y, type)? TRUE : FALSE;
	}

	static const char* XLUE_STDCALL GetCursorIDCallBack(void* /*userData*/, void* objHandle, long x, long y, unsigned long wParam, unsigned long lParam)
	{
		return ThisFromObjHandle(objHandle)->GetCursorID(x, y, wParam, lParam);
	}

	static void XLUE_STDCALL GetCaretLimitRectCallBack(void* /*userData*/, void* objHandle, RECT* lpLimitRect)
	{
		return ThisFromObjHandle(objHandle)->GetCaretLimitRect(lpLimitRect);
	}

	static int XLUE_STDCALL OnGetWantKeyCallBack(void* /*userData*/, void* objHandle)
	{
		return ThisFromObjHandle(objHandle)->OnGetWantKey();
	}

	static int XLUE_STDCALL OnQueryFocusCallBack(void* /*userData*/, void* objHandle)
	{
		return ThisFromObjHandle(objHandle)->OnQueryFocus();
	}

	static void XLUE_STDCALL OnSetFocusCallBack(void* /*userData*/, void* objHandle, BOOL bFocus, XLUE_LAYOUTOBJ_HANDLE hOppositeObj, FocusReason reason)
	{
		return ThisFromObjHandle(objHandle)->OnSetFocus(bFocus, hOppositeObj, reason);
	}

	static void XLUE_STDCALL OnSetControlFocusCallBack(void* /*userData*/, void* objHandle, BOOL bFocus,XLUE_LAYOUTOBJ_HANDLE hOppositeObj, FocusReason reason)
	{
		return ThisFromObjHandle(objHandle)->OnSetControlFocus(bFocus, hOppositeObj, reason);
	}

	static void XLUE_STDCALL OnControlMouseEnterCallBack(void* /*userData*/, void* objHandle, short x, short y, unsigned long flags)
	{
		return ThisFromObjHandle(objHandle)->OnControlMouseEnter(x, y, flags);
	}

	static void XLUE_STDCALL OnControlMouseLeaveCallBack(void* /*userData*/, void* objHandle, short x, short y, unsigned long flags)
	{
		return ThisFromObjHandle(objHandle)->OnControlMouseLeave(x, y, flags);
	}

	static void XLUE_STDCALL OnControlMouseWheelCallBack(void* /*userData*/, void* objHandle, short x, short y, unsigned long flags)
	{
		return ThisFromObjHandle(objHandle)->OnControlMouseWheel(x, y, flags);
	}

	// 对象的style发生变化，只对control有效
	static void XLUE_STDCALL OnStyleUpdateCallBack(void* /*userData*/, void* objHandle)
	{
		return ThisFromObjHandle(objHandle)->OnStyleUpdate();
	}

	static void XLUE_STDCALL OnPaintCallBack(void* /*userData*/, void* objHandle, XL_BITMAP_HANDLE hBitmapDest, const RECT* lpDestClipRect, const RECT* lpSrcClipRect, unsigned char alpha)
	{
		return ThisFromObjHandle(objHandle)->OnPaint(hBitmapDest, lpDestClipRect, lpSrcClipRect, alpha);
	}

	static void XLUE_STDCALL OnPaintExCallBack(void* /*userData*/, void* objHandle, XL_BITMAP_HANDLE hBitmapDest, const RECT* lpDestClipRect, const RECT* lpSrcClipRect, unsigned char alpha, XLGraphicHint* lpHint)
	{
		return ThisFromObjHandle(objHandle)->OnPaintEx(hBitmapDest, lpDestClipRect, lpSrcClipRect, alpha, lpHint);
	}

	static void XLUE_STDCALL OnPosChangedCallBack(void* /*userData*/, void* objHandle, const RECT* lpOldPos, const RECT* lpNewPos)
	{
		return ThisFromObjHandle(objHandle)->OnPosChanged(lpOldPos, lpNewPos);
	}

	static void XLUE_STDCALL OnAbsPosChangedCallBack(void* /*userData*/, void* objHandle, const RECT* lpOldAbsPos, const RECT* lpNewAbsPos)
	{
		return ThisFromObjHandle(objHandle)->OnAbsPosChanged(lpOldAbsPos, lpNewAbsPos);
	}

	static void XLUE_STDCALL OnFatherAbsPosChangedCallBack(void* /*userData*/, void* objHandle)
	{
		return ThisFromObjHandle(objHandle)->OnFatherAbsPosChanged();
	}

	static void XLUE_STDCALL OnVisibleChangeCallBack(void* /*userData*/, void* objHandle, BOOL bVisible)
	{
		return ThisFromObjHandle(objHandle)->OnVisibleChange(bVisible);
	}

	static void XLUE_STDCALL OnEnableChangeCallBack(void* /*userData*/, void* objHandle, BOOL bEnable)
	{
		return ThisFromObjHandle(objHandle)->OnEnableChange(bEnable);
	}

	static void XLUE_STDCALL OnCaptureChangeCallBack(void* /*userData*/, void* objHandle, BOOL bCapture)
	{
		return ThisFromObjHandle(objHandle)->OnCaptureChange(bCapture);
	}	

	static void XLUE_STDCALL OnZOrderChangeCallBack(void* /*userData*/, void* objHandle)
	{
		return ThisFromObjHandle(objHandle)->OnZOrderChange();
	}

	static void XLUE_STDCALL OnAlphaChangeCallBack(void* /*userData*/, void* objHandle, unsigned char newAlpha, unsigned char oldAlpha)
	{
		return ThisFromObjHandle(objHandle)->OnAlphaChange(newAlpha, oldAlpha);
	}

	static void XLUE_STDCALL OnCursorIDChangeCallBack(void* /*userData*/, void* objHandle)
	{
		return ThisFromObjHandle(objHandle)->OnCursorIDChange();
	}

	static void XLUE_STDCALL OnResProviderChangeCallBack(void* /*userData*/, void* objHandle, XLUE_RESPROVIDER_HANDLE hResProvider)
	{
		return ThisFromObjHandle(objHandle)->OnResProviderChange(hResProvider);
	}

	static void XLUE_STDCALL OnBindLayerChangeCallBack(void* /*userData*/, void* objHandle, XLUE_LAYOUTOBJ_HANDLE hNewLayerObject, XLUE_LAYOUTOBJ_HANDLE hOldLayerObject)
	{
		return ThisFromObjHandle(objHandle)->OnBindLayerChange(hNewLayerObject, hOldLayerObject);
	}

	static BOOL XLUE_STDCALL OnDragEventCallBack(void* /*userData*/, void* objHandle, DragEventType type, void* lpDragDataObj, unsigned long grfKeyState, POINT pt, unsigned long* lpEffect)
	{
		this_class* lpThis = ThisFromObjHandle(objHandle);
		bool ret = false;

		if (type == DragEventType_query)
		{
			ret = lpThis->OnDragQuery(lpDragDataObj, grfKeyState, pt, lpEffect);
		}
		else if (type == DragEventType_enter)
		{
			ret = lpThis->OnDragEnter(lpDragDataObj, grfKeyState, pt, lpEffect);
		}
		else if (type == DragEventType_over)
		{
			ret = lpThis->OnDragOver(lpDragDataObj, grfKeyState, pt, lpEffect);
		}
		else if (type == DragEventType_leave)
		{
			ret = lpThis->OnDragLeave();
		}
		else if (type == DragEventType_drop)
		{
			ret = lpThis->OnDrop(lpDragDataObj, grfKeyState, pt, lpEffect);
		}
		else
		{
			assert(false);
		}

		return (ret? TRUE : FALSE);
	}

	static BOOL XLUE_STDCALL CanHandleInputCallBack(void* /*userData*/, void* objHandle)
	{
		return ThisFromObjHandle(objHandle)->CanHandleInput()? TRUE : FALSE;
	}

	static long XLUE_STDCALL PreInputFilterCallBack(void* /*userData*/, void* objHandle, unsigned long actionType, unsigned long wParam, unsigned long lParam, BOOL* lpHandled)
	{
		return ThisFromObjHandle(objHandle)->PreInputFilter(actionType, wParam, lParam, lpHandled);
	}

	static long XLUE_STDCALL PostInputFilterCallBack(void* /*userData*/, void* objHandle, unsigned long actionType, unsigned long wParam, unsigned long lParam, BOOL* lpHandled)
	{
		return ThisFromObjHandle(objHandle)->PostInputFilter(actionType, wParam, lParam, lpHandled);
	}

	
	static void XLUE_STDCALL OnBindHostWndCallBack(void* /*userData*/, void* objHandle, XLUE_OBJTREE_HANDLE hTree, XLUE_HOSTWND_HANDLE hHostWnd, BOOL bBind)
	{
		return ThisFromObjHandle(objHandle)->OnBindHostWnd(hTree, hHostWnd, bBind);
	}

	static void XLUE_STDCALL OnCreateHostWndCallBack(void* /*userData*/, void* objHandle, XLUE_OBJTREE_HANDLE hTree, XLUE_HOSTWND_HANDLE hHostWnd, BOOL bCreate)
	{
		return ThisFromObjHandle(objHandle)->OnCreateHostWnd(hTree, hHostWnd, bCreate);
	}
};

} // Bolt
} // Xunlei

#endif // __EXTLAYOUTOBJMETHODSIMPL_H__