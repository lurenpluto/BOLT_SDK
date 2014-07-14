/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2012              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   XLUEIPC
*   Author      :   xlue group
*   Create      :   2012-2-2 11:29
*   LastChange  :   2012-2-2 11:29
*   History     :	
*
*   Description :   进程间通信
*********************************************************************/ 

#ifndef __XLUEIPC_H__ 
#define __XLUEIPC_H__ 

#include <XLLuaRuntime.h>

#ifndef XLUEIPC_EXTERN_C
	#ifdef __cplusplus	
		#define XLUEIPC_EXTERN_C extern "C"
	#else
		#define XLUEIPC_EXTERN_C 
	#endif // __cplusplus
#endif // XLUEIPC_EXTERN_C

#if defined(XLUE_UNIONLIB)
	#define XLUEIPC_API(x) XLUEIPC_EXTERN_C  x __stdcall 
#elif defined(XLUEIPC_EXPORTS)
	#define XLUEIPC_API(x) XLUEIPC_EXTERN_C __declspec(dllexport) x __stdcall 
#else // XLUEIPC_EXPORTS
	#define XLUEIPC_API(x) XLUEIPC_EXTERN_C __declspec(dllimport) x __stdcall 
#endif // XLUEIPC_EXPORTS


// 句柄定义
#define DECLARE_XLIPC_HANDLE(name) struct __XLIPC_SAFE_HANDLE_##name { int unused; }; typedef __XLIPC_SAFE_HANDLE_##name *name;

DECLARE_XLIPC_HANDLE(XLIPC_STREAM)
DECLARE_XLIPC_HANDLE(XLIPC_CHANNEL)
DECLARE_XLIPC_HANDLE(XLIPC_SERVER)
DECLARE_XLIPC_HANDLE(XLIPC_SESSION)
DECLARE_XLIPC_HANDLE(XLIPC_INTERFACE)


// 错误码定义
#define XLIPC_RESULT_SUCCESS			0
#define XLIPC_RESULT_FAILED				-1

#define XLIPC_RESULT_BUFFERTOOSMALL		1
#define XLIPC_RESULT_INVALID_PARAM		2
#define XLIPC_RESULT_ERROR_TYPE			3
#define XLIPC_RESULT_NOMOREDATA			4
#define XLIPC_RESULT_CHANNELEXIST		5
#define XLIPC_RESULT_INVALID_STATUS		6
#define XLIPC_RESULT_OUT_OF_MEMORY		7
#define XLIPC_RESULT_SEND_FAILED		8
#define XLIPC_RESULT_CREATE_SESSION_FAILED	9
#define XLIPC_RESULT_WAITTHREAD_NONE	10
#define XLIPC_RESULT_SERVER_NOT_FOUND	11
#define XLIPC_RESULT_TIMEOUT			12
#define XLIPC_RESULT_ALREADY_WAITTING	13
#define XLIPC_RESULT_ALREADY_EXISTS		14
#define XLIPC_RESULT_NOT_FOUND			15
#define XLIPC_RESULT_INVALID_BUFFER		16
#define XLIPC_RESULT_UNKNOWN_CALLTYPE	17
#define XLIPC_RESULT_INVALID_SESSION	18
#define XLIPC_RESULT_INVALID_PLUS		19
#define XLIPC_RESULT_MINUS_ALREADY_EXISTS	20
#define XLIPC_RESULT_CREATE_WINDOW_FAILED	21
#define XLIPC_RESULT_CONNECT_FAILED			22
#define XLIPC_RESULT_INIT_REMOTEMEMORY_FAILED	23
#define XLIPC_RESULT_ALREADY_CONNECT	24
#define XLIPC_RESULT_INVALID_MIUNS		25
#define XLIPC_RESULT_INVALID_STRUCT		26
#define XLIPC_RESULT_UNEXPECTED			27
#define XLIPC_RESULT_SERVER_CLOSED		28
#define XLIPC_RESULT_DISCONNNECT		29
#define XLIPC_RESULT_ASYNCALLFRAME_NOT_FOUND	30
#define XLIPC_RESULT_SYNCCALLFRAME_NOT_FOUND	31
#define XLIPC_RESULT_REPLY_FAILED	32
#define XLIPC_RESULT_NOT_IMPL	33

