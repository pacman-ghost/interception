#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>
#include <set>

#include "api.hpp"
#include "device.hpp"
#include "deviceConfig.hpp"
#include "utils.hpp"

using namespace std ;

typedef set<string> StringSet ;

// --- LOCAL DATA ------------------------------------------------------

static HMODULE ghInterceptionDll = NULL ;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef IntPtrMap<Device> DeviceTable ;
static DeviceTable gDeviceTable ;

typedef IntPtrMap<DeviceConfig> DeviceConfigTable ;
static DeviceConfigTable gDeviceConfigTable ;

// --- LOCAL DATA ------------------------------------------------------

static bool gEnableConsole = false ;

static wstring gLogFilename ;
static ofstream gLogFile ;

static StringSet gLogging ;
static bool isLoggingEnabled( const string& s ) { return gLogging.find(s) != gLogging.end() ; }

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
openApi(
    const ApiAppConfig* pAppConfig ,
    const ApiDevice* pDevices , int nDevices ,
    const ApiDeviceConfig* pDeviceConfigs , int nDeviceConfigs ,
    const ApiAppProfile* pAppProfiles , int nAppProfiles ,
    const ApiEvent* pEvents , int nEvents ,
    const ApiAction* pActions , int nActions ,
    const ApiDebugConfig* pDebugConfig ,
    bool initConsole
)
{
    // check if we are open
    if ( ghInterceptionDll != NULL )
        throw runtime_error( "API is already open." ) ;

    // initialize
    gEnableConsole = initConsole ;
    reloadConfig(
        pAppConfig ,
        pDevices , nDevices ,
        pDeviceConfigs , nDeviceConfigs ,
        pAppProfiles , nAppProfiles ,
        pEvents , nEvents ,
        pActions , nActions ,
        pDebugConfig
    ) ;

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
    if ( isLoggingEnabled( "startup" ) )
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
reloadConfig(
    const ApiAppConfig* pAppConfig ,
    const ApiDevice* pDevices , int nDevices ,
    const ApiDeviceConfig* pDeviceConfigs , int nDeviceConfigs ,
    const ApiAppProfile* pAppProfiles , int nAppProfiles ,
    const ApiEvent* pEvents , int nEvents ,
    const ApiAction* pActions , int nActions ,
    const ApiDebugConfig* pDebugConfig
)
{
    // validate the parameters
    if ( pAppConfig == NULL )
        throw runtime_error( "Missing AppConfig." ) ;
    if ( pDevices == NULL )
        throw runtime_error( "Missing Device's." ) ;
    if ( nDevices < 0 )
        throw runtime_error( "Invalid Device count." ) ;
    if ( pDeviceConfigs == NULL )
        throw runtime_error( "Missing DeviceConfig's." ) ;
    if ( nDeviceConfigs < 0 )
        throw runtime_error( "Invalid DeviceConfig count." ) ;
    if ( pAppProfiles == NULL )
        throw runtime_error( "Missing AppProfile's." ) ;
    if ( nAppProfiles < 0 )
        throw runtime_error( "Invalid AppProfile count." ) ;
    if ( pEvents == NULL )
        throw runtime_error( "Missing Event's." ) ;
    if ( nEvents < 0 )
        throw runtime_error( "Invalid Event count." ) ;
    if ( pActions == NULL )
        throw runtime_error( "Missing Action's." ) ;
    if ( nActions < 0 )
        throw runtime_error( "Invalid Action count." ) ;
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

    // initialize logging
    gLogging.clear() ;
    const wchar_t* p = pDebugConfig->mpLogging ;
    for ( ; ; )
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

    // load the DeviceConfig's
    gDeviceConfigTable.deleteAll() ;
    for ( int i=0 ; i < nDeviceConfigs ; ++i )
    {
        const ApiDeviceConfig* pDeviceConfig = pDeviceConfigs + i ;
        // validate the AppProfile index/count
        if ( pDeviceConfig->mAppProfileStartIndex < 0 || pDeviceConfig->mAppProfileStartIndex >= nAppProfiles )
        {
            throw runtime_error(
                MAKE_STRING(
                    "Invalid AppProfile start index: " << pDeviceConfig->mAppProfileStartIndex
                    << " (#=" << nAppProfiles << ")"
                )
            ) ;
        }
        if ( pDeviceConfig->mAppProfileCount < 0 || pDeviceConfig->mAppProfileStartIndex + pDeviceConfig->mAppProfileCount > nAppProfiles )
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
