#include "device.hpp"
#include "api.hpp"
#include "utils.hpp"

using namespace std ;

// --- CONSTRUCTORS ----------------------------------------------------

Device::Device( const ApiDevice* pDevice )
{
    // initialize the Device
    mDeviceId = pDevice->mDeviceId ;
    mHID = toUtf8( pDevice->mpHID ) ;
    mDeviceNumber = pDevice->mDeviceNumber ;
    mDisplayName = toUtf8( pDevice->mpDisplayName ) ;
    mIsEnabled = pDevice->mIsEnabled ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Device::Device()
    : mDeviceId(-1)
    , mHID() , mDeviceNumber(-1)
    , mDisplayName() , mIsEnabled(false)
{
}

// --- COPY/ASSIGNMENT -------------------------------------------------

Device& Device::operator=( const Device& rhs )
{
    // copy the Device
    mDeviceId = rhs.deviceId() ;
    mHID = rhs.hid() ;
    mDeviceNumber = rhs.deviceNumber() ;
    mDisplayName = rhs.displayName() ;
    mIsEnabled = rhs.isEnabled() ;
    return *this ;
}

Device::Device( const Device& rhs ) { operator=(rhs) ; }

// ---------------------------------------------------------------------

void
Device::dumpDevice( ostream& os , const char* pPrefix ) const
{
    // dump the device
    if ( pPrefix == NULL )
        pPrefix = "" ;
    os << pPrefix << "Device (id=" << deviceId() << "):" << endl ;
    os << pPrefix << "- hid = \"" << hid() << "\"" << endl ;
    os << pPrefix << "- deviceNumber = " << deviceNumber() << endl ;
    os << pPrefix << "- displayName = \"" << displayName() << "\"" << endl ;
    os << pPrefix << "- isEnabled = " << isEnabled() << endl ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ostream&
operator<<( ostream& os , const Device& device )
{
    // insert the Device
    os << "[" << device.deviceId() << ":" << device.hid() << "/" << device.deviceNumber() << "]" ;
    return os ;
}

// ---------------------------------------------------------------------

int Device::deviceId() const { return mDeviceId ; }
const string& Device::hid() const { return mHID ; }
int Device::deviceNumber() const { return mDeviceNumber ; }
const string& Device::displayName() const { return mDisplayName ; }
bool Device::isEnabled() const { return mIsEnabled ; }