// 通道相关函数
#define CHANNEL_CONNECTTYPE_CONNECT		1
#define CHANNEL_CONNECTTYPE_DISCONNECT	0

typedef long (__stdcall* RECVDATACALLBACK)(void* userData, XLIPC_CHANNEL hChannel, const void* lpBuffer, size_t len);
typedef long (__stdcall* DATAACKCALLBACK)(void* userData, XLIPC_CHANNEL hChannel, size_t id);
typedef long (__stdcall* CONNECTCALLBACK)(void* userData, XLIPC_CHANNEL hChannel, int connectType, long result);
typedef long (__stdcall* STATUSCHANGECALLBACK)(void* userData, XLIPC_CHANNEL hChannel, long newStatus, long oldStatus);

typedef struct tagChannelCallBackDef
{
	unsigned long		cbSize;		// 指定ChannelCallBackDef结构体的大小

	void*				userData;

	RECVDATACALLBACK	recvDataCallBack;
	DATAACKCALLBACK		dataAckCallBack;
	CONNECTCALLBACK		connectCallBack;
	STATUSCHANGECALLBACK	statusChangeCallBack;

}ChannelCallBackDef;

// 创建一个channel，可以用来等待对方连接
XLUEIPC_API(long) XLIPC_CreateChannel(const wchar_t* id, int flags, const ChannelCallBackDef* lpCallBackDef, XLIPC_CHANNEL* lpRet);

//同步连接指定channel
XLUEIPC_API(long) XLIPC_SyncConnectChannel(const wchar_t* id, int flags, const ChannelCallBackDef* lpCallBackDef, XLIPC_CHANNEL* lpRet);

// 异步连接指定channel，连接是否成功，会通过connectCallBack回调来通知
// hChannel如果不为空则表示连接成功，type=CHANNEL_CONNECTTYPE_CONNECT,result指定了错误码
XLUEIPC_API(long) XLIPC_AsynConnectChannel(const wchar_t* id, int flags, const ChannelCallBackDef* lpCallBackDef);

XLUEIPC_API(long) XLIPC_CloseChannel(XLIPC_CHANNEL hChannel);

XLUEIPC_API(long) XLIPC_SetChannelCallBack(XLIPC_CHANNEL hChannel, const ChannelCallBackDef* lpCallBackDef);

// 设置channel的连接超时时间，默认为500ms
XLUEIPC_API(long) XLIPC_SetChannelTimeout(XLIPC_CHANNEL hChannel, unsigned long timeout);

// 查询channel id
XLUEIPC_API(const wchar_t*) XLIPC_GetChannelID(XLIPC_CHANNEL hChannel);

// 设置状态和查询状态
XLUEIPC_API(long) XLIPC_SetChannelStatus(XLIPC_CHANNEL hChannel, long newStatus);
XLUEIPC_API(long) XLIPC_GetChannelStatus(XLIPC_CHANNEL hChannel, long* lpStatus);

// 设置心跳等待超时时间间隔，默认为500ms
XLUEIPC_API(long) XLIPC_SetChannelAliveTimeout(XLIPC_CHANNEL hChannel, unsigned long timeout);
XLUEIPC_API(long) XLIPC_GetChannelAliveTimeout(XLIPC_CHANNEL hChannel, unsigned long* lpTimeout);

// 同步和异步发送数据
XLUEIPC_API(long) XLIPC_AsynSendData(XLIPC_CHANNEL hChannel, const void* lpBuffer, size_t len, size_t* lpID);
XLUEIPC_API(long) XLIPC_SyncSendData(XLIPC_CHANNEL hChannel, const void* lpBuffer, size_t len, int timeout);



// 会话服务器相关接口
typedef long (__stdcall* ACCEPTSESSIONCALLBACK)(void* userData, XLIPC_SERVER hServer, XLIPC_SESSION hNewSession);
typedef long (__stdcall* NEWSESSIONCALLBACK)(void* userData, XLIPC_SERVER hServer);
typedef struct tagServerCallBackDef
{
	unsigned long		cbSize;		// 指定ServerCallBackDef结构体的大小

	void*				userData;

	ACCEPTSESSIONCALLBACK	acceptSessionCallBack;
	NEWSESSIONCALLBACK	newSessionCallBack;

}ServerCallBackDef;

