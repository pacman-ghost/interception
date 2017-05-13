/*
 * Copyright 2017 Taka Muraoka
 *
 * This file is part of the MouseInterception project.
 *
 * The MouseInterception project is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * The MouseInterception project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the MouseInterception project.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    if ( GetAsyncKeyState( VK_CAPITAL ) < 0 )
        flags |= kmCapsLock ;
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
        { KeyboardState::kmCapsLock , "CapsLock" } ,
        { -1 , NULL } ,
    } ;
    return MAKE_STRING( bitFlagsString( stringTable , mFlags , '+' ) ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool KeyboardState::isCtrlDown() const { return (mFlags & kmCtrl) != 0 ; }
bool KeyboardState::isAltDown() const { return (mFlags & kmAlt) != 0 ; }
bool KeyboardState::isShiftDown() const { return (mFlags & kmShift) != 0 ; }
bool KeyboardState::isCapsLockDown() const { return (mFlags & kmCapsLock) != 0 ; }
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
