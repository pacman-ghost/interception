#ifndef APPPROFILE_HPP
#define APPPROFILE_HPP

#include "event.hpp"

struct ApiAppProfile ;

// ---------------------------------------------------------------------

class AppProfile
{

// constructors/destructor:
public:
    AppProfile(
        const ApiAppProfile* pAppProfile ,
        const ApiEvent* pEvents , int nEvents ,
        const ApiAction* pActions , int nActions
    ) ;
    DISABLE_COPY_ASSIGNMENT( AppProfile ) ;

// access methods:
public:
    const std::string& app() const ;
    const EventPtrVector& events() const ;

// miscellaneous methods:
public:
    void dumpAppProfile( std::ostream& os , const char* pPrefix="" ) const ;

// data members:
private:
    std::string mApp ;
    EventPtrVector mEvents ;
} ;

typedef PtrVector<AppProfile> AppProfilePtrVector ;

// inserter
std::ostream& operator<<( std::ostream& , const AppProfile& ) ;

// ---------------------------------------------------------------------

#endif // APPPROFILE_HPP