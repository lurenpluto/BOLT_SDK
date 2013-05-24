#include "StdAfx.h"
#include ".\luawin32shell.h"

void Unicode_to_UTF8(const wchar_t* in, unsigned int len, std::string& out)
{   
	size_t out_len = len * 3 + 1;
	char* pBuf = new char[out_len];
	if ( NULL == pBuf )
	{
		return;
	}
	char* pResult = pBuf;
	memset(pBuf, 0, out_len);

	out_len = ::WideCharToMultiByte(CP_UTF8, 0, in, len, pBuf, len * 3, NULL, NULL);
	out.assign( pResult, out_len );

	delete [] pResult;
	pResult = NULL;
	return;
}

void UTF8_to_Unicode(const char* in, unsigned int len, std::wstring& out)
{
	wchar_t* pBuf = new wchar_t[len + 1];
	if ( NULL == pBuf )
	{
		return;
	}
	size_t out_len = (len + 1) * sizeof(wchar_t);
	memset(pBuf, 0, (len + 1) * sizeof(wchar_t));
	wchar_t* pResult = pBuf;

	out_len = ::MultiByteToWideChar(CP_UTF8, 0, in, len, pBuf, len * sizeof(wchar_t));
	out.assign( pResult, out_len );


	delete [] pResult;
	pResult = NULL;
}

void Unicode_to_ANSI(const wchar_t* in, unsigned int len, std::string& out)
{
    int bufferlen = (int)::WideCharToMultiByte(CP_ACP,0,in,(int)len,NULL,0,NULL,NULL);
    char* pBuffer = new char[bufferlen + 4];
    if ( NULL == pBuffer )
    {
        return;
    }
    int out_len = ::WideCharToMultiByte(CP_ACP,0,in,(int)len,pBuffer,bufferlen+2,NULL,NULL);   
    pBuffer[bufferlen] = '\0';
    out.assign( pBuffer, out_len );
    delete[] pBuffer;
}

void ANSI_to_Unicode(const char* in, unsigned int len, std::wstring& out)
{
    int wbufferlen = (int)::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,in,(int)len,NULL,0);
    wchar_t* pwbuffer = new wchar_t[wbufferlen+4];
    if ( NULL == pwbuffer )
    {
        return;
    }
    wbufferlen = (int)::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,in,(int)len,pwbuffer,wbufferlen+2);
    pwbuffer[wbufferlen] = '\0';
    out.assign( pwbuffer, wbufferlen );
    delete[] pwbuffer;
    return;
}


LuaWin32Shell::LuaWin32Shell(void)
{
}

LuaWin32Shell::~LuaWin32Shell(void)
{
}

int LuaWin32Shell::GetCursorPos(lua_State* luaState)
{
	POINT pt;
	::GetCursorPos(&pt);
	lua_pushinteger(luaState, pt.x);
	lua_pushinteger(luaState, pt.y);
	return 2;
}

void* __stdcall LuaWin32Shell::GetObject( void*ud )
{
	static LuaWin32Shell s_shell;
	return &s_shell;
}

static XLLRTGlobalAPI LuaOSShellMemberFunctions[] = 
{
	{"GetCursorPos",LuaWin32Shell::GetCursorPos},
	{"GetIEMainPage",LuaWin32Shell::GetIEMainPage},
	{"SetIEMainPage",LuaWin32Shell::SetIEMainPage},
	{NULL,NULL}
};


long LuaWin32Shell::RegisterObject( XL_LRT_ENV_HANDLE hEnv )
{
	assert(hEnv);
	if(hEnv == NULL)
	{
		return XLLRT_RESULT_ENV_INVALID;
	}

	XLLRTObject theObject;
	theObject.ClassName = XLUE_OSSHELL_CLASSNAME;
	theObject.MemberFunctions = LuaOSShellMemberFunctions;
	theObject.ObjName = XLUE_OSSHELL_OBJNAME;
	theObject.userData = NULL;
	theObject.pfnGetObject = LuaWin32Shell::GetObject;

	long lRet = XLLRT_RegisterGlobalObj(hEnv,theObject); 
	assert(lRet == XLLRT_RESULT_SUCCESS);
	if (lRet != XLLRT_RESULT_SUCCESS)
	{
		return lRet;
	}
	assert(lRet == XLLRT_RESULT_SUCCESS);

	return lRet;
}

int LuaWin32Shell::GetIEMainPage(lua_State* luaState)
{
	CRegKey reg;
	if (reg.Open(HKEY_CURRENT_USER, L"Software\\Microsoft\\Internet Explorer\\Main") == ERROR_SUCCESS)
	{
		wchar_t strUrl[1024] = {0};
		ULONG ulSize = 1024;
		if (reg.QueryStringValue(L"Start Page", strUrl, &ulSize) == ERROR_SUCCESS)
		{
			std::string utf8Url;
			Unicode_to_UTF8(strUrl, ::wcslen(strUrl), utf8Url);
			lua_pushstring(luaState, utf8Url.c_str());
			return 1;
		}
	}

	lua_pushnil(luaState);
	return 1;
}

int LuaWin32Shell::SetIEMainPage(lua_State* luaState)
{
	const char* lpUrl = luaL_checkstring(luaState, 2);
	if (lpUrl)
	{
		CRegKey reg;
		if (reg.Open(HKEY_CURRENT_USER, L"Software\\Microsoft\\Internet Explorer\\Main") == ERROR_SUCCESS)
		{
			std::wstring strUrl;
			UTF8_to_Unicode(lpUrl, ::strlen(lpUrl), strUrl);
			if (reg.SetStringValue(L"Start Page", strUrl.c_str()) == ERROR_SUCCESS)
			{
				lua_pushboolean(luaState, 1);
				return 1;
			}
		}
	}

	lua_pushboolean(luaState, 0);
	return 1;
}