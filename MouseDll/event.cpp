#include "event.hpp"
#include "globals.hpp"

using namespace std ;

// --- CONSTRUCTORS ----------------------------------------------------

Event::Event( const ApiEvent* pEvent , const ApiAction* pActions , int nActions )
{
    // initialize the Event
    mEventType = (eEventType) pEvent->mEventType ;
    mKeyModifiers = pEvent->mKeyModifiers ;

    // initialize the Event
    for ( int i=0 ; i < nActions ; ++i )
        mActions.push_back( Action::allocAction( pActions+i ) ) ;
}

// ---------------------------------------------------------------------

Event::eEventType Event::eventType() const { return mEventType ; }
int Event::keyModifiers() const { return mKeyModifiers ; }
const ActionPtrVector& Event::actions() const { return mActions ; }

// ---------------------------------------------------------------------

void
Event::dumpEvent( ostream& os , const char* pPrefix ) const
{
    // dump the Event
    os << pPrefix << *this << ":" << endl ;
    for ( ActionPtrVector::const_iterator it=actions().begin() ; it != actions().end() ; ++it )
        os << pPrefix << "  " << *(*it) << endl ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ostream&
operator<<( ostream& os , const Event& evt )
{
    // insert the Event
    os << "{Event:" << evt.eventType() ;
    if ( evt.keyModifiers() != 0 )
        os << ":" << keyModifiersString(evt.keyModifiers()) ;
    os << "}" ;
    return os ;
}

ostream&
operator<<( ostream& os , const Event::eEventType& eventType )
{
    // insert the eEventType
    static EnumStringInfo stringTable[] = {
        { Event::etMouseLeft , "mouseLeft" } ,
        { Event::etMouseRight , "mouseRight" } ,
        { Event::etMouseUp , "mouseUp" } ,
        { Event::etMouseDown , "mouseDown" } ,
        { Event::etWheelLeft , "wheelLeft" } ,
        { Event::etWheelRight , "wheelRight" } ,
        { Event::etWheelUp , "wheelUp" } ,
        { Event::etWheelDown , "wheelDown" } ,
        { -1 , NULL }
    } ;
    os << enumString(stringTable,eventType) ;
    return os ;
}
