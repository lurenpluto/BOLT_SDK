// core.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "./LuaMyClass.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    if(ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        XL_LRT_ENV_HANDLE hEnv = XLLRT_GetEnv(NULL);
        LuaMyClass::RegisterClass(hEnv);
        LuaMyClassFactory::RegisterObj(hEnv);
    }

    return TRUE;
}

__declspec(dllexport) void NoUse()
{

}