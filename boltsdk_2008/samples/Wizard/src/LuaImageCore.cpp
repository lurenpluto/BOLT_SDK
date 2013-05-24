#include "StdAfx.h"
#include ".\luaimagecore.h"
#include "./Transcode.h"

LuaImageCore::LuaImageCore(void)
{
}

LuaImageCore::~LuaImageCore(void)
{
}

void* __stdcall LuaImageCore::GetInstance( void*ud )
{
	return ImageCore::GetInstance();
}

static XLLRTGlobalAPI LuaWizardImageCoreMemberFunctions[] = 
{
	{"ParseDataObject", LuaImageCore::ParseDataObject},
	{"LoadImage", LuaImageCore::LoadImage},
	{"GetBitmapMainColor", LuaImageCore::GetBitmapMainColor},

	{NULL,NULL}
};

void LuaImageCore::RegisterSelf( XL_LRT_ENV_HANDLE hEnv )
{
	assert(hEnv);
	if(hEnv == NULL)
	{
		return ;
	}

	XLLRTObject theObject;
	theObject.ClassName = WIZARD_IMAGECORE_LUA_CLASS;
	theObject.MemberFunctions = LuaWizardImageCoreMemberFunctions;
	theObject.ObjName = WIZARD_IMAGECORE_LUA_OBJ;
	theObject.userData = NULL;
	theObject.pfnGetObject = (fnGetObject)LuaImageCore::GetInstance;

	long result = XLLRT_RegisterGlobalObj(hEnv,theObject); 
	assert(!result);
}

int LuaImageCore::ParseDataObject( lua_State* luaState )
{
	IDataObject* lpObj = (IDataObject*)lua_touserdata(luaState, 2);
	assert(lpObj);
	if (lpObj == NULL)
	{
		lua_pushboolean(luaState, 0);
		lua_pushnil(luaState);

		return 2;
	}

	std::wstring strFile;
	bool ret = ImageCore::GetInstance()->ParseDataObject(lpObj, strFile);
	if (!ret)
	{
		lua_pushboolean(luaState, 0);
		lua_pushnil(luaState);

		return 2;
	}

	lua_pushboolean(luaState, 1);
	
	std::string strUTF8File;
	Transcode::Unicode_to_UTF8(strFile.c_str(), strFile.length(), strUTF8File);

	lua_pushstring(luaState, strUTF8File.c_str());

	return 2;
}

int LuaImageCore::LoadImage( lua_State* luaState )
{
	const char* lpFile = lua_tostring(luaState, 2);
	assert(lpFile);
	if (lpFile == NULL)
	{
		lua_pushnil(luaState);

		return 1;
	}

	std::wstring strFile;
	Transcode::UTF8_to_Unicode(lpFile, strlen(lpFile), strFile);

	XL_BITMAP_HANDLE hBitmap = ImageCore::GetInstance()->LoadImage(strFile.c_str());
	if (hBitmap == NULL)
	{
		lua_pushnil(luaState);

		return 1;
	}

	XLUE_PushBitmap(luaState, hBitmap);
	XL_ReleaseBitmap(hBitmap);

	return 1;
}

int LuaImageCore::GetBitmapMainColor( lua_State* luaState )
{
	XL_BITMAP_HANDLE hBitmap = NULL;
	BOOL ret = XLUE_CheckBitmap(luaState, 2, &hBitmap);
	if (!ret || hBitmap == NULL)
	{
		XLUE_PushColor2(luaState, XLCOLOR_BGR(255,255,255));

		return 1;
	}

	XL_Color cr = ImageCore::GetInstance()->GetBitmapMainColor(hBitmap);

	if (hBitmap != NULL)
	{
		XL_ReleaseBitmap(hBitmap);
	}

	XLUE_PushColor2(luaState, cr);

	return 1;
}