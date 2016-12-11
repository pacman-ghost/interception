#include <stdexcept>
#include "api.hpp"
#include "globals.hpp"
#include "utils.hpp"

using namespace std ;

// ---------------------------------------------------------------------

extern "C" __declspec(dllexport) BSTR
open_api( PCALLBACKFN pCallbackFn , const ApiDebugConfig* pDebugConfig )
{
    // open the API
    try
    {
        openApi( pCallbackFn , pDebugConfig ) ;
        return NULL ;
    }
    catch ( exception& xcptn )
    {
        return SysAllocString( fromUtf8( MAKE_STRING(xcptn) ).c_str() ) ;
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
        return SysAllocString( fromUtf8( MAKE_STRING(xcptn) ).c_str() ) ;
    }
}

// ---------------------------------------------------------------------

extern "C" __declspec(dllexport) BSTR
reload_config(
    const ApiAppConfig* pAppConfig ,
    const ApiDevice* pDevices , int nDevices ,
    const ApiDeviceConfig* pDeviceConfigs , int nDeviceConfigs ,
    const ApiAppProfile* pAppProfiles , int nAppProfiles ,
    const ApiEvent* pEvents , int nEvents ,
    const ApiAction* pActions , int nActions
)
{
    // reload the config
    try
    {
        reloadConfig(
            pAppConfig ,
            pDevices , nDevices ,
            pDeviceConfigs , nDeviceConfigs ,
            pAppProfiles , nAppProfiles ,
            pEvents , nEvents ,
            pActions , nActions
        ) ;
        return NULL ;
    }
    catch ( exception& xcptn )
    {
        return SysAllocString( fromUtf8( MAKE_STRING(xcptn) ).c_str() ) ;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

extern "C" __declspec(dllexport) BSTR
reload_debug_config( const ApiDebugConfig* pDebugConfig )
{
    // reload the debug config
    try
    {
        reloadDebugConfig( pDebugConfig ) ;
        return NULL ;
    }
    catch ( exception& xcptn )
    {
        return SysAllocString( fromUtf8( MAKE_STRING(xcptn) ).c_str() ) ;
    }
}

// ---------------------------------------------------------------------

extern "C" __declspec(dllexport) BSTR
run_main_loop( int* pExitFlag )
{
    // run the main loop
    try
    {
        runMainLoop( pExitFlag ) ;
        return NULL ;
    }
    catch ( exception& xcptn )
    {
        return SysAllocString( fromUtf8( MAKE_STRING(xcptn) ).c_str() ) ;
    }
}
