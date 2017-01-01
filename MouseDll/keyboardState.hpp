#ifndef KEYBOARDSTATE_HPP
#define KEYBOARDSTATE_HPP

#include <string>
#include <iosfwd>

class CSendInput ;

// ---------------------------------------------------------------------

class KeyboardState
{

// data types:
public:
    enum eKeyModifiers { kmCtrl=0x0001 , kmAlt=0x0002 , kmShift=0x0004 , kmCapsLock=0x0008 } ;

// constructors/destructor:
public:
    explicit KeyboardState( int flags ) ;

// access methods:
public:
    bool isCtrlDown() const ;
    bool isAltDown() const ;
    bool isShiftDown() const ;
    bool isCapsLockDown() const ;
    bool isAnythingDown() const ;
    std::string asString() const ;

// keyboard state methods:
public:
    static KeyboardState getCurrKeyboardState() ;

// operators:
public:
    bool operator==( const KeyboardState& rhs ) const ;
    bool operator!=( const KeyboardState& rhs ) const ;

// data members:
private:
    int mFlags ;

} ;

// inserters:
std::ostream& operator<<( std::ostream& , const KeyboardState& ) ;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct KeyboardStateGuard
{
    KeyboardStateGuard( CSendInput& rSendInput , const KeyboardState& newKeyboardState ) ;
    ~KeyboardStateGuard() ;
private:
    CSendInput& mrSendInput ;
    KeyboardState mPrevKeyboardState ;
    KeyboardState mNewKeyboardState ;
} ;

// ---------------------------------------------------------------------

#endif // KEYBOARDSTATE_HPP
