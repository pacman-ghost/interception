#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <fstream>

// ---------------------------------------------------------------------

extern void loadDebugConfig( const wchar_t* pDebugConfigFilename ) ;

// ---------------------------------------------------------------------

#define LOG_MSG( msg ) \
{ \
    string _buf_ = makeLogMsg( MAKE_STRING( msg ) ) ; \
    cout << _buf_ ; \
    cout.flush() ; \
    if ( gLogFile.is_open() ) \
    { \
        gLogFile << _buf_ ; \
        gLogFile.flush() ; \
    } \
}
#define LOG_CMSG( c , msg ) \
{ \
    if ( isLoggingEnabled( c ) ) \
        LOG_MSG( msg ) ; \
}

extern bool isLoggingEnabled( const std::string& ) ;
extern std::string makeLogMsg( const std::string& ) ;

extern std::ofstream gLogFile ;

// ---------------------------------------------------------------------

#endif // DEBUG_HPP