// 开始和关闭指定id的server，该server是全局唯一的
XLUEIPC_API(long) XLIPC_StartServer(const wchar_t* id, int flags, const ServerCallBackDef* lpCallBack, XLIPC_SERVER* lpServer);
XLUEIPC_API(long) XLIPC_CloseServer(XLIPC_SERVER hServer);

XLUEIPC_API(long) XLIPC_AddRefServer(XLIPC_SERVER hServer);
XLUEIPC_API(long) XLIPC_ReleaseServer(XLIPC_SERVER hServer);

XLUEIPC_API(long) XLIPC_GetServerCallback(XLIPC_SERVER server, ServerCallBackDef* lpCallBack);
XLUEIPC_API(long) XLIPC_GetServerIsWorking(XLIPC_SERVER server, bool* lpIsWorking);

// 在服务器上等待(非阻塞消息)连接，返回的session只可以在对应的线程里面使用
XLUEIPC_API(long) XLIPC_AcceptSession(XLIPC_SERVER hServer, XLIPC_SESSION* lpSession);

// 提交一个异步等待，该操作会立即返回，如果有新session连接或者出错，会通过回调来通知
// 发起调用的线程必须有自己的消息循环，并且返回的session只可以在对应的线程里面使用
typedef long (__stdcall* SESSIONACCEPTCALLBACK)(void* userData, XLIPC_SESSION hNewSession, long result);
XLUEIPC_API(long) XLIPC_AsynAcceptSession(XLIPC_SERVER hServer, SESSIONACCEPTCALLBACK lpCallBack, void* userData);

// 同步连接服务器，返回的session只可以在对应的线程里面使用
XLUEIPC_API(long) XLIPC_SyncConnectServer(const wchar_t* serverID, int type, int timeout, XLIPC_SESSION* lpSession);

// 根据一个已经建立的channel，直接生成一个session，channel会托管于该session
XLUEIPC_API(long) XLIPC_NewSession(XLIPC_CHANNEL hChannel, XLIPC_SESSION* lpSession);

// 根据一个自定义通道，生成一个session
typedef long (__stdcall* SESSIONCHANNEL_ASYNSENDDATA)(void* userData, const void* lpBuffer, size_t bufferLen);
typedef long (__stdcall* SESSIONCHANNEL_SYNCSENDDATA)(void* userData, const void* lpBuffer, size_t bufferLen, int timeout);
typedef long (__stdcall* SESSIONCHANNEL_SETSTATUS)(void* userData, long status);
typedef long (__stdcall* SESSIONCHANNEL_GETSTATUS)(void* userData, long* lpStatus);
typedef long (__stdcall* SESSIONCHANNEL_CLOSE)(void* userData);


typedef struct tagSessionChannelDef
{
	unsigned long cbSize;

	void* userData;

	SESSIONCHANNEL_ASYNSENDDATA lpfnAsynSendData;
	SESSIONCHANNEL_SYNCSENDDATA lpfnSyncSendData;

	SESSIONCHANNEL_SETSTATUS lpfnSetStatus;
	SESSIONCHANNEL_GETSTATUS lpfnGetStatus;

	SESSIONCHANNEL_CLOSE lpfnClose;

}SessionChannelDef;

// 自定义通道向session推送的数据和断开通知
XLUEIPC_API(long) XLIPC_SessionChannelOnRevData(XLIPC_SESSION hSession, const void* lpBuffer, size_t bufferLen);
XLUEIPC_API(long) XLIPC_SessionChannelOnDisconnect(XLIPC_SESSION hSession);

XLUEIPC_API(long) XLIPC_CreateSession(const SessionChannelDef* lpSessionChannelDef, XLIPC_SESSION* lpSession);

// 异步连接服务器
typedef long (__stdcall* SESSIONCONNECTCALLBACK)(void* userData, XLIPC_SESSION hNewSession, long result);
XLUEIPC_API(long) XLIPC_AsynConnectServer(const wchar_t* serverID, int type, SESSIONCONNECTCALLBACK lpCallBack, void* userData);

