#include <windows.h>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include "main.hpp"
#include "utils.hpp"


using namespace std ;

// --- LOCAL DATA ------------------------------------------------------

static HMODULE ghInterceptionDll = NULL ;

static bool gEnableConsole = false ;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define LOG_MSG( msg ) \
{ \
    if ( gEnableConsole ) \
        cout << msg << endl ; \
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
