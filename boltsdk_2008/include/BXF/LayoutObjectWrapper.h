/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   LayoutObjectWrapper
*   Author      :   李亚星
*   Create      :   2013-5-19 0:05
*   LastChange  :   2013-5-19 0:05
*   History     :	
*
*   Description :   BOLT的元对象C++辅助类
*
********************************************************************/ 
#ifndef __LAYOUTOBJECTWRAPPER_H__
#define __LAYOUTOBJECTWRAPPER_H__

#include <XLUE.h>
#include <assert.h>
#include <BXF/ObjectTreeWrapper.h>
#include <BXF/HostWndWrapper.h>

namespace Xunlei
{
namespace Bolt
{

class LayoutObjectWrapper;

// 此类不可直接实例化
class LayoutObjectBaseWrapper
{
public:

	XLUE_LAYOUTOBJ_HANDLE m_hObj;

public:

	bool IsValid() const
	{
		return ((m_hObj !=NULL) && (!!XLUE_IsObjValid(m_hObj)));
	}

	// LayoutObject Functions
	const char* GetClass() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjClassName(m_hObj);
	}

	const char* GetID() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjID(m_hObj);
	}

	XLUE_OBJTREE_HANDLE GetOwner() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjOwner(m_hObj);
	}

	XLUE_HOSTWND_HANDLE GetBindHostWnd() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjTreeBindHostWnd(XLUE_GetObjOwner(m_hObj));
	}

	XLUE_LAYOUTOBJ_HANDLE GetFather() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjFather(m_hObj);
	}

	bool IsChild(XLUE_LAYOUTOBJ_HANDLE hObj) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_IsChild(m_hObj, hObj);
	}

	long AddChild(XLUE_LAYOUTOBJ_HANDLE hObj) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_AddObjChild(m_hObj, hObj);
	}

	long RemoveChild(XLUE_LAYOUTOBJ_HANDLE hObj) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_RemoveObjChild(m_hObj, hObj);
	}

	long RemoveAllChild() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_RemoveObjAllChildren(m_hObj);
	}

	size_t GetChildCount() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjChildCount(m_hObj);
	}

	XLUE_LAYOUTOBJ_HANDLE GetChild(size_t index) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjChild(m_hObj, index);
	}

	XLUE_LAYOUTOBJ_HANDLE GetChildByID(const char* id) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjChildByID(m_hObj, id);
	}

	//通过定位指定来获取指定的object
	XLUE_LAYOUTOBJ_HANDLE GetObject(const char* cmd) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjChildByCmd(m_hObj, cmd);
	}

	XLUE_LAYOUTOBJ_HANDLE GetControlObject(const char* id) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetControlObject(m_hObj, id);
	}

	XLUE_LAYOUTOBJ_HANDLE GetControlObjectEx(const char* id) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetControlObjectEx(m_hObj, id);
	}

	XLUE_LAYOUTOBJ_HANDLE GetOwnerControl() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetOwnerControl(m_hObj);
	}

	bool IsControl() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_IsControl(m_hObj);
	}

	// 位置相关函数
	const RECT* GetPos() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjPos(m_hObj);
	}

	const RECT* GetAbsPos() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjAbsPos(m_hObj);
	}

	long SetPos(const RECT* lpNewPos) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjPos(m_hObj, lpNewPos);
	}

	long SetPos(const char* lpLeft, const char* lpTop, const char* lpWidth, const char* lpHeight) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjPosExp(m_hObj, lpLeft, lpTop, lpWidth, lpHeight);
	}

	long SetPos2(const char* lpLeft, const char* lpTop, const char* lpRight, const char* lpBottom) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjPosExp2(m_hObj, lpLeft, lpTop, lpRight, lpBottom);
	}

	// 光标相关函数
	const char* GetCursorID(long x, long y) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjCursorID(m_hObj, x, y);
	}

	void SetCursorID(const char* id)
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjCursorID(m_hObj, id);
	}

	// 对象的limit属性相关函数
	bool IsLimitChild() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_IsObjLimitChild(m_hObj);
	}

	void SetLimitChild(bool limit)
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjLimitChild(m_hObj, limit? TRUE : FALSE);
	}

	// 对象是否被parent的limitchild属性限制
	bool IsLimit() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_IsObjLimit(m_hObj);
	}

	void SetLimit(bool limit) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjLimit(m_hObj, limit? TRUE : FALSE);
	}

	//得到被限制的矩形,如果没被限制,则返回false
	bool GetLimitRect(RECT* lpLimitRect) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_GetObjLimitRect(m_hObj, lpLimitRect);
	}

	//得到光标限制的区域
	bool GetCaretLimitRect(RECT* lpLimitRect) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_GetObjCaretLimitRect(m_hObj, lpLimitRect);
	}

	// 如果pRect = NULL则表示把整个对象都设置为无效
	// pDirty是元对象坐标系，相对于元对象的左上角
	void PushDirtyRect(const RECT* lpRect) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_ObjPushDirtyRect(m_hObj, lpRect);
	}

	// 对象的visible相关属性
	bool GetVisible() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_GetObjVisible(m_hObj);
	}

	void SetVisible(bool visible, bool isRecursive = false) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjVisible(m_hObj, visible? TRUE : FALSE, isRecursive? TRUE : FALSE);
	}

	bool GetChildrenVisible() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_GetObjChildrenVisible(m_hObj);
	}

	void SetChildrenVisible(bool visible)
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjChildrenVisible(m_hObj, visible? TRUE : FALSE);
	}

	bool GetFatherVisible() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_GetObjFatherVisible(m_hObj);
	}

	bool GetPrivateVisible() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_GetObjPrivateVisible(m_hObj);
	}

	// 元对象的enable属性相关方法
	bool GetEnable() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_GetObjEnable(m_hObj);
	}

	void SetEnable(bool enable, bool isRecursive = false) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjEnableEx(m_hObj, enable? TRUE : FALSE, isRecursive? TRUE : FALSE);
	}

	bool GetChildrenEnable() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_GetObjChildrenEnable(m_hObj);
	}

	void SetChildrenEnable(bool bEnable) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjChildrenEnable(m_hObj, bEnable? TRUE : FALSE);
	}

	bool GetFatherEnable() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_GetObjFatherEnable(m_hObj);
	}

	bool GetPrivateEnable() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_GetObjPrivateEnable(m_hObj);
	}

	// 元对象的zorder属性相关方法
	long GetZorder() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjZorder(m_hObj);
	}

	void SetZorder(long zorder) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjZorder(m_hObj, zorder);
	}

	long GetAbsZorder() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjAbsZorder(m_hObj);
	}


	// 鼠标和键盘输入事件相关函数
	void EnableInputTarget(bool enable) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_EnableObjInputTarget(m_hObj, enable? TRUE : FALSE);
	}

	bool CanHandleInput() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_CanObjHandleInput(m_hObj);
	}

	// 焦点相关函数
	void SetFocus(bool focus) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjFocus(m_hObj, focus? TRUE : FALSE);
	}

	void SetFocusEx(bool isFocus, FocusReason reason) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjFocusEx(m_hObj, isFocus? TRUE : FALSE, reason);
	}

	bool GetFocus() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_GetObjFocus(m_hObj);
	}

	// 焦点策略
	void SetFocusStrategy(FocusStrategy strategy) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjFocusStrategy(m_hObj, strategy);
	}

	FocusStrategy GetFocusStrategy() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjFocusStrategy(m_hObj);
	}

	// 元对象的鼠标捕获状态相关函数
	void SetCaptureMouse(bool isCapture) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjCaptureMouse(m_hObj, isCapture? TRUE : FALSE);
	}

	bool GetCaptureMouse() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_GetObjCaptureMouse(m_hObj);
	}

	// 元对象的taborder属性相关函数
	void SetTabOrder(long newOrder) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjTabOrder(m_hObj, newOrder);
	}

	long GetTabOrder() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjTabOrder(m_hObj);
	}

	void SetTabGroup(long newTabGroup) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjTabGroup(m_hObj, newTabGroup);
	}

	long GetTabGroup() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_GetObjTabGroup(m_hObj);
	}

	void SetTabStop(bool tabStop) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjTabStop(m_hObj, tabStop);
	}

	bool GetTabStop() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_GetObjTabStop(m_hObj);
	}

	unsigned char GetAlpha() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetRenderObjAlpha(m_hObj);
	}

	void SetAlpha(unsigned char alpha, bool isRecursive = false) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetRenderObjAlphaEx(m_hObj, alpha, isRecursive? TRUE : FALSE);
	}

	// 元对象的mask相关函数
	void SetInheritMask(bool isInherit, bool isRecursive = false) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjInheritMask(m_hObj, isInherit? TRUE : FALSE, isRecursive? TRUE : FALSE);
	}

	bool IsInheritMask() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_GetObjInheritMask(m_hObj);
	}

	const RECT* GetMaskPos() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjMaskPos(m_hObj);
	}

	void SetMaskBlendType(unsigned long blendtype) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjMaskBlendType(m_hObj, blendtype);
	}

	unsigned long GetMaskBlendType() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjMaskBlendType(m_hObj);
	}

	// 元对象拖拽相关函数
	void SetDropEnable(bool bEnableDrop) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetObjMaskBlendType(m_hObj, bEnableDrop? TRUE : FALSE);
	}

	bool GetDropEnable() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_GetObjDropEnable(m_hObj);
	}

	// 层渲染相关函数
	bool BindLayer(XLUE_LAYOUTOBJ_HANDLE hLayerObj, bool isRecursive = false) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_ObjBindLayer(m_hObj, hLayerObj, isRecursive? TRUE : FALSE);
	}

	XLUE_LAYOUTOBJ_HANDLE GetLayer() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_ObjGetLayer(m_hObj);
	}

	// 元对象的resprovider相关函数
	XLUE_RESPROVIDER_HANDLE GetResProvider() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetObjResProvider(m_hObj);
	}

	bool SetResProvider(XLUE_RESPROVIDER_HANDLE hResProvider) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_SetObjResProvider(m_hObj, hResProvider);
	}

	// ImageObject Functions
	bool SetImageResID(const char* id) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_SetImageObjResID(m_hObj, id);
	}

	const char* GetImageResID() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetImageObjResID(m_hObj);
	}

	bool SetImageBitmap(XL_BITMAP_HANDLE hBitmap) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return !!XLUE_SetImageObjBitmap(m_hObj, hBitmap);
	}

	XL_BITMAP_HANDLE GetImageBitmap() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetImageObjBitmap(m_hObj);
	}

	// RealObject Functions
	OS_HOSTWND_HANDLE SetRealWindow(OS_HOSTWND_HANDLE hWnd) const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_SetRealObjWindow(m_hObj, hWnd);
	}

	OS_HOSTWND_HANDLE GetRealWindow() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetRealObjWindow(m_hObj);
	}

	OS_HOSTWND_HANDLE GetRealHostWindow() const
	{
		assert(m_hObj);
		assert(XLUE_IsObjValid(m_hObj));

		return XLUE_GetRealObjHostWindow(m_hObj);
	}
};

class LayoutObjectWrapper
	: public LayoutObjectBaseWrapper
{
public:

	LayoutObjectWrapper(XLUE_LAYOUTOBJ_HANDLE hObj = NULL)
	{
		m_hObj = hObj;
	}

	LayoutObjectWrapper& operator=(XLUE_LAYOUTOBJ_HANDLE hObj)
	{
		m_hObj = hObj;

		return *this;
	}

	void Attach(XLUE_LAYOUTOBJ_HANDLE hNewObj)
	{
		assert(m_hObj == NULL);
		assert((hNewObj == NULL) || XLUE_IsObjValid(hNewObj));

		m_hObj = hNewObj;
	}

	XLUE_LAYOUTOBJ_HANDLE Detach()
	{
		XLUE_LAYOUTOBJ_HANDLE hObj = m_hObj;
		m_hObj = NULL;

		return hObj;
	}

	operator XLUE_LAYOUTOBJ_HANDLE() const
	{ 
		return m_hObj; 
	}
};

} // Bolt
} // Xunlei

#endif // __LAYOUTOBJECTWRAPPER_H__