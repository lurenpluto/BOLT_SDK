#pragma once
#include <fstream>

#define LUAHISTORYFILE_CLASS "BoltFox.HistoryFile.Class"
#define LUAHISTORYFILE_OBJECT "BoltFox.HistoryFile"

class LuaHistoryFile
{
public:
	LuaHistoryFile(void);
	virtual ~LuaHistoryFile(void);

protected:
	const static char s_cszFilePath[MAX_PATH]; 
	
public:
	static void* __stdcall GetInstance(void* pUserData); 
	static void RegisterObject(XL_LRT_ENV_HANDLE hEnv);
	static int Load(lua_State* luaState);
	static int Save(lua_State* luaState);

protected:
	static void SaveLuaTable(lua_State* luaState, int nIndex, size_t nBufferLen, std::fstream& fileStream);
	static int LoadLuaTable(lua_State* luaState, size_t nBufferLen, std::fstream& fileStream);
private:
	static XLLRTGlobalAPI s_luaMembers[];
};
