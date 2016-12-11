#include <ctime>

#include "globals.hpp"

using namespace std ;

// ---------------------------------------------------------------------

// FIXME! We changed MainApp from AnyCPU to x86, to simplify calling into the mouse DLL (which we build as 32-bit).
// We should really probably provide separate 32- and 64-bit builds :-/
#pragma comment( lib , "../interception/library/x86/interception.lib" )

// ---------------------------------------------------------------------

HMODULE ghInterceptionDll = NULL ;

DeviceTable gDeviceTable ;
DeviceConfigTable gDeviceConfigTable ;
InterceptionDeviceSet gUnknownDevices ;

PCALLBACKFN gpCallbackFn = NULL ;

wstring gLogFilename ;
ofstream gLogFile ;

// ---------------------------------------------------------------------

static StringSet gLogging ;

void initLogging( const wchar_t* pLogging )
{
    // initialize logging
    gLogging.clear() ;
    if ( pLogging == NULL )
        return ;
    for ( const wchar_t* p=pLogging ; ; )
    {
        const wchar_t* q = wcschr( p , L'|' ) ;
        if ( q == NULL )
        {
            gLogging.insert( toUtf8(p) ) ;
            break ;
        }
        gLogging.insert( toUtf8(p,q-p) ) ;
        p = q + 1 ;
    }
}

bool isLoggingEnabled( const string& s ) { return gLogging.find(s) != gLogging.end() ; }

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

// ---------------------------------------------------------------------

ostream&
operator<<( ostream& os , const exception& xcptn )
{
    // insert the exception
    os << xcptn.what() ;
    return os ;
}
