using System ;
using System.Runtime.InteropServices ;

namespace MouseInterception
{
    class MouseDll
    {
        // IMPORTANT! The definitions here must be kept in sync with their C equivalents in api.hpp

        // NOTE: The first place DLL's are loaded from are the application directory.
        private const string DLL_NAME = "mouse.dll" ;

        [DllImport( @DLL_NAME , CallingConvention=CallingConvention.Cdecl )]
        [return: MarshalAs(UnmanagedType.BStr)]
        private static extern string open_api( ref DebugConfig.ApiSettings pDebugSettings , int initConsole ) ;

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

        public MouseDll( bool initConsole )
        {
            // open the mouse API
            DebugConfig.ApiSettings debugSettings = Program.debugConfig.settings ;
            string errorMsg = open_api( ref debugSettings , initConsole?1:0 ) ;
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

    }
}
