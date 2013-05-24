#include "StdAfx.h"
#include ".\luadataobjecthelper.h"
#include "./Transcode.h"

LuaDataObjectHelper::LuaDataObjectHelper(void)
{
}

LuaDataObjectHelper::~LuaDataObjectHelper(void)
{
}

static XLLRTGlobalAPI LuaDatObjectHelperMemberFunctions[] = 
{
    {"CreateDataObjectFromText", LuaDataObjectHelper::CreateDataObjectFromText},
    {"PraseDataObject", LuaDataObjectHelper::PraseDataObject},
    {"IsCFTEXTData", LuaDataObjectHelper::IsCFTEXTData},
    {NULL,NULL}
};

void LuaDataObjectHelper::RegisterSelf( XL_LRT_ENV_HANDLE hEnv )
{
    assert(hEnv);
    if(hEnv == NULL)
    {
        return ;
    }

    XLLRTObject theObject;
    theObject.ClassName = DATAOBJECT_HELPER_LUA_CLASS;
    theObject.MemberFunctions = LuaDatObjectHelperMemberFunctions;
    theObject.ObjName = DATAOBJECT_HELPER_LUA_OBJ;
    theObject.userData = NULL;
    theObject.pfnGetObject = (fnGetObject)LuaDataObjectHelper::GetInstance;

    long result = XLLRT_RegisterGlobalObj(hEnv,theObject); 
    assert(!result);
}

void* __stdcall LuaDataObjectHelper::GetInstance( void*ud )
{
    return CDataObjectHelper::GetInstance();
}

int LuaDataObjectHelper::CreateDataObjectFromText(lua_State* luaState)
{
    const char* lpText = lua_tostring(luaState, 2);
    if (lpText)
    {
        std::wstring text;
        Transcode::UTF8_to_Unicode(lpText, strlen(lpText), text);
        IDataObject* lpDataObject = CDataObjectHelper::GetInstance()->CreateDataObjectFromText(text);
        lua_pushlightuserdata(luaState, lpDataObject);
        return 1;
    }
    lua_pushnil(luaState);
    return 1;
}

int LuaDataObjectHelper::PraseDataObject(lua_State* luaState)
{
    IDataObject* lpObj = (IDataObject*)lua_touserdata(luaState, 2);
    if (lpObj)
    {
        std::wstring wData;
        CDataObjectHelper::GetInstance()->PraseDataObject(lpObj, wData);
        std::string data;
        Transcode::Unicode_to_UTF8(wData.c_str(), wData.length(), data);
        if (data.length() > 0)
        {
            lua_pushboolean(luaState, 1);
            lua_pushstring(luaState, data.c_str());
        }
        else
        {
            lua_pushboolean(luaState, 0);
            lua_pushnil(luaState);
        }
    }
    else
    {
        lua_pushboolean(luaState, 0);
        lua_pushnil(luaState);
    }
    return 2;
}

int LuaDataObjectHelper::IsCFTEXTData(lua_State* luaState)
{
    IDataObject* lpObj = (IDataObject*)lua_touserdata(luaState, 2);
    if (lpObj)
    {
        lua_pushboolean(luaState, CDataObjectHelper::GetInstance()->IsCFTEXTData(lpObj));
    }
    else
    {
        lua_pushboolean(luaState, 0);
    }
    return 1;
}