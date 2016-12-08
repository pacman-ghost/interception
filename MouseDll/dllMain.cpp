#include <windows.h>

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
            // clean up 
            break ; 
        default:
            break ;
	}

	return TRUE ;
}
