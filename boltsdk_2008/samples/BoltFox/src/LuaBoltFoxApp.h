#pragma once
#include "./BoltFoxApp.h"

#define BOLTFOX_APP_LUA_CLASS "BoltFox.App.Class"
#define BOLTFOX_APP_LUA_OBJ "BoltFox.App"

class LuaBoltFoxApp
{
public:
	LuaBoltFoxApp(void);
	~LuaBoltFoxApp(void);

	static void RegisterSelf(XL_LRT_ENV_HANDLE hEnv);
	static void* __stdcall GetInstance(void* ud); 

public:

	static int Quit(lua_State* luaState);
};
