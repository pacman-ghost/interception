/*
 * Copyright 2017 Taka Muraoka
 *
 * This file is part of the MouseInterception project.
 *
 * The MouseInterception project is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * The MouseInterception project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the MouseInterception project.  If not, see <http://www.gnu.org/licenses/>.
 */

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

extern HMODULE ghInterceptionDll ; // handle to interception.dll

typedef IntPtrMap<Device> DeviceTable ;
extern DeviceTable gDeviceTable ; // configured devices

typedef IntPtrMap<DeviceConfig> DeviceConfigTable ;
extern DeviceConfigTable gDeviceConfigTable ; // device configuration

typedef std::set<InterceptionDevice> InterceptionDeviceSet ;
extern InterceptionDeviceSet gUnknownDevices ; // devices we've received events for, but have no configuration for

extern PCALLBACKFN gpCallbackFn ; // for calling back into the main C# app

// ---------------------------------------------------------------------

#define isSlash( ch ) ( (ch) == '/' || (ch) == '\\' )

extern std::ostream& operator<<( std::ostream& os , const std::exception& xcptn ) ;

// ---------------------------------------------------------------------

#endif // GLOBALS_HPP
