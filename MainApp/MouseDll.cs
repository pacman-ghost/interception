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

        [DllImport( @DLL_NAME , CallingConvention=CallingConvention.Cdecl )]
        [return: MarshalAs(UnmanagedType.BStr)]
        private static extern string reload_config( ref AppConfig.Settings pAppSettings , ref DebugConfig.Settings pDebugSettings ) ;

        public MouseDll( bool initConsole )
        {
            // open the mouse API
            string errorMsg = open_api( initConsole ? 1 : 0 ) ;
            if ( errorMsg != null )
                throw new Exception( errorMsg ) ;
            // initialize
            reloadConfig() ;
        }

        ~MouseDll()
        {
            // close the mouse API
            string errorMsg = close_api() ;
            if ( errorMsg != null )
                throw new Exception( errorMsg ) ;
        }

        public void reloadConfig()
        {
            // reload the config
            AppConfig.Settings appSettings = Program.appConfig.settings ;
            DebugConfig.Settings debugSettings = Program.debugConfig.settings ;
            string errorMsg = reload_config( ref appSettings , ref debugSettings ) ;
            if ( errorMsg != null )
                throw new Exception( errorMsg ) ;
        }
    }
}
