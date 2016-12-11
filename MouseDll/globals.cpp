#include <set>

#include "globals.hpp"

using namespace std ;

// ---------------------------------------------------------------------

HMODULE ghInterceptionDll = NULL ;

DeviceTable gDeviceTable ;
DeviceConfigTable gDeviceConfigTable ;

PCALLBACKFN gpCallbackFn = NULL ;

wstring gLogFilename ;
ofstream gLogFile ;

// ---------------------------------------------------------------------

typedef set<string> StringSet ;
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
