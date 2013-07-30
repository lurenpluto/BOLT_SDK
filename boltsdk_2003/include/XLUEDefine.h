/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   XLUEDefine.h
*   Author      :   xlue group(xlue@xunlei.com)
*   Create      :   2013-5-16
*   LastChange  :   2013-7-23
*   History     :	
*
*   Description :   XLUE主模块的相关数据结构定义
*
********************************************************************/ 
#ifndef __XLUEDEFINE_H__
#define __XLUEDEFINE_H__

#include "./XLUEHandle.h"
#include <XLLuaRuntime.h>
#include <XLGraphic.h>

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

typedef void* OS_HOSTWND_HANDLE;
typedef void* XLUE_XML_HANDLE;

// 鼠标消息定义
#define XLUE_ACTION_LMOUSE_DOWN      0x10
#define XLUE_ACTION_LMOUSE_UP        0x11
#define XLUE_ACTION_LMOUSE_DBCLICK   0x12
#define XLUE_ACTION_RMOUSE_DOWN      0x13
#define XLUE_ACTION_RMOUSE_UP        0x14
#define XLUE_ACTION_RMOUSE_DBCLICK   0x15
#define XLUE_ACTION_MMOUSE_DOWN      0x16
#define XLUE_ACTION_MMOUSE_UP        0x17
#define XLUE_ACTION_MMOUSE_DBCLICK   0x18

#define XLUE_ACTION_MOUSE_MOVE       0x20
#define XLUE_ACTION_MOUSE_HOVER      0x21
#define XLUE_ACTION_MOUSE_LEAVE      0x22
#define XLUE_ACTION_MOUSE_WHEEL      0x23
#define XLUE_ACTION_MOUSE_ENTER      0x24

#define XLUE_ACTION_MOUSEFIRST       XLUE_ACTION_LMOUSE_DOWN
#define XLUE_ACTION_MOUSELAST        XLUE_ACTION_MOUSE_ENTER

// 键盘消息定义
#define XLUE_ACTION_KEY_DOWN         0x30
#define XLUE_ACTION_KEY_UP           0x31
#define XLUE_ACTION_CHAR             0x32
#define XLUE_ACTION_SYS_INPUTLANGCHANGE		 0x33
#define XLUE_ACTION_SYS_INPUTLANGCHANGEREQUEST  0x34
#define XLUE_ACTION_SYSKEY_DOWN      0x35
#define XLUE_ACTION_SYSKEY_UP		 0x36
#define XLUE_ACTION_SYSCHAR			 0x37
#define XLUE_ACTION_HOTKEY           0x40

#define XLUE_ACTION_KEYFIRST       XLUE_ACTION_KEY_DOWN
#define XLUE_ACTION_KEYLAST        XLUE_ACTION_HOTKEY

#define XLUE_IME_STARTCOMPOSITION	0x50
#define XLUE_IME_ENDCOMPOSITION		0x51
#define XLUE_IME_COMPOSITION		0x52

#define XLUE_IME_KEYFIRST			XLUE_IME_STARTCOMPOSITION
#define XLUE_IME_KEYLAST			0x5F


//关于鼠标消息的WPARAM
#define XLUE_MK_LBUTTON          0x0001
#define XLUE_MK_RBUTTON          0x0002
#define XLUE_MK_SHIFT            0x0004
#define XLUE_MK_CONTROL          0x0008
#define XLUE_MK_MBUTTON          0x0010

typedef enum __CtrlTestType
{
	CtrlTestType_none = 0,
	CtrlTestType_top = 1,
	CtrlTestType_bottom = 2,
	CtrlTestType_left = 3,
	CtrlTestType_right = 4,
	CtrlTestType_topleft = 5,
	CtrlTestType_topright = 6,
	CtrlTestType_bottomleft = 7,
	CtrlTestType_bottomright = 8,
	CtrlTestType_caption = 9

}CtrlTestType;

typedef enum __ObjWantKey
{
	ObjWantKey_None  = 0x00,		// 不需要任何键
	ObjWantKey_Tab	 = 0x01,		// 需要处理tab键
	ObjWantKey_Arrow = 0x02			// 需要处理四向导航键
}ObjWantKey;

