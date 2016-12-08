#include <windows.h>
#include <cassert>
#include "utils.hpp"

using namespace std ;

// ---------------------------------------------------------------------

string
getErrorString( int errorCode )
{
    // get the WIN32 error message
    char buf[ 4*1024 ] = "" ;
    if ( errorCode != 0 )
    {
        DWORD nBytes = FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK ,
            NULL ,
            errorCode ,
            MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT) ,
            buf , sizeof(buf) ,
            NULL
        ) ;
    }
    if ( buf[0] != '\0' )
        return buf ;

    // just return the WIN32 error code
    sprintf_s( buf , sizeof(buf) , "WIN32 ERROR: %08lX" , errorCode ) ;
    return buf ;
}

string getLastErrorString() { return getErrorString( GetLastError() ) ; }

// ---------------------------------------------------------------------

string
toUtf8( const wchar_t* pStr , int len )
{
    if ( pStr == NULL || len == 0 )
        return "" ;
    assert( len > 0 || len == -1 ) ;

    // figure out how many UTF-8 characters we are going to get
    int nChars = WideCharToMultiByte( CP_UTF8 , 0 , pStr , len , NULL , 0 , NULL , NULL ) ;
    if ( len == -1 )
        -- nChars ;
    if ( nChars == 0 )
        return "" ;

    // convert the string to UTF-8
    // nb: slightly naughty to write directly into the string like this
    string buf ;
    buf.resize( nChars ) ;
    WideCharToMultiByte( CP_UTF8 , 0 , pStr , len , const_cast<char*>(buf.c_str()) , nChars , NULL , NULL ) ;

    return buf ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

wstring 
fromUtf8( const char* pStr , int len )
{
    if ( pStr == NULL || len == 0 )
        return L"" ;
    assert( len > 0 || len == -1 ) ;

    // figure out how many wide characters we are going to get 
    int nChars = MultiByteToWideChar( CP_UTF8 , 0 , pStr , len , NULL , 0 ) ; 
    if ( len == -1 )
        -- nChars ; 
    if ( nChars == 0 )
        return L"" ; 

    // convert the string from UTF-8
    // nb: slightly naughty to write directly into the string like this
    wstring buf ; 
    buf.resize( nChars ) ; 
    MultiByteToWideChar( CP_UTF8 , 0 , pStr , len , const_cast<wchar_t*>(buf.c_str()) , nChars ) ; 

    return buf ; 
}
