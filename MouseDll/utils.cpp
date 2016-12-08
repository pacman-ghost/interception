#include <windows.h>
#include <cassert>
#include "utils.hpp"

using namespace std ;

// ---------------------------------------------------------------------

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
