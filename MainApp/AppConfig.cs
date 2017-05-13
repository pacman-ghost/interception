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
using System.IO ;
using System.Xml ;
using System.Runtime.InteropServices ;
using System.Collections.Generic ;

namespace MouseInterception
{
    class AppConfig
    {

        // IMPORTANT! The definitions here must be kept in sync with their C equivalents in api.hpp
        // FUDGE: Marshaling multiple levels of nested arrays of struct's proved to be tricky. C# seems to be
        // able to handle it, but unpacking it on the C++ side was fiddly. Since we can't simply get
        // a plain old pointer to an array (since they're managed objects), and we have to marshal arrays ByVal,
        // defining C-struct's was impossible (one level of nesting was OK, but no deeper). We could have maybe
        // done some trickery by manipulating pointers and using sizeof(), but it's risky since there are
        // alignment issues. It's safer to send through a separate array for each type of struct, and then
        // a start index & item count specifying which ones are required. Sigh...

        public enum KeyModifiers { ctrl=0x0001 , alt=0x0002 , shift=0x0004 , caps=0x0008 }

        // --- SETTINGS: application settings ---
        [StructLayout( LayoutKind.Sequential , CharSet=CharSet.Unicode , Pack=1 )]
        public struct ApiSettings
        {
        }
        private ApiSettings mSettings ;
        public ApiSettings settings { get { return mSettings ; } }

        // --- DEVICE: device attributes ---
        [StructLayout( LayoutKind.Sequential , CharSet=CharSet.Unicode , Pack=1 )]
        public struct ApiDevice
        {
            public int mDeviceId ;
            public string mHID ;
            public int mDeviceNumber ;
            public string mDisplayName ;
            public bool mIsEnabled ;
        }
        private ApiDevice[] mDevices ;
        public ApiDevice[] devices { get { return mDevices ; } }

        // --- DEVICE CONFIG: device configuration ---
        [StructLayout( LayoutKind.Sequential , CharSet=CharSet.Unicode , Pack=1 )]
        public struct ApiDeviceConfig
        {
            public int mDeviceId ;
            public int mStrokeHistoryResetInterval ;
            public int mAppProfileStartIndex ;
            public int mAppProfileCount ;
        }
        private ApiDeviceConfig[] mDeviceConfigs ;
        public ApiDeviceConfig[] deviceConfigs { get { return mDeviceConfigs ; } }

        // --- APP PROFILE: device configuration for an application ---
        [StructLayout( LayoutKind.Sequential , CharSet=CharSet.Unicode , Pack=1 )]
        public struct ApiAppProfile
        {
            public string mApp ;
            public int mSensitivityX ;
            public int mSensitivityY ;
            public bool mFallbackToDefaultAppProfile ;
            public int mEventStartIndex ;
            public int mEventCount ;
        }
        private ApiAppProfile[] mAppProfiles ;
        public ApiAppProfile[] appProfiles { get { return mAppProfiles ; } }

        // --- API EVENT: device event ---
        [StructLayout( LayoutKind.Sequential , CharSet=CharSet.Unicode , Pack=1 )]
        public struct ApiEvent
        {
            public enum EventType {
                mouseLeft=1 , mouseRight=2 , mouseUp=3 , mouseDown=4 ,
                wheelLeft=5 , wheelRight=6 , wheelUp=7 , wheelDown=8
            }
            public int mEventType ;
            public int mKeyModifiers ;
            public int mActionStartIndex ;
            public int mActionCount ;
        }
        private ApiEvent[] mEvents ;
        public ApiEvent[] events { get { return mEvents ; } }

        // --- API ACTION: action to perform in response to a device event ---
        [StructLayout( LayoutKind.Sequential , CharSet=CharSet.Unicode , Pack=1 )]
        public struct ApiAction
        {
            public enum ActionType {
                mouseLeft=1 , mouseRight=2 , mouseUp=3 , mouseDown=4 ,
                wheelLeft=5 , wheelRight=6 , wheelUp=7 , wheelDown=8 ,
                keyPress=10
            }
            public int mActionType ;
            public int mActionParam ;
            public int mKeyModifiers ;
        }
        public ApiAction[] mActions ;
        public ApiAction[] actions { get { return mActions ; } }

        // ------------------------------------------------------------