// 会话相关接口
typedef long (__stdcall* SESSIONCLOSECALLBACK)(void* userData, XLIPC_SESSION hSession);
typedef struct tagSessionCallBackDef
{
	unsigned long		cbSize;		// 指定SessionCallBackDef结构体的大小

	void*				userData;

	SESSIONCLOSECALLBACK	sessionCloseCallBack; // session主动或者被动关闭事件

}SessionCallBackDef;

XLUEIPC_API(long) XLIPC_SetSessionCallBack(XLIPC_SESSION hSession, const SessionCallBackDef* lpCallBack);
XLUEIPC_API(long) XLIPC_GetSessionCallBack(XLIPC_SESSION hSession, SessionCallBackDef* lpCallBack);

// 设置同步发送数据和应答超时时间
XLUEIPC_API(long) XLIPC_SetSessionTimeout(XLIPC_SESSION hSession, long timeout);
XLUEIPC_API(long) XLIPC_GetSessionTimeout(XLIPC_SESSION hSession, long* lpTimeout);

// 设置心跳等待超时时间间隔，默认为500ms
XLUEIPC_API(long) XLIPC_SetSessionAliveTimeout(XLIPC_SESSION hSession, unsigned long timeout);
XLUEIPC_API(long) XLIPC_GetSessionAliveTimeout(XLIPC_SESSION hSession, unsigned long* lpTimeout);

// 设置和查询session状态
XLUEIPC_API(long) XLIPC_SetSessionStatus(XLIPC_SESSION hSession, long newStatus);
XLUEIPC_API(long) XLIPC_GetSessionStatus(XLIPC_SESSION hSession, long* lpStatus);

XLUEIPC_API(long) XLIPC_RegisterSessionInterface(XLIPC_SESSION hSession, XLIPC_INTERFACE hInterface);
XLUEIPC_API(long) XLIPC_UnRegisterSessionInterface(XLIPC_SESSION hSession, const char* id);
XLUEIPC_API(long) XLIPC_QuerySessionInterface(XLIPC_SESSION hSession, const char* id);

//根据id获取interface,使用者在使用完毕之后一定要release
XLUEIPC_API(long) XLIPC_GetSessionInterface(XLIPC_SESSION hSession, const char *id, XLIPC_INTERFACE* lpInterface);

XLUEIPC_API(long) XLIPC_QuerySessionRemoteInterface(XLIPC_SESSION hSession, const char* interfaceID, int timeout);
XLUEIPC_API(long) XLIPC_QuerySessionRemoteMethod(XLIPC_SESSION hSession, const char* interfaceID, const char* methodID, int timeout);


// 同步设置为受信状态，并等待远程受信后返回
XLUEIPC_API(long) XLIPC_SyncSignalAndWait(XLIPC_SESSION hSession, int timeout);

// 异步设置为受信状态，并立即返回；远程受信后会通过回调函数来通知
typedef long (__stdcall* SESSIONSIGNALCALLBACK)(void* userData, long result, XLIPC_SESSION hSession);
XLUEIPC_API(long) XLIPC_AsynSignalAndCallBack(XLIPC_SESSION hSession, int timeout, SESSIONSIGNALCALLBACK lpCallBack, void* userData);

// 同步调用指定的方法，包括不带应答和带应答两种，使用session自身的超时设置
XLUEIPC_API(long) XLIPC_SyncCall(XLIPC_SESSION hSession, const char* intefaceID, const char* methodID, XLIPC_STREAM hParamStream);
XLUEIPC_API(long) XLIPC_SyncCallReply(XLIPC_SESSION hSession, const char* intefaceID, const char* methodID, XLIPC_STREAM hParamStream, XLIPC_STREAM* lpReturnStream);

// 带超时的同步调用，可以定义该次同步调用的超时时间；-1为不超时等待调用返回
XLUEIPC_API(long) XLIPC_SyncCallTimeout(XLIPC_SESSION hSession, const char* intefaceID, const char* methodID, XLIPC_STREAM hParamStream, int timeout);
XLUEIPC_API(long) XLIPC_SyncCallReplyTimeout(XLIPC_SESSION hSession, const char* intefaceID, const char* methodID, XLIPC_STREAM hParamStream, XLIPC_STREAM* lpReturnStream, int timeout);

