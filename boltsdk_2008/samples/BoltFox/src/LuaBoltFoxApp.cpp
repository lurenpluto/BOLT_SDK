#include "StdAfx.h"
#include ".\luaBoltFoxapp.h"

LuaBoltFoxApp::LuaBoltFoxApp(void)
{
}

LuaBoltFoxApp::~LuaBoltFoxApp(void)
{
}

void* __stdcall LuaBoltFoxApp::GetInstance( void*ud )
{
	return BoltFoxApp::GetInstance();
}

static XLLRTGlobalAPI LuaWizardAppMemberFunctions[] = 
{
	{"Quit", LuaBoltFoxApp::Quit},

	{NULL,NULL}
};

void LuaBoltFoxApp::RegisterSelf( XL_LRT_ENV_HANDLE hEnv )
{
	assert(hEnv);
	if(hEnv == NULL)
	{
		return ;
	}

	XLLRTObject theObject;
	theObject.ClassName = BOLTFOX_APP_LUA_CLASS;
	theObject.MemberFunctions = LuaWizardAppMemberFunctions;
	theObject.ObjName = BOLTFOX_APP_LUA_OBJ;
	theObject.userData = NULL;
	theObject.pfnGetObject = (fnGetObject)LuaBoltFoxApp::GetInstance;

	long result = XLLRT_RegisterGlobalObj(hEnv,theObject); 
	assert(!result);
}

int LuaBoltFoxApp::Quit( lua_State* luaState )
{
	int exitCode = (int)lua_tointeger(luaState, 2);

	lua_pushboolean(luaState, BoltFoxApp::GetInstance()->Quit(exitCode));

	return 1;
}

