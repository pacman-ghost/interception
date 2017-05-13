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

#include "globals.hpp"
#include "utils.hpp"

using namespace std ;

// ---------------------------------------------------------------------

void
openApi( PCALLBACKFN pCallbackFn , const wchar_t* pDebugConfigFilename )
{
    assert( pCallbackFn != NULL ) ;
    assert( pDebugConfigFilename != NULL ) ;

    // check if we are open
    if ( ghInterceptionDll != NULL )
        throw runtime_error( "API is already open." ) ;

    // initialize
    assert( gpCallbackFn == NULL ) ;
    gpCallbackFn = pCallbackFn ;
    loadDebugConfig( pDebugConfigFilename ) ;

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
    LOG_CMSG( "system" , "Loading Interception: " << toUtf8(buf) ) ;
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

    // clean up
    assert( gpCallbackFn != NULL ) ;
    gpCallbackFn = NULL ;
}

// ---------------------------------------------------------------------

void
reloadConfig(
    const ApiAppConfig* pAppConfig ,
    const ApiDevice* pDevices , int nDevices ,
    const ApiDeviceConfig* pDeviceConfigs , int nDeviceConfigs ,
    const ApiAppProfile* pAppProfiles , int nAppProfiles ,
    const ApiEvent* pEvents , int nEvents ,
    const ApiAction* pActions , int nActions
)
{
    assert( pAppConfig != NULL ) ;
    assert( pDevices != NULL ) ;
    assert( nDevices >= 0 ) ;
    assert( pDeviceConfigs != NULL ) ;
    assert( nDeviceConfigs >= 0 ) ;
    assert( pAppProfiles != NULL ) ;
    assert( nAppProfiles >= 0 ) ;
    assert( pEvents != NULL ) ;
    assert( nEvents >= 0 ) ;
    assert( pActions != NULL ) ;
    assert( nActions >= 0 ) ;

    // load the Device's
    gDeviceTable.deleteAll() ;
    for ( int i=0 ; i < nDevices ; ++i )
    {
        const ApiDevice* pDevice = pDevices+i ;
        gDeviceTable[ pDevice->mDeviceId ] = new Device( pDevice ) ;
    }
    if ( isLoggingEnabled( "config" ) )
    {
        stringstream buf ;
        buf << "Loaded devices:" << endl ;
        for ( DeviceTable::const_iterator it=gDeviceTable.begin() ; it != gDeviceTable.end() ; ++it )
            (*it).second->dumpDevice( buf , "  " ) ;
        LOG_MSG( buf.str() ) ;
    }
    gUnknownDevices.clear() ;

    // load the DeviceConfig's
    gDeviceConfigTable.deleteAll() ;
    for ( int i=0 ; i < nDeviceConfigs ; ++i )
    {
        const ApiDeviceConfig* pDeviceConfig = pDeviceConfigs + i ;
        // validate the AppProfile start index and count
        if ( pDeviceConfig->mAppProfileStartIndex < 0 || (pDeviceConfig->mAppProfileCount > 0 && pDeviceConfig->mAppProfileStartIndex >= nAppProfiles) )
        {
            throw runtime_error(
                MAKE_STRING(
                    "Invalid AppProfile start index: " << pDeviceConfig->mAppProfileStartIndex
                    << " (#=" << nAppProfiles << ")"
                )
            ) ;
        }
        if ( pDeviceConfig->mAppProfileCount < 0 || (pDeviceConfig->mAppProfileCount > 0 && pDeviceConfig->mAppProfileStartIndex+pDeviceConfig->mAppProfileCount > nAppProfiles) )
        {
            throw runtime_error(
                MAKE_STRING(
                    "Invalid AppProfile count: " << pDeviceConfig->mAppProfileCount
                    << " (s=" << pDeviceConfig->mAppProfileStartIndex << ", #=" << nAppProfiles << ")"
                )
            ) ;
        }
        // add the next DeviceConfig
        gDeviceConfigTable[ pDeviceConfig->mDeviceId ] = new DeviceConfig(
            pDeviceConfig ,
            pAppProfiles+pDeviceConfig->mAppProfileStartIndex , pDeviceConfig->mAppProfileCount ,
            pEvents , nEvents ,
            pActions , nActions
        ) ;
    }
    if ( isLoggingEnabled( "config" ) )
    {
        stringstream buf ;
        buf << "Loaded device configs:" << endl ;
        for ( DeviceConfigTable::const_iterator it=gDeviceConfigTable.begin() ; it != gDeviceConfigTable.end() ; ++it )
            (*it).second->dumpDeviceConfig( buf , "  " ) ;
        LOG_MSG( buf.str() ) ;
    }
}
