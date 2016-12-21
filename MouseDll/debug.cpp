#include <windows.h>
#include <fstream>
#include <set>
#include <ctime>

#include "debug.hpp"
#include "globals.hpp"

using namespace std ;

// --- GLOBAL DATA -----------------------------------------------------

int gEventWaitTimeout = 200 ; // nb: milliseconds
int gMaxStrokeHistory = 20 ;
int gDefaultStrokeResetHistoryInterval = 100 ; // nb: milliseconds
int gDetectMouseMove_WindowSize = 10 ;
int gDetectMouseMove_HorzBias = 120 ; // nb: scaled by 100

// --- LOCAL DATA ------------------------------------------------------

ostream* gpLogStream = NULL ;
static ofstream gLogFile ;
static wstring gLogFilename = L"<unset>" ;

static StringSet gEnabledLogging ; // FIXME! make this case-insensitive

// ---------------------------------------------------------------------

void
loadDebugConfig( const wchar_t* pDebugConfigFilename )
{
    // NOTE: We have to do everything using wide strings, so that we can have a wide file path :-/

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
        if ( _wcsicmp( pLogFilename , L"console" ) == 0 || pLogFilename[0] == L'\0' )
        {
            // FIXME! When the main app calls AttachConsole(), we don't seem to be able to access cout/cerr separately.
            gpLogStream = & cout ;
        }
        else
        {
            gLogFile.open( pLogFilename ) ;
            gpLogStream = & gLogFile ;
        }
        gLogFilename = pLogFilename ;
    }

    // load the debug settings
    LOG_CMSG( "config" , "Debug settings (" << toUtf8(pDebugConfigFilename) << "):" ) ;
    gEventWaitTimeout = GetPrivateProfileInt( L"Debug" , L"EventWaitTimeout" , gEventWaitTimeout , pDebugConfigFilename ) ;
    LOG_CMSG( "config" , "- EventWaitTimeout = " << gEventWaitTimeout ) ;
    gMaxStrokeHistory = GetPrivateProfileInt( L"Debug" , L"MaxStrokeHistory" , gMaxStrokeHistory , pDebugConfigFilename ) ;
    LOG_CMSG( "config" , "- MaxStrokeHistory = " << gMaxStrokeHistory ) ;
    gDefaultStrokeResetHistoryInterval = GetPrivateProfileInt( L"Debug" , L"DefaultStrokeResetHistoryInterval" , gDefaultStrokeResetHistoryInterval , pDebugConfigFilename ) ;
    LOG_CMSG( "config" , "- DefaultStrokeResetHistory = " << gDefaultStrokeResetHistoryInterval ) ;
    gDetectMouseMove_WindowSize = GetPrivateProfileInt( L"Debug" , L"DetectMouseMove_WindowSize" , gDetectMouseMove_WindowSize , pDebugConfigFilename ) ;
    LOG_CMSG( "config" , "- DetectMouseMove_WindowSize = " << gDetectMouseMove_WindowSize ) ;
    gDetectMouseMove_HorzBias = GetPrivateProfileInt( L"Debug" , L"DetectMouseMove_HorzBias" , gDetectMouseMove_HorzBias , pDebugConfigFilename ) ;
    LOG_CMSG( "config" , "- DetectMouseMove_HorzBias = " << (float)gDetectMouseMove_HorzBias/100 ) ;
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
            strftime( buf2 , ARRAY_SIZE(buf2) , "%d/%b/%y %H:%M:%S" , &tmNow ) ;
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