// 元对象焦点策略
typedef enum __FocusStrategy
{
	FocusStrategy_fuzzy = 0,	// 默认策略，不去主动判断对象需不需要焦点，只要按键点击就会被设置焦点，包括由于重定向收到的，做模糊处理
	FocusStrategy_auto = 1		// 智能焦点策略，如果对象没有挂接键盘事件，或者不是edit、flash等元对象，那么点击后，默认不设置焦点
}FocusStrategy;

// 元对象焦点原因
typedef enum __FocusReason
{
	FocusReason_system = 1,		// 系统内部触发的焦点改变
	FocusReason_user = 2,		// 用户显示调用SetFocus导致的焦点改变
	FocusReason_tab = 3			// 用户使用tab和方向键导航导致的焦点改变
}FocusReason;

//center mode
typedef enum tagAngleChangeAniCentreMode
{	
	LeftTopCoordinate = 0,//以左上为0,0的绝对坐标
	WidthHeightRate = 1,//以与长宽的比例表示中心

}AngleChangeAniCentreMode;

//position mode
typedef enum tagAngleChangeAniPositionMode
{
	CentreStay = 0,

}AngleChangeAniPositionMode;

//size limit mode
typedef enum tagAngleChangeAniSizeLimitMode
{
	NoLimit = 0,
	SourceObjectSize = 1,

}AngleChangeAniSizeLimitMode;

//turn object flag
typedef enum tagTurnObjectFlag
{
	RoundX = 0,
	RoundY = 1,

	Deasil = 10,
	Widdershins = 11,

	EffectNormal = 21,
	EffectBlur = 22,

}TurnObjectFlag;

typedef enum tagCmdShow
{
	CS_SW_HIDE = 0,
	CS_SW_SHOWNORMAL = 1,
	CS_SW_SHOWMINIMIZED = 2,
	CS_SW_SHOWMAXIMIZED = 3,
	CS_SW_SHOWNOACTIVATE = 4,
	CS_SW_SHOW = 5,
	CS_SW_MINIMIZE = 6,
	CS_SW_SHOWMINNOACTIVE = 7,
	CS_SW_SHOWNA = 8,
	CS_SW_RESTORE = 9,
	CS_SW_SHOWDEFAULT = 10,
	CS_SW_FORCEMINIMIZE = 11
}CMDSHOW, *LPCMDSHOW;

// 扩展元对象的类型
typedef enum __ExtObjType
{
	ExtObjType_layoutObj = 0,		// 扩展目标对象为LayoutObject，默认为不可渲染对象
	ExtObjType_renderableObj = 1,	// 扩展目标对象为LayoutObject，默认可渲染
	ExtObjType_imageObj= 2,			// 扩展目标对象为ImageObject
	ExtObjType_realObj = 3			// 扩展对象为实窗口对象，比如WebBrowseObject

}ExtObjType;

// 对象上的拖放事件类型
typedef enum __DragEventType
{
	DragEventType_query = 0,
	DragEventType_enter = 1,
	DragEventType_over = 2,
	DragEventType_leave = 3,
	DragEventType_drop = 4

}DragEventType;

/*------------------ 扩展模块的相关接口定义 -----------------*/

// 所有引擎DLL扩展模块都需要实现的两个导出接口，分别用以初始化和反初始化操作
// 返回TRUE表示初始化成功，FALSE表示失败
// 导出的函数名称定义应该如下:
// BOOL XLUE_STDCALL XLUE_InitExtModule();
// BOOL XLUE_STDCALL XLUE_UninitExtModule();
typedef BOOL (XLUE_STDCALL *LPFNEXTMODULEINITFUNC)();
typedef BOOL (XLUE_STDCALL *LPFNEXTMODULEUNINITFUNC)();

/*------------------ 扩展类型的预注册相关定义 -----------------*/

// 扩展类型的预注册种类
typedef enum __ExtPreRegisterType
{
	ExtPreRegisterType_module = 0,		// dll+导出接口的形式
	ExtPreRegisterType_callBack = 1		// 回调函数的形式

}ExtPreRegisterType;

// 引擎可扩展内部类型的类别定义
#define XLUE_EXTCATEGORY_OBJ		"extobject"
#define XLUE_EXTCATEGORY_RES		"extresource"
#define XLUE_EXTCATEGORY_HOSTWND	"exthostwnd"

// 外部各种扩展需要实现的导出接口或者回调，当真正需要创建该扩展类型时候，会调用次接口/回调
// 外部扩展模块可以在这个函数里面真正注册扩展类型
// lpExtCategory是扩展类别
// lpExtType对应具体的扩展类型
typedef BOOL (XLUE_STDCALL *LPFNREGISTEREXTTYPECALLBACK)(const char* lpExtCategory, const char* lpExtType);

