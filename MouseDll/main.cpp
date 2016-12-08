#include <stdexcept>
#include "main.hpp"

using namespace std ;

// --- LOCAL DATA ------------------------------------------------------

static bool gIsOpen = false ;

// ---------------------------------------------------------------------

void
openApi()
{
    // open the API
    if ( gIsOpen )
        throw runtime_error( "API is already open." ) ;
    gIsOpen = true ;
}

// ---------------------------------------------------------------------

void
closeApi()
{
    // close the API
    if ( ! gIsOpen )
        throw runtime_error( "API is not open." ) ;
    gIsOpen = false ;
}
