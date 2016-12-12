#ifndef DEVICECONFIG_HPP
#define DEVICECONFIG_HPP

#include "appProfile.hpp"
#include "utils.hpp"

struct ApiDeviceConfig ;

// ---------------------------------------------------------------------

class DeviceConfig
{

// constructors/destructor:
public:
    DeviceConfig(
        const ApiDeviceConfig* pDeviceConfig ,
        const ApiAppProfile* pAppProfiles , int nAppProfiles ,
        const ApiEvent* pEvents , int nEvents ,
        const ApiAction* pActions , int nActions
    ) ;
    DISABLE_COPY_ASSIGNMENT( DeviceConfig ) ;

// access methods:
public:
    int deviceId() const ;
    const int strokeHistoryResetInterval() const ;
    const AppProfilePtrVector& appProfiles() const ;

// miscellaneous methods:
public:
    void dumpDeviceConfig( std::ostream& os , const char* pPrefix="" ) const ;

// data members:
private:
    int mDeviceId ;
    int mStrokeHistoryResetInterval ;
    AppProfilePtrVector mAppProfiles ;

} ;

// inserter
std::ostream& operator<<( std::ostream& , const DeviceConfig& ) ;

// ---------------------------------------------------------------------

#endif // DEVICECONFIG_HPP
