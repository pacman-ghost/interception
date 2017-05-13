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

#ifndef KEYBOARDSTATE_HPP
#define KEYBOARDSTATE_HPP

#include <string>
#include <iosfwd>

class CSendInput ;

// ---------------------------------------------------------------------

// Represents the state of special keys.

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

// This guard class lets us temporarily change the state of special keys.

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
