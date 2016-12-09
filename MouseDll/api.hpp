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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct ApiDebugConfig
{
    const wchar_t* mpLogFilename ;
} ;

// ---------------------------------------------------------------------

// IMPORTANT! The definitions here must be kept in sync with their C# equivalents in MouseDll.cs.

extern void openApi( 
    const ApiAppConfig* pAppConfig , 
    const ApiDevice* pDevices , int nDevices ,
    const ApiDebugConfig* pDebugConfig ,
    bool initConsole
) ;
extern void closeApi() ;

extern void reloadConfig(
    const ApiAppConfig* pAppConfig , 
    const ApiDevice* pDevices , int nDevices ,
    const ApiDebugConfig* pDebugConfig
) ;

// ---------------------------------------------------------------------

#endif // API_HPP