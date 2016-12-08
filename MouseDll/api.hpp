#ifndef API_HPP
#define API_HPP

// ---------------------------------------------------------------------

// IMPORTANT! The definitions here must be kept in sync with their C# equivalents in App/DebugConfig.cs.

struct AppConfig
{
} ;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct DebugConfig
{
    const wchar_t* mpLogFilename ;
} ;

// ---------------------------------------------------------------------

// IMPORTANT! The definitions here must be kept in sync with their C# equivalents in MouseDll.cs.

extern void openApi( const AppConfig* pAppConfig , const DebugConfig* pDebugConfig , bool initConsole ) ;
extern void closeApi() ;

extern void reloadConfig( const AppConfig* pAppConfig , const DebugConfig* pDebugConfig ) ;

// ---------------------------------------------------------------------

#endif // API_HPP