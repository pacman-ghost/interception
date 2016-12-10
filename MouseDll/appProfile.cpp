#include "appProfile.hpp"
#include "api.hpp"

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

    // initialize the Device
    for ( int i=0 ; i < nEvents ; ++i )
    {
        const ApiEvent* pEvent = pEvents + i ;
        // validate the Action index/count
        if ( pEvent->mActionStartIndex < 0 || pEvent->mActionStartIndex >= nActions )
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

void
AppProfile::dumpAppProfile( ostream& os , const char* pPrefix ) const
{
    // dump the AppProfile
    if ( pPrefix == NULL )
        pPrefix = "" ;
    os << pPrefix << "AppProfile: \"" << app() << "\"" << endl ;
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

// ---------------------------------------------------------------------

const string& AppProfile::app() const { return mApp ; }
const EventPtrVector& AppProfile::events() const { return mEvents ; }
