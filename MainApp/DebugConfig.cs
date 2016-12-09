using System.IO ;
using System.Xml ;
using System.Runtime.InteropServices ;

namespace MouseInterception
{
    class DebugConfig
    {

        // IMPORTANT! The definitions here must be kept in sync with their C equivalents in api.hpp

        [StructLayout( LayoutKind.Sequential , CharSet=CharSet.Unicode , Pack=1 )]
        public struct ApiSettings
        {
            public string mLogFilename ;
        }

        private ApiSettings mSettings ;
        public ApiSettings settings { get { return mSettings ; } }

        public DebugConfig( string fname )
        {
            // load the DebugConfig
            mSettings = new ApiSettings() ;
            if ( ! File.Exists( fname ) )
                return ;
            XmlReaderSettings xmlReaderSettings = new XmlReaderSettings() ;
            xmlReaderSettings.IgnoreComments = true ;
            xmlReaderSettings.IgnoreProcessingInstructions = true ;
            xmlReaderSettings.IgnoreWhitespace = true ;
            XmlDocument xmlDoc = new XmlDocument() ;
            using ( XmlReader xmlReader = XmlReader.Create( fname , xmlReaderSettings ) )
                xmlDoc.Load( xmlReader ) ;

            // parse the values
            XmlNode configXmlNode = xmlDoc.SelectSingleNode( "/debug" ) ;
            XmlNode xmlNode = configXmlNode.SelectSingleNode( "logFilename" ) ;
            if ( xmlNode != null )
                mSettings.mLogFilename = xmlNode.InnerText.Trim() ;
        }

    }
}
