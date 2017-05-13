/*
 * Copyright 2017 Taka Muraoka
 *
 * This file is part of the MouseInterception project.
 *
 * The MouseInterception project is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * The MouseInterception project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the MouseInterception project.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include "keyboardState.hpp"
#include "utils.hpp"

struct ApiAction ;
class CSendInput ;

// ---------------------------------------------------------------------

// These classes define events we can emit e.g. mouse move messages, scroll-wheel up/down, key-presses.

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
