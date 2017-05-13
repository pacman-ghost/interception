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
