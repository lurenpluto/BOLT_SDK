//
// 这个lua封装是对C++里core.dll工程的lua封装的直接移植
// 实际上对于C#这种有完整RTTI信息的语言，完全可以自己编写自动化封装代码
// 这里仅展示最基础的原理，如何减少封装工作量的开发方法请自己思考
//

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Bolt;
using System.Runtime.InteropServices;
using System.Reflection;
using System.Collections;

namespace HelloXLUE
{
    //在C#里要反调lua,需要event对象保存luaState指针和保存在lua注册表里的ref index
    class LuaMyClassAddOb
    {
        public LuaMyClassAddOb(IntPtr L,int luafunctionRef)
        {
            m_luaState = L;
            m_luaFunctionRef = luafunctionRef;
        }

        ~LuaMyClassAddOb()
        {
            //TODO:
            //这里没有正确释放资源，试着自己加上代码？
        }

        public void OnAddFinish(int lhs, int rhs, int result)
        {
            int nNowTop = XLLRTWarper.lua_gettop(m_luaState);
            //C#->Lua,注意维护lua栈平衡！
            XLLRTWarper.lua_rawgeti(m_luaState,(int)LuaInnerIndex.LUA_REGISTRYINDEX,m_luaFunctionRef );

            XLLRTWarper.lua_pushinteger(m_luaState,result);
            //Bolt中要求使用XLLRT_LuaCall代替调用lua_pcall
	        //以获得更高稳定性和更多虚拟机状态信息
            int nLuaResult = XLLRTWarper.XLLRT_LuaCall(m_luaState,1,0,null);
            
	        //调用完成之后回退luaState到调用之前的状态
	        //如果被调用的lua代码段有返回值，
	        //请在此前处理并且注意XLLRT_LuaCall的返回值（nLuaResult变量）确认脚本正确执行
            XLLRTWarper.lua_settop(m_luaState,nNowTop);    
        }

        protected IntPtr m_luaState;
        protected int m_luaFunctionRef;
    }

    class LuaMyClass
    {
        private static int s_currentIndex = 0;
        private static Dictionary<int, MyClass> allLuaMyClassObj = new Dictionary<int, MyClass>();
        private static int Add(IntPtr L)
        {
            //考虑到unsafe代码里传递对象指针生命周期管理非常麻烦，这里配合s_currentIndex,allLuaMyClassObj
            //牺牲了一点性能实现了一个简单的方案.
            //每个封装函数的开始就是得到对象的C# object instance
            IntPtr ppMyClass = XLLRTWarper.luaL_checkudata(L, 1, "HelloBolt.MyClass");
            IntPtr myclassIndex = new IntPtr(Marshal.ReadInt32(ppMyClass));
            MyClass theObj = allLuaMyClassObj[myclassIndex.ToInt32()];

            int lhs = XLLRTWarper.lua_tointeger(L, 2);
            int rhs = XLLRTWarper.lua_tointeger(L, 3);
            int result  = theObj.Add(lhs,rhs);
            XLLRTWarper.lua_pushinteger(L, result);
            return 1;
        }

        private static int AttachResultListener(IntPtr L)
        {
            IntPtr ppMyClass = XLLRTWarper.luaL_checkudata(L, 1, "HelloBolt.MyClass");
            IntPtr myclassIndex = new IntPtr(Marshal.ReadInt32(ppMyClass));
            MyClass theObj = allLuaMyClassObj[myclassIndex.ToInt32()];

            if(XLLRTWarper.lua_type(L,2) == (int)LuaTypes.LUA_TFUNCTION)
            {
                int functionRef = XLLRTWarper.luaL_ref(L, (int)LuaInnerIndex.LUA_REGISTRYINDEX);
                LuaMyClassAddOb theOb = new LuaMyClassAddOb(L,functionRef);
                theObj.OnAddFinish += theOb.OnAddFinish;
            }
            return 0;
        }

        private static int DeleteSelf(IntPtr L)
        {
            IntPtr ppMyClass = XLLRTWarper.luaL_checkudata(L, 1, "HelloBolt.MyClass");
            IntPtr myclassIndex = new IntPtr(Marshal.ReadInt32(ppMyClass));
            allLuaMyClassObj[myclassIndex.ToInt32()] = null;
            return 0;
        }

