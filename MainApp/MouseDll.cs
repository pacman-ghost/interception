using System ;
using System.Runtime.InteropServices ;

namespace MouseInterception
{
    class MouseDll
    {

        [DllImport( @"mouse.dll" , CallingConvention=CallingConvention.Cdecl )]
        [return: MarshalAs(UnmanagedType.BStr)]
        private static extern string open_api() ;

        [DllImport( @"mouse.dll" , CallingConvention=CallingConvention.Cdecl )]
        [return: MarshalAs(UnmanagedType.BStr)]
        private static extern string close_api() ;

        public MouseDll()
        {
            // open the mouse API
            string errorMsg = open_api() ;
            if ( errorMsg != null )
                throw new Exception( errorMsg ) ;
        }

        ~MouseDll()
        {
            // close the mouse API
            string errorMsg = close_api() ;
            if ( errorMsg != null )
                throw new Exception( errorMsg ) ;
        }

    }
}
