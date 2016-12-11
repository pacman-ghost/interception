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
    enum eEventType { etMouseLeft=1 , etMouseRight=2 , etMouseUp=3 , etMouseDown=4 } ;
    enum eKeyModifiers { kmCtrl=0x0001 , kmAlt=0x0002 , kmShift=0x0004 } ;

// constructors/destructor:
public:
    Event( const ApiEvent* pEvent , const ApiAction* pActions , int nActions ) ;
    DISABLE_COPY_ASSIGNMENT( Event ) ;

// access methods:
public:
    int eventType() const ;
    int keyModifiers() const ;
    const ActionPtrVector& actions() const ;
public:
    void dumpEvent( std::ostream& os , const char* pPrefix="" ) const ;

// miscellaneous methods:
public:
    static std::string keyModifiersString( int keyModifiers ) ;

// data members:
private:
    int mEventType ;
    int mKeyModifiers ;
    ActionPtrVector mActions ;

} ;

typedef PtrVector<Event> EventPtrVector ;

// inserter
std::ostream& operator<<( std::ostream& , const Event& ) ;

// ---------------------------------------------------------------------

#endif // EVENT_HPP
