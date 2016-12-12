#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

#include "interception.h"
#include "device.hpp"
#include "deviceConfig.hpp"
#include "api.hpp"
#include "utils.hpp"

typedef std::vector<char> CharVector ;
typedef std::vector<wchar_t> WideCharVector ;
typedef std::set<int> IntSet ;
typedef std::set<std::string> StringSet ;

// ---------------------------------------------------------------------

enum eDirn { dUnknown=-1 , dLeft=1 , dRight=2 , dUp=3 , dDown=4 } ;
extern std::string toString( eDirn ) ;

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

#define LOG_MSG( msg ) \
{ \
    string _buf_ = makeLogMsg( MAKE_STRING( msg ) ) ; \
    cout << _buf_ ; \
    cout.flush() ; \
    if ( gLogFile.is_open() ) \
    { \
        gLogFile << _buf_ ; \
        gLogFile.flush() ; \
    } \
}
#define LOG_CMSG( c , msg ) \
{ \
    if ( isLoggingEnabled( c ) ) \
        LOG_MSG( msg ) ; \
}

extern void initLogging( const wchar_t* ) ;
extern bool isLoggingEnabled( const std::string& ) ;
extern std::string makeLogMsg( const std::string& ) ;

extern std::wstring gLogFilename ;
extern std::ofstream gLogFile ;

// ---------------------------------------------------------------------

extern std::ostream& operator<<( std::ostream& os , const std::exception& xcptn ) ;

// ---------------------------------------------------------------------

#endif // GLOBALS_HPP
