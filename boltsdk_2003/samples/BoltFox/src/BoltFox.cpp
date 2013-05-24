// BoltFox.cpp : main source file for BoltFox.exe
//

#include "stdafx.h"

#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include "LuaBoltFoxApp.h"
#include "HistoryFile.h"
#include "LuaWin32Shell.h"

CAppModule _Module;

const wchar_t* GetXARPath()
{
	static wchar_t path[MAX_PATH] = {0};
	GetModuleFileName(NULL, path, MAX_PATH);
	PathAppend(path, L"..\\..\\samples\\BoltFox\\xar\\");
	return path;
}

void RunLuaFunc(XL_LRT_RUNTIME_HANDLE hRuntime, const wchar_t* chunkName, const wchar_t* fileName, const char* funcName)
{
	lua_State* luaState = XLLRT_GetLuaState(hRuntime);
	XL_LRT_CHUNK_HANDLE hThisChunk = NULL;
	XLLRT_CreateChunkFromModule(chunkName, fileName, funcName, &hThisChunk);
	if ( hThisChunk != NULL )
	{
		long lRet = XLLRT_PrepareChunk(hRuntime, hThisChunk);
		if (lRet == 0)
		{
			if (XLLRT_LuaCall(luaState, 0, 0, fileName))
			{
				MessageBoxA(NULL, XLLRT_GetLastError(luaState), 0, 0);
			}
		}
		else
		{
			MessageBoxA(NULL, XLLRT_GetLastError(luaState), 0, 0);
		}

		XLLRT_ReleaseChunk(hThisChunk);
	}
}

void FindLuaCodes( const wchar_t* lpFolder, std::vector<std::wstring>& vtFiles )
{
	assert(lpFolder);

	CFindFile find;
	wchar_t szPath[MAX_PATH];
	::PathCombine(szPath, lpFolder, L"*.lua");

	if (!find.FindFile(szPath))
	{
		return;
	}

	do
	{
		if (find.IsDots() || find.IsDirectory())
		{
			continue;
		}

		if (!find.GetFileName(szPath, MAX_PATH))
		{
			assert(false);
			continue;
		}

		vtFiles.push_back(szPath);
	}while (find.FindNextFile());
}

void InitLuaHelper()
{
	XL_LRT_ENV_HANDLE hEnv = XLLRT_GetEnv(NULL);
	XL_LRT_RUNTIME_HANDLE hRuntime = XLLRT_GetRuntime(hEnv, NULL);

	

	std::wstring strLuaCodePath = GetXARPath();
	strLuaCodePath += L"BoltFox/layout/luacode/";
	
	std::vector<std::wstring> vtLuaCodes;
	FindLuaCodes(strLuaCodePath.c_str(), vtLuaCodes);

	std::vector<std::wstring>::const_iterator cp, end = vtLuaCodes.end();
	for (cp = vtLuaCodes.begin(); cp != end; ++cp)
	{
		std::wstring strLuaCodeFile = strLuaCodePath + *cp;

		RunLuaFunc(hRuntime,
			cp->c_str(), 
			strLuaCodeFile.c_str(), 
			"RegisterObject");
	}


	XLLRT_ReleaseRunTime(hRuntime);
	XLLRT_ReleaseEnv(hEnv);
}


int __stdcall LuaErrorHandle(lua_State* luaState,const wchar_t* pExtInfo,const wchar_t* luaErrorString, PXL_LRT_ERROR_STACK pStackInfo)
{
	static long s_lEnter = 0;
	long prev = ::InterlockedCompareExchange(&s_lEnter, 1, 0);
	if (prev == 0)
	{
		int ret = 0;
		if(pExtInfo != NULL)
		{
			assert(luaErrorString);

			std::wstring str = luaErrorString;
			str += L" @ ";
			str += pExtInfo;
			str += L"\r\n\r\n点击确定查看调用堆栈。";
			ret = ::MessageBox(0, str.c_str(), L"脚本错误", MB_ICONERROR | MB_OKCANCEL);
		}
		else
		{
			std::wstring str = luaErrorString;
			str += L"\r\n\r\n点击确定查看调用堆栈。";
			ret = ::MessageBox(0, str.c_str(), L"脚本错误", MB_ICONERROR | MB_OKCANCEL);
		}
		if (ret == IDOK)
		{
			std::string callstack;
			if (pStackInfo->logs != NULL)
			{
				const char* stack = NULL;
				while(XLLRT_RESULT_SUCCESS == XLLRT_DebugLogsPopNextLog(&stack, pStackInfo->logs) && stack != NULL)
				{
					callstack.append(stack);
				}
			}
			MessageBoxA(0, callstack.c_str(), "调用堆栈", MB_ICONERROR | MB_OK);
		}

		::InterlockedExchange(&s_lEnter, 0);
	}

	return 0;
}

bool InitXLUE()
{
	XLFS_Init();

	XLGraphicParam param;
	XL_PrepareGraphicParam(&param);
	param.textType = XLTEXT_TYPE_FREETYPE;
	XL_InitGraphicLib(&param);

	XL_SetFreeTypeEnabled(TRUE);

	XLUE_InitLoader(NULL);

	// 设置脚本错误回调
	XLLRT_ErrorHandle(&LuaErrorHandle);

	return true;
}

bool UninitXLUE()
{
	XLUE_Uninit(NULL);
	XLUE_UninitLuaHost(NULL);
	XL_UnInitGraphicLib();
	XLUE_UninitHandleMap(NULL);
	XLFS_Uninit();

	return true;
}

// 加载主xar
bool LoadXAR()
{
	XLUE_AddXARSearchPath(GetXARPath());

	if(XLUE_LoadXAR("BoltFox") != 0)
	{
		::MessageBox(0, L"Load XAR failed!", 0 , 0);
		return false;
	}

	return true;
}

bool InitLuaCore()
{
	XL_LRT_ENV_HANDLE hEnv = XLLRT_GetEnv(NULL);

	LuaBoltFoxApp::RegisterSelf(hEnv);
	LuaHistoryFile::RegisterObject(hEnv);
	LuaWin32Shell::RegisterObject(hEnv);

	XLLRT_ReleaseEnv(hEnv);

	return true;
}

bool InitApp()
{
	if (!InitXLUE())
	{
		return false;
	}

	if (!InitLuaCore())
	{
		return false;
	}

	InitLuaHelper();
	//WizardApp::GetInstance()->Init();

	if (!LoadXAR())
	{
		return false;
	}

	return true;
}

bool UninitApp()
{
	//WizardApp::GetInstance()->Uinit();

	UninitXLUE();

	return true;
}

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	//CMainDlg dlgMain;

	//if(dlgMain.Create(NULL) == NULL)
	//{
	//	ATLTRACE(_T("Main dialog creation failed!\n"));
	//	return 0;
	//}

	//dlgMain.ShowWindow(nCmdShow);

	if (!InitApp())
	{
		return -1;
	}

	int nRet = theLoop.Run();

	UninitApp();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	OleInitialize(NULL);
	//HRESULT hRes = ::CoInitialize(NULL);
	// If you are running on NT 4.0 or higher you can use the following call instead to 
	// make the EXE free threaded. This means that calls come in on a random RPC thread.
	//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	//ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	HRESULT hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
