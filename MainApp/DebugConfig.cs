using System.IO ;
using System.Xml ;
using System.Runtime.InteropServices ;

namespace MouseInterception
{
    class DebugConfig
    {

        [StructLayout( LayoutKind.Sequential , CharSet=CharSet.Unicode , Pack=1 )]
        public struct Settings
        {
            public string mLogFilename ;
        }

        private Settings mSettings ;
        public Settings settings { get { return mSettings ; } }

        public DebugConfig( string fname )
        {
            // load the DebugConfig
            mSettings = new Settings() ;
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
