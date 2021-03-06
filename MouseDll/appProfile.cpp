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

#include "appProfile.hpp"
#include "globals.hpp"

using namespace std ;

// --- CONSTRUCTORS ----------------------------------------------------

AppProfile::AppProfile(
    const ApiAppProfile* pAppProfile ,
    const ApiEvent* pEvents , int nEvents ,
    const ApiAction* pActions , int nActions
)
{
    // initialize the AppProfile
    mApp = toUtf8( pAppProfile->mpApp ) ;
    mSensitivityX = pAppProfile->mSensitivityX ;
    mSensitivityY = pAppProfile->mSensitivityY ;
    mFallbackToDefaultAppProfile = pAppProfile->mFallbackToDefaultAppProfile ;

    // initialize the AppProfile
    for ( int i=0 ; i < nEvents ; ++i )
    {
        const ApiEvent* pEvent = pEvents + i ;
        // validate the Action index/count
        if ( pEvent->mActionStartIndex < 0 || (pEvent->mActionCount > 0 && pEvent->mActionStartIndex >= nActions) )
        {
            throw runtime_error(
                MAKE_STRING(
                    "Invalid Action start index: " << pEvent->mActionStartIndex
                    << " (#=" << nActions << ")"
                )
            ) ;
        }
        if ( pEvent->mActionCount < 0 || pEvent->mActionStartIndex + pEvent->mActionCount > nActions )
        {
            throw runtime_error(
                MAKE_STRING(
                    "Invalid Action count: " << pEvent->mActionCount
                    << " (s=" << pEvent->mActionStartIndex << ", #=" << nActions << ")"
                )
            ) ;
        }
        // add the next Event
        mEvents.push_back(
            new Event(
                pEvent ,
                pActions+pEvent->mActionStartIndex , pEvent->mActionCount
            )
        ) ;
    }
}

// ---------------------------------------------------------------------

const Event*
AppProfile::findEvent( Event::eEventType eventType , const KeyboardState& keyboardState ) const
{
    // find the specified event
    for ( EventPtrVector::const_iterator it=events().begin() ; it != events().end() ; ++it )
    {
        const Event* pEvent = *it ;
        // FIXME! if no key modifier events are configured, then ignore keyModifiers?
        if  ( pEvent->eventType() == eventType && pEvent->keyboardState() == keyboardState )
            return pEvent ;
    }

    return NULL ;
}

// ---------------------------------------------------------------------

int
AppProfile::sensitivity( Event::eEventType eventType ) const
{
    // convert the externally-configured values to internally-usable values
    switch( eventType ) // FIXME! shouldn't have a switch
    {
        case Event::etMouseLeft:
        case Event::etMouseRight:
            return 5 - sensitivityX() ; // FIXME! base value s.b. configurable
            break ;
        case Event::etMouseUp:
        case Event::etMouseDown:
            return 5 - sensitivityY() ; // FIXME! base value s.b. configurable
            break ;
        case Event::etWheelLeft:
        case Event::etWheelRight:
        case Event::etWheelUp:
        case Event::etWheelDown:
            return 1 ;
            break ;
        default:
            assert( false ) ;
            return 1 ;
            break ;
    }
}

// ---------------------------------------------------------------------

const string& AppProfile::app() const { return mApp ; }
int AppProfile::sensitivityX() const { return mSensitivityX ; }
int AppProfile::sensitivityY() const { return mSensitivityY ; }
bool AppProfile::fallbackToDefaultAppProfile() const { return mFallbackToDefaultAppProfile ; }
const EventPtrVector& AppProfile::events() const { return mEvents ; }

// ---------------------------------------------------------------------

void
AppProfile::dumpAppProfile( ostream& os , const char* pPrefix ) const
{
    // dump the AppProfile
    if ( pPrefix == NULL )
        pPrefix = "" ;
    os << pPrefix << "AppProfile: \"" << app() << "\"" << endl ;
    os << pPrefix << "  sensitivity: x=" << sensitivityX() << " ; y=" << sensitivityY() << endl ;
    os << pPrefix << "  fallbackToDefaultAppProfile = " << fallbackToDefaultAppProfile() << endl ;
    for ( EventPtrVector::const_iterator it=events().begin() ; it != events().end() ; ++it )
        (*it)->dumpEvent( os , MAKE_CSTRING(pPrefix << "   ") ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ostream&
operator<<( ostream& os , const AppProfile& appProfile )
{
    // insert the AppProfile
    os << "[AppProfile:" << appProfile.app() << "]" ;
    return os ;
}
