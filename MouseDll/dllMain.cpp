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
