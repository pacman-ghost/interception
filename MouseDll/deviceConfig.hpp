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

#ifndef DEVICECONFIG_HPP
#define DEVICECONFIG_HPP

#include "appProfile.hpp"
#include "utils.hpp"

struct ApiDeviceConfig ;

// ---------------------------------------------------------------------

// This represent the configuration associated with an input device (i.e. a mouse).
//  AppProfile's are associated with specific DeviceConfig's, so that different devices can be used
//  to control different applications.

class DeviceConfig
{

// constructors/destructor:
public:
    DeviceConfig(
        const ApiDeviceConfig* pDeviceConfig ,
        const ApiAppProfile* pAppProfiles , int nAppProfiles ,
        const ApiEvent* pEvents , int nEvents ,
        const ApiAction* pActions , int nActions
    ) ;
    DISABLE_COPY_ASSIGNMENT( DeviceConfig ) ;

// access methods:
public:
    int deviceId() const ;
    const int strokeHistoryResetInterval() const ;
    const AppProfilePtrVector& appProfiles() const ;

// miscellaneous methods:
public:
    void dumpDeviceConfig( std::ostream& os , const char* pPrefix="" ) const ;

// data members:
private:
    int mDeviceId ;
    int mStrokeHistoryResetInterval ;
    AppProfilePtrVector mAppProfiles ;

} ;

// inserter
std::ostream& operator<<( std::ostream& , const DeviceConfig& ) ;

// ---------------------------------------------------------------------

#endif // DEVICECONFIG_HPP
