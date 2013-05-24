#pragma once

#define XLUE_OSSHELL_CLASSNAME "Xunlei.UIEngine.OSShell.Class"
#define XLUE_OSSHELL_OBJNAME   "Xunlei.UIEngine.OSShell"


void Unicode_to_UTF8(const wchar_t* in, unsigned int len, std::string& out);

void UTF8_to_Unicode(const char* in, unsigned int len, std::wstring& out);

void Unicode_to_ANSI(const wchar_t* in, unsigned int len, std::string& out);

void ANSI_to_Unicode(const char* in, unsigned int len, std::wstring& out);

class LuaWin32Shell
{
public:
	LuaWin32Shell(void);
	~LuaWin32Shell(void);

	static void* __stdcall GetObject(void*ud); 
	static long RegisterObject(XL_LRT_ENV_HANDLE hEnv);

	int static GetCursorPos(lua_State* luaState);

	int static GetIEMainPage(lua_State* luaState);
	int static SetIEMainPage(lua_State* luaState);
};
