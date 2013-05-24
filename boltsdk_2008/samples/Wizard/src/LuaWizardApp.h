#pragma once
#include "./WizardApp.h"

#define WIZARD_APP_LUA_CLASS "Wizard.App.Class"
#define WIZARD_APP_LUA_OBJ "Wizard.App"

class LuaWizardApp
{
public:
	LuaWizardApp(void);
	~LuaWizardApp(void);

	static void RegisterSelf(XL_LRT_ENV_HANDLE hEnv);
	static void* __stdcall GetInstance(void* ud); 

public:

	static int Quit(lua_State* luaState);

	static int GetString(lua_State* luaState);
	static int SetString(lua_State* luaState);

	static int GetInt(lua_State* luaState);
	static int SetInt(lua_State* luaState);

	static int AttachListener(lua_State* luaState);
	static int RemoveListener(lua_State* luaState);

	static int GetWebBrowserDisp(lua_State* luaState);

	static int Crash(lua_State* luaState);

	static int GetFlashFile(lua_State* luaState);
};
