#include "event.hpp"
#include "globals.hpp"

using namespace std ;

// --- LOCAL DATA ------------------------------------------------------

static EnumStringInfo gEventTypeStringTable[] =
{
    { Event::etMouseLeft , "mouseLeft" } ,
    { Event::etMouseRight , "mouseRight" } ,
    { Event::etMouseUp , "mouseUp" } ,
    { Event::etMouseDown , "mouseDown" } ,
    { -1 , NULL }
} ;

static BitFlagsInfo gKeyModifiersStringTable[] =
{
    { Event::kmCtrl , "Ctrl" } ,
    { Event::kmAlt , "Alt" } ,
    { Event::kmShift , "Shift" } ,
    { -1 , NULL } ,
} ;

// --- CONSTRUCTORS ----------------------------------------------------

Event::Event( const ApiEvent* pEvent , const ApiAction* pActions , int nActions )
{
    // initialize the Event
    mEventType = pEvent->mEventType ;
    mKeyModifiers = pEvent->mKeyModifiers ;

    // initialize the Event
    for ( int i=0 ; i < nActions ; ++i )
        mActions.push_back( Action::allocAction( pActions+i ) ) ;
}

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
    os << "{Event:" << enumString(gEventTypeStringTable,evt.eventType()) ;
    if ( evt.keyModifiers() != 0 )
        os << ":" << Event::keyModifiersString(evt.keyModifiers()) ;
    os << "}" ;
    return os ;
}

// ---------------------------------------------------------------------

string Event::keyModifiersString( int km ) { return bitFlagsString(gKeyModifiersStringTable,km,'+') ; }

int Event::eventType() const { return mEventType ; }
int Event::keyModifiers() const { return mKeyModifiers ; }
const ActionPtrVector& Event::actions() const { return mActions ; }
