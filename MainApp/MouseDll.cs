using System ;
using System.Runtime.InteropServices ;

namespace MouseInterception
{
    class MouseDll
    {

        // IMPORTANT! The definitions here must be kept in sync with their C equivalents in api.hpp

        // NOTE: The first place DLL's are loaded from are the application directory.
        private const string DLL_NAME = "mouse.dll" ;

        public delegate void callbackDelegate( int callbackType , IntPtr pCallbackMsg ) ; // nb: pCallbackMsg is UTF8
        public const int CBTYPE_STARTED = 1 ;
        public const int CBTYPE_STOPPED = 2 ;
        public const int CBTYPE_FATAL_ERROR = 3 ;
        public const int CBTYPE_NEW_DEVICE = 10 ;

        [DllImport( @DLL_NAME , CallingConvention=CallingConvention.Cdecl )]
        [return: MarshalAs(UnmanagedType.BStr)]
        private static extern string open_api( callbackDelegate deleg , ref DebugConfig.ApiSettings pDebugSettings ) ;

        [DllImport( @DLL_NAME , CallingConvention=CallingConvention.Cdecl )]
        [return: MarshalAs(UnmanagedType.BStr)]
        private static extern string close_api() ;

        [DllImport( @DLL_NAME , CallingConvention=CallingConvention.Cdecl )]
        [return: MarshalAs(UnmanagedType.BStr)]
        private static extern string reload_config(
            ref AppConfig.ApiSettings pAppSettings ,
            [MarshalAs(UnmanagedType.LPArray)] AppConfig.ApiDevice[] pDevices , int nDevices ,
            [MarshalAs(UnmanagedType.LPArray)] AppConfig.ApiDeviceConfig[] pDeviceConfigs , int nDeviceConfigs ,
            [MarshalAs(UnmanagedType.LPArray)] AppConfig.ApiAppProfile[] pAppProfiles , int nAppProfiles ,
            [MarshalAs(UnmanagedType.LPArray)] AppConfig.ApiEvent[] pEvents , int nEvents ,
            [MarshalAs(UnmanagedType.LPArray)] AppConfig.ApiAction[] pActions , int nActions
        ) ;

        [DllImport( @DLL_NAME , CallingConvention=CallingConvention.Cdecl )]
        [return: MarshalAs(UnmanagedType.BStr)]
        private static extern string reload_debug_config( ref DebugConfig.ApiSettings pDebugSettings ) ;

        [DllImport( @DLL_NAME , CallingConvention=CallingConvention.Cdecl )]
        [return: MarshalAs(UnmanagedType.BStr)]
        private static extern string run_main_loop( out int pExitFlag ) ;

        public MouseDll( callbackDelegate cbDeleg )
        {
            // open the mouse API
            DebugConfig.ApiSettings debugSettings = Program.debugConfig.settings ;
            string errorMsg = open_api( cbDeleg , ref debugSettings ) ;
            if ( errorMsg != null )
                throw new Exception( errorMsg ) ;
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
            AppConfig.ApiSettings appSettings = Program.appConfig.settings ;
            AppConfig.ApiDevice[] devices = Program.appConfig.devices ;
            AppConfig.ApiDeviceConfig[] deviceConfigs = Program.appConfig.deviceConfigs ;
            AppConfig.ApiAppProfile[] appProfiles = Program.appConfig.appProfiles ;
            AppConfig.ApiEvent[] events = Program.appConfig.events ;
            AppConfig.ApiAction[] actions = Program.appConfig.actions ;
            string errorMsg = reload_config(
                ref appSettings ,
                devices , devices.Length ,
                deviceConfigs , deviceConfigs.Length ,
                appProfiles , appProfiles.Length ,
                events , events.Length ,
                actions , actions.Length
            ) ;
            if ( errorMsg != null )
                throw new Exception( errorMsg ) ;
        }

        public void reloadDebugConfig()
        {
            // reload the debug config
            DebugConfig.ApiSettings debugSettings = Program.debugConfig.settings ;
            string errorMsg = reload_debug_config( ref debugSettings ) ;
            if ( errorMsg != null )
                throw new Exception( errorMsg ) ;
        }

        public void runMainLoop( out int exitFlag )
        {
            // run the main loop
            string errorMsg = run_main_loop( out exitFlag ) ;
            if ( errorMsg != null )
                throw new Exception( errorMsg ) ;
        }

    }
}
