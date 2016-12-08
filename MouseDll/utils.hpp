#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <sstream>

// ---------------------------------------------------------------------

extern std::string getLastErrorString() ;
extern std::string getErrorString( int errorCode ) ;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

extern std::string toUtf8( const wchar_t* pStr , int len=-1 ) ;
inline std::string toUtf8( const std::wstring& str ) { return toUtf8(str.c_str(),str.length()) ; }
extern std::wstring fromUtf8( const char* pStr , int len=-1 ) ;
inline std::wstring fromUtf8( const std::string& str ) { return fromUtf8(str.c_str(),str.length()) ; }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// NOTE: These are not implemented and will generate a link error if we accidentally
//  try to write a string to the wrong kind of stream.

extern std::ostream& operator<<( std::ostream& , const wchar_t* ) ;
extern std::ostream& operator<<( std::ostream& , const std::wstring& ) ;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define MAKE_STRING( msg ) ( ((std::ostringstream&)((std::ostream&)std::ostringstream() << msg)).str() )

// ---------------------------------------------------------------------

#define ARRAY_SIZE( arr ) ( sizeof(arr) / sizeof((arr)[0]) )

// ---------------------------------------------------------------------

#endif // UTILS_HPP