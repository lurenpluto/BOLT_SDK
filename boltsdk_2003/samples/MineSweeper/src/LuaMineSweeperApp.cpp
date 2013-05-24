#include "StdAfx.h"
#include ".\luaMineSweeperApp.h"
#include <assert.h>

LuaMineSweeperApp::LuaMineSweeperApp(void)
{
}

LuaMineSweeperApp::~LuaMineSweeperApp(void)
{
}

MineSweeperApp* __stdcall LuaMineSweeperApp::Instance( void* )
{
	return MineSweeperApp::Instance();
}

int LuaMineSweeperApp::Quit( lua_State *luaState )
{
	MineSweeperApp *App = GetMineSweeperApp(luaState);
	if (App)
	{
		App->Quit();
	}
	return 0;
}

MineSweeperApp* LuaMineSweeperApp::GetMineSweeperApp( lua_State* luaState )
{
	assert(luaState);
	MineSweeperApp** lplpLuaObj = (MineSweeperApp**)luaL_checkudata(luaState,1, XUNLEI_IDEAPP_CLASS);   
	if(lplpLuaObj != NULL)
	{
		return *lplpLuaObj;
	}

	return NULL;   
}

static XLLRTGlobalAPI LuaIDEAppMemberFunctions[] = 
{
	{"Quit", LuaMineSweeperApp::Quit },
	{NULL, NULL}
};

long LuaMineSweeperApp::RegisterObj( XL_LRT_ENV_HANDLE hEnv )
{
	assert(hEnv);
	if(hEnv == NULL)
	{
		return XLLRT_RESULT_ENV_INVALID;
	}

	XLLRTObject theObject;
	theObject.ClassName = XUNLEI_IDEAPP_CLASS;
	theObject.MemberFunctions = LuaIDEAppMemberFunctions;
	theObject.ObjName = XUNLEI_IDEAPP_OBJ;
	theObject.userData = NULL;
	theObject.pfnGetObject = (fnGetObject)LuaMineSweeperApp::Instance;

	long lRet = XLLRT_RegisterGlobalObj(hEnv,theObject); 
	assert(lRet == XLLRT_RESULT_SUCCESS);
	return lRet;
}