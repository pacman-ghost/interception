#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <sstream>

// ---------------------------------------------------------------------

extern std::string getLastErrorString() ;
extern std::string getErrorString( int errorCode ) ;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

extern std::wstring fromUtf8( const char* pStr , int len=-1 ) ;
inline std::wstring fromUtf8( const std::string& str ) { return fromUtf8(str.c_str(),str.length()) ; }

#define MAKE_STRING( msg ) ( ((std::ostringstream&)((std::ostream&)std::ostringstream() << msg)).str() )

// ---------------------------------------------------------------------

#define ARRAY_SIZE( arr ) ( sizeof(arr) / sizeof((arr)[0]) )

// ---------------------------------------------------------------------

#endif // UTILS_HPP