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

        [StructLayout( LayoutKind.Sequential , CharSet=CharSet.Unicode , Pack=1 )]
        public struct ApiSettings
        {
        }

        private ApiSettings mSettings ;
        public ApiSettings settings { get { return mSettings ; } }

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

        public AppConfig( string fname )
        {
            // initialize
            mSettings = new ApiSettings() ;
            mDevices = new ApiDevice[ 0 ] ;

            // load the AppConfig
            if ( ! File.Exists( fname ) )
                return ;
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
            foreach ( XmlNode xn in configXmlNode.SelectNodes("device") )
            {
                ApiDevice device = new ApiDevice() ;
                device.mDeviceId = Int32.Parse( xn.Attributes["id"].Value ) ;
                device.mHID = xn.SelectSingleNode("hid").InnerText.Trim() ;
                device.mDeviceNumber = Int32.Parse( xn.SelectSingleNode("deviceNumber").InnerText ) ;
                device.mDisplayName = Utils.getXmlChildNodeVal( xn , "displayName" ) ;
                device.mIsEnabled = Utils.getXmlChildNodeVal( xn , "enabled" , false ) ;
                devices.Add( device ) ;
            }
            mDevices = devices.ToArray() ;
        }

    }
}
