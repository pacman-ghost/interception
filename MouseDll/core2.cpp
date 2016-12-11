#include "interception.h"

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
        LOG_CMSG( "system" , "Main loop exception: " << xcptn ) ;
        (*gpCallbackFn)( CBTYPE_FATAL_ERROR , MAKE_CSTRING(xcptn) ) ;
    }
    catch ( ... )
    {
        LOG_CMSG( "system" , "Main loop unknown exception." ) ;
        (*gpCallbackFn)( CBTYPE_FATAL_ERROR , "Unknown error." ) ;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void
doRunMainLoop( int* pExitFlag )
{
    assert( pExitFlag != NULL ) ;

    // initialize
    LOG_CMSG( "system" , "Main loop started." ) ;
	SetPriorityClass( GetCurrentProcess() , HIGH_PRIORITY_CLASS ) ;

    // configure Interception
    LOG_CMSG( "system" , "Creating context..." ) ;
    InterceptionContext hContext = interception_create_context() ;
    if ( hContext == NULL )
        throw runtime_error( "Can't create context." ) ;
    LOG_CMSG( "system" , "- hContext = " << hContext ) ;
    LOG_CMSG( "system" , "Setting mouse filter..." ) ;
	interception_set_filter(
        hContext ,
        interception_is_mouse ,
        INTERCEPTION_FILTER_MOUSE_MOVE | INTERCEPTION_FILTER_MOUSE_WHEEL | INTERCEPTION_FILTER_MOUSE_HWHEEL
    ) ;

    // run the main loop
    InterceptionStroke stroke ;
    for ( ; ; )
    {
        // wait for the next event
        InterceptionDevice hDevice = interception_wait_with_timeout( hContext , 200 ) ;
        if ( hDevice == NULL )
        {
            // check if we should exit
            if ( *pExitFlag != 0 )
                break ;
            continue ;
        }
        int nStrokes = interception_receive( hContext , hDevice , &stroke , 1 ) ;
        if ( nStrokes <= 0 )
            break ;
        if ( ! interception_is_mouse( hDevice ) )
            continue ;

        // get the event
        InterceptionMouseStroke* pStroke = (InterceptionMouseStroke*) &stroke ;
        int keyModifiers = 0 ;
        if ( GetAsyncKeyState(VK_CONTROL) < 0 )
            keyModifiers |= Event::kmCtrl ;
        if ( GetAsyncKeyState(VK_MENU) < 0 )
            keyModifiers |= Event::kmAlt ;
        if ( GetAsyncKeyState(VK_SHIFT) < 0 )
            keyModifiers |= Event::kmShift ;

        // log the raw event
        if ( isLoggingEnabled( "rawEvents" ) )
        {
            stringstream buf ;
            if ( keyModifiers != 0 )
                buf << " " << Event::keyModifiersString(keyModifiers) << " ;" ;
            const char* pEventType ;
            if ( pStroke->state & INTERCEPTION_MOUSE_WHEEL )
                pEventType = "WHEEL" ;
            else if ( pStroke->state & INTERCEPTION_MOUSE_HWHEEL )
                pEventType = "HWHEEL" ;
            else
                pEventType = "MOVE" ;
            LOG_MSG(
                pEventType << ":" << buf.str() << " hDevice=" << hDevice 
			    << " ; state=0x" << hexString(pStroke->state)
			    << " ; flags=0x" << hexString(pStroke->flags)
			    << " ; rolling=" << pStroke->rolling
			    << " ; x=" << pStroke->x
			    << " ; y=" << pStroke->y
			    << " ; info=" << pStroke->information
            ) ;
        }

        interception_send( hContext , hDevice ,&stroke , 1 ) ;
    }

    // clean up
    LOG_CMSG( "system" , "Destroying context..." ) ;
    interception_destroy_context( hContext ) ;

    LOG_CMSG( "system" , "Main loop ended." ) ;
}
