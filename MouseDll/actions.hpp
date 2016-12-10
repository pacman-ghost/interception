#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include "utils.hpp"

struct ApiAction ;

// ---------------------------------------------------------------------

class Action
{

// data types:
public:
    enum eActionType { atMouseLeft=1 , atMouseRight=2 , atMouseUp=3 , atMouseDown=4 } ;

// constructors/destructor:
public:
    static Action* allocAction( const ApiAction* pAction ) ;
protected:
    Action( const ApiAction* pAction ) ;
    DISABLE_COPY_ASSIGNMENT( Action ) ;

// action methods:
    virtual void doAction() const = 0 ;

// access methods:
public:
    virtual std::string asString() const = 0 ;

} ;

typedef PtrVector<Action> ActionPtrVector ;

// inserter
std::ostream& operator<<( std::ostream& , const Action& ) ;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define DEFINE_ACTION_CLASS( ClassName ) \
    public: \
        ClassName( const ApiAction* pAction ) ; \
        public: \
            virtual std::string asString() const ; \
            virtual void doAction() const ;

// ---------------------------------------------------------------------

class MouseLeftAction : public Action
{
    DEFINE_ACTION_CLASS( MouseLeftAction ) ;
} ;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class MouseRightAction : public Action
{
    DEFINE_ACTION_CLASS( MouseRightAction ) ;
} ;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class MouseUpAction : public Action
{
    DEFINE_ACTION_CLASS( MouseUpAction ) ;
} ;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class MouseDownAction : public Action
{
    DEFINE_ACTION_CLASS( MouseDownAction ) ;
} ;

// ---------------------------------------------------------------------

#endif // ACTIONS_HPP