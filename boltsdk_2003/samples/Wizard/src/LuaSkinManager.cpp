#include "StdAfx.h"
#include ".\luaskinmanager.h"

LuaSkinManager::LuaSkinManager(void)
{
}

LuaSkinManager::~LuaSkinManager(void)
{
}

void* __stdcall LuaSkinManager::GetInstance( void*ud )
{
    return CSkinManager::GetInstance();
}

static XLLRTGlobalAPI LuaWizardSkinManagerMemberFunctions[] = 
{
    {"GetBitmapHSLColor", LuaSkinManager::GetBitmapHSLColor},
    {"GetColorByPixel", LuaSkinManager::GetColorByPixel},

    {"GetBitmapIdByIndex", LuaSkinManager::GetBitmapIdByIndex},
    {"GetTextureIdByIndex", LuaSkinManager::GetTextureIdByIndex},
    {"GetColorIdByIndex", LuaSkinManager::GetColorIdByIndex},

    {"GetColorableBitmapCount", LuaSkinManager::GetColorableBitmapCount},
    {"GetColorableColorCount", LuaSkinManager::GetColorableColorCount},
    {"GetColorableTextureCount", LuaSkinManager::GetColorableTextureCount},

    {NULL,NULL}
};

void LuaSkinManager::RegisterSelf( XL_LRT_ENV_HANDLE hEnv )
{
    assert(hEnv);
    if(hEnv == NULL)
    {
        return ;
    }

    XLLRTObject theObject;
    theObject.ClassName = WIZARD_SKINMANAGER_LUA_CLASS;
    theObject.MemberFunctions = LuaWizardSkinManagerMemberFunctions;
    theObject.ObjName = WIZARD_SKINMANAGER_LUA_OBJ;
    theObject.userData = NULL;
    theObject.pfnGetObject = (fnGetObject)LuaSkinManager::GetInstance;

    long result = XLLRT_RegisterGlobalObj(hEnv,theObject); 
    assert(!result);
}

int LuaSkinManager::GetColorableBitmapCount(lua_State* luaState)
{
    int count = CSkinManager::GetInstance()->GetColorableBitmapCount();
    lua_pushinteger(luaState, count);
    return 1;
}

int LuaSkinManager::GetColorableColorCount(lua_State* luaState)
{
    int count = CSkinManager::GetInstance()->GetColorableColorCount();
    lua_pushinteger(luaState, count);
    return 1;
}

int LuaSkinManager::GetColorableTextureCount(lua_State* luaState)
{
    int count = CSkinManager::GetInstance()->GetColorableTextureCount();
    lua_pushinteger(luaState, count);
    return 1;
}

int LuaSkinManager::GetBitmapIdByIndex(lua_State* luaState)
{
    int index = lua_tointeger(luaState, 2);
    const char* id = CSkinManager::GetInstance()->GetBitmapIdByIndex(index);
    if (id)
    {
        lua_pushstring(luaState, id);
        return 1;
    }
    return 0;
}

int LuaSkinManager::GetTextureIdByIndex(lua_State* luaState)
{
    int index = lua_tointeger(luaState, 2);
    const char* id = CSkinManager::GetInstance()->GetTextureIdByIndex(index);
    if (id)
    {
        lua_pushstring(luaState, id);
        return 1;
    }
    return 0;
}

int LuaSkinManager::GetColorIdByIndex(lua_State* luaState)
{
    int index = lua_tointeger(luaState, 2);
    const char* id = CSkinManager::GetInstance()->GetColorIdByIndex(index);
    if (id)
    {
        lua_pushstring(luaState, id);
        return 1;
    }
    return 0;
}

int LuaSkinManager::GetBitmapHSLColor(lua_State* luaState)
{
    XL_BITMAP_HANDLE hBitmap = NULL;
    BOOL ret = XLUE_CheckBitmap(luaState, 2, &hBitmap);
    if (!ret || hBitmap == NULL)
    {
        lua_pushinteger(luaState, 0);
        lua_pushinteger(luaState, 0);
        lua_pushinteger(luaState, 0);

        return 3;
    }

    XL_Color cr = CSkinManager::GetInstance()->GetBitmapHSLColor(hBitmap);

    if (hBitmap != NULL)
    {
        XL_ReleaseBitmap(hBitmap);
    }

    lua_pushinteger(luaState, XLCOLOR_HSL_H(cr));
    lua_pushinteger(luaState, XLCOLOR_HSL_S(cr));
    lua_pushinteger(luaState, XLCOLOR_HSL_L(cr));

    return 3;
}

int LuaSkinManager::GetColorByPixel(lua_State* luaState)
{
    XL_BITMAP_HANDLE hBitmap = NULL;
    BOOL ret = XLUE_CheckBitmap(luaState, 2, &hBitmap);
    int nx = lua_tointeger(luaState, 3);
    int ny = lua_tointeger(luaState, 4);
    if (!ret || hBitmap == NULL)
    {
        lua_pushinteger(luaState, 0);
        lua_pushinteger(luaState, 0);
        lua_pushinteger(luaState, 0);

        return 3;
    }

    XL_Color cr = CSkinManager::GetInstance()->GetColorByPixel(hBitmap, nx, ny);

    if (hBitmap != NULL)
    {
        XL_ReleaseBitmap(hBitmap);
    }

    lua_pushinteger(luaState, XLCOLOR_BGRA_R(cr));
    lua_pushinteger(luaState, XLCOLOR_BGRA_G(cr));
    lua_pushinteger(luaState, XLCOLOR_BGRA_B(cr));

    return 3;
}