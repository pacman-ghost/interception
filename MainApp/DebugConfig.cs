using System ;
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
            public string mLogging ;
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
            XmlNode debugXmlNode = xmlDoc.SelectSingleNode( "/debug" ) ;
            XmlNode xmlNode = debugXmlNode.SelectSingleNode( "logFilename" ) ;
            if ( xmlNode != null )
                mSettings.mLogFilename = xmlNode.InnerText.Trim() ;
            mSettings.mLogging = "" ;
            xmlNode = debugXmlNode.SelectSingleNode( "logging" ) ;
            if ( xmlNode != null )
            {
                foreach( XmlAttribute xa in xmlNode.Attributes )
                {
                    if ( Boolean.Parse( xa.Value ) )
                    {
                        if ( mSettings.mLogging.Length > 0 )
                            mSettings.mLogging += "|" ;
                        mSettings.mLogging += xa.Name.ToLower() ;
                    }
                }
            }
        }

    }
}
