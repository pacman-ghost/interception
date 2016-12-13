#ifndef SENDINPUT_HPP
#define SENDINPUT_HPP

#include <windows.h>

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
    void sendKeyboardInput( WORD keyCode , bool keyDown , int keyModifiers ) ;
    void sendMouseMoveInput( int dx , int dy , int keyModifiers ) ;
    void sendWheelInput( int scrollSize , int keyModifiers ) ;
    void sendHorzWheelInput( int scrollSize , int keyModifiers ) ;
protected:
    void doSendInput( INPUT* pInputRec , int keyModifiers ) ;

// miscellaneous methods:
public:
    static void getKeyboardState( bool* pIsCtrlDown , bool* pIsAltDown , bool* pIsShiftDown ) ;
    static int getKeyboardState() ;

} ;

// ---------------------------------------------------------------------

#endif // SENDINPUT_HPP
