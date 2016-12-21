#ifndef SENDINPUT_HPP
#define SENDINPUT_HPP

#include <windows.h>

#include "keyboardState.hpp"
#include "utils.hpp"

// ---------------------------------------------------------------------

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