        public AppConfig( string fname )
        {
            // initialize
            HashSet<int> deviceIds = new HashSet<int>() ;
            HashSet<int> deviceNumbers = new HashSet<int>() ;
            mSettings = new ApiSettings() ;
            mDevices = new ApiDevice[ 0 ] ;
            mDeviceConfigs = new ApiDeviceConfig[ 0 ] ;
            mAppProfiles = new ApiAppProfile[ 0 ] ;
            mEvents = new ApiEvent[ 0 ] ;
            mActions = new ApiAction[ 0 ] ;

            // load the AppConfig
            if ( ! File.Exists( fname ) )
                return ; // FIXME! can't do this - we have to notify the DLL
            XmlReaderSettings xmlReaderSettings = new XmlReaderSettings() ;
            xmlReaderSettings.IgnoreComments = true ;
            xmlReaderSettings.IgnoreProcessingInstructions = true ;
            xmlReaderSettings.IgnoreWhitespace = true ;
            XmlDocument xmlDoc = new XmlDocument() ;
            using ( XmlReader xmlReader = XmlReader.Create( fname , xmlReaderSettings ) )
                xmlDoc.Load( xmlReader ) ;

            // parse the app config
            XmlNode configXmlNode = xmlDoc.SelectSingleNode( "/config" ) ;

            // parse the devices
            List<ApiDevice> devices = new List<ApiDevice>() ;
            foreach( XmlNode xn in configXmlNode.SelectNodes("device") )
            {
                ApiDevice device = new ApiDevice() ;
                device.mDeviceId = Int32.Parse( xn.Attributes["id"].Value ) ;
                if ( deviceIds.Contains( device.mDeviceId ) )
                    throw new Exception( String.Format( "Duplicate device ID: {0}" , device.mDeviceId ) ) ;
                deviceIds.Add( device.mDeviceId ) ;
                device.mHID = xn.SelectSingleNode("hid").InnerText.Trim() ;
                device.mDeviceNumber = Int32.Parse( xn.SelectSingleNode("deviceNumber").InnerText ) ;
                if ( deviceNumbers.Contains( device.mDeviceNumber ) )
                    throw new Exception( String.Format( "Duplicate device number: {0}" , device.mDeviceNumber ) ) ;
                deviceNumbers.Add( device.mDeviceNumber ) ;
                if ( deviceNumbers.Contains ( device.mDeviceNumber ) )
                device.mDisplayName = Utils.getXmlChildVal( xn , "displayName" ) ;
                device.mIsEnabled = Utils.getXmlChildVal( xn , "enabled" , false ) ;
                devices.Add( device ) ;
            }
            mDevices = devices.ToArray() ;

            // parse the device configs
            List<ApiDeviceConfig> deviceConfigs = new List<ApiDeviceConfig>() ;
            List<ApiAppProfile> appProfiles = new List<ApiAppProfile>() ;
            List<ApiEvent> events = new List<ApiEvent>() ;
            List<ApiAction> actions = new List<ApiAction>() ;
            foreach( XmlNode deviceConfigXmlNode in configXmlNode.SelectNodes("deviceConfig") )
            {
                ApiDeviceConfig deviceConfig = new ApiDeviceConfig() ;
                deviceConfig.mDeviceId = Int32.Parse( deviceConfigXmlNode.Attributes["id"].Value ) ;
                deviceConfig.mStrokeHistoryResetInterval = Utils.getXmlChildVal( deviceConfigXmlNode , "strokeHistoryResetInterval" , -1 ) ;
                deviceConfig.mAppProfileStartIndex = appProfiles.Count ;
                deviceConfig.mAppProfileCount = 0 ;
                // parse the app profiles
                foreach( XmlNode appProfileXmlNode in deviceConfigXmlNode.SelectNodes("appProfile") )
                {
                    ApiAppProfile appProfile = new ApiAppProfile() ;
                    appProfile.mApp = Utils.getXmlAttr( appProfileXmlNode , "app" , "" ) ;
                    XmlNode xn = appProfileXmlNode.SelectSingleNode( "sensitivity" ) ;
                    if ( xn != null )
                    {
                        appProfile.mSensitivityX = Utils.getXmlAttr( xn , "x" , 0 ) ;
                        appProfile.mSensitivityY = Utils.getXmlAttr( xn , "y" , 0 ) ;
                    }
                    appProfile.mFallbackToDefaultAppProfile = Utils.getXmlChildVal( appProfileXmlNode , "fallbackToDefaultAppProfile" , false ) ;
                    appProfile.mEventStartIndex = events.Count ;
                    appProfile.mEventCount = 0 ;
                    // parse the events
                    foreach( XmlNode eventXmlNode in appProfileXmlNode.SelectNodes("event" ) )
                    {
                        ApiEvent evt = new ApiEvent() ;
                        evt.mEventType = (int) Enum.Parse( typeof(ApiEvent.EventType) , eventXmlNode.Attributes["type"].Value , true ) ;
                        evt.mKeyModifiers = getKeyModifiers( eventXmlNode ) ;
                        // parse the actions
                        evt.mActionStartIndex = actions.Count ;
                        evt.mActionCount = 0 ;
                        foreach( XmlNode actionXmlNode in eventXmlNode.SelectNodes("action") )
                        {
                            ApiAction action = new ApiAction() ;
                            action.mActionType = (int) Enum.Parse( typeof(ApiAction.ActionType) , actionXmlNode.Attributes["type"].Value , true ) ;
                            if ( action.mActionType == (int)ApiAction.ActionType.keyPress )
                            {
                                action.mActionParam = Utils.getXmlAttr( actionXmlNode , "vKey" , 0 ) ;
                                if ( action.mActionParam <= 0 )
                                    throw new Exception( "Missing vKey." ) ;
                            }
                            else
                                action.mActionParam = Utils.getXmlAttr( actionXmlNode , "speed" , 0 ) ;
                            action.mKeyModifiers = getKeyModifiers( actionXmlNode ) ;
                            actions.Add( action ) ;
                            evt.mActionCount ++ ;
                        }
                        events.Add( evt ) ;
                        appProfile.mEventCount ++ ;
                    }
                    // add the app profile to the table
                    appProfiles.Add( appProfile ) ;
                    deviceConfig.mAppProfileCount ++ ;
                }
                // add the device config to the table
                deviceConfigs.Add( deviceConfig ) ;
            }
            mDeviceConfigs = deviceConfigs.ToArray() ;
            mAppProfiles = appProfiles.ToArray() ;
            mEvents = events.ToArray() ;
            mActions = actions.ToArray() ;
        }

        private static int getKeyModifiers( XmlNode xmlNode )
        {
            // get the key modifiers
            int keyModifierFlags = 0 ;
            string keyModifiers = Utils.getXmlAttr( xmlNode , "keyModifiers" , "" ) ;
            if ( keyModifiers.Length > 0 )
            {
                foreach( string km in keyModifiers.Split('+') )
                    keyModifierFlags += (int) Enum.Parse( typeof(KeyModifiers) , km.ToLower() , true ) ;
            }
            return keyModifierFlags ;
        }

    }
}
