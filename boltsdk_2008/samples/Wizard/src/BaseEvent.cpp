#include "StdAfx.h"
#include ".\baseevent.h"


LuaConfigChangeEvent::LuaConfigChangeEvent( lua_State* luaState,long luaRef )
:LuaBaseEventFunctor(luaState, luaRef)
{

}

long LuaConfigChangeEvent::operator()( const char* lpSection, const char* lpKey, const char* lpValue )
{
	assert(m_luaState);

	lua_State* luaState = m_luaState;

	// ◊¢“‚±£¥Êlua’ª∂•Œª÷√
	long nowTop = lua_gettop(m_luaState);

	lua_rawgeti(m_luaState, LUA_REGISTRYINDEX, m_luaFunRef);
	
	lua_pushstring(m_luaState, lpSection);
	lua_pushstring(m_luaState, lpKey);
	lua_pushstring(m_luaState, lpValue);

	int nRet = XLLRT_LuaCall(m_luaState, 3, 1, L"LuaConfigChangeEvent");
	long result = 0;
	if (nRet == 0)
	{
		result = (long)lua_tointeger(luaState,-1);
	}

	lua_settop(luaState,nowTop);

	return result;
}

long LuaConfigChangeEvent::operator()( const char* lpSection, const char* lpKey, int value )
{
	assert(m_luaState);

	lua_State* luaState = m_luaState;

	// ◊¢“‚±£¥Êlua’ª∂•Œª÷√
	long nowTop = lua_gettop(m_luaState);

	lua_rawgeti(m_luaState, LUA_REGISTRYINDEX, m_luaFunRef);

	lua_pushstring(m_luaState, lpSection);
	lua_pushstring(m_luaState, lpKey);
	lua_pushinteger(m_luaState, value);

	int nRet = XLLRT_LuaCall(m_luaState, 3, 1, L"LuaConfigChangeEvent");
	long result = 0;
	if (nRet == 0)
	{
		result = (long)lua_tointeger(luaState,-1);
	}

	lua_settop(luaState,nowTop);

	return result;
}