// 异步调用指定的方法，包括不带应答和带应答两种
typedef long (__stdcall* ASYNCALLREPLY)(void* userData, XLIPC_SESSION hSession, XLIPC_STREAM hReplyStream, unsigned long callID);
XLUEIPC_API(long) XLIPC_AsynCall(XLIPC_SESSION hSession, const char* intefaceID, const char* methodID, XLIPC_STREAM hParamStream);
XLUEIPC_API(long) XLIPC_AsynCallReply(XLIPC_SESSION hSession, const char* intefaceID, const char* methodID, XLIPC_STREAM hParamStream, ASYNCALLREPLY lpCallBack, void* userData, unsigned long* lpCallID);

// 关闭会话
XLUEIPC_API(long) XLIPC_CloseSession(XLIPC_SESSION hSession);


// 基于stream的远程和本地统一接口(线程安全)
XLUEIPC_API(XLIPC_INTERFACE) XLIPC_CreateInterface(const char* id);
XLUEIPC_API(long) XLIPC_AddRefInterface(XLIPC_INTERFACE hInterface);
XLUEIPC_API(long) XLIPC_ReleaseInterface(XLIPC_INTERFACE hInterface);

typedef long (__stdcall* INTERFACEMETHOD)(void* userData, XLIPC_STREAM hParamStream, XLIPC_STREAM* lpRetStream);
XLUEIPC_API(long) XLIPC_RegisterMethod(XLIPC_INTERFACE hInterface, const char* methodID, INTERFACEMETHOD method, void* userData);

// method方法被释放时候的回调
typedef void (__stdcall *LPFNMETHODRELEASECALLBACK)(XLIPC_INTERFACE hInterface, const char *methodID, INTERFACEMETHOD method, void *userData);
XLUEIPC_API(long) XLIPC_RegisterMethodEx(XLIPC_INTERFACE hInterface, const char* methodID, INTERFACEMETHOD method, void* userData, LPFNMETHODRELEASECALLBACK lpfnReleaseCallBack);

XLUEIPC_API(long) XLIPC_UnRegisterMethod(XLIPC_INTERFACE hInterface, const char* methodID);
XLUEIPC_API(long) XLIPC_QueryMethod(XLIPC_INTERFACE hInterface, const char* methodID, INTERFACEMETHOD* lpMethod, void** lpUserData);

typedef struct tagXLIPCInterfaceMethod
{
	const char*	methodName;
	INTERFACEMETHOD	methodProc;

}XLIPCInterfaceMethod;

// 注册一组method
XLUEIPC_API(long) XLIPC_RegisterMethods(XLIPC_INTERFACE hInterface, const XLIPCInterfaceMethod* lpMethodVector, void* userData);

// 清除所有已经注册的方法
XLUEIPC_API(long) XLIPC_ClearAllMethods(XLIPC_INTERFACE hInterface);

// 参数和返回值流相关定义和接口
#define XLIPC_PTYPE_NULL		0
#define XLIPC_PTYPE_STREAM		1
#define XLIPC_PTYPE_STRING		2
#define XLIPC_PTYPE_WSTRING		3
#define XLIPC_PTYPE_BYTE		4
#define XLIPC_PTYPE_WORD		5
#define XLIPC_PTYPE_DWORD		6
#define XLIPC_PTYPE_INT			7
#define XLIPC_PTYPE_LONG		8
#define XLIPC_PTYPE_BOOLEAN		9
#define XLIPC_PTYPE_BYTES		10
#define XLIPC_PTYPE_INT64		11
#define XLIPC_PTYPE_DOUBLE      12

XLUEIPC_API(XLIPC_STREAM) XLIPC_CreateStream();
XLUEIPC_API(long) XLIPC_StreamAddRef(XLIPC_STREAM hStream);
XLUEIPC_API(long) XLIPC_StreamRelease(XLIPC_STREAM hStream);

// 获取下一个参数类型
XLUEIPC_API(long) XLIPC_StreamGetParamType(XLIPC_STREAM hStream, unsigned long* lpType);

