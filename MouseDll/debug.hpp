#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>

// ---------------------------------------------------------------------

extern int gEventWaitTimeout ;
extern int gMaxStrokeHistory ;
extern int gDefaultStrokeResetHistoryInterval ;
extern int gDetectMouseMove_WindowSize ;
extern int gDetectMouseMove_HorzBias ;

extern void loadDebugConfig( const wchar_t* pDebugConfigFilename ) ;

// ---------------------------------------------------------------------

#define LOG_MSG( msg ) \
{ \
    if ( gpLogStream != NULL ) \
    { \
        *gpLogStream << makeLogMsg( MAKE_STRING( msg ) ) ; \
        gpLogStream->flush() ; \
    } \
}

#define LOG_CMSG( c , msg ) \
{ \
    if ( isLoggingEnabled( c ) ) \
        LOG_MSG( msg ) ; \
}

extern bool isLoggingEnabled( const std::string& ) ;
extern std::string makeLogMsg( const std::string& ) ;

extern std::ostream* gpLogStream ;

// ---------------------------------------------------------------------

#endif // DEBUG_HPP
