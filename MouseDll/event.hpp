#ifndef EVENT_HPP
#define EVENT_HPP

#include "actions.hpp"
#include "utils.hpp"

struct ApiEvent ;

// ---------------------------------------------------------------------

class Event
{

// data types:
public:
    enum eEventType {
        etMouseLeft=1 , etMouseRight=2 , etMouseUp=3 , etMouseDown=4 ,
        etWheelLeft=5 , etWheelRight=6 , etWheelUp=7 , etWheelDown=8
    } ;

// constructors/destructor:
public:
    Event( const ApiEvent* pEvent , const ApiAction* pActions , int nActions ) ;
    DISABLE_COPY_ASSIGNMENT( Event ) ;

// access methods:
public:
    eEventType eventType() const ;
    int keyModifiers() const ;
    const ActionPtrVector& actions() const ;
public:
    void dumpEvent( std::ostream& os , const char* pPrefix="" ) const ;

// data members:
private:
    eEventType mEventType ;
    int mKeyModifiers ;
    ActionPtrVector mActions ;

} ;

typedef PtrVector<Event> EventPtrVector ;

// inserters
std::ostream& operator<<( std::ostream& , const Event& ) ;
std::ostream& operator<<( std::ostream& , const Event::eEventType& ) ;

// ---------------------------------------------------------------------

#endif // EVENT_HPP
