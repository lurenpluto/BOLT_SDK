#pragma once
#include "./DataObjectHelper.h"

#define DATAOBJECT_HELPER_LUA_CLASS "Wizard.DataObjectHelper.Class"
#define DATAOBJECT_HELPER_LUA_OBJ "Wizard.DataObjectHelper"

class LuaDataObjectHelper
{
public:
    LuaDataObjectHelper(void);
    ~LuaDataObjectHelper(void);

    static void RegisterSelf(XL_LRT_ENV_HANDLE hEnv);
    static void* __stdcall GetInstance(void* ud);

    static int CreateDataObjectFromText(lua_State* luaState);
    static int PraseDataObject(lua_State* luaState);
    static int IsCFTEXTData(lua_State* luaState);
};
