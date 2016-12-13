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
CSendInput::sendKeyboardInput( WORD keyCode , bool keyDown , int keyModifiers )
{
    // send the keyboard input
    INPUT inputRec ;
    memset( &inputRec , 0 , sizeof(INPUT) ) ;
    inputRec.type = INPUT_KEYBOARD ;
    inputRec.ki.wVk = keyCode ;
    if ( ! keyDown )
        inputRec.ki.dwFlags |= KEYEVENTF_KEYUP ;
    doSendInput( &inputRec , keyModifiers ) ;
}

// ---------------------------------------------------------------------

void
CSendInput::sendMouseMoveInput( int dx , int dy , int keyModifiers )
{
    // send the mouse movement input
    INPUT inputRec ;
    memset( &inputRec , 0 , sizeof(INPUT) ) ;
    inputRec.type = INPUT_MOUSE ;
    inputRec.mi.dx = dx ;
    inputRec.mi.dy = dy ;
    inputRec.mi.dwFlags = MOUSEEVENTF_MOVE ;
    doSendInput( &inputRec , keyModifiers ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
CSendInput::sendWheelInput( int scrollSize , int keyModifiers )
{
    // send the mouse-wheel input
    INPUT inputRec ;
    memset( &inputRec , 0 , sizeof(INPUT) ) ;
    inputRec.type = INPUT_MOUSE ;
    inputRec.mi.dwFlags = MOUSEEVENTF_WHEEL ;
    inputRec.mi.mouseData = scrollSize ; // FIXME! s.b. dwData for vista
    doSendInput( &inputRec , keyModifiers ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
CSendInput::sendHorzWheelInput( int scrollSize , int keyModifiers )
{
    // send the mouse-wheel input
    INPUT inputRec ;
    memset( &inputRec , 0 , sizeof(INPUT) ) ;
    inputRec.type = INPUT_MOUSE ;
    inputRec.mi.dwFlags = MOUSEEVENTF_HWHEEL ;
    inputRec.mi.mouseData = scrollSize ; // FIXME! s.b. dwData for vista
    doSendInput( &inputRec , keyModifiers ) ;
}

// ---------------------------------------------------------------------

void
CSendInput::doSendInput( INPUT* pInputRec , int keyModifiers )
{
    // set the keyboard state
    bool isCtrlDown , isAltDown , isShiftDown ;
    if ( keyModifiers >= 0 )
    {
        getKeyboardState( &isCtrlDown , &isAltDown , &isShiftDown ) ;
        if ( (keyModifiers & kmCtrl) && !isCtrlDown )
            sendKeyboardInput( VK_CONTROL , true , -1 ) ;
        else if ( !(keyModifiers & kmCtrl) && isCtrlDown )
            sendKeyboardInput( VK_CONTROL , false , -1 ) ;
        if ( (keyModifiers & kmAlt) && !isAltDown )
            sendKeyboardInput( VK_MENU , true , -1 ) ;
        else if ( !(keyModifiers & kmAlt) && isAltDown )
            sendKeyboardInput( VK_MENU , false , -1 ) ;
        if ( (keyModifiers & kmShift) && !isShiftDown )
            sendKeyboardInput( VK_SHIFT , true , -1 ) ;
        else if ( !(keyModifiers & kmShift) && isShiftDown )
            sendKeyboardInput( VK_SHIFT , false , -1 ) ;
    }

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
    Sleep( 5 ) ;

    // restore the keyboard state
    if ( keyModifiers >= 0 )
    {
        if ( (keyModifiers & kmShift) && !isShiftDown )
            sendKeyboardInput( VK_SHIFT , false , -1 ) ;
        else if ( !(keyModifiers & kmShift) && isShiftDown )
            sendKeyboardInput( VK_SHIFT , true , -1 ) ;
        if ( (keyModifiers & kmAlt) && !isAltDown )
            sendKeyboardInput( VK_MENU , false , -1 ) ;
        else if ( !(keyModifiers & kmAlt) && isAltDown )
            sendKeyboardInput( VK_MENU , true , -1 ) ;
        if ( (keyModifiers & kmCtrl) && !isCtrlDown )
            sendKeyboardInput( VK_CONTROL , false , -1 ) ;
        else if ( !(keyModifiers & kmCtrl) && isCtrlDown )
            sendKeyboardInput( VK_CONTROL , true , -1 ) ;
    }
}

// ---------------------------------------------------------------------

void
CSendInput::getKeyboardState( bool* pIsCtrlDown , bool* pIsAltDown , bool* pIsShiftDown )
{
    // get the keyboard state
    *pIsCtrlDown = GetAsyncKeyState( VK_CONTROL ) < 0 ;
    *pIsAltDown = GetAsyncKeyState( VK_MENU ) < 0 ;
    *pIsShiftDown = GetAsyncKeyState( VK_SHIFT ) < 0 ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int
CSendInput::getKeyboardState()
{
    // get the keyboard state
    bool isCtrlDown , isAltDown , isShiftDown ;
    getKeyboardState( &isCtrlDown , &isAltDown , &isShiftDown ) ;
    return (isCtrlDown ? kmCtrl : 0) | (isAltDown ? kmAlt : 0) | (isShiftDown ? kmShift : 0 ) ;
}
