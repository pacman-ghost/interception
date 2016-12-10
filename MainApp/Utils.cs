using System ;
using System.Xml ;

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
