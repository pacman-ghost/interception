#include "sendInput.hpp"
#include "globals.hpp"

using namespace std ;

// --- CONSTRUCTORS ----------------------------------------------------

CSendInput::CSendInput()
{
}

// --- DESTRUCTOR ------------------------------------------------------

CSendInput::~CSendInput()
{
}

// ---------------------------------------------------------------------

void
CSendInput::setKeyboardState( const KeyboardState& newKeyboardState , const KeyboardState& currKeyboardState )
{
    // set the new keyboard state
    if ( newKeyboardState.isCtrlDown() && !currKeyboardState.isCtrlDown() )
        sendKeyboardInput( VK_CONTROL , true ) ;
    else if ( !newKeyboardState.isCtrlDown() && currKeyboardState.isCtrlDown() )
        sendKeyboardInput( VK_CONTROL , false ) ;

    // set the new keyboard state
    if ( newKeyboardState.isAltDown() && !currKeyboardState.isAltDown() )
        sendKeyboardInput( VK_MENU , true ) ;
    else if ( !newKeyboardState.isAltDown() && currKeyboardState.isAltDown() )
        sendKeyboardInput( VK_MENU , false ) ;

    // set the new keyboard state
    if ( newKeyboardState.isShiftDown() && !currKeyboardState.isShiftDown() )
        sendKeyboardInput( VK_SHIFT , true ) ;
    else if ( !newKeyboardState.isShiftDown() && currKeyboardState.isShiftDown() )
        sendKeyboardInput( VK_SHIFT , false ) ;

    // set the new keyboard state
    if ( newKeyboardState.isCapsLockDown() && !currKeyboardState.isCapsLockDown() )
        sendKeyboardInput( VK_CAPITAL , true ) ;
    else if ( !newKeyboardState.isCapsLockDown() && currKeyboardState.isCapsLockDown() )
        sendKeyboardInput( VK_CAPITAL , false ) ;
}

// ---------------------------------------------------------------------

void
CSendInput::sendKeyboardInput( WORD keyCode , bool keyDown )
{
    // send the keyboard input
    INPUT inputRec ;
    memset( &inputRec , 0 , sizeof(INPUT) ) ;
    inputRec.type = INPUT_KEYBOARD ;
    inputRec.ki.wVk = keyCode ;
    if ( ! keyDown )
        inputRec.ki.dwFlags |= KEYEVENTF_KEYUP ;
    doSendInput( &inputRec ) ;
}

// ---------------------------------------------------------------------

void
CSendInput::sendMouseMoveInput( int dx , int dy , const KeyboardState& keyboardState )
{
    // send the mouse movement input
    KeyboardStateGuard keyboardStateGuard( *this , keyboardState ) ;
    INPUT inputRec ;
    memset( &inputRec , 0 , sizeof(INPUT) ) ;
    inputRec.type = INPUT_MOUSE ;
    inputRec.mi.dx = dx ;
    inputRec.mi.dy = dy ;
    inputRec.mi.dwFlags = MOUSEEVENTF_MOVE ;
    doSendInput( &inputRec ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
CSendInput::sendWheelInput( int scrollSize , const KeyboardState& keyboardState )
{
    // send the mouse-wheel input
    KeyboardStateGuard keyboardStateGuard( *this , keyboardState ) ;
    INPUT inputRec ;
    memset( &inputRec , 0 , sizeof(INPUT) ) ;
    inputRec.type = INPUT_MOUSE ;
    inputRec.mi.dwFlags = MOUSEEVENTF_WHEEL ;
    inputRec.mi.mouseData = scrollSize ; // FIXME! s.b. dwData for vista
    doSendInput( &inputRec ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
CSendInput::sendHorzWheelInput( int scrollSize , const KeyboardState& keyboardState )
{
    // send the mouse-wheel input
    KeyboardStateGuard keyboardStateGuard( *this , keyboardState ) ;
    INPUT inputRec ;
    memset( &inputRec , 0 , sizeof(INPUT) ) ;
    inputRec.type = INPUT_MOUSE ;
    inputRec.mi.dwFlags = MOUSEEVENTF_HWHEEL ;
    inputRec.mi.mouseData = scrollSize ; // FIXME! s.b. dwData for vista
    doSendInput( &inputRec ) ;
}

// ---------------------------------------------------------------------

void
CSendInput::doSendInput( INPUT* pInputRec )
{
    // log the input
    if ( isLoggingEnabled( "sendInput" ) )
    {
        if ( pInputRec->type == INPUT_MOUSE )
        {
            LOG_MSG(
                "SEND INPUT (mouse):"
                << " dx=" << pInputRec->mi.dx << " ; dy=" << pInputRec->mi.dy
                << " ; mouseData=" << (int)pInputRec->mi.mouseData
                << " ; flags=0x" << hexString(pInputRec->mi.dwFlags)
            ) ;
        }
        else if ( pInputRec->type == INPUT_KEYBOARD )
        {
            LOG_MSG(
                "SEND INPUT (keyboard):"
                << " vkey=" << pInputRec->ki.wVk << " ; scan=" << pInputRec->ki.wScan
                << " ; flags=0x" << hexString(pInputRec->ki.dwFlags)
            ) ;
        }
        else
            assert( false ) ;
    }

    // send the input
    // NOTE: The documentation says that we can send multiple events at once, and they will be guaranteed
    //  to not have other events interspersed between them, but things don't work :-( and the only way
    //  I could get it to work was to send events one at a time, with a (non-zero) sleep between them.
    //  Maybe because we're manipulating the Ctrl/Alt/Shift keys...?
    UINT nEventRecs = SendInput( 1 , pInputRec , sizeof(INPUT) ) ;
    assert( nEventRecs == 1 ) ;
    Sleep( 5 ) ; // FIXME! get rid of this
}
