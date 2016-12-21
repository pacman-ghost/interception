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

#define isSlash( ch ) ( (ch) == '/' || (ch) == '\\' )

extern std::ostream& operator<<( std::ostream& os , const std::exception& xcptn ) ;

// ---------------------------------------------------------------------

#endif // GLOBALS_HPP
