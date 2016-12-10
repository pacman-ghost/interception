#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <windows.h>
#include <iostream>
#include <fstream>

#include "device.hpp"
#include "deviceConfig.hpp"
#include "utils.hpp"

// ---------------------------------------------------------------------

extern HMODULE ghInterceptionDll ;

typedef IntPtrMap<Device> DeviceTable ;
extern DeviceTable gDeviceTable ;

typedef IntPtrMap<DeviceConfig> DeviceConfigTable ;
extern DeviceConfigTable gDeviceConfigTable ;

// ---------------------------------------------------------------------

#define LOG_MSG( msg ) \
{ \
    if ( gEnableConsole || gLogFile.is_open() ) \
    { \
        string _buf_ = MAKE_STRING( msg ) ; \
        if ( gEnableConsole ) \
            cout << _buf_ << endl ; \
        if ( gLogFile.is_open() ) \
            gLogFile << _buf_ << endl ; \
    } \
}

extern void initLogging( const wchar_t* ) ;
extern bool isLoggingEnabled( const std::string& ) ;

extern bool gEnableConsole ;
extern std::wstring gLogFilename ;
extern std::ofstream gLogFile ;

// ---------------------------------------------------------------------

#endif // GLOBALS_HPP
