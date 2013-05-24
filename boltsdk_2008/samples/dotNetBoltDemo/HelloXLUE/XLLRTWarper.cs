using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Bolt
{

    public enum LuaInnerIndex
    {
        LUA_REGISTRYINDEX = -10000,
        LUA_ENVIRONINDEX = -10001,
        LUA_GLOBALSINDEX = -10002,
    }
    public enum LuaTypes
    {
        LUA_TNIL = 0,
        LUA_TBOOLEAN = 1,
        LUA_TLIGHTUSERDATA = 2,
        LUA_TNUMBER = 3,
        LUA_TSTRING = 4,
        LUA_TTABLE = 5,
        LUA_TFUNCTION = 6,
        LUA_TUSERDATA = 7,
        LUA_TTHREAD = 8
    }
    [UnmanagedFunctionPointerAttribute(CallingConvention.Cdecl)]
    public delegate int lua_CFunction (IntPtr L);
    public delegate IntPtr fnGetObject(IntPtr ud);

    [StructLayout(LayoutKind.Sequential)]
    public struct XLLRTGlobalAPI
    {
        [MarshalAs(UnmanagedType.LPStr)]
        public String funName;
        public lua_CFunction func;
        public UInt32 permission;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    public struct XLLRTObject
    {
        public fnGetObject pfnGetObject;
        public IntPtr userData;
        [MarshalAs(UnmanagedType.LPStr)]
        public String objName;
        [MarshalAs(UnmanagedType.LPStr)]
        public String className;
        public IntPtr memberFunctions;//XLLRTGlobalAPI数组
        public UInt32 permission;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct XLLRTClass
    {
        [MarshalAs(UnmanagedType.LPStr)]
	    public String className;
	    [MarshalAs(UnmanagedType.LPStr)]
        public String fahterClassName;
        public IntPtr MemberFunctions;//XLLRTGlobalAPI数组
	    public UInt32 permission;
    }

    class XLLRTWarper
    {
        
        public void pushString(IntPtr L,string s)
        {
            lua_pushstring(L, s);
        }

        public static XLLRTWarper Instance()
        {
            if (theOne == null)
            {
                theOne = new XLLRTWarper();
            }
            return theOne;
        }

        static XLLRTWarper theOne = null;

        //仅仅暴露了例子需要的 C API,你可以根据实际需要添加
        [DllImport("XLLuaRuntime.dll",CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr XLLRT_GetEnv(IntPtr ud);
        [DllImport("XLLuaRuntime.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr XLLRT_RegisterGlobalObj(IntPtr env, XLLRTObject theObject);
        [DllImport("XLLuaRuntime.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr XLLRT_RegisterClass(IntPtr ud,string className,IntPtr memberFunctions,string fatherClassname,UInt32 permission);
        [DllImport("XLLuaRuntime.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int XLLRT_LuaCall(IntPtr L,int arg,int ret,string ustr);
        [DllImport("XLLuaRuntime.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int XLLRT_PushXLObject(IntPtr L,string className,IntPtr pRealObj);

        [DllImport("XLLuaRuntime.dll")]
        public static extern IntPtr luaL_checkudata (IntPtr L, int ud, string tname);
        [DllImport("XLLuaRuntime.dll")]
        public static extern int lua_type(IntPtr L, int idx);
        [DllImport("XLLuaRuntime.dll")]    
        public static extern int luaL_ref (IntPtr L, int t);
        [DllImport("XLLuaRuntime.dll")] 
        public static extern void luaL_unref (IntPtr L, int t, int n);
        [DllImport("XLLuaRuntime.dll")]
        public static extern void lua_pushstring(IntPtr L, string s);
        [DllImport("XLLuaRuntime.dll")]
        public static extern string lua_tostring(IntPtr L, int idx);
        [DllImport("XLLuaRuntime.dll")]
        public static extern int lua_tointeger (IntPtr L, int idx);
        [DllImport("XLLuaRuntime.dll")]
        public static extern void lua_pushinteger (IntPtr L, int n);
        [DllImport("XLLuaRuntime.dll")]
        public static extern void lua_pushnil (IntPtr L);
        [DllImport("XLLuaRuntime.dll")]
        public static extern int lua_gettop(IntPtr L);
        [DllImport("XLLuaRuntime.dll")]
        public static extern void lua_settop(IntPtr L,int newTop);
        [DllImport("XLLuaRuntime.dll")]
        public static extern void lua_rawgeti(IntPtr L,int index,int n);



    }
}
