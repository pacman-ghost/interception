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

#ifndef APPPROFILE_HPP
#define APPPROFILE_HPP

#include "event.hpp"

struct ApiAppProfile ;

// ---------------------------------------------------------------------

// An AppProfile is the configuration that is to be used for each application (identified by EXE path).
//  This lets us have different events and actions, sensitivity tuning, etc. for different programs.

class AppProfile
{

// constructors/destructor:
public:
    AppProfile(
        const ApiAppProfile* pAppProfile ,
        const ApiEvent* pEvents , int nEvents ,
        const ApiAction* pActions , int nActions
    ) ;
    DISABLE_COPY_ASSIGNMENT( AppProfile ) ;

// access methods:
public:
    const std::string& app() const ;
    int sensitivity( Event::eEventType eventType ) const ;
    int sensitivityX() const ;
    int sensitivityY() const ;
    bool fallbackToDefaultAppProfile() const ;
    const EventPtrVector& events() const ;
public:
    const Event* findEvent( Event::eEventType eventType , const KeyboardState& keyboardState ) const ;

// miscellaneous methods:
public:
    void dumpAppProfile( std::ostream& os , const char* pPrefix="" ) const ;

// data members:
private:
    std::string mApp ;
    int mSensitivityX ;
    int mSensitivityY ;
    bool mFallbackToDefaultAppProfile ;
    EventPtrVector mEvents ;
} ;

typedef PtrVector<AppProfile> AppProfilePtrVector ;

// inserter
std::ostream& operator<<( std::ostream& , const AppProfile& ) ;

// ---------------------------------------------------------------------

#endif // APPPROFILE_HPP