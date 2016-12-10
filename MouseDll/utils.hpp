#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <cassert>

// ---------------------------------------------------------------------

struct EnumStringInfo { int mVal ; const char* mpName ; } ;
extern const char* enumString( const EnumStringInfo* pEnumStringTable , int val ) ;

struct BitFlagsInfo { int mFlag ; const char* mpName ; } ;
extern std::string bitFlagsString( const BitFlagsInfo* pBitFlagsTable , int val , char sep ) ;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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

#define MAKE_STRING( msg )  ( ((std::ostringstream&)((std::ostream&)std::ostringstream() << msg)).str() )
#define MAKE_CSTRING( msg ) ( MAKE_STRING( msg ).c_str() )

// ---------------------------------------------------------------------

// NOTE: These classes are not 100% correct (e.g. if an item is replaced with a new one,
//  the old object won't get delete'd), but they will suffice for our purposes.

template< typename T >
class PtrVector : public std::vector<T*>
{
public:
    ~PtrVector()
    {
        // clean up
        for ( iterator it=begin() ; it != end() ; ++it )
            delete *it ;
    }
} ;

template< typename T >
class IntPtrMap : public std::map<int,T*>
{
public:
    ~IntPtrMap() { deleteAll() ; }
    void deleteAll()
    {
        // delete all items
        for ( iterator it=begin() ; it != end() ; ++it )
            delete (*it).second ;
        clear() ;
    }
} ;

// ---------------------------------------------------------------------

#define ARRAY_SIZE( arr ) ( sizeof(arr) / sizeof((arr)[0]) )

#define DISABLE_COPY_ASSIGNMENT( ClassName ) \
    private: \
        ClassName( const ClassName& rhs ) ; \
        ClassName& operator=( const ClassName& rhs ) ;

// ---------------------------------------------------------------------

#endif // UTILS_HPP
