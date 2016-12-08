#ifndef MAIN_HPP
#define MAIN_HPP

// ---------------------------------------------------------------------

struct DebugConfig
{
    const wchar_t* mpLogFilename ;
} ;

// ---------------------------------------------------------------------

extern void openApi( bool initConsole ) ;
extern void closeApi() ;

extern void reloadConfig( const DebugConfig* pDebugConfig ) ;

// ---------------------------------------------------------------------

#endif // MAIN_HPP