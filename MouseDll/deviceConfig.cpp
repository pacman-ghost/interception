#include "deviceConfig.hpp"
#include "globals.hpp"

using namespace std ;

// --- CONSTRUCTORS ----------------------------------------------------

DeviceConfig::DeviceConfig(
    const ApiDeviceConfig* pDeviceConfig ,
    const ApiAppProfile* pAppProfiles , int nAppProfiles ,
    const ApiEvent* pEvents , int nEvents ,
    const ApiAction* pActions , int nActions
)
{
    // initialize the DeviceConfig
    mDeviceId = pDeviceConfig->mDeviceId ;
    mStrokeHistoryResetInterval = pDeviceConfig->mStrokeHistoryResetInterval ;

    // initialize the DeviceConfig
    for ( int i=0 ; i < nAppProfiles ; ++i )
    {
        const ApiAppProfile* pAppProfile = pAppProfiles + i ;
        // validate the Event start index and count
        if ( pAppProfile->mEventStartIndex < 0 || (pAppProfile->mEventCount > 0 && pAppProfile->mEventStartIndex >= nEvents) )
        {
            throw runtime_error(
                MAKE_STRING(
                    "Invalid Event start index: " << pAppProfile->mEventStartIndex
                    << " (#=" << nEvents << ")"
                )
            ) ;
        }
        if ( pAppProfile->mEventCount < 0 || (pAppProfile->mEventCount > 0 && pAppProfile->mEventStartIndex+pAppProfile->mEventCount > nEvents) )
        {
            throw runtime_error(
                MAKE_STRING(
                    "Invalid Event count: " << pAppProfile->mEventCount
                    << " (s=" << pAppProfile->mEventStartIndex << ", #=" << nEvents << ")"
                )
            ) ;
        }
        // add the next Event
        mAppProfiles.push_back(
            new AppProfile(
                pAppProfile ,
                pEvents+pAppProfile->mEventStartIndex , pAppProfile->mEventCount ,
                pActions , nActions
            )
        ) ;
    }
}

// ---------------------------------------------------------------------

int DeviceConfig::deviceId() const { return mDeviceId ; }
const int DeviceConfig::strokeHistoryResetInterval() const { return mStrokeHistoryResetInterval ; }
const AppProfilePtrVector& DeviceConfig::appProfiles() const { return mAppProfiles ; }

// ---------------------------------------------------------------------

void
DeviceConfig::dumpDeviceConfig( ostream& os , const char* pPrefix ) const
{
    // dump the DeviceConfig
    if ( pPrefix == NULL )
        pPrefix = "" ;
    os << pPrefix << *this << ":" << endl ;
    os << pPrefix << "  deviceId = " << deviceId() << endl ;
    os << pPrefix << "  strokeHistoryResetInterval = " << strokeHistoryResetInterval() << endl ;
    for ( AppProfilePtrVector::const_iterator it=appProfiles().begin() ; it != appProfiles().end() ; ++it )
        (*it)->dumpAppProfile( os , MAKE_CSTRING(pPrefix << "    ") ) ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ostream&
operator<<( ostream& os , const DeviceConfig& deviceConfig )
{
    // insert the DeviceConfig
    os << "[DeviceConfig:" << deviceConfig.deviceId() << "]" ;
    return os ;
}
