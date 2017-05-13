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
