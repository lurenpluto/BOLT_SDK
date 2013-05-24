#pragma once
#include "./SkinManager.h"

#define WIZARD_SKINMANAGER_LUA_CLASS "Wizard.SkinManager.Class"
#define WIZARD_SKINMANAGER_LUA_OBJ "Wizard.SkinManager"

class LuaSkinManager
{
public:
    LuaSkinManager(void);
    ~LuaSkinManager(void);

    static void RegisterSelf(XL_LRT_ENV_HANDLE hEnv);
    static void* __stdcall GetInstance(void* ud);

    static int GetBitmapHSLColor(lua_State* luaState);
    static int GetColorByPixel(lua_State* luaState);

    static int GetBitmapIdByIndex(lua_State* luaState);
    static int GetTextureIdByIndex(lua_State* luaState);
    static int GetColorIdByIndex(lua_State* luaState);

    static int GetColorableBitmapCount(lua_State* luaState);
    static int GetColorableColorCount(lua_State* luaState);
    static int GetColorableTextureCount(lua_State* luaState);
};