// 扩展类型的预注册结构体，用以实现按需加载和初始化指定的扩展类型
typedef struct __ExtPreRegisterInfo
{
	// 结构体的大小
	size_t size;

	// 预注册类别
	ExtPreRegisterType type;

	// 扩展类型所属的类别，可取值XLUE_EXTCATEGORY_XXX
	const char* categroyName;

	// 对应扩展类型的类型名称，不能和内置类型名称重复
	const char* typeName;

	// module模式下的参数
	// 初始化该扩展类型时候，会先加载lpModulePath指定的文件，然后调用该module上的lpInitFuncName导出函数进行初始化
	// lpInitFuncName导出函数的签名式必须和LPFNREGISTEREXTTYPECALLBACK一致; 返回TRUE标识该元对象真正注册成功
	const wchar_t* lpModulePath;
	const char* lpInitFuncName;

	// callback模式下的参数
	// 初始化该扩展类型时候，会先调用该函数，返回TRUE标识该元对象真正注册成功
	LPFNREGISTEREXTTYPECALLBACK lpRegisterCallBack;

}ExtPreRegisterInfo;

/*------------------ 扩展元对象的相关定义 -----------------------------*/

// 扩展元对象的属性
typedef enum __ExtObjAttribute
{
	ExtObjAttribute_clipsens = 0x01,

}ExtObjAttribute;

// 扩展元对象的创建和销毁管理回调
typedef struct __ExtObjCreator
{
	// 结构体的大小
	size_t size;

	// 用户自定义数据，作为回调函数第一个参数
	void* userData;

	// 创建扩展元对象的外部实例，如果成功则返回一个handle用来唯一标识
	void* (XLUE_STDCALL *lpfnCreateObj)(void* userData, const char* lpObjClass, XLUE_LAYOUTOBJ_HANDLE hObj);

	// 销毁扩展元对象的外部实例，objHandle为创建时候返回的handle
	void (XLUE_STDCALL *lpfnDestroyObj)(void* userData, void* objHandle);

}ExtObjCreator;

// 根对象LayoutObject的相关方法定义
typedef struct __ExtLayoutObjMethodsVector
{
	// 结构体的大小
	size_t size;

	// 该对象是否可扩展，如果该字段为空，那么ExternalObjType=ExternalObjType_renderable时候才认为该对象可渲染
	// 否则根据该回调函数的返回值来决定是否可以渲染
	// 对于一个可渲染对象，需要绘制时候会调用OnPaint/OnPaintEx回调
	BOOL (XLUE_STDCALL *lpfnIsRenderable)(void* userData, void* objHandle);

	// 元对象的OnBind事件
	void (XLUE_STDCALL *lpfnOnBind)(void* userData, void* objHandle);

	// 元对象的OnInitControl事件
	void (XLUE_STDCALL *lpfnOnInitControl)(void* userData, void* objHandle);

	// 元对象的OnDestroy事件
	void (XLUE_STDCALL *lpfnOnDestroy)(void* userData, void* objHandle);

	// 对象的命中测试，当对象有区别于默认的命中测试操作时候才需要使用
	BOOL (XLUE_STDCALL *lpfnOnHitTest)(void* userData, void* objHandle, short x, short y);

	// 对象的ctrl测试事件，一般只有控制类对象才需要
	BOOL (XLUE_STDCALL *lpfnOnCtrlHitTest)(void* userData, void* objHandle, short x, short y, CtrlTestType& type);

	// 获取当前对象指定位置的cursorid，当对象需要精确区分内部cursor时候才需要使用
	const char* (XLUE_STDCALL *lpfnGetCursorID)(void* userData, void* objHandle, long x, long y, unsigned long wParam, unsigned long lParam);

	// 获取当前对象对光标的限制区域，基于对象树坐标系
	void (XLUE_STDCALL *lpfnGetCaretLimitRect)(void* userData, void* objHandle, RECT* lpLimitRect);

	// 判断对象自身需要处理哪些按键，主要包括导航键和tab键
	// 默认不接收tab和导航键
	int (XLUE_STDCALL *lpfnOnGetWantKey)(void* userData, void* objHandle);

	// 判断对象是否需要设置被动焦点,返回0表示不需要，非0表示需要
	// 默认取决于对象的焦点策略，模糊焦点策略下只要挂接任意一个键盘和鼠标事件变可以接收焦点
	// 智能焦点策略下，只有挂接了配置了taborder或键盘事件或键盘事件重定向，才可以接收焦点
	int (XLUE_STDCALL *lpfnOnQueryFocus)(void* userData, void* objHandle);

	// 对象的焦点状况发生改变
	void (XLUE_STDCALL *lpfnOnSetFocus)(void* userData, void* objHandle, BOOL bFocus, XLUE_LAYOUTOBJ_HANDLE hOppositeObj, FocusReason reason);

	// 对象的控件焦点状态发生改变
	void (XLUE_STDCALL *lpfnOnSetControlFocus)(void* userData, void* objHandle, BOOL bFocus,XLUE_LAYOUTOBJ_HANDLE hOppositeObj, FocusReason reason);

	// 对象的控件鼠标事件，包括enter/leave/wheel等
	void (XLUE_STDCALL *lpfnOnControlMouseEnter)(void* userData, void* objHandle, short x, short y, unsigned long flags);
	void (XLUE_STDCALL *lpfnOnControlMouseLeave)(void* userData, void* objHandle, short x, short y, unsigned long flags);
	void (XLUE_STDCALL *lpfnOnControlMouseWheel)(void* userData, void* objHandle, short x, short y, unsigned long flags);

	// 对象的style发生变化，只对control有效
	void (XLUE_STDCALL *lpfnOnStyleUpdate)(void* userData, void* objHandle);

	// 下面是两个核心绘制函数，OnPaint用以不关心mask的绘制；OnPaintEx是关心mask的绘制，使用起来更复杂些
	// 在定义了OnPaintEx的情况下，优先使用该函数

	// 绘制对象的lpSrcClipRect区域到目标位图的lpDestClipRect，lpDestClipRect大小和lpSrcClipRect大小一致
	// lpDestClipRect是基于目标位图坐标系的，也即相对于目标位图的(0,0)位置
	// lpSrcClipRect是基于元对象坐标系的，也即相对于该对象的左上角位置
	// hBitmapDest有可能Clip出来的子位图，所以内存不一定是连续的！获取某一行的buffer必须使用XL_GetBitmapBuffer
	void (XLUE_STDCALL *lpfnOnPaint)(void* userData, void* objHandle, XL_BITMAP_HANDLE hBitmapDest, const RECT* lpDestClipRect, const RECT* lpSrcClipRect, unsigned char alpha);
	void (XLUE_STDCALL *lpfnOnPaintEx)(void* userData, void* objHandle, XL_BITMAP_HANDLE hBitmapDest, const RECT* lpDestClipRect, const RECT* lpSrcClipRect, unsigned char alpha, XLGraphicHint* lpHint);


	// 对象的位置改变事件，基于对象自身坐标系
	void (XLUE_STDCALL *lpfnOnPosChanged)(void* userData, void* objHandle, const RECT* lpOldPos, const RECT* lpNewPos);

	// 对象的绝对位置改变事件，基于对象树坐标系，只有被绑定到UIObjectTree后该事件才会被触发
	void (XLUE_STDCALL *lpfnOnAbsPosChanged)(void* userData, void* objHandle, const RECT* lpOldAbsPos, const RECT* lpNewAbsPos);

	// 对象的直接或者间接父对象的绝对位置发生改变， 如果子对象依赖父对象的limitrect结果，那么需要响应该事件并处理
	void (XLUE_STDCALL *lpfnOnFatherAbsPosChanged)(void* userData, void* objHandle);

	// 对象的可见状态发生改变
	void (XLUE_STDCALL *lpfnOnVisibleChange)(void* userData, void* objHandle, BOOL bVisible);

	// 对象的enable状态发生改变
	void (XLUE_STDCALL *lpfnOnEnableChange)(void* userData, void* objHandle, BOOL bEnable);

	// 对象的capture状态发生改变
	void (XLUE_STDCALL *lpfnOnCaptureChange)(void* userData, void* objHandle, BOOL bCapture);
	
	// 对象的zorder发生改变
	void (XLUE_STDCALL *lpfnOnZOrderChange)(void* userData, void* objHandle);

	// 对象的alpha发生改变
	void (XLUE_STDCALL *lpfnOnAlphaChange)(void* userData, void* objHandle, unsigned char newAlpha, unsigned char oldAlpha);

	// 对象的cursor发生改变
	void (XLUE_STDCALL *lpfnOnCursorIDChange)(void* userData, void* objHandle);

	// 对象的resprovider发生改变
	void (XLUE_STDCALL* lpfnOnResProviderChange)(void* userData, void* objHandle, XLUE_RESPROVIDER_HANDLE hResProvider);

	// 对象绑定的layer发生改变
	void (XLUE_STDCALL* lpfnOnBindLayerChange)(void* userData, void* objHandle, XLUE_LAYOUTOBJ_HANDLE hNewLayerObject, XLUE_LAYOUTOBJ_HANDLE hOldLayerObject);

	// 对象拖放事件
	// type = DragEventType_leave时候，lpDragDataObj、grfKeyState和pt三个参数无效
	// 返回TRUE表示已经被处理，否则会转默认处理
	BOOL (XLUE_STDCALL* lpfnOnDragEvent)(void* userData, void* objHandle, DragEventType type, void* lpDragDataObj, unsigned long grfKeyState, POINT pt, unsigned long* lpEffect);

	// 对象是否需要鼠标和键盘输入事件，默认只要lpfnPreInputFilter和lpfnPostInputFilter两个字段不为空就需要
	BOOL (XLUE_STDCALL* lpfnCanHandleInput)(void* userData, void* objHandle);

	// 前置事件处理器，用以处理鼠标和键盘事件，在事件路由的最开始调用，handled=true会拦截事件路由的后续处理
	long (XLUE_STDCALL *lpfnPreInputFilter)(void* userData, void* objHandle, unsigned long actionType, unsigned long wParam, unsigned long lParam, BOOL* lpHandled);
	
	// 后置事件处理器，在事件路由的最后实际调用
	long (XLUE_STDCALL *lpfnPostInputFilter)(void* userData, void* objHandle, unsigned long actionType, unsigned long wParam, unsigned long lParam, BOOL* lpHandled);

	// 元对象所在的对象树绑定到hostwnd和从hostwnd解绑的事件
	void (XLUE_STDCALL *lpfnOnBindHostWnd)(void* userData, void* objHandle,XLUE_OBJTREE_HANDLE hTree, XLUE_HOSTWND_HANDLE hHostWnd, BOOL bBind);

	// 元对象所在的对象树的hostwnd的创建和销毁事件
	void (XLUE_STDCALL *lpfnOnCreateHostWnd)(void* userData, void* objHandle, XLUE_OBJTREE_HANDLE hTree, XLUE_HOSTWND_HANDLE hHostWnd, BOOL bCreate);
	
}ExtLayoutObjMethodsVector;

