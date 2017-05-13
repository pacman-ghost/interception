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

#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>

// ---------------------------------------------------------------------

extern int gEventWaitTimeout ;
extern int gMaxStrokeHistory ;
extern int gDefaultStrokeResetHistoryInterval ;
extern int gDetectMouseMove_WindowSize ;
extern int gDetectMouseMove_HorzBias ;

extern void loadDebugConfig( const wchar_t* pDebugConfigFilename ) ;

// ---------------------------------------------------------------------

#define LOG_MSG( msg ) \
{ \
    if ( gpLogStream != NULL ) \
    { \
        *gpLogStream << makeLogMsg( MAKE_STRING( msg ) ) ; \
        gpLogStream->flush() ; \
    } \
}

#define LOG_CMSG( c , msg ) \
{ \
    if ( isLoggingEnabled( c ) ) \
        LOG_MSG( msg ) ; \
}

extern bool isLoggingEnabled( const std::string& ) ;
extern std::string makeLogMsg( const std::string& ) ;

extern std::ostream* gpLogStream ;

// ---------------------------------------------------------------------

#endif // DEBUG_HPP
