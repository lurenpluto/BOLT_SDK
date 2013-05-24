#pragma once

#include "MineSweeperApp.h"

#define XUNLEI_IDEAPP_CLASS		"Xunlei.MineSweeperApp.Class"
#define XUNLEI_IDEAPP_OBJ		"Xunlei.MineSweeperApp"

class LuaMineSweeperApp
{
public:
	LuaMineSweeperApp(void);
	~LuaMineSweeperApp(void);

	static MineSweeperApp* __stdcall Instance(void*);

	static int Quit(lua_State *luaState);

	static long RegisterObj(XL_LRT_ENV_HANDLE hEnv);

protected:
	static MineSweeperApp* GetMineSweeperApp( lua_State* luaState );
};
