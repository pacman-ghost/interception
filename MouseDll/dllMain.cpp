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

#include <windows.h>
#include "globals.hpp"

// --------------------------------------------------------------------

BOOL APIENTRY
DllMain( HINSTANCE hInst , DWORD dwReason , LPVOID lpReserved )
{
    switch( dwReason )
    {
        case DLL_PROCESS_ATTACH:
            // initialize the DLL
            break ; 
        case DLL_PROCESS_DETACH:
            // make sure that everything was closed properly
            assert( ghInterceptionDll == NULL ) ;
            assert( gpCallbackFn == NULL ) ;
            break ; 
        default:
            break ;
	}

	return TRUE ;
}
