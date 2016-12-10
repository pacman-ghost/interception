#include <stdexcept>
#include <windows.h>
#include "api.hpp"
#include "utils.hpp"

using namespace std ;

// ---------------------------------------------------------------------

extern "C" __declspec(dllexport) BSTR
open_api(
    const ApiAppConfig* pAppConfig ,
    const ApiDevice* pDevices , int nDevices ,
    const ApiDeviceConfig* pDeviceConfigs , int nDeviceConfigs ,
    const ApiAppProfile* pAppProfiles , int nAppProfiles ,
    const ApiEvent* pEvents , int nEvents ,
    const ApiAction* pActions , int nActions ,
    const ApiDebugConfig* pDebugConfig ,
    int initConsole
)
{
    // open the API
    try
    {
        openApi( 
            pAppConfig ,
            pDevices , nDevices ,
            pDeviceConfigs , nDeviceConfigs ,
            pAppProfiles , nAppProfiles ,
            pEvents , nEvents ,
            pActions , nActions ,
            pDebugConfig ,
            initConsole != 0
        ) ;
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
reload_config(
    const ApiAppConfig* pAppConfig ,
    const ApiDevice* pDevices , int nDevices ,
    const ApiDeviceConfig* pDeviceConfigs , int nDeviceConfigs ,
    const ApiAppProfile* pAppProfiles , int nAppProfiles ,
    const ApiEvent* pEvents , int nEvents ,
    const ApiAction* pActions , int nActions ,
    const ApiDebugConfig* pDebugConfig
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
            pActions , nActions ,
            pDebugConfig
        ) ;
        return NULL ;
    }
    catch ( exception& xcptn )
    {
        return SysAllocString( fromUtf8( MAKE_STRING( xcptn.what() ) ).c_str() ) ;
    }
}
