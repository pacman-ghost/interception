#include "api.hpp"
#include "globals.hpp"
#include "utils.hpp"

using namespace std ;

// ---------------------------------------------------------------------

void
openApi( const ApiDebugConfig* pDebugConfig , bool initConsole )
{
    // check if we are open
    if ( ghInterceptionDll != NULL )
        throw runtime_error( "API is already open." ) ;

    // initialize
    reloadDebugConfig( pDebugConfig ) ;
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
    const ApiAction* pActions , int nActions
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
reloadDebugConfig( const ApiDebugConfig* pDebugConfig )
{
    // validate the parameters
    if ( pDebugConfig == NULL )
        throw runtime_error( "Missing DebugConfig." ) ;

    // initialize the log file
    const wchar_t* pLogFilename = pDebugConfig->mpLogFilename ;
    if ( pLogFilename == NULL )
        pLogFilename = L"" ;
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
    initLogging( pDebugConfig->mpLogging ) ;
}
