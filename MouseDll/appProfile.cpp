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
    // initialize the Device
    mApp = toUtf8( pAppProfile->mpApp ) ;
    mSensitivityX = pAppProfile->mSensitivityX ;
    mSensitivityY = pAppProfile->mSensitivityY ;

    // initialize the Device
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
AppProfile::findEvent( Event::eEventType eventType , int keyModifiers ) const
{
    // find the specified event
    for ( EventPtrVector::const_iterator it=events().begin() ; it != events().end() ; ++it )
    {
        const Event* pEvent = *it ;
        // FIXME! if no key modifier events are configured, then ignore keyModifiers?
        if  ( pEvent->eventType() == eventType && pEvent->keyModifiers() == keyModifiers )
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
const EventPtrVector& AppProfile::events() const { return mEvents ; }

// ---------------------------------------------------------------------

void
AppProfile::dumpAppProfile( ostream& os , const char* pPrefix ) const
{
    // dump the AppProfile
    if ( pPrefix == NULL )
        pPrefix = "" ;
    os << pPrefix << "AppProfile: \"" << app() << "\"" << endl ;
    os << "  sensitivity: x=" << sensitivityX() << " ; y=" << sensitivityY() << endl ;
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
