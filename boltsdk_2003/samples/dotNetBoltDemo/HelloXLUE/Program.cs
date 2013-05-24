using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using Bolt;

namespace HelloXLUE
{
    static class Program
    {
        class AddOb
        {
            public void OnAddFinish(int lhs,int rhs,int result)
            {
                Console.WriteLine("{0} + {1} = {2}", lhs, rhs, result);
            }
        }
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            
            //这个Class是等会要注册到Lua环境里的，这里演示了一下这个对象的主要接口的功能
            MyClass theClass = new MyClass();
            AddOb theOb = new AddOb();
            theClass.OnAddFinish += theOb.OnAddFinish;
            theClass.Add(100, 200);
            
            BoltWarper.Instance().InitBolt("");

            LuaMyClass.RegisterClass();
            string searPath = System.Windows.Forms.Application.StartupPath + "\\..\\samples\\HelloBolt\\XAR\\";
            BoltWarper.Instance().AddXARSearchPath(searPath);
            BoltWarper.Instance().LoadXAR("HelloBolt7");



            Application.Run();
        }
    }
}
