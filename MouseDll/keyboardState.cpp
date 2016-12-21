#include <windows.h>

#include "keyboardState.hpp"
#include "sendInput.hpp"
#include "utils.hpp"

using namespace std ;

// ---------------------------------------------------------------------

KeyboardState::KeyboardState( int flags )
    : mFlags( flags )
{ }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

KeyboardState
KeyboardState::getCurrKeyboardState()
{
    // get the current keyboard state
    int flags = 0 ;
    if ( GetAsyncKeyState( VK_CONTROL ) < 0 )
        flags |= kmCtrl ;
    if ( GetAsyncKeyState( VK_MENU ) < 0 )
        flags |= kmAlt ;
    if ( GetAsyncKeyState( VK_SHIFT ) < 0 )
        flags |= kmShift ;
    return KeyboardState( flags ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

string
KeyboardState::asString() const
{
    // return the KeyboardState as a string
    static BitFlagsInfo stringTable[] =
    {
        { KeyboardState::kmCtrl , "Ctrl" } ,
        { KeyboardState::kmAlt , "Alt" } ,
        { KeyboardState::kmShift , "Shift" } ,
        { -1 , NULL } ,
    } ;
    return MAKE_STRING( bitFlagsString( stringTable , mFlags , '+' ) ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool KeyboardState::isCtrlDown() const { return (mFlags & kmCtrl) != 0 ; }
bool KeyboardState::isAltDown() const { return (mFlags & kmAlt) != 0 ; }
bool KeyboardState::isShiftDown() const { return (mFlags & kmShift) != 0 ; }
bool KeyboardState::isAnythingDown() const { return (mFlags != 0) ; }

bool KeyboardState::operator==( const KeyboardState& rhs ) const { return mFlags == rhs.mFlags ; }
bool KeyboardState::operator!=( const KeyboardState& rhs ) const { return !operator==(rhs) ; }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ostream&
operator<<( ostream& os , const KeyboardState& keyboardState )
{
    // insert the KeyboardState
    os << keyboardState.asString() ;
    return os ;
}

// ---------------------------------------------------------------------

KeyboardStateGuard::KeyboardStateGuard( CSendInput& rSendInput , const KeyboardState& newKeyboardState )
    : mrSendInput( rSendInput )
    , mPrevKeyboardState( KeyboardState::getCurrKeyboardState() )
    , mNewKeyboardState( newKeyboardState )
{ 
    // set the keyboard state
    mrSendInput.setKeyboardState( newKeyboardState , mPrevKeyboardState ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

KeyboardStateGuard::~KeyboardStateGuard()
{
    // restore the previous keyboard state
    mrSendInput.setKeyboardState( mPrevKeyboardState , mNewKeyboardState ) ;
}
