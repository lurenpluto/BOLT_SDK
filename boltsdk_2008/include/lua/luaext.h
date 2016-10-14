/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2014              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   luaext
*   Author      :   xlue group
*   Create      :   2014-6-9 
*   History     :	
*
*   Description :   对lua虚拟机的一些扩展
*
********************************************************************/ 
#ifndef __LUAEXT_H__
#define __LUAEXT_H__

#include "lua.h"

// 无符号数
LUALIB_API lua_Unsigned (luaL_checkunsigned) (lua_State *L, int arg);
LUALIB_API lua_Unsigned (luaL_optunsigned) (lua_State *L, int arg,
                                            lua_Unsigned def);

// 32bit整型数，内有溢出检测
LUALIB_API int (luaL_checkint) (lua_State *L, int arg);
LUALIB_API int (luaL_optint) (lua_State *L, int arg,
                              int def);
LUALIB_API unsigned int (luaL_checkuint) (lua_State *L, int arg);
LUALIB_API unsigned int (luaL_optuint) (lua_State *L, int arg,
                                        unsigned int def);

LUALIB_API size_t (luaL_checksize_t) (lua_State *L, int arg);
LUALIB_API size_t (luaL_optsize_t) (lua_State *L, int arg,
                              size_t def);

LUALIB_API long (luaL_checklong) (lua_State *L, int arg);
LUALIB_API long (luaL_optlong) (lua_State *L, int arg,
                              long def);

#ifndef lua_unref
#define lua_unref(L,ref)        luaL_unref(L, LUA_REGISTRYINDEX, (ref))
#endif // !lua_unref

#ifndef lua_getref
#define lua_getref(L,ref)       lua_rawgeti(L, LUA_REGISTRYINDEX, (ref))
#endif //!lua_getref

#endif // __LUAEXT_H__