XLUEIPC_API(long) XLIPC_StreamWriteString(XLIPC_STREAM hStream, const char* lpString);
XLUEIPC_API(long) XLIPC_StreamWriteWString(XLIPC_STREAM hStream, const wchar_t* lpString);
XLUEIPC_API(long) XLIPC_StreamWriteByte(XLIPC_STREAM hStream, BYTE bValue);
XLUEIPC_API(long) XLIPC_StreamWriteWord(XLIPC_STREAM hStream, WORD wValue);
XLUEIPC_API(long) XLIPC_StreamWriteDWord(XLIPC_STREAM hStream, DWORD dwValue);
XLUEIPC_API(long) XLIPC_StreamWriteInt(XLIPC_STREAM hStream, int iValue);
XLUEIPC_API(long) XLIPC_StreamWriteLong(XLIPC_STREAM hStream, long lValue);
XLUEIPC_API(long) XLIPC_StreamWriteBoolean(XLIPC_STREAM hStream, BOOL bValue);
XLUEIPC_API(long) XLIPC_StreamWriteInt64(XLIPC_STREAM hStream, long long value);
XLUEIPC_API(long) XLIPC_StreamWriteDouble(XLIPC_STREAM hStream, double value);
XLUEIPC_API(long) XLIPC_StreamWriteBytes(XLIPC_STREAM hStream, const unsigned char* lpBuffer, int nLength);
XLUEIPC_API(long) XLIPC_StreamWriteStream(XLIPC_STREAM hStream, XLIPC_STREAM hSrc);


XLUEIPC_API(long) XLIPC_StreamReadString(XLIPC_STREAM hStream, char* lpString, size_t nLength, size_t* lpcbLength);
XLUEIPC_API(long) XLIPC_StreamReadWString(XLIPC_STREAM hStream, wchar_t* lpString, size_t nLength, size_t* lpcbLength);
XLUEIPC_API(long) XLIPC_StreamReadByte(XLIPC_STREAM hStream, BYTE* bValue);
XLUEIPC_API(long) XLIPC_StreamReadWord(XLIPC_STREAM hStream, WORD* wValue);
XLUEIPC_API(long) XLIPC_StreamReadDWord(XLIPC_STREAM hStream, DWORD* dwValue);
XLUEIPC_API(long) XLIPC_StreamReadInt(XLIPC_STREAM hStream, int* iValue);
XLUEIPC_API(long) XLIPC_StreamReadLong(XLIPC_STREAM hStream, long* lValue);
XLUEIPC_API(long) XLIPC_StreamReadBoolean(XLIPC_STREAM hStream, BOOL* bValue);
XLUEIPC_API(long) XLIPC_StreamReadInt64(XLIPC_STREAM hStream, long long* value);
XLUEIPC_API(long) XLIPC_StreamReadDouble(XLIPC_STREAM hStream, double* value);
XLUEIPC_API(long) XLIPC_StreamReadBytes(XLIPC_STREAM hStream, unsigned char* lpBuffer, size_t nLength, size_t* lpcbLength);
XLUEIPC_API(long) XLIPC_StreamReadStream(XLIPC_STREAM hStream, XLIPC_STREAM* hDest);

XLUEIPC_API(void) XLIPC_StreamClear(XLIPC_STREAM hStream);

XLUEIPC_API(long) XLIPC_StreamEncode(XLIPC_STREAM hStream, char* lpBuffer, size_t nLength, size_t* lpcbLength);
XLUEIPC_API(long) XLIPC_StreamDecode(XLIPC_STREAM hStream, const char* lpBuffer, size_t nLength);

//lua相关
XLUEIPC_API(BOOL) XLIPC_RegisterLuaHost(XL_LRT_ENV_HANDLE hEnv);
XLUEIPC_API(XLIPC_SESSION) XLIPC_CheckSessionFromLua(lua_State*luaState, int index);
XLUEIPC_API(BOOL) XLIPC_PushSessionToLua(lua_State *luaState, XLIPC_SESSION session);
XLUEIPC_API(XLIPC_SERVER) XLIPC_CheckServerFromLua(lua_State*luaState, int index);
XLUEIPC_API(BOOL) XLIPC_PushServerToLua(lua_State *luaState, XLIPC_SERVER server);

#endif //__XLIPC_H__ 