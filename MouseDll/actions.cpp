#include "actions.hpp"
#include "globals.hpp"

using namespace std ;

// --- Action ---------------------------------------------------------

Action::Action( const ApiAction* pAction )
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
        default:
            assert( false ) ;
            return NULL ;
    }
}

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
MouseLeftAction::doAction() const
{
    assert(0);//FIXME
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

string MouseLeftAction::asString() const { return "<MouseLeftAction>" ; }

// --- MouseRightAction -----------------------------------------------

MouseRightAction::MouseRightAction( const ApiAction* pAction )
    : Action( pAction )
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
MouseRightAction::doAction() const
{
    assert(0);//FIXME
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

string MouseRightAction::asString() const { return "<MouseRightAction>" ; }

// --- MouseUpAction --------------------------------------------------

MouseUpAction::MouseUpAction( const ApiAction* pAction )
    : Action( pAction )
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
MouseUpAction::doAction() const
{
    assert(0);//FIXME
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

string MouseUpAction::asString() const { return "<MouseUpAction>" ; }

// --- MouseDownAction ------------------------------------------------

MouseDownAction::MouseDownAction( const ApiAction* pAction )
    : Action( pAction )
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
MouseDownAction::doAction() const
{
    assert(0);//FIXME
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

string MouseDownAction::asString() const { return "<MouseDownAction>" ; }
