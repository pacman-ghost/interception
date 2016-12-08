#include <stdexcept>
#include <windows.h>
#include "main.hpp"
#include "utils.hpp"

using namespace std ;

// ---------------------------------------------------------------------

extern "C" __declspec(dllexport) BSTR
open_api( const AppConfig* pAppConfig , const DebugConfig* pDebugConfig , int initConsole )
{
    // open the API
    try
    {
        openApi( pAppConfig , pDebugConfig , initConsole != 0 ) ;
        return NULL ;
    }
    catch ( exception& xcptn )
    {
        return SysAllocString( fromUtf8( MAKE_STRING( xcptn.what() ) ).c_str() ) ;
    }
}

// ---------------------------------------------------------------------

extern "C" __declspec(dllexport) BSTR
close_api()
{
    // close the API
    try
    {
        closeApi() ;
        return NULL ;
    }
    catch ( exception& xcptn )
    {
        return SysAllocString( fromUtf8( MAKE_STRING( xcptn.what() ) ).c_str() ) ;
    }
}

// ---------------------------------------------------------------------

extern "C" __declspec(dllexport) BSTR
reload_config( const AppConfig* pAppConfig , const DebugConfig* pDebugConfig )
{
    // reload the config
    try
    {
        reloadConfig( pAppConfig , pDebugConfig ) ;
        return NULL ;
    }
    catch ( exception& xcptn )
    {
        return SysAllocString( fromUtf8( MAKE_STRING( xcptn.what() ) ).c_str() ) ;
    }
}
