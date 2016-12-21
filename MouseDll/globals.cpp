#include "globals.hpp"

using namespace std ;

// ---------------------------------------------------------------------

// FIXME! We changed MainApp from AnyCPU to x86, to simplify calling into the mouse DLL (which we build as 32-bit).
// We should really probably provide separate 32- and 64-bit builds :-/
#pragma comment( lib , "../interception/library/x86/interception.lib" )

// ---------------------------------------------------------------------

HMODULE ghInterceptionDll = NULL ;

DeviceTable gDeviceTable ;
DeviceConfigTable gDeviceConfigTable ;
InterceptionDeviceSet gUnknownDevices ;

PCALLBACKFN gpCallbackFn = NULL ;

// ---------------------------------------------------------------------

ostream&
operator<<( ostream& os , const exception& xcptn )
{
    // insert the exception
    os << xcptn.what() ;
    return os ;
}
