using System ;
using System.Runtime.InteropServices ;

namespace MouseInterception
{
    class MouseDll
    {
        // NOTE: The first place DLL's are loaded from are the application directory.
        private const string DLL_NAME = "mouse.dll" ;

        [DllImport( @DLL_NAME , CallingConvention=CallingConvention.Cdecl )]
        [return: MarshalAs(UnmanagedType.BStr)]
        private static extern string open_api( int initConsole ) ;

        [DllImport( @DLL_NAME , CallingConvention=CallingConvention.Cdecl )]
        [return: MarshalAs(UnmanagedType.BStr)]
        private static extern string close_api() ;

        public MouseDll( bool initConsole )
        {
            // open the mouse API
            string errorMsg = open_api( initConsole ? 1 : 0 ) ;
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
