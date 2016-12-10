#ifndef API_HPP
#define API_HPP

// ---------------------------------------------------------------------

// IMPORTANT! The definitions here must be kept in sync with their C# equivalents in App/ApiDebugConfig.cs.

struct ApiAppConfig
{
} ;

struct ApiDevice
{
    int mDeviceId ;
    const wchar_t* mpHID ;
    int mDeviceNumber ;
    const wchar_t* mpDisplayName ;
    bool mIsEnabled ;
} ;

struct ApiDeviceConfig
{
    int mDeviceId ;
    int mAppProfileStartIndex ;
    int mAppProfileCount ;
} ;

struct ApiAppProfile
{
    const wchar_t* mpApp ;
    int mEventStartIndex ;
    int mEventCount ;
} ;

struct ApiEvent
{
    int mEventType ;
    int mKeyModifiers ;
    int mActionStartIndex ;
    int mActionCount ;
} ;

struct ApiAction
{
    int mActionType ;
} ;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct ApiDebugConfig
{
    const wchar_t* mpLogFilename ;
    const wchar_t* mpLogging ;
} ;

// ---------------------------------------------------------------------

// IMPORTANT! The definitions here must be kept in sync with their C# equivalents in MouseDll.cs.

extern void openApi( const ApiDebugConfig* pDebugConfig , bool initConsole ) ;
extern void closeApi() ;

extern void reloadConfig(
    const ApiAppConfig* pAppConfig , 
    const ApiDevice* pDevices , int nDevices ,
    const ApiDeviceConfig* pDeviceConfigs , int nDeviceConfigs ,
    const ApiAppProfile* pAppProfiles , int nAppProfiles ,
    const ApiEvent* pEvents , int nEvents ,
    const ApiAction* pActions , int nActions
) ;
extern void reloadDebugConfig( const ApiDebugConfig* pDebugConfig ) ;

// ---------------------------------------------------------------------

#endif // API_HPP