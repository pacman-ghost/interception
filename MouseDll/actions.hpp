#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include "keyboardState.hpp"
#include "utils.hpp"

struct ApiAction ;
class CSendInput ;

// ---------------------------------------------------------------------

class Action
{

// data types:
public:
    enum eActionType {
        atMouseLeft=1 , atMouseRight=2 , atMouseUp=3 , atMouseDown=4 ,
        atWheelLeft=5 , atWheelRight=6 , atWheelUp=7 , atWheelDown=8 ,
        atKeyPress=10
        } ;

// constructors/destructor:
public:
    static Action* allocAction( const ApiAction* pAction ) ;
protected:
    Action( const ApiAction* pAction ) ;
    DISABLE_COPY_ASSIGNMENT( Action ) ;

// action methods:
public:
    virtual void doAction( void* pInfo , CSendInput* pSendInput ) const = 0 ;

// access methods:
public:
    int actionParam() const ;
    const KeyboardState& keyboardState() const ;
    virtual std::string asString() const ;
protected:
    virtual const char* pActionName() const = 0 ;
    int adjustForSpeed( int val ) const ;

// data members:
private:
    int mActionParam ;
    KeyboardState mKeyboardState ;

} ;

typedef PtrVector<Action> ActionPtrVector ;

// inserter
std::ostream& operator<<( std::ostream& , const Action& ) ;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define DEFINE_ACTION_CLASS( ClassName ) \
    public: \
        ClassName( const ApiAction* pAction ) ; \
        public: \
            virtual void doAction( void* pInfo , CSendInput* pSendInput ) const ; \
            virtual const char* pActionName() const ;

// ---------------------------------------------------------------------

class MouseLeftAction : public Action { DEFINE_ACTION_CLASS( MouseLeftAction ) ; } ;
class MouseRightAction : public Action { DEFINE_ACTION_CLASS( MouseRightAction ) ; } ;
class MouseUpAction : public Action { DEFINE_ACTION_CLASS( MouseUpAction ) ; } ;
class MouseDownAction : public Action { DEFINE_ACTION_CLASS( MouseDownAction ) ; } ;

class WheelLeftAction : public Action { DEFINE_ACTION_CLASS( WheelLeftAction ) ; } ;
class WheelRightAction : public Action { DEFINE_ACTION_CLASS( WheelRightAction ) ; } ;
class WheelUpAction : public Action { DEFINE_ACTION_CLASS( WheelUpAction ) ; } ;
class WheelDownAction : public Action { DEFINE_ACTION_CLASS( WheelDownAction ) ; } ;

class KeyPressAction : public Action { DEFINE_ACTION_CLASS( KeyPressAction ) ; } ;

// ---------------------------------------------------------------------

#endif // ACTIONS_HPP
