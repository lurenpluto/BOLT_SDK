using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Bolt
{
    class BoltWarper
    {
        public bool InitBolt(String boltPath)
        {
            XL_InitGraphicLib(0);
            XL_SetFreeTypeEnabled(1);
            XLUE_InitLoader(0);
            return true;
        }

        public void AddXARSearchPath(String newFolderPath)
        {
            XLUE_AddXARSearchPath(newFolderPath);
        }

        public void LoadXAR(String xarName)
        {
            XLUE_LoadXAR(xarName);
        }

        public static BoltWarper Instance()
        {
            if(theOne == null)
            {
                theOne = new BoltWarper();
            }
            return theOne;
        }

        static BoltWarper theOne = null;

        //仅仅暴露了例子需要的 C API,你可以根据实际需要添加
        [DllImport("XLGraphic.dll",EntryPoint="XL_InitGraphicLib")]
        static extern long XL_InitGraphicLib(int theParam);
        [DllImport("XLGraphic.dll",EntryPoint="XL_SetFreeTypeEnabled")]
        static extern long XL_SetFreeTypeEnabled(int isEnable);
        [DllImport("XLUE.dll",EntryPoint="XLUE_InitLoader")]
        protected static extern long XLUE_InitLoader(int theParam);
        [DllImport("XLUE.dll",CharSet=CharSet.Unicode, EntryPoint="XLUE_AddXARSearchPath")]
        protected static extern long XLUE_AddXARSearchPath(String theParam);
        [DllImport("XLUE.dll",CharSet=CharSet.Ansi,EntryPoint="XLUE_LoadXAR")]
        protected static extern long XLUE_LoadXAR(String xarName);
    }
}
