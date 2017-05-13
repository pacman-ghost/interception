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

#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "utils.hpp"

struct ApiDevice ;

// ---------------------------------------------------------------------

// This represents an input device (i.e. a mouse or trackball).

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