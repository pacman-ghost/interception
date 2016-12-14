#include <windows.h>
#include <set>
#include <ctime>

#include "debug.hpp"
#include "globals.hpp"

using namespace std ;

// --- LOCAL DATA ------------------------------------------------------

ofstream gLogFile ;
static wstring gLogFilename ;

static StringSet gEnabledLogging ; // FIXME! make this case-insensitive

// ---------------------------------------------------------------------

void
loadDebugConfig( const wchar_t* pDebugConfigFilename )
{
    // initialize logging
    gEnabledLogging.clear() ;
    wchar_t buf[ 4*1024 ] ;
    GetPrivateProfileString( L"Logging" , NULL , NULL , buf , ARRAY_SIZE(buf) , pDebugConfigFilename ) ;
    for ( const wchar_t* p=buf ; *p != L'\0' ; p+=wcslen(p)+1 )
    {
        if ( GetPrivateProfileInt( L"Logging" , p , false , pDebugConfigFilename ) )
            gEnabledLogging.insert( toUtf8(p) ) ;
    }

    // initialize the log file
    GetPrivateProfileString( L"Logging" , L"LogFilename" , L"" , buf , ARRAY_SIZE(buf) , pDebugConfigFilename ) ;
    const wchar_t* pLogFilename = buf ;
    if ( _wcsicmp( gLogFilename.c_str() , pLogFilename ) != 0 )
    {
        if ( gLogFile.is_open() )
            gLogFile.close() ;
        if ( pLogFilename[0] != L'\0' )
        {
            gLogFile.open( pLogFilename ) ;
            gLogFilename = pLogFilename ;
        }
    }
}

// ---------------------------------------------------------------------

string
makeLogMsg( const string& msg )
{
    // prepare the log message
    stringstream buf ;
    const char* p = msg.c_str() ;
    for ( ; ; )
    {
        // output the timestamp/tab
        if ( p == msg.c_str() )
        {
            char buf2[80] ;
            time_t now = time( NULL ) ;
            struct tm tmNow ;
            (void) localtime_s( &tmNow , &now ) ;
            strftime( buf2 , sizeof(buf2) , "%d/%b/%y %H:%M:%S" , &tmNow ) ;
            for ( int i=0 ; buf2[i] != ' ' ; ++i )
                buf2[i] = toupper( buf2[i] ) ;
            buf << buf2 ;
        }
        else
            buf << "                  " ;
        buf << " | " ;

        // output the next line
        const char* q = strchr( p , '\n' ) ;
        if ( q == NULL )
        {
            buf << p << endl ;
            break ; // nb: this was the last line
        }
        buf.write( p , q-p ) ;
        buf << endl ;
        p = q + 1 ;
        if ( *p == '\0' )
            break ;
    }

    return buf.str() ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool isLoggingEnabled( const string& s ) { return gEnabledLogging.find(s) != gEnabledLogging.end() ; }