// ImageObject的相关方法定义
typedef struct __ExtImageObjMethodsVector
{
	// 结构体的大小
	size_t size;

}ExtImageObjMethodsVector;

// RealObject的相关方法定义
typedef struct __ExtRealObjMethodsVector
{
	// 结构体的大小
	size_t size;

	// 指示父窗口已经创建/将要销毁，外部扩展对象需要在这个里面创建/销毁自己的系统窗口,
	// 并调用XLUE_SetRealObjectWindow设置到内置RealObject上或者移除
	void (XLUE_STDCALL *lpfnOnCreateRealWindow)(void* userData, void* objHandle, BOOL bCreate, OS_HOSTWND_HANDLE hWnd);

	// Real类型元对象的事件，指示设置焦点到自己的系统窗口上,，或者移除焦点
	void (XLUE_STDCALL *lpfnOnSetRealFocus)(void* userData, void* objHandle, BOOL focus);

	// Real类型元对象的事件，获取当前对象显示的真正窗口句柄
	OS_HOSTWND_HANDLE (XLUE_STDCALL *lpfnOnGetRenderWindow)(void* userData, void* objHandle);

}ExtRealObjMethodsVector;

// ExtObjType_layoutObj 和 EExtObjType_renderableObj需要的methods注册参数
typedef struct __ExtLayoutObjMethods
{
	// 结构体的大小
	size_t size;

	// 用户自定义数据，作为回调函数第一个参数
	void* userData;

	// 基类LayoutObject的方法
	ExtLayoutObjMethodsVector layoutObjMethods;

}ExtLayoutObjMethods;

// ExtObjType_imageObj类型扩展对象需要用到的methods注册参数
typedef struct __ExtImageObjMethods
{
	// 结构体的大小
	size_t size;

	// 用户自定义数据，作为回调函数第一个参数
	void* userData;

	// 基类LayoutObject的方法
	ExtLayoutObjMethodsVector layoutObjMethodVector;

	// ImageObject自身的方法
	ExtImageObjMethodsVector imageObjMethodVector;

}ExtImageObjMethods;

// ExtObjType_realObj类型扩展对象需要用到的methods注册参数
typedef struct __ExtRealObjMethods
{
	// 结构体的大小
	size_t size;

	// 用户自定义数据，作为回调函数第一个参数
	void* userData;

	// 基类LayoutObject的方法
	ExtLayoutObjMethodsVector layoutObjMethodVector;

	// ImageObject自身的方法
	ExtRealObjMethodsVector realObjMethodVector;

}ExtRealObjMethods;

typedef struct __ExtObjParser
{
	// 结构体的大小
	size_t size;

	// 用户自定义数据，作为回调函数第一个参数
	void* userData;

	// 元对象的xml属性列表解析事件
	BOOL (XLUE_STDCALL *lpfnParserAttribute)(void* userData, void* objHandle, const char* key, const char* value);

	// 元对象的xml事件列表解析事件，为空表示使用引擎内部的默认事件解析策略，扩展对象绝大多数情况下不需要设置此字段！
	// 如果扩展对象需要定制的事件解析方式，那么需要设置该字段并自己管理解析结果
	BOOL (XLUE_STDCALL *lpfnParserEvent)(void* userData, void* objHandle, const char* eventName, XL_LRT_CHUNK_HANDLE hCodeChunk, XL_LRT_RUNTIME_HANDLE hRunTime);
	

}ExtObjParser;

typedef struct __ExtObjLuaHost
{
	// 结构体的大小
	size_t size;

	// 用户自定义数据，作为回调函数第一个参数
	void* userData;

	// 获取所有的lua扩展api
	BOOL (XLUE_STDCALL *lpfnGetLuaFunctions)(void* userData, const char* className, const XLLRTGlobalAPI** lplpLuaFunctions, size_t* lpFuncCount);

	// 注册额外的辅助lua类或者全局对象
	BOOL (XLUE_STDCALL *lpfnRegisterAuxClass)(void* userData, const char* className, XL_LRT_ENV_HANDLE hEnv);

}ExtObjLuaHost;

/*
扩展元对象的事件机制分为托管模式和非托管模式(均为lua层的事件)
托管模式下事件的管理和fire等都需要外部来处理，步骤比较繁琐，但是自由度很大
非托管模式下事件的管理和fire都由内部来处理，外部只需要调用XLUE_ExtObj_FireEvent等几个函数便可以触发指定事件

如果ExtObjEvent结构体的lpfnAttachListener和lpfnRemoveListener字段不为空，那么视为非托管模式；否则视为托管模式
*/

/*
扩展元对象的事件原型应该满足如下形式:
ret, ret1, ret2, ..., retm, handled, callnext OnEvent(param1, param2, ..., paramn)
定义了一个有n个参数和m+3个返回值的标准事件OnEvent
需要注意的就是返回值的三个默认参数：第一个ret为标准返回值(long)，倒数第二个handled标识该事件是否被处理(bool)，
最后一个callnext标识是否调用下一个事件(bool)，这三个返回值是内置的事件管理器需要的，其余的返回值ret1-retm是用户真正关心的返回值
外部元对象定义的事件必须满足该形式，否则在托管模式下fire事件会导致返回值不正确！
*/ 

