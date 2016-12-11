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
            XmlNode childNode = xmlNode.SelectSingleNode( childNodeName ) ;
            if ( childNode != null )
                return childNode.InnerText.Trim() ;
            return defaultVal ;
        }
        public static string getXmlChildVal( XmlNode xn , string s ) { return getXmlChildVal(xn,s,"") ; }

        public static bool getXmlChildVal( XmlNode xmlNode , string childNodeName , bool defaultVal )
        {
            // get the child node's value
            string val = getXmlChildVal( xmlNode , childNodeName , null ) ;
            if ( val != null )
                return Boolean.Parse( val ) ;
            return defaultVal ;
        }

        public static string getXmlAttr( XmlNode xmlNode , string attrName , string defaultVal )
        {
            // get the attribute value
            XmlAttribute xmlAttr = xmlNode.Attributes[ attrName ] ;
            if ( xmlAttr != null )
                return xmlAttr.Value ;
            return defaultVal ;
        }
        public static string getXmlAttr( XmlNode xn , string s ) { return getXmlAttr(xn,s,"") ; }

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
