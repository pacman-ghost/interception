/*
 * Copyright 2017 Taka Muraoka
 *
 * This file is part of the MouseInterception project.
 *
 * The MouseInterception project is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * The MouseInterception project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the MouseInterception project.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <limits>

#include "utils.hpp"
#include "globals.hpp"

using namespace std ;

// ---------------------------------------------------------------------

const char*
enumString( const EnumStringInfo* pEnumStringTable , int val )
{
    // return the enum value's display string
    for ( const EnumStringInfo* pInfo=pEnumStringTable ; pInfo->mpName != NULL ; ++pInfo )
    {
        if ( pInfo->mVal == val )
            return pInfo->mpName ;
    }
    assert( false ) ;
    return "???" ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

string
bitFlagsString( const BitFlagsInfo* pBitFlagsTable , int val , char sep )
{
    // convert the bit flags to a string
    vector<const char*> bitFlags ;
    for ( const BitFlagsInfo* pInfo=pBitFlagsTable ; pInfo->mpName != NULL ; ++pInfo )
    {
        if ( val & pInfo->mFlag )
            bitFlags.push_back( pInfo->mpName ) ;
    }
    stringstream buf ;
    for ( size_t i=0 ; i < bitFlags.size() ; ++i )
    {
        if ( i > 0 )
            buf << sep ;
        buf << bitFlags[i] ;
    }
    return buf.str() ;
}

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

// ---------------------------------------------------------------------

template<class T>
string
hexString( T val , int fieldWidth )
{
    // convert the numeric value to a hex string
    stringstream buf ;
    if ( fieldWidth < 0 )
        fieldWidth = (numeric_limits<T>::digits + 3) / 4 ;
    buf.width( fieldWidth ) ;
    buf.fill( '0' ) ;
    buf << hex << val ;
    string bufVal = buf.str() ;
    char* p = const_cast<char*>( bufVal.c_str() ) ;
    for ( ; *p != '\0' ; ++p )
        *p = toupper( *p ) ;
    return bufVal ;
}

template string hexString<int>( int , int ) ; // nb: instantiate the template
template string hexString<unsigned int>( unsigned int , int ) ; // nb: instantiate the template
template string hexString<short>( short , int ) ; // nb: instantiate the template
template string hexString<unsigned short>( unsigned short , int ) ; // nb: instantiate the template
template string hexString<long>( long , int ) ; // nb: instantiate the template
template string hexString<unsigned long>( unsigned long , int ) ; // nb: instantiate the template
template string hexString<void*>( void* , int ) ; // nb: instantiate the template
