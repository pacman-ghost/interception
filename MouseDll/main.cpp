#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cassert>
#include "main.hpp"
#include "utils.hpp"

using namespace std ;

// --- LOCAL DATA ------------------------------------------------------

static HMODULE ghInterceptionDll = NULL ;

static bool gEnableConsole = false ;
static wstring gLogFilename ;
static ofstream gLogFile ;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define LOG_MSG( msg ) \
{ \
    if ( gEnableConsole || gLogFile.is_open() ) \
    { \
        string _buf_ = MAKE_STRING( msg ) ; \
        if ( gEnableConsole ) \
            cout << _buf_ << endl ; \
        if ( gLogFile.is_open() ) \
            gLogFile << _buf_ << endl ; \
    } \
}

// ---------------------------------------------------------------------

void
openApi( bool initConsole )
{
    // check if we are open
    if ( ghInterceptionDll != NULL )
        throw runtime_error( "API is already open." ) ;

    // initialize
    gEnableConsole = initConsole ;

    // load Interception
    wchar_t buf[ _MAX_PATH+1 ] ;
    GetModuleFileName( NULL , buf , ARRAY_SIZE(buf) ) ;
    wchar_t drive[_MAX_DIR+1] , dir[_MAX_DIR+1] , fname[_MAX_FNAME+1] , extn[_MAX_EXT+1] ;
    errno_t rc = _wsplitpath_s( buf , drive , ARRAY_SIZE(drive) , dir , ARRAY_SIZE(dir) , fname , ARRAY_SIZE(fname) , extn , ARRAY_SIZE(extn) ) ;
    if ( rc == 0 )
        _wmakepath_s( buf , ARRAY_SIZE(buf) , drive , dir , L"interception" , L".dll" ) ;
    else
    {
        assert( false ) ;
        wcscpy_s( buf , ARRAY_SIZE(buf) , L"interception.dll" ) ;
    }
    LOG_MSG( "Loading Interception: " << toUtf8(buf) ) ;
    ghInterceptionDll = LoadLibrary( buf ) ;
    if ( ghInterceptionDll == NULL )
        throw runtime_error( MAKE_STRING( "Can't load Interception: " << getLastErrorString() ) ) ;
}

// ---------------------------------------------------------------------

void
closeApi()
{
    // check if we are open
    if ( ghInterceptionDll == NULL )
        throw runtime_error( "API is not open." ) ;

    // close Interception
    FreeLibrary( ghInterceptionDll ) ;
    ghInterceptionDll = NULL ;
}

// ---------------------------------------------------------------------

void
reloadConfig( const DebugConfig* pDebugConfig )
{
    // validate the parameters
    if ( pDebugConfig == NULL )
        throw runtime_error( "Missing DebugConfig." ) ;
    const wchar_t* pLogFilename = pDebugConfig->mpLogFilename ;
    if ( pLogFilename == NULL )
        pLogFilename = L"" ;

    // initialize the log file
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
