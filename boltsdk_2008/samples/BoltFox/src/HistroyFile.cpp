#include "StdAfx.h"
#include ".\histroyfile.h"
#include <vector>
#include <string>
#include "LuaWin32Shell.h"
using namespace std;

LuaHistoryFile::LuaHistoryFile(void)
{
}

LuaHistoryFile::~LuaHistoryFile(void)
{
}


void* __stdcall LuaHistoryFile::GetInstance(void* pUserData)
{
	 static LuaHistoryFile s_instance;

	 return &s_instance;
}

const char LuaHistoryFile::s_cszFilePath[MAX_PATH] = "./history.txt"; 

XLLRTGlobalAPI LuaHistoryFile::s_luaMembers[] = {
	{"Load", LuaHistoryFile::Load},
	{"Save", LuaHistoryFile::Save},
	{NULL, NULL},
};

void LuaHistoryFile::RegisterObject(XL_LRT_ENV_HANDLE hEnv)
{
	assert(hEnv);
	if(hEnv == NULL)
	{
		return ;
	}

	XLLRTObject theObject;
	theObject.ClassName = LUAHISTORYFILE_CLASS;
	theObject.MemberFunctions = LuaHistoryFile::s_luaMembers;
	theObject.ObjName = LUAHISTORYFILE_OBJECT;
	theObject.userData = NULL;
	theObject.pfnGetObject = (fnGetObject)LuaHistoryFile::GetInstance;

	long result = XLLRT_RegisterGlobalObj(hEnv,theObject); 
	assert(!result);
}

#define MAXURLLEN			2084
#define MAXKEYWORDLEN		1024

void LuaHistoryFile::SaveLuaTable(lua_State* luaState, int nIndex, size_t nBufferLen, std::fstream& fileStream)
{
	vector<string> vecStrings;
	if(lua_istable(luaState,nIndex))
	{
		int nStrCnt = lua_objlen(luaState,nIndex);
		const char* cszWrite = NULL;
		for(int ix = 1; ix <= nStrCnt; ++ix)
		{
			lua_rawgeti(luaState, nIndex , ix);
			if(lua_isstring(luaState, -1))
			{
				cszWrite = luaL_checkstring(luaState, -1);
				wstring wstrWrite;
				UTF8_to_Unicode(cszWrite, strlen(cszWrite), wstrWrite);
				string astrWrite;
				wstrWrite += WCHAR('\n');
				Unicode_to_ANSI(wstrWrite.c_str(), wstrWrite.size(),astrWrite);
				vecStrings.push_back(astrWrite);
				
			}
			lua_pop(luaState,1);
		}
	}
	char szStrCnt[100];
	itoa(vecStrings.size(), szStrCnt, 100);
	int nStrCntLen = strlen(szStrCnt);
	szStrCnt[nStrCntLen] = '\n';
	fileStream.write(szStrCnt, nStrCntLen + 1);
	for(size_t ix = 0; ix < vecStrings.size(); ++ix)
	{
		const char* buf = vecStrings[ix].c_str();
		size_t buflen = vecStrings[ix].size();
		fileStream.write(buf, buflen);
		
	}
	
}

int LuaHistoryFile::LoadLuaTable(lua_State* luaState, size_t nBufferLen, std::fstream& fileStream)
{
	char* szRead = new char[4 * nBufferLen];
	lua_newtable(luaState);
	if(fileStream.getline(szRead, 4 * nBufferLen))
	{
		int nStrCnt = atoi(szRead);
		for(int ix = 1; ix <= nStrCnt; ++ix)
		{
			if(fileStream.getline(szRead, 4 * nBufferLen))
			{
				string strRead;
				wstring wstrRead;
				ANSI_to_Unicode(szRead, strlen(szRead), wstrRead);
				Unicode_to_UTF8(wstrRead.c_str(), wstrRead.size(), strRead);
				lua_pushstring(luaState, strRead.c_str());
				lua_rawseti(luaState, -2, ix);
			}
		}
	}
	delete []szRead;
	return 1;
}
	
int LuaHistoryFile::Load(lua_State* luaState)
{
	LuaHistoryFile** ppHistoryFile = reinterpret_cast<LuaHistoryFile**>(luaL_checkudata(luaState,1,LUAHISTORYFILE_CLASS));  
	int nTableCnt = 0;
	if(ppHistoryFile && *ppHistoryFile)
	{
		fstream fileStream;
		fileStream.open(s_cszFilePath, ios_base::in);
		nTableCnt += LoadLuaTable(luaState, MAXURLLEN, fileStream);
		nTableCnt += LoadLuaTable(luaState, MAXKEYWORDLEN, fileStream);
		fileStream.close();
	}
	return nTableCnt;
}

int LuaHistoryFile::Save(lua_State* luaState)
{
	LuaHistoryFile** ppHistoryFile = reinterpret_cast<LuaHistoryFile**>(luaL_checkudata(luaState,1,LUAHISTORYFILE_CLASS));  
	if(ppHistoryFile && *ppHistoryFile)
	{
		fstream fileStream;
		fileStream.open(s_cszFilePath, ios_base::out | ios_base::trunc );
		if(!fileStream)
		{
			return 0;
		}
		
		SaveLuaTable(luaState, 2, MAXURLLEN, fileStream);
		SaveLuaTable(luaState, 3, MAXKEYWORDLEN, fileStream);
		fileStream.close();
	}
	return 0;
}	
