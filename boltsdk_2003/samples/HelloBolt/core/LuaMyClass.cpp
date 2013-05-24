 #include "StdAfx.h"
#include ".\luamyclass.h"

int LuaMyClass::Add(lua_State* luaState)
{
    MyClass** ppMyClass= reinterpret_cast<MyClass**>(luaL_checkudata(luaState,1,MY_CLASS_LUA_CLASS));   
    if(ppMyClass && (*ppMyClass))
    {
        int lhs = static_cast<int>(lua_tointeger(luaState,2));
        int rhs = static_cast<int>(lua_tointeger(luaState,3));
        int result = (*ppMyClass)->Add(lhs,rhs);
        lua_pushinteger(luaState,result);
        return 1;     
    }
    
    lua_pushnil(luaState);
    return 1;
}

int LuaMyClass::AttachResultListener(lua_State* luaState)
{
    MyClass** ppMyClass= reinterpret_cast<MyClass**>(luaL_checkudata(luaState,1,MY_CLASS_LUA_CLASS));   
    if(ppMyClass && (*ppMyClass))
    {
        if(!lua_isfunction(luaState,2))
        {
            return 0;
        }
        //如果事件可以Detach,如何正确的释放这个lua function
        long functionRef = luaL_ref(luaState,LUA_REGISTRYINDEX);
        (*ppMyClass)->AttachResultListener(reinterpret_cast<DWORD>(luaState),functionRef,LuaMyClass::LuaListener);
    }
    return 0;
}

int LuaMyClass::DeleteSelf(lua_State* luaState)
{
     MyClass** ppMyClass= reinterpret_cast<MyClass**>(luaL_checkudata(luaState,1,MY_CLASS_LUA_CLASS));   
     if(ppMyClass)
     {
         delete (*ppMyClass);
     }
    return 0;
}

void LuaMyClass::LuaListener(DWORD dwUserData1,DWORD dwUserData2,int nResult)
{
    lua_State* luaState = reinterpret_cast<lua_State*>(dwUserData1);
    int nNowTop = lua_gettop(luaState);
    lua_rawgeti(luaState,LUA_REGISTRYINDEX,dwUserData2 );

    lua_pushinteger(luaState,nResult);
    //Bolt中要求使用XLLRT_LuaCall代替调用lua_pcall
	//以获得更高稳定性和更多虚拟机状态信息
    int nLuaResult = XLLRT_LuaCall(luaState,1,0,L"LuaMyClass::LuaListener");
    
	//调用完成之后回退luaState到调用之前的状态
	//如果被调用的lua代码段有返回值，
	//请在此前处理并且注意XLLRT_LuaCall的返回值（nLuaResult变量）确认脚本正确执行
    lua_settop(luaState,nNowTop);
    return ;   
}

static XLLRTGlobalAPI LuaMyClassMemberFunctions[] = 
{
    {"Add",LuaMyClass::Add},
    {"AttachResultListener",LuaMyClass::AttachResultListener},
    {"__gc",LuaMyClass::DeleteSelf},
    {NULL,NULL}
};

void LuaMyClass::RegisterClass(XL_LRT_ENV_HANDLE hEnv)
{
    if(hEnv == NULL)
    {
        return;
    }

    long nLuaResult = XLLRT_RegisterClass(hEnv,MY_CLASS_LUA_CLASS,LuaMyClassMemberFunctions,NULL,0);
}
//------------------------------------------------------------------
MyClass* LuaMyClassFactory::CreateInstance()
{
    return new MyClass();
}

int LuaMyClassFactory::CreateInstance(lua_State* luaState)
{
    MyClass* pResult = new MyClass();
    XLLRT_PushXLObject(luaState,MY_CLASS_LUA_CLASS,pResult);
    return 1;
}

LuaMyClassFactory* __stdcall LuaMyClassFactory::Instance(void*)
{
    static LuaMyClassFactory* s_pTheOne = NULL;
    if(s_pTheOne == NULL)
    {
        s_pTheOne = new LuaMyClassFactory();
    }
    return s_pTheOne;
}

static XLLRTGlobalAPI LuaMyClassFactoryMemberFunctions[] = 
{
    {"CreateInstance",LuaMyClassFactory::CreateInstance},
    {NULL,NULL}
};

void LuaMyClassFactory::RegisterObj(XL_LRT_ENV_HANDLE hEnv)
{
    if(hEnv == NULL)
	{
        return ;
	}

    XLLRTObject theObject;
    theObject.ClassName = MY_CLASS_FACTORY_LUA_CLASS;
    theObject.MemberFunctions = LuaMyClassFactoryMemberFunctions;
    theObject.ObjName = MY_CLASS_FACTORY_LUA_OBJ;
    theObject.userData = NULL;
    theObject.pfnGetObject = (fnGetObject)LuaMyClassFactory::Instance;

    XLLRT_RegisterGlobalObj(hEnv,theObject); 
}