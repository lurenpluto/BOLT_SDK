// HelloXLUE.cpp : main source file for HelloXLUE.exe
//

#include "stdafx.h"

#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include "LuaMineSweeperApp.h"

CAppModule _Module;

const wchar_t* GetXARPath()
{
	static wchar_t path[MAX_PATH] = {0};
	GetModuleFileName(NULL, path, MAX_PATH);
	PathAppend(path, L"..\\..\\samples\\MineSweeper\\xar\\");
	return path;
}

bool InitXLUE()
{
	// 初始化LuaRuntime的调试接口
#ifdef TSLOG
	XLLRT_DebugInit("IDE",XLLRT_DEBUG_TYPE_HOOK);
#else
	XLLRT_DebugInit("IDE",XLLRT_DEBUG_TYPE_NOHOOK);
#endif

	XLGraphicParam param;
	XL_PrepareGraphicParam(&param);
	param.textType = XLTEXT_TYPE_GDI;
	XL_InitGraphicLib(&param);
	XL_SetFreeTypeEnabled(TRUE);

	

	// 初始化XLUE,这函数是一个符号初始化函数
	// 完成了初始化Lua环境,标准对象,XLUELoader的工作
	XLFS_Init();
	XLUE_InitLoader(NULL);

	return true;
}


void UninitXLUE()
{
	XLUE_Uninit(NULL);
	XLUE_UninitLuaHost(NULL);
	XL_UnInitGraphicLib();
	XLUE_UninitHandleMap(NULL);
	XLFS_Uninit();
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

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	UninitXLUE();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	if(!InitXLUE())
	{
		return -1;
	}

	XL_LRT_ENV_HANDLE hEnv = XLLRT_GetEnv(NULL);
	LuaMineSweeperApp::RegisterObj(hEnv);
	XLLRT_ReleaseEnv(hEnv);

	XLUE_AddXARSearchPath(GetXARPath());
	XLUE_LoadXAR("MineSweeper");

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
