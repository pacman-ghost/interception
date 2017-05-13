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

// NOTE: This file contains the main entry points into the DLL, so everything needs "C" linkage.
//  It's called from C#, so strings need to be BSTR's.

#include <stdexcept>
#include "api.hpp"
#include "globals.hpp"
#include "utils.hpp"

using namespace std ;

// ---------------------------------------------------------------------

extern "C" __declspec(dllexport) BSTR
open_api( PCALLBACKFN pCallbackFn , const wchar_t* pDebugConfigFilename )
{
    // open the API
    try
    {
        openApi( pCallbackFn , pDebugConfigFilename ) ;
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
