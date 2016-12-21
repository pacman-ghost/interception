#include "actions.hpp"
#include "sendInput.hpp"
#include "globals.hpp"

using namespace std ;

// --- Action ---------------------------------------------------------

Action::Action( const ApiAction* pAction )
    : mActionParam( pAction->mActionParam )
    , mKeyboardState( pAction->mKeyModifiers )
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Action*
Action::allocAction( const ApiAction* pAction )
{
    switch( pAction->mActionType )
    {
        case Action::atMouseLeft: return new MouseLeftAction(pAction) ;
        case Action::atMouseRight: return new MouseRightAction(pAction) ;
        case Action::atMouseUp: return new MouseUpAction(pAction) ;
        case Action::atMouseDown: return new MouseDownAction(pAction) ;
        case Action::atWheelLeft: return new WheelLeftAction(pAction) ;
        case Action::atWheelRight: return new WheelRightAction(pAction) ;
        case Action::atWheelUp: return new WheelUpAction(pAction) ;
        case Action::atWheelDown: return new WheelDownAction(pAction) ;
        case Action::atKeyPress: return new KeyPressAction(pAction) ;
        default:
            assert( false ) ;
            return NULL ;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int Action::adjustForSpeed( int val ) const
{
    // NOTE: Speed values are percentages, offset by 100.
    int percentDelta = 100 + actionParam() ;
    if ( percentDelta <= 0 )
        percentDelta = 1 ; // nb: never go below 0%, always return something
    return val * percentDelta / 100 ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

string
Action::asString() const
{
    // return the Action as a string
    stringstream buf ;
    buf << "<" << pActionName() ;
    if ( keyboardState().isAnythingDown() )
        buf << ":" << keyboardState() ;
    if ( actionParam() != 0 )
        buf << ":" << actionParam() ;
    buf << ">" ;
    return buf.str() ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const KeyboardState& Action::keyboardState() const { return mKeyboardState ; }
int Action::actionParam() const { return mActionParam ; }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ostream&
operator<<( ostream& os , const Action& action )
{
    // insert the Action
    os << action.asString() ;
    return os ;
}

// --- MouseLeftAction ------------------------------------------------

MouseLeftAction::MouseLeftAction( const ApiAction* pAction )
    : Action( pAction )
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
MouseLeftAction::doAction( void* pInfo , CSendInput* pSendInput ) const
{
    // send a "mouse move left" event
    int scrollSize = (int) pInfo ;
    scrollSize = scrollSize / 100 ; // unscale the movement (100 = 1 unit)
    scrollSize = adjustForSpeed( scrollSize ) ;
    pSendInput->sendMouseMoveInput( -scrollSize , 0 , keyboardState() ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const char* MouseLeftAction::pActionName() const { return "MouseLeftAction" ; }

// --- MouseRightAction -----------------------------------------------

MouseRightAction::MouseRightAction( const ApiAction* pAction )
    : Action( pAction )
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
MouseRightAction::doAction( void* pInfo , CSendInput* pSendInput ) const
{
    // send a "mouse move right" event
    int scrollSize = (int) pInfo ;
    scrollSize = scrollSize / 100 ; // unscale the movement (100 = 1 unit)
    scrollSize = adjustForSpeed( scrollSize ) ;
    pSendInput->sendMouseMoveInput( scrollSize , 0 , keyboardState() ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const char* MouseRightAction::pActionName() const { return "MouseRightAction" ; }

// --- MouseUpAction --------------------------------------------------

MouseUpAction::MouseUpAction( const ApiAction* pAction )
    : Action( pAction )
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
MouseUpAction::doAction( void* pInfo , CSendInput* pSendInput ) const
{
    // send a "mouse move up" event
    int scrollSize = (int) pInfo ;
    scrollSize = scrollSize / 100 ; // unscale the movement (100 = 1 unit)
    scrollSize = adjustForSpeed( scrollSize ) ;
    pSendInput->sendMouseMoveInput( 0 , -scrollSize , keyboardState() ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const char* MouseUpAction::pActionName() const { return "MouseUpAction" ; }

// --- MouseDownAction ------------------------------------------------

MouseDownAction::MouseDownAction( const ApiAction* pAction )
    : Action( pAction )
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
MouseDownAction::doAction( void* pInfo , CSendInput* pSendInput ) const
{
    // send a "mouse move down" event
    int scrollSize = (int) pInfo ;
    scrollSize = scrollSize / 100 ; // unscale the movement (100 = 1 unit)
    scrollSize = adjustForSpeed( scrollSize ) ;
    pSendInput->sendMouseMoveInput( 0 , scrollSize , keyboardState() ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const char* MouseDownAction::pActionName() const { return "MouseDownAction" ; }

// --- WheelLeftAction ------------------------------------------------

WheelLeftAction::WheelLeftAction( const ApiAction* pAction )
    : Action( pAction )
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
WheelLeftAction::doAction( void* pInfo , CSendInput* pSendInput ) const
{
    // send a "scroll wheel left" event
    int scrollSize = (int) pInfo ;
    scrollSize = WHEEL_DELTA * scrollSize / 100 ; // unscale the movement (100 = 1 unit)
    scrollSize = adjustForSpeed( scrollSize ) ;
    pSendInput->sendHorzWheelInput( -scrollSize , keyboardState() ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const char* WheelLeftAction::pActionName() const { return "WheelLeftAction" ; }

// --- WheelRightAction -----------------------------------------------

WheelRightAction::WheelRightAction( const ApiAction* pAction )
    : Action( pAction )
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
WheelRightAction::doAction( void* pInfo , CSendInput* pSendInput ) const
{
    // send a "scroll wheel left" event
    int scrollSize = (int) pInfo ;
    scrollSize = WHEEL_DELTA * scrollSize / 100 ; // unscale the movement (100 = 1 unit)
    scrollSize = adjustForSpeed( scrollSize ) ;
    pSendInput->sendHorzWheelInput( scrollSize , keyboardState() ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const char* WheelRightAction::pActionName() const { return "WheelRightAction" ; }

// --- WheelUpAction --------------------------------------------------

WheelUpAction::WheelUpAction( const ApiAction* pAction )
    : Action( pAction )
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
WheelUpAction::doAction( void* pInfo , CSendInput* pSendInput ) const
{
    // send a "scroll wheel up" event
    int scrollSize = (int) pInfo ;
    scrollSize = WHEEL_DELTA * scrollSize / 100 ; // unscale the movement (100 = 1 unit)
    scrollSize = adjustForSpeed( scrollSize ) ;
    pSendInput->sendWheelInput( scrollSize , keyboardState() ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const char* WheelUpAction::pActionName() const { return "WheelUpAction" ; }

// --- WheelDownAction ------------------------------------------------

WheelDownAction::WheelDownAction( const ApiAction* pAction )
    : Action( pAction )
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
WheelDownAction::doAction( void* pInfo , CSendInput* pSendInput ) const
{
    // send a "scroll wheel down" event
    int scrollSize = (int) pInfo ;
    scrollSize = WHEEL_DELTA * scrollSize / 100 ; // unscale the movement (100 = 1 unit)
    scrollSize = adjustForSpeed( scrollSize ) ;
    pSendInput->sendWheelInput( -scrollSize , keyboardState() ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const char* WheelDownAction::pActionName() const { return "WheelDownAction" ; }

// --- KeyPressAction -------------------------------------------------

KeyPressAction::KeyPressAction( const ApiAction* pAction )
    : Action( pAction )
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
KeyPressAction::doAction( void* pInfo , CSendInput* pSendInput ) const
{
    // send a "keypress" event
    int vKey = actionParam() ;
    KeyboardStateGuard keyboardStateGuard( *pSendInput , keyboardState() ) ;
    pSendInput->sendKeyboardInput( vKey , true ) ;
    pSendInput->sendKeyboardInput( vKey , false ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const char* KeyPressAction::pActionName() const { return "KeyPressAction" ; }
