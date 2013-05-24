using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HelloXLUE
{
   
    class MyClass
    {
        public delegate void OnAddFinishDelegate( int lhs, int rhs, int result);
        public event OnAddFinishDelegate OnAddFinish;

        public int Add(int lhs,int rhs)
        {
            int result = lhs + rhs;
            if(OnAddFinish  != null)
            {
                OnAddFinish(lhs, rhs, result);
            }
            return result;
        }

        public string myname;
    }
}
