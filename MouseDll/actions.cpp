#include "actions.hpp"
#include "sendInput.hpp"
#include "globals.hpp"

using namespace std ;

// --- Action ---------------------------------------------------------

Action::Action( const ApiAction* pAction )
    : mKeyModifiers( pAction->mKeyModifiers )
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
        default:
            assert( false ) ;
            return NULL ;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

string
Action::asString() const
{
    // return the Action as a string
    stringstream buf ;
    buf << "<" << pActionName() ;
    if ( keyModifiers() != 0 )
        buf << ":" << keyModifiersString(keyModifiers()) ;
    buf << ">" ;
    return buf.str() ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int Action::keyModifiers() const { return mKeyModifiers ; }

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
    pSendInput->sendMouseMoveInput( -scrollSize , 0 , keyModifiers() ) ;
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
    pSendInput->sendMouseMoveInput( scrollSize , 0 , keyModifiers() ) ;
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
    pSendInput->sendMouseMoveInput( 0 , -scrollSize , keyModifiers() ) ;
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
    pSendInput->sendMouseMoveInput( 0 , scrollSize , keyModifiers() ) ;
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
    pSendInput->sendHorzWheelInput( -scrollSize , keyModifiers() ) ;
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
    pSendInput->sendHorzWheelInput( scrollSize , keyModifiers() ) ;
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
    pSendInput->sendWheelInput( scrollSize , keyModifiers() ) ;
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
    pSendInput->sendWheelInput( -scrollSize , keyModifiers() ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const char* WheelDownAction::pActionName() const { return "WheelDownAction" ; }
