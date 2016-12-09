﻿using System ;
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
        private static extern string open_api(
            ref AppConfig.ApiSettings pAppSettings ,
            [MarshalAs(UnmanagedType.LPArray)] AppConfig.ApiDevice[] pDevices , int nDevices ,
            ref DebugConfig.ApiSettings pDebugSettings ,
            int initConsole
        ) ;

        [DllImport( @DLL_NAME , CallingConvention=CallingConvention.Cdecl )]
        [return: MarshalAs(UnmanagedType.BStr)]
        private static extern string close_api() ;

        [DllImport( @DLL_NAME , CallingConvention=CallingConvention.Cdecl )]
        [return: MarshalAs(UnmanagedType.BStr)]
        private static extern string reload_config(
            ref AppConfig.ApiSettings pAppSettings ,
            [MarshalAs(UnmanagedType.LPArray)] AppConfig.ApiDevice[] pDevices , int nDevices ,
            ref DebugConfig.ApiSettings pDebugSettings
        ) ;

        public MouseDll( bool initConsole )
        {
            // open the mouse API
            AppConfig.ApiSettings appSettings = Program.appConfig.settings ;
            AppConfig.ApiDevice[] devices = Program.appConfig.devices ;
            DebugConfig.ApiSettings debugSettings = Program.debugConfig.settings ;
            string errorMsg = open_api(
                ref appSettings ,
                devices , devices.Length ,
                ref debugSettings ,
                initConsole ? 1 : 0
            ) ;
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

        public void reloadConfig()
        {
            // reload the config
            AppConfig.ApiSettings appSettings = Program.appConfig.settings ;
            AppConfig.ApiDevice[] devices = Program.appConfig.devices ;
            DebugConfig.ApiSettings debugSettings = Program.debugConfig.settings ;
            string errorMsg = reload_config(
                ref appSettings ,
                devices , devices.Length ,
                ref debugSettings
            ) ;
            if ( errorMsg != null )
                throw new Exception( errorMsg ) ;
        }
    }
}
