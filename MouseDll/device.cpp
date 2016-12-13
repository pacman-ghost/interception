#include "device.hpp"
#include "globals.hpp"

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

// ---------------------------------------------------------------------

int Device::deviceId() const { return mDeviceId ; }
const string& Device::hid() const { return mHID ; }
int Device::deviceNumber() const { return mDeviceNumber ; }
const string& Device::displayName() const { return mDisplayName ; }
bool Device::isEnabled() const { return mIsEnabled ; }

// ---------------------------------------------------------------------

void
Device::dumpDevice( ostream& os , const char* pPrefix ) const
{
    // dump the Device
    if ( pPrefix == NULL )
        pPrefix = "" ;
    os << pPrefix << *this << ":" << endl ;
    os << pPrefix << "  hid = \"" << hid() << "\"" << endl ;
    os << pPrefix << "  deviceNumber = " << deviceNumber() << endl ;
    os << pPrefix << "  displayName = \"" << displayName() << "\"" << endl ;
    os << pPrefix << "  isEnabled = " << isEnabled() << endl ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ostream&
operator<<( ostream& os , const Device& device )
{
    // insert the Device
    os << "[Device-" << device.deviceId() << ":" << device.hid() << "/" << device.deviceNumber() << "]" ;
    return os ;
}