/* 托管事件模式下fire元对象的内置事件，调用序列如下
// push n个参数到lua栈
lua_push(luaState, param1);
...
lua_push(luaState, paramn);

long ret = XLUE_FireExtObjEvent(hObj, eventName, luaState, n, m, handled);

// 获取m个返回值
ret1 = lua_toxxx(luaState, -1);
...
retm = lua_toxxx(luaState, -m);
*/
typedef struct __ExtObjEvent
{
	// 结构体的大小
	size_t size;

	// 用户自定义数据，作为回调函数第一个参数
	void* userData;

	// 判断指定的内置元对象事件是否存在
	BOOL (XLUE_STDCALL *lpfnEventExists)(void* userData, void* objHandle, const char* eventName);

	/*------------------------------非托管模式---------------------------------*/
	// 挂接指定的事件，luaFuncRef指定了该事件的luafunction，isPushBack指定该事件是放到事件队列前还是末尾(使用者可以根据具体情况来使用)
	// 如果挂接成功，那么返回TRUE，并且lpEventCookie指定一个大于0的值；否则返回FALSE
	BOOL (XLUE_STDCALL *lpfnAttachListener)(void* userData, void* objHandle, const char* eventName, lua_State* luaState, long luaFuncRef, BOOL isPushBack, unsigned long* lpEventCookie);
	BOOL (XLUE_STDCALL *lpfnRemoveListener)(void* userData, void* objHandle, const char* eventName, unsigned long eventCookie);

	/*------------------------------托管模式-----------------------------------*/
	// 该模式下事件由引擎内部管理，挂接和移除一个事件会有OnAttachEvent和OnRemoveEvent通知(如果需要关注的话)
	void (XLUE_STDCALL *lpfnOnAttachListener)(void* userData, void* objHandle, const char* eventName, unsigned long eventCookie);
	void (XLUE_STDCALL *lpfnOnRemoveListener)(void* userData, void* objHandle, const char* eventName, unsigned long eventCookie);

	// 每个事件触发时候会调用OnFireEvent，使用者可以指定一个回调函数来关注fire事件状态，返回FALSE则cancel该次事件Listener触发
	//BOOL (XLUE_STDCALL *lpfnOnFireEvent)(void* userData, void* objHandle, const char* eventName, unsigned long eventCookie, lua_State* luaState, int paramCount, int retCount);

}ExtObjEvent;

// 扩展元对象的完备注册信息
typedef struct __ExtObjRegisterInfo
{
	// 结构体的大小
	size_t size;

	// 扩展元对象的类别
	ExtObjType type;

	// 扩展元对象的class，不能和内置元对象class重复
	const char* className;

	// 扩展元对象的属性，可以取ExtObjAttribute值的一个或者多个
	unsigned long attribute;

	// 扩展元对象的创建/销毁管理器，必须不为空
	ExtObjCreator* lpExtObjCreator;

	// 扩展元对象的核心方法，具体参数类型取决于扩展元对象类别
	// ExtObjType_layoutObj/ExtObjType_renderableObj -> ExtObjMethods
	// ExtObjType_imageObj -> ExtImageObjMethods
	// ExtObjType_realObj -> ExtRealObjMethods
	void* lpExtObjMethods;

	// 扩展元对象的xml解析器，如果需要从xml里面生成该扩展对象，那么该字段不得为空
	ExtObjParser* lpExtObjParser;

	// 扩展元对象的lua扩展，如果需要在lua里面操作该扩展对象，那么该字段不得为空
	ExtObjLuaHost* lpExtObjLuaHost;

	// 扩展元对象的lua事件，如果需要在lua里面操作监听对象的内置事件，那么该字段不得为空
	ExtObjEvent* lpExtObjEvent;

}ExtObjRegisterInfo;

/*------------------ 扩展资源的相关定义 --------------------------------*/

// 引擎扩展资源句柄，基于引用计数管理
typedef void* XLUE_RESOURCE_HANDLE;

// 内置资源类型定义
#define XLUE_RESTYPE_UNKNOWN	"unknown"

#define XLUE_RESTYPE_BITMAP		"bitmap"
#define XLUE_RESTYPE_TEXTURE	"texture"
#define XLUE_RESTYPE_FONT		"font"
#define XLUE_RESTYPE_COLOR		"color"
#define XLUE_RESTYPE_CURVE		"curve"
#define XLUE_RESTYPE_IMAGELIST	"imagelist"
#define XLUE_RESTYPE_IMAGESEQ	"imageseq"
#define XLUE_RESTYPE_PEN		"pen"
#define XLUE_RESTYPE_BRUSH		"brush"
#define XLUE_RESTYPE_GIF		"gif"
#define XLUE_RESTYPE_CURSOR		"cursor"

// 资源事件的flag定义
typedef enum __ResEventFlag
{
	ResEventFlag_load = 0,
	ResEventFlag_update = 1,
	ResEventFlag_user = 2,

}ResEventFlag;

// 扩展资源的属性
typedef enum __ExtResourceAttribute
{
	// 该资源在xml里面的配置没有子节点，形如
	// <resname id="xxx" value="xxxx"/>
	// 指定该属性资源xml解析会有较高的效率
	ExtResourceAttribute_multiLevelXML = 0x01,	

}ExtResourceAttribute;

