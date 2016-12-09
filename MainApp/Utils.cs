using System ;
using System.Xml ;

namespace MouseInterception
{
    static class Utils
    {

        public static string getXmlChildNodeVal( XmlNode xmlNode , string childNodeName , string defaultVal )
        {
            // get the child node's value
            XmlNode childNode = xmlNode.SelectSingleNode( childNodeName ) ;
            if ( childNode == null )
                return defaultVal ;
            return childNode.InnerText.Trim() ;
        }
        public static string getXmlChildNodeVal( XmlNode xn , string s ) { return getXmlChildNodeVal(xn,s,"") ; }

        public static bool getXmlChildNodeVal( XmlNode xmlNode , string childNodeName , bool defaultVal )
        {
            // get the child node's value
            string val = getXmlChildNodeVal( xmlNode , childNodeName , null ) ;
            if ( val == null )
                return defaultVal ;
            return Boolean.Parse( val ) ;
        }

    }
}
