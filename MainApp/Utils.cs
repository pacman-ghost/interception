using System ;
using System.Runtime.InteropServices ;
using System.Xml ;
using System.Text ;

namespace MouseInterception
{
    static class Utils
    {

        public static string getXmlChildVal( XmlNode xmlNode , string childNodeName , string defaultVal )
        {
            // get the child node's value
            string val = getOptStringXmlChildVal( xmlNode , childNodeName ) ;
            return (val != null) ? val : defaultVal ;
        }
        public static string getXmlChildVal( XmlNode xn , string s ) { return getXmlChildVal(xn,s,"") ; }

        public static int getXmlChildVal( XmlNode xmlNode , string childNodeName , int defaultVal )
        {
            // get the child node's value
            int? val = getOptIntXmlChildVal( xmlNode , childNodeName ) ;
            return (val != null) ? (int)val : defaultVal ;
        }

        public static bool getXmlChildVal( XmlNode xmlNode , string childNodeName , bool defaultVal )
        {
            // get the child node's value
            bool? val = getOptBoolXmlChildVal( xmlNode , childNodeName ) ;
            return (val != null) ? (bool)val : defaultVal ;
        }

        public static string getOptStringXmlChildVal( XmlNode xmlNode , string childNodeName )
        {
            // get the child node's value
            XmlNode childNode = xmlNode.SelectSingleNode( childNodeName ) ;
            if ( childNode != null )
                return childNode.InnerText ;
            return null ;
        }

        public static int? getOptIntXmlChildVal( XmlNode xmlNode , string childNodeName )
        {
            // get the child node's value
            XmlNode childNode = xmlNode.SelectSingleNode( childNodeName ) ;
            if ( childNode != null )
                return Int32.Parse( childNode.InnerText ) ;
            return null ;
        }

        public static bool? getOptBoolXmlChildVal( XmlNode xmlNode , string childNodeName )
        {
            // get the child node's value
            XmlNode childNode = xmlNode.SelectSingleNode( childNodeName ) ;
            if ( childNode != null )
                return Boolean.Parse( childNode.InnerText ) ;
            return null ;
        }

        public static string getXmlAttr( XmlNode xmlNode , string attrName , string defaultVal )
        {
            // get the attribute value
            XmlAttribute xmlAttr = xmlNode.Attributes[ attrName ] ;
            if ( xmlAttr != null )
                return xmlAttr.Value ;
            return defaultVal ;
        }

        public static int getXmlAttr( XmlNode xmlNode , string attrName , int defaultVal )
        {
            // get the attribute value
            XmlAttribute xmlAttr = xmlNode.Attributes[ attrName ] ;
            if ( xmlAttr != null )
                return Int32.Parse( xmlAttr.Value ) ;
            return defaultVal ;
        }

        public static IntPtr toUtf8( string val )
        {
            // convert the string to UTF8
            byte[] buf = Encoding.UTF8.GetBytes( val ) ;
            Array.Resize( ref buf , buf.Length+1 ) ;
            buf[buf.Length-1] = 0 ;
            IntPtr pUtf8Val = Marshal.AllocHGlobal( buf.Length ) ;
            Marshal.Copy( buf , 0 , pUtf8Val , buf.Length ) ;
            return pUtf8Val ;
        }

        public static string fromUtf8( IntPtr pVal )
        {
            // convert the string from UTF8
            int nChars = 0 ;
            while ( Marshal.ReadByte( pVal , nChars ) != 0 )
                ++ nChars ;
            byte[] buf = new byte[ nChars ] ;
            Marshal.Copy( pVal , buf , 0 , buf.Length ) ;
            return Encoding.UTF8.GetString( buf ) ;
        }

        public static bool? getKeyState( XmlNode xmlNode , string attrName )
        {
            // get the key state
            XmlAttribute xmlAttr = xmlNode.Attributes[ attrName ] ;
            if ( xmlAttr == null )
                return null ;
            string attrVal = xmlAttr.Value.Trim() ;
            if ( attrVal == "down" )
                return true ;
            if ( attrVal == "up" )
                return false ;
            throw new Exception( "Invalid key state: " + attrVal ) ;
        }
    }
}