        private static int CreateMyClassInstance(IntPtr luaState)
        {
            //这里要把一个C#的obj instance传递到lua虚拟机中
            //注意result的生命周期管理
            MyClass result = new MyClass();
            result.myname = "MyClass:"+s_currentIndex;
            allLuaMyClassObj[s_currentIndex] = result;
            XLLRTWarper.XLLRT_PushXLObject(luaState, "HelloBolt.MyClass", new IntPtr(s_currentIndex));
            s_currentIndex++;
            return 1;
        }

        private static IntPtr LuaMyClassFactoryInstance(IntPtr ud)
        {
            IntPtr pNULL = new IntPtr(0);
            return pNULL;
        }

        //要传递给C++的委托要注意生命周期管理，这里全部用静态生命周期管理
        protected static lua_CFunction _CreateInstance = new lua_CFunction(CreateMyClassInstance);
        protected static lua_CFunction _Add = new lua_CFunction(Add);
        protected static lua_CFunction _AttachResultListener = new lua_CFunction(AttachResultListener);
        protected static lua_CFunction _DeleteSelf = new lua_CFunction(DeleteSelf); 
        public static void RegisterClass()
        {
            //得到Lua环境句柄
            IntPtr pNULL = new IntPtr(0);
            IntPtr hEnv;
            hEnv = XLLRTWarper.XLLRT_GetEnv(pNULL);

            //注册MyClassFactory 工厂类，通常这个类只有一个方法就是创造MyClass 实例
            IntPtr pMember = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(XLLRTGlobalAPI))*2);
            XLLRTGlobalAPI pAPI = new XLLRTGlobalAPI();
            
            pAPI.funName = "CreateInstance";
            pAPI.permission =0;
            pAPI.func = _CreateInstance;//注意delegate对象的生命周期管理
            Marshal.StructureToPtr(pAPI, pMember, true);

            IntPtr pMember2 = new IntPtr( pMember.ToInt32() + Marshal.SizeOf(typeof(XLLRTGlobalAPI)));
            XLLRTGlobalAPI pAPI2 = new XLLRTGlobalAPI();
            pAPI.func = null;
            pAPI.funName = null;
            pAPI.permission = 0;
            Marshal.StructureToPtr(pAPI2, pMember2, true);

            fnGetObject pFun = new fnGetObject(LuaMyClassFactoryInstance);
            XLLRTObject theObject = new XLLRTObject();
            theObject.className = "HelloBolt.MyClass.Factory.Class";
            theObject.objName = "HelloBolt.MyClass.Factory";
            theObject.userData = pNULL;
            theObject.pfnGetObject = pFun;
            theObject.memberFunctions = pMember;

            //真正注册
            XLLRTWarper.XLLRT_RegisterGlobalObj(hEnv, theObject);

            //注册MyClass的类型信息
            IntPtr pMyClassMember = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(XLLRTGlobalAPI)) * 4);
            XLLRTGlobalAPI pMyClassAPI = new XLLRTGlobalAPI();
            pMyClassAPI.permission = 0;
            pMyClassAPI.funName = "Add";
            pMyClassAPI.func = _Add;//注意delegate对象的生命周期管理
            Marshal.StructureToPtr(pMyClassAPI, pMyClassMember, false);
            IntPtr pMyClassMember2 = new IntPtr(pMyClassMember.ToInt32() + Marshal.SizeOf(typeof(XLLRTGlobalAPI)));
            pMyClassAPI.funName = "AttachResultListener";
            pMyClassAPI.func = _AttachResultListener;//注意delegate对象的生命周期管理
            Marshal.StructureToPtr(pMyClassAPI, pMyClassMember2, false);
            IntPtr pMyClassMember3 = new IntPtr(pMyClassMember2.ToInt32() + Marshal.SizeOf(typeof(XLLRTGlobalAPI)));
            pMyClassAPI.funName = "__gc";
            pMyClassAPI.func = _DeleteSelf;//注意delegate对象的生命周期管理
            Marshal.StructureToPtr(pMyClassAPI, pMyClassMember3, false);
            IntPtr pMyClassMember4 = new IntPtr(pMyClassMember3.ToInt32() + Marshal.SizeOf(typeof(XLLRTGlobalAPI)));
            pMyClassAPI.func = null;
            pMyClassAPI.funName = null;
            Marshal.StructureToPtr(pMyClassAPI, pMyClassMember4, false);

            //真正注册
            XLLRTWarper.XLLRT_RegisterClass(hEnv,"HelloBolt.MyClass",pMyClassMember,null,0);
            
            //清理资源
            Marshal.FreeHGlobal(pMember);
            Marshal.FreeHGlobal(pMyClassMember);
        }
    }
}
