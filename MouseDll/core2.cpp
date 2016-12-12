#include <deque>

#include "globals.hpp"

using namespace std ;

typedef deque<InterceptionMouseStroke> InterceptionMouseStrokeDeque ;
typedef map< InterceptionDevice , InterceptionMouseStrokeDeque > InterceptionMouseStrokeHistory ;

// --- LOCAL DATA ------------------------------------------------------

#define MAX_STROKE_HISTORY 20
#define DIRN_DETECT_WINDOW_SIZE 10
#define DIRN_DETECT_HORZ_BIAS 1.2 // FIXME! s.b. configurable

// local functions:
static void doRunMainLoop( int* pExitFlag ) ;
static bool detectDirn( const InterceptionMouseStrokeDeque* pStrokeHistory , eDirn* pDirn , int* pMagnitude ) ;
static bool findDevice( InterceptionDevice hDevice , Device** ppDevice ) ;

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

    // initialize
    InterceptionMouseStrokeHistory strokeHistory ;

    // run the main loop
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
        InterceptionStroke stroke ;
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

        // find the device that generated the event
        Device* pDevice ;
        if ( ! findDevice( hDevice , &pDevice ) )
        {
            // can't find the the device - check if we've seen it before
            if ( gUnknownDevices.find( hDevice ) == gUnknownDevices.end() )
            {
                // nope - notify the front-end about the new device
                // NOTE: We seem to get multiple hardware ID's back, but the first one seems to be the one we want :shrug:
                WideCharVector buf( 4*1024 ) ;
                size_t nChars = interception_get_hardware_id( hContext , hDevice , &buf[0] , buf.size()*sizeof(wchar_t) ) ;
                (*gpCallbackFn)( CBTYPE_NEW_DEVICE , MAKE_CSTRING(hDevice << "|" << toUtf8(&buf[0],nChars)) ) ;
                gUnknownDevices.insert( hDevice ) ;
            }

            // forward the event on (for normal processing)
            interception_send( hContext , hDevice ,&stroke , 1 ) ;
            continue ;
        }

        // check if the device is enabled
        if ( ! pDevice->isEnabled() )
        {
            // nope - just forward the event (for normal processing)
            interception_send( hContext , hDevice ,&stroke , 1 ) ;
            continue ;
        }

        // record the stroke
        InterceptionMouseStrokeDeque* pStrokeHistory = & strokeHistory[hDevice] ;
        pStrokeHistory->push_back( *pStroke ) ;
        while ( pStrokeHistory->size() > MAX_STROKE_HISTORY )
            pStrokeHistory->pop_front() ;

        // figure out which way the mouse is moving
        eDirn dirn ;
        int dirnMagnitude ;
        (void) detectDirn( pStrokeHistory , &dirn , &dirnMagnitude ) ;

        // FIXME! handle the event
        interception_send( hContext , hDevice ,&stroke , 1 ) ;
    }

    // clean up
    LOG_CMSG( "system" , "Destroying context..." ) ;
    interception_destroy_context( hContext ) ;

    LOG_CMSG( "system" , "Main loop ended." ) ;
}

// ---------------------------------------------------------------------

static bool
detectDirn( const InterceptionMouseStrokeDeque* pStrokeHistory , eDirn* pDirn , int* pDirnMagnitude )
{
    // check if we have enough stroke history
    if ( pStrokeHistory->size() < DIRN_DETECT_WINDOW_SIZE )
    {
        LOG_CMSG( "dirnDetect" , "DIRN DETECT: #=" << pStrokeHistory->size() << " ; dirn=" << toString(dUnknown) ) ;
        return false ;
    }

    // figure out which direction the mouse is moving in
    // FIXME! limit to left/right for h-wheel
    LOG_CMSG( "dirnDetect2" , "DIRN DETECT: #=" << DIRN_DETECT_WINDOW_SIZE << "/" << pStrokeHistory->size() ) ;
    int cumX=0 , cumY=0 , nStrokes=0 ;
    for ( InterceptionMouseStrokeDeque::const_reverse_iterator it=pStrokeHistory->rbegin() ; it != pStrokeHistory->rend() ; ++it )
    {
        const InterceptionMouseStroke& stroke = *it ;
        cumX += (*it).x ;
        cumY += (*it).y ; 
        LOG_CMSG( "dirnDetect2" , "  x=" << (*it).x << " ; y=" << (*it).y << " ; cum=" << cumX << "/" << cumY ) ;
        if ( ++nStrokes >= DIRN_DETECT_WINDOW_SIZE )
            break ;
    }
    // NOTE: It's easier to move a device up/down without drifting left/right, so we bias left/right detection.
    int biasedCumX = (int)(1000*cumX * DIRN_DETECT_HORZ_BIAS) / 1000 ;
    if ( abs(biasedCumX) >= abs(cumY) )
    {
        *pDirn = biasedCumX < 0 ? dLeft : dRight ;
        *pDirnMagnitude = biasedCumX / nStrokes ;
    }
    else
    {
        *pDirn = cumY < 0 ? dUp : dDown ;
        *pDirnMagnitude = cumY / nStrokes ;
    }
    LOG_CMSG( "dirnDetect" ,
        "DIRN DETECT: #=" << DIRN_DETECT_WINDOW_SIZE << "/" << pStrokeHistory->size()
        << " ; cumX=" << biasedCumX << "(" << cumX << ") ; cumY=" << cumY
        << " ; dirn=" << toString(*pDirn) << "/" << *pDirnMagnitude
    ) ;
    return true ;
}

// ---------------------------------------------------------------------

static bool
findDevice( InterceptionDevice hDevice , Device** ppDevice )
{
    // find the specified device
    for( DeviceTable::iterator it=gDeviceTable.begin() ; it != gDeviceTable.end() ; ++it )
    {
        Device* pDevice = (*it).second ;
        // FIXME! have to check HID as well
        if ( pDevice->deviceNumber() == hDevice )
        {
            *ppDevice = pDevice ;
            return true ;
        }
    }
    return false ;
}
