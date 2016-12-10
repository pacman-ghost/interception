#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "utils.hpp"

struct ApiDevice ;

// ---------------------------------------------------------------------

class Device
{

// constructors/destructor:
public:
    Device( const ApiDevice* pDevice ) ;
    DISABLE_COPY_ASSIGNMENT( Device ) ;

// access methods:
public:
    int deviceId() const ;
    const std::string& hid() const ;
    int deviceNumber() const ;
    const std::string& displayName() const ;
    bool isEnabled() const ;

// miscellaneous methods:
public:
    void dumpDevice( std::ostream& os , const char* pPrefix="" ) const ;

// data members:
private:
    int mDeviceId ;
    std::string mHID ;
    int mDeviceNumber ;
    std::string mDisplayName ;
    bool mIsEnabled ;

} ;

// inserter
std::ostream& operator<<( std::ostream& , const Device& ) ;

// ---------------------------------------------------------------------

#endif // DEVICE_HPP