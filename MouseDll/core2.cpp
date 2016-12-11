#include "globals.hpp"

using namespace std ;

// --- LOCAL DATA ------------------------------------------------------

// local functions:
static void doRunMainLoop( int* pExitFlag ) ;

// ---------------------------------------------------------------------

void
runMainLoop( int* pExitFlag )
{
    assert( pExitFlag != NULL ) ;

    // initialize
    *pExitFlag = 0 ;
    assert( gpCallbackFn != NULL ) ;
    (*gpCallbackFn)( CBTYPE_STARTED , "Main loop started." ) ;

    // run the main loop
    try
    {
        doRunMainLoop( pExitFlag ) ;
        (*gpCallbackFn)( CBTYPE_STOPPED , "Main loop stopped." ) ;
    }
    catch ( exception& xcptn )
    {
        (*gpCallbackFn)( CBTYPE_FATAL_ERROR , MAKE_CSTRING(xcptn.what()) ) ;
    }
    catch ( ... )
    {
        (*gpCallbackFn)( CBTYPE_FATAL_ERROR , "Unknown error." ) ;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void
doRunMainLoop( int* pExitFlag )
{
    assert( pExitFlag != NULL ) ;

    // run the main loop
    for ( ; ; )
    {
        cout << "zzz..." << endl ;
        Sleep( 1*1000 ) ;

        // check if we should exit
        if ( *pExitFlag != 0 )
            break ;
    }
}
