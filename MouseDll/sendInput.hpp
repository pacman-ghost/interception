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

#ifndef SENDINPUT_HPP
#define SENDINPUT_HPP

#include <windows.h>

#include "keyboardState.hpp"
#include "utils.hpp"

// ---------------------------------------------------------------------

// Wrapper around SendInput().

class CSendInput
{

// constructors/destructor:
public:
    CSendInput() ;
    ~CSendInput() ;
    DISABLE_COPY_ASSIGNMENT( CSendInput ) ;

// input methods:
public:
    void setKeyboardState( const KeyboardState& newKeyboardState , const KeyboardState& currKeyboardState ) ;
public:
    void sendKeyboardInput( WORD keyCode , bool keyDown ) ;
    void sendMouseMoveInput( int dx , int dy , const KeyboardState& keyboardState ) ;
    void sendWheelInput( int scrollSize , const KeyboardState& keyboardState ) ;
    void sendHorzWheelInput( int scrollSize , const KeyboardState& keyboardState ) ;
protected:
    void doSendInput( INPUT* pInputRec ) ;

} ;

// ---------------------------------------------------------------------

#endif // SENDINPUT_HPP
