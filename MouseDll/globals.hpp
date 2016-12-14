#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <windows.h>
#include <iosfwd>
#include <vector>
#include <set>

#include "interception.h"
#include "device.hpp"
#include "deviceConfig.hpp"
#include "api.hpp"
#include "utils.hpp"
#include "debug.hpp"

// ---------------------------------------------------------------------

typedef std::vector<char> CharVector ;
typedef std::vector<wchar_t> WideCharVector ;
typedef std::set<int> IntSet ;
typedef std::set<std::string> StringSet ;

enum eKeyModifiers { kmCtrl=0x0001 , kmAlt=0x0002 , kmShift=0x0004 } ;
extern std::string keyModifiersString( int keyModifiers ) ;

// ---------------------------------------------------------------------

extern HMODULE ghInterceptionDll ;

typedef IntPtrMap<Device> DeviceTable ;
extern DeviceTable gDeviceTable ;

typedef IntPtrMap<DeviceConfig> DeviceConfigTable ;
extern DeviceConfigTable gDeviceConfigTable ;

typedef std::set<InterceptionDevice> InterceptionDeviceSet ;
extern InterceptionDeviceSet gUnknownDevices ;

extern PCALLBACKFN gpCallbackFn ;

// ---------------------------------------------------------------------

extern std::ostream& operator<<( std::ostream& os , const std::exception& xcptn ) ;

// ---------------------------------------------------------------------

#endif // GLOBALS_HPP
