/*
 * Copyright 2017 Taka Muraoka
 *
 * This file is part of the MouseInterception project.
 *
 * The MouseInterception project is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * The MouseInterception project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the MouseInterception project.  If not, see <http://www.gnu.org/licenses/>.
 */

using System ;
using System.Runtime.InteropServices ;

namespace MouseInterception
{
    class MouseDll
    {

        // This wraps mouse.dll, which is our interface to the Interception mouse filter driver.
        //  We pass configuration into the DLL, and it calls back into us when things happen.

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
        private static extern string open_api(
            callbackDelegate deleg ,
            [MarshalAs(UnmanagedType.LPWStr)] string pDebugConfigFilename
        ) ;

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
        private static extern string run_main_loop( out int pExitFlag ) ;

        private bool mIsLoaded = false ;

        public MouseDll( callbackDelegate cbDeleg , string debugConfigFilename )
        {
            // open the mouse API
            string errorMsg = open_api( cbDeleg , debugConfigFilename ) ;
            if ( errorMsg != null )
                throw new Exception( errorMsg ) ;
            mIsLoaded = true ;
            reloadConfig() ;
        }

        ~MouseDll()
        {
            // close the mouse API
            if ( mIsLoaded )
            {
                string errorMsg = close_api() ;
                if ( errorMsg != null )
                    throw new Exception( errorMsg ) ;
            }
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

        public void runMainLoop( out int exitFlag )
        {
            // run the main loop
            string errorMsg = run_main_loop( out exitFlag ) ;
            if ( errorMsg != null )
                throw new Exception( errorMsg ) ;
        }

    }
}