// 扩展资源的创建和销毁回调管理
typedef struct __ExtResourceCreator
{
	// 结构体的大小
	size_t size;

	// 用户自定义数据，作为回调函数第一个参数
	void* userData;

	// 创建一个指定类型的扩展资源，如果成功返回一个唯一的handle来标识
	void* (XLUE_STDCALL *lpfnCreateRes)(void* userData, const char* lpResType, XLUE_RESOURCE_HANDLE hResHandle);
	
	// 销毁扩展资源的外部实例，objHandle为创建时候返回的handle
	void (XLUE_STDCALL *lpfnDestroyRes)(void* userData, void* resHandle);

}ExtResourceCreator;

// 扩展资源的具体方法定义
typedef struct __ExtResourceMethods
{
	// 结构体的大小
	size_t size;

	// 用户自定义数据，作为回调函数第一个参数
	void* userData;

	// 加载和释放内部资源，用以实现用时加载和闲时的垃圾回收
	BOOL (XLUE_STDCALL *lpfnLoadRes)(void* userData, void* lpResHandle, const wchar_t* lpResFolder);
	BOOL (XLUE_STDCALL* lpfnFreeRes)(void* userData, void* lpResHandle);

	// 获取对应的真正资源句柄，比如XL_BITMAP_HANDLE，XLGP_ICON_HANDLE等
	// 返回值不需增加引用计数！！
	void* (XLUE_STDCALL *lpfnGetRealHandle)(void* userData, void* lpResHandle);

	// 真正资源句柄的生命周期基于引用计数管理
	long (XLUE_STDCALL *lpfnAddRefRealHandle)(void* userData, void* lpResHandle, void* lpRealHandle);
	long (XLUE_STDCALL *lpfnReleaseRealHandle)(void* userData, void* lpResHandle, void* lpRealHandle);

}ExtResourceMethods;

// 扩展资源的xml解析器回调方法定义
typedef struct __ExtResourceParser
{
	// 结构体的大小
	size_t size;

	// 用户自定义数据，作为回调函数第一个参数
	void* userData;

	// 从xml解析资源，hResXML只可在该函数里面访问，不可超出该函数的范围
	BOOL (XLUE_STDCALL *lpfnParseFromXML)(void* userData, void* lpResHandle, XLUE_XML_HANDLE hResXML);

	// 从lua table动态解析资源，资源属性在lua table里面定义和对应的xml定义结构须保持一致
	BOOL (XLUE_STDCALL *lpfnParseFromLua)(void* userData, void* lpResHandle, lua_State* luaState, int index);

}ExtResourceParser;

// 扩展资源的lua扩展相关回调定义
typedef struct __ExtResourceLuaHost
{
	// 结构体的大小
	size_t size;

	// 用户自定义数据，作为回调函数第一个参数
	void* userData;

	// 获取所有的lua扩展api
	BOOL (XLUE_STDCALL *lpfnGetLuaFunctions)(void* userData, const char* lpResType, const XLLRTGlobalAPI** lplpLuaFunctions, size_t* lpFuncCount);

	// 注册额外的辅助lua类或者全局对象
	BOOL (XLUE_STDCALL *lpfnRegisterAuxClass)(void* userData, const char* lpResType, XL_LRT_ENV_HANDLE hEnv);

}ExtResourceLuaHost;

// 扩展元对象的完备注册信息
typedef struct __ExtResourceRegisterInfo
{
	// 结构体的大小
	size_t size;

	// 扩展资源的类型，不能和内置资源类型XLUE_RESTYPE_XXX重复
	// 资源类型应该是全小写字母，该类型值也是资源xml里面使用的
	// 形如<resType id="xxx" value="xxx"/>
	const char* resType;

	// 扩展资源的属性，可以取ExtResourceAttribute值的一个或者多个
	unsigned long attribute;

	// 扩展资源的创建/销毁管理器，必须不为空
	ExtResourceCreator* lpExtResCreator;

	// 扩展资源的核心方法
	ExtResourceMethods* lpExtResMethods;

	// 扩展资源的xml解析器，如果需要从xml里面配置该资源，那么该字段不得为空
	ExtResourceParser* lpExtResParser;

	// 扩展资源的lua扩展，如果需要在lua里操作该资源对象，那么该字段不得为空
	ExtResourceLuaHost* lpExtResLuaHost;

}ExtResourceRegisterInfo;


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus


#endif // __XLUEDEFINE_H__