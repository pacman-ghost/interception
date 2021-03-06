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
#include <psapi.h>
#include <deque>

#include "globals.hpp"
#include "sendInput.hpp"

using namespace std ;

typedef pair< DWORD , InterceptionMouseStroke > TimestampMouseStrokePair ;
typedef deque<TimestampMouseStrokePair> MouseStrokeHistory ;
typedef map< InterceptionDevice , MouseStrokeHistory > MouseStrokeHistoryTable ;

typedef map< Event::eEventType , size_t > EventTypeSizetMap ;
typedef map< InterceptionDevice , EventTypeSizetMap > EventCountTable ;

// --- LOCAL DATA ------------------------------------------------------

// local functions:
static void doRunMainLoop( int* pExitFlag ) ;
static bool detectMouseMove( const MouseStrokeHistory* pStrokeHistory , Event::eEventType* pEventType , int* pMagnitude ) ;
static bool findDevice( InterceptionDevice hDevice , const Device** ppDevice , const DeviceConfig** ppDeviceConfig ) ;
static const AppProfile* findAppProfile( const DeviceConfig* pDeviceConfig, const AppProfile** ppDefaultAppProfile ) ;
static bool isAbsolutePath( const string& path ) ;
static void getFilename( string* pPath ) ;

// --- LOCAL DATA ------------------------------------------------------

enum eStrokeType { stMouseMove , stMouseWheel , stMouseHorzWheel } ;
static EnumStringInfo gStrokeTypeStringTable[] = {
    { stMouseMove , "MOVE" } ,
    { stMouseWheel , "WHEEL" } ,
    { stMouseHorzWheel , "H-WHEEL" } ,
    { -1 , NULL }
} ;

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
    DWORD startTime = GetTickCount() ;
    MouseStrokeHistoryTable mouseMovesHistoryTable ;
    EventCountTable eventCounts ;
    int nWaitFailures = 0 ;
    for ( ; ; )
    {
        // wait for the next event
        // NOTE: If the driver is not installed, this will return immediately :-/
        InterceptionDevice hDevice = interception_wait_with_timeout( hContext , gEventWaitTimeout ) ;
        if ( hDevice == NULL )
        {
            // check if we should exit
            if ( *pExitFlag != 0 )
                break ;
            // FUDGE! interception_wait_with_timeout() doesn't distinguish between a timeout and a failure, so we try
            //  to figure out what's happened here (otherwise we get into a tight infinite loop :-/).
            if ( mouseMovesHistoryTable.empty() && ++nWaitFailures > 10 && GetTickCount()-startTime < 500 )
                throw runtime_error( "Can't connect to the mouse driver." ) ;
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
        DWORD strokeTimestamp = GetTickCount() ;
        eStrokeType strokeType ;
        if ( pStroke->state & INTERCEPTION_MOUSE_WHEEL )
            strokeType = stMouseWheel ;
        else if ( pStroke->state & INTERCEPTION_MOUSE_HWHEEL )
            strokeType = stMouseHorzWheel ;
        else
            strokeType = stMouseMove ;
        string strokeTypeString = enumString( gStrokeTypeStringTable , strokeType ) ;
        KeyboardState strokeKeyboardState = KeyboardState::getCurrKeyboardState() ;

        // log the raw event
        if ( isLoggingEnabled( "rawEvents" ) )
        {
            string strokeKeyboardStateString ;
            if ( strokeKeyboardState.isAnythingDown() )
                strokeKeyboardStateString = MAKE_STRING( " " << strokeKeyboardState << " ;" ) ;
            LOG_MSG(
                strokeTypeString << ":" << strokeKeyboardStateString
                << " hDevice=" << hDevice 
			    << " ; state=0x" << hexString(pStroke->state)
			    << " ; flags=0x" << hexString(pStroke->flags)
			    << " ; rolling=" << pStroke->rolling
			    << " ; x=" << pStroke->x
			    << " ; y=" << pStroke->y
			    << " ; info=" << pStroke->information
            ) ;
        }

        // find the device that generated the event
        const Device* pDevice ;
        const DeviceConfig* pDeviceConfig ;
        if ( ! findDevice( hDevice , &pDevice , &pDeviceConfig ) )
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

            // forward the event (for normal processing)
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

        // find the required AppProfile
        const AppProfile* pDefaultAppProfile ;
        const AppProfile* pAppProfile = findAppProfile( pDeviceConfig , &pDefaultAppProfile ) ;
        if ( pAppProfile == NULL )
            pAppProfile = pDefaultAppProfile ;
        if ( pAppProfile == NULL )
        {
            // can't find one - just forward the event (for normal processing)
            interception_send( hContext , hDevice ,&stroke , 1 ) ;
            continue ;
        }

        // figure out what we should do
        const Event* pEvent = NULL ;
        void* pActionInfo = NULL ;
        if ( strokeType == stMouseMove )
        {
            // record the stroke
            MouseStrokeHistory* pStrokeHistory = & mouseMovesHistoryTable[ hDevice ] ;
            int strokeHistoryResetInterval = pDeviceConfig->strokeHistoryResetInterval() ;
            if ( strokeHistoryResetInterval <= 0 )
                strokeHistoryResetInterval = gDefaultStrokeResetHistoryInterval ;
            if ( ! pStrokeHistory->empty() && (int)(strokeTimestamp-pStrokeHistory->back().first) >= strokeHistoryResetInterval )
            {
                // we haven't seen a move event for a while - reset the history
                pStrokeHistory->clear() ;
            }
            pStrokeHistory->push_back( make_pair( strokeTimestamp , *pStroke ) ) ;
            while ( (int)pStrokeHistory->size() > gMaxStrokeHistory )
                pStrokeHistory->pop_front() ;
            // figure out which way the mouse is moving
            Event::eEventType eventType ;
            int magnitude ;
            if ( detectMouseMove( pStrokeHistory , &eventType , &magnitude ) )
            {
                LOG_CMSG( "events" , strokeTypeString << ": " << eventType << "/" << magnitude )
                // check if this event has been configured
                pEvent = pAppProfile->findEvent( eventType , strokeKeyboardState ) ;
                if ( pEvent == NULL && pAppProfile != pDefaultAppProfile && pAppProfile->fallbackToDefaultAppProfile() )
                    pEvent = pDefaultAppProfile->findEvent( eventType , strokeKeyboardState ) ;
                // scale the movement (100 = 1 unit)
                pActionInfo = (void*) (100 * magnitude) ;
            }
        }
        else if ( strokeType == stMouseWheel )
        {
            // figure out which way the wheel is moving
            int dirn = (pStroke->rolling) < 0 ? -1 : +1 ;
            int wheelSize = abs( pStroke->rolling ) ;
            LOG_CMSG( "events" , strokeTypeString << ": " << (dirn < 0?"down":"up") << "/" << wheelSize ) ;
            // check if this event has been configured
            Event::eEventType eventType = (dirn < 0) ? Event::etWheelDown : Event::etWheelUp ;
            pEvent = pAppProfile->findEvent( eventType , strokeKeyboardState ) ;
            if ( pEvent == NULL && pAppProfile != pDefaultAppProfile && pAppProfile->fallbackToDefaultAppProfile() )
                pEvent = pDefaultAppProfile->findEvent( eventType , strokeKeyboardState ) ;
            // scale the movement (100 = 1 unit)
            pActionInfo = (void*) (100 * wheelSize / (WHEEL_DELTA/10)) ; // FIXME! scaling s.b. configurable
        }
        else if ( strokeType == stMouseHorzWheel )
        {
            // figure out which way the wheel is moving
            int dirn = (pStroke->rolling) < 0 ? -1 : +1 ;
            int wheelSize = abs( pStroke->rolling ) ;
            LOG_CMSG( "events" , strokeTypeString << ": " << (dirn < 0?"left":"right") << "/" << wheelSize ) ;
            // check if this event has been configured
            Event::eEventType eventType = (dirn < 0) ? Event::etWheelLeft : Event::etWheelRight ;
            pEvent = pAppProfile->findEvent( eventType , strokeKeyboardState ) ;
            if ( pEvent == NULL && pAppProfile != pDefaultAppProfile && pAppProfile->fallbackToDefaultAppProfile() )
                pEvent = pDefaultAppProfile->findEvent( eventType , strokeKeyboardState ) ;
            // scale the movement (100 = 1 unit)
            pActionInfo = (void*) (100 * wheelSize / (WHEEL_DELTA/10)) ; // FIXME! scaling s.b. configurable
        }
        else
            assert( false ) ;

        // check for sensitivity
        if ( pEvent != NULL )
        {
            // nb: we reduce sensitivity by ignoring events
            int sensitivity = pAppProfile->sensitivity( pEvent->eventType() ) ;
            if ( sensitivity > 0 && (++eventCounts[hDevice][pEvent->eventType()] % sensitivity) != 0 )
                continue ; // nb: we don't forward the event
        }

        // handle the event
        if ( pEvent != NULL )
        {
            // NOTE: I tried managing the keyboard state here, so that we don't keep sending up/down events for Ctrl/Alt/Shift
            //  before/after every action, but it interferes with event detection, because the keyboard state is now wrong :-/
            CSendInput sendInput ;
            for ( ActionPtrVector::const_iterator it=pEvent->actions().begin() ; it != pEvent->actions().end() ; ++it )
            {
                const Action* pAction = *it ;
                LOG_CMSG( "actions" , "ACTION: " << *pAction << " (" << dec << (int)pActionInfo << ")" ) ;
                pAction->doAction( pActionInfo , &sendInput ) ;
            }
            continue ;
        }

        // the event wasn't handled - forward the event (for normal processing)
        interception_send( hContext , hDevice , &stroke , 1 ) ;
    }

    // clean up
    LOG_CMSG( "system" , "Destroying context..." ) ;
    interception_destroy_context( hContext ) ;

    LOG_CMSG( "system" , "Main loop ended." ) ;
}

// ---------------------------------------------------------------------

static bool
detectMouseMove( const MouseStrokeHistory* pStrokeHistory , Event::eEventType* pEventType , int* pMagnitude )
{
    // check if we have enough stroke history
    if ( (int)pStrokeHistory->size() < gDetectMouseMove_WindowSize )
        return false ;

    // figure out which direction the mouse is moving in
    LOG_CMSG( "detectMouseMove" , "DETECT MOUSE MOVE: #=" << gDetectMouseMove_WindowSize << "/" << pStrokeHistory->size() ) ;
    int cumX=0 , cumY=0 , nStrokes=0 ;
    const InterceptionMouseStroke* pPrevStroke = NULL ;
    for ( MouseStrokeHistory::const_reverse_iterator it=pStrokeHistory->rbegin() ; it != pStrokeHistory->rend() ; ++it )
    {
        const InterceptionMouseStroke& stroke = (*it).second ;
        if ( (stroke.flags & MOUSE_MOVE_ABSOLUTE) )
        {
            // VMware sends us absolute mouse co-ordinates. The screen seems to be sized as 65,535x65,535 and this post
            //  seems to confirm that: http://stackoverflow.com/a/35141494
            if ( pPrevStroke != NULL )
            {
                // FIXME! handle multiple monitors
                int deltaX = pPrevStroke->x - stroke.x ;
                cumX += deltaX * GetSystemMetrics(SM_CXSCREEN) / 0xFFFF ;
                int deltaY = pPrevStroke->y - stroke.y ;
                cumY += deltaY * GetSystemMetrics(SM_CYSCREEN) / 0xFFFF ;
                LOG_CMSG( "detectMouseMove" , "  x=" << stroke.x << " ; y=" << stroke.y << " ; cum=" << cumX << "/" << cumY ) ;
            }
            else
                LOG_CMSG( "detectMouseMove" , "  x=" << stroke.x << " ; y=" << stroke.y ) ;
            pPrevStroke = &stroke ;
        }
        else
        {
            // nb: we assume MOUSE_MOVE_RELATIVE (which is defined as 0)
            // FIXME! What is MOUSE_VIRTUAL_DESKTOP? Doco for SM_CXVIRTUALSCREEN implies that
            //  it's all the monitors merged into one big desktop.
            cumX += stroke.x ;
            cumY += stroke.y ;
            LOG_CMSG( "detectMouseMove" , "  x=" << stroke.x << " ; y=" << stroke.y << " ; cum=" << cumX << "/" << cumY ) ;
        }
        if ( ++nStrokes >= gDetectMouseMove_WindowSize )
            break ;
    }
    // NOTE: It's easier to move a device up/down without drifting left/right, so we bias left/right detection.
    int biasedCumX = (int)(1000*cumX * gDetectMouseMove_HorzBias/100) / 1000 ;
    if ( abs(biasedCumX) >= abs(cumY) )
    {
        *pEventType = biasedCumX < 0 ? Event::etMouseLeft : Event::etMouseRight;
        *pMagnitude = abs( biasedCumX / nStrokes ) ;
    }
    else
    {
        *pEventType = cumY < 0 ? Event::etMouseUp : Event::etMouseDown ;
        *pMagnitude = abs( cumY / nStrokes ) ;
    }
    LOG_CMSG( "detectMouseMove" ,
        "  cumX=" << biasedCumX << "(" << cumX << ") ; cumY=" << cumY
        << " ; " << *pEventType << "/" << *pMagnitude
    ) ;

    return true ;
}

// ---------------------------------------------------------------------

static bool
findDevice( InterceptionDevice hDevice , const Device** ppDevice , const DeviceConfig** ppDeviceConfig )
{
    // find the specified device
    for( DeviceTable::const_iterator it=gDeviceTable.begin() ; it != gDeviceTable.end() ; ++it )
    {
        const Device* pDevice = (*it).second ;
        // FIXME! Windows seems to occasionally assign new device numbers at boot time, so this check won't work properly
        //  if that happens :-/ We should also check the HID (and maybe display name as well), since it's unlikely someone
        //  will have multiple identical meeces connected at the same time.
        if ( pDevice->deviceNumber() == hDevice )
        {
            // found it - found the corresponding DeviceConfig
            DeviceConfigTable::const_iterator it2 = gDeviceConfigTable.find( pDevice->deviceId() ) ;
            if ( it2 != gDeviceConfigTable.end() )
            {
                *ppDevice = pDevice ;
                *ppDeviceConfig = (*it2).second ;
                return true ;
            }
        }
    }
    return false ;
}

// ---------------------------------------------------------------------

const AppProfile*
findAppProfile( const DeviceConfig* pDeviceConfig , const AppProfile** ppDefaultAppProfile )
{
    // get the EXE that owns the foreground window
    string processExeName ;
    HWND hWnd = GetForegroundWindow() ;
    if ( hWnd != NULL )
    {
        DWORD processId ;
        DWORD rc = GetWindowThreadProcessId( hWnd , &processId ) ;
        if ( rc )
        {
            HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION , FALSE , processId ) ;
            if ( hProcess != NULL )
            {
                wchar_t buf[ MAX_PATH+1 ] ;
                DWORD nChars = ARRAY_SIZE( buf ) ;
                BOOL rc = QueryFullProcessImageName( hProcess , 0 , buf , &nChars ) ;
                if ( rc )
                    processExeName = toUtf8( buf , nChars ) ;
                CloseHandle( hProcess ) ;
            }
        }
    }

    // find the AppProfile that belongs to the EXE
    const AppProfile* pAppProfileToReturn = NULL ;
    const AppProfile* pDefaultAppProfile = NULL ;
    for ( AppProfilePtrVector::const_iterator it=pDeviceConfig->appProfiles().begin() ; it != pDeviceConfig->appProfiles().end() ; ++it )
    {
        const AppProfile* pAppProfile = *it ;
        // check if the AppProfile has an absolute path
        if ( !pAppProfile->app().empty() && !isAbsolutePath(pAppProfile->app()) )
        {
            // nope - just compare the EXE filename (without its directory)
            getFilename( &processExeName ) ;
        }
        // check if this is the AppProfile we want
        if ( _stricmp( pAppProfile->app().c_str() , processExeName.c_str() ) == 0 )
            pAppProfileToReturn = pAppProfile ;
        // check if this is the default AppProfile
        if ( pAppProfile->app().empty() )
            pDefaultAppProfile = *it ;
    }

    *ppDefaultAppProfile = pDefaultAppProfile ;
    return pAppProfileToReturn ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static bool
isAbsolutePath( const string& path )
{
    // check if the specified path is absolute
    for ( string::const_iterator it=path.begin() ; it != path.end() ; ++it )
    {
        if ( *it == ':' || isSlash(*it) )
            return true ;
    }
    return false ;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void
getFilename( string* pPath )
{
    // extract the filename from the specified path
    for ( int i=pPath->length()-1 ; i >= 0 ; --i )
    {
        if ( isSlash( (*pPath)[i] ) )
        {
            *pPath = pPath->c_str() + i+1 ;
            break ;
        }
    }
}
