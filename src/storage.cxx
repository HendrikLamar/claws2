// =
// ====================================================================================
// 
//       Filename:  storage.cxx
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03.10.2017 04:34:02
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================


#include "storage.h"
#include "utility.h"

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/filesystem.hpp>

#include <TH1I.h>






///////////////////////////////////////////////////////////////////////////////
//
//              START Constructor and destructor
//
///////////////////////////////////////////////////////////////////////////////





Storage::Storage()
//        std::string picoLocation, 
//        std::string saveLocation
//        ) :
//    m_location_pico( picoLocation ),
//    m_location_save( saveLocation )
{}






///////////////////////////////////////////////////////////////////////////////





Storage::~Storage()
{}







///////////////////////////////////////////////////////////////////////////////
//
//              END Constructor and destructor
//
///////////////////////////////////////////////////////////////////////////////





































///////////////////////////////////////////////////////////////////////////////
//
//              START Public member functions
//
///////////////////////////////////////////////////////////////////////////////






void Storage::intermediate( unsigned long runNum, unsigned int subRunNum)
{
    std::string tpath = makePath( runNum, Utility::Dir_Struct::INTER );
    


    return;
}





///////////////////////////////////////////////////////////////////////////////






void Storage::physics( unsigned long runNum, unsigned int subRunNum )
{

    std::string tpath = makePath( runNum, Utility::Dir_Struct::PHYSICS );


    return;
}








///////////////////////////////////////////////////////////////////////////////






void Storage::setSaveLocation( std::string saveLocation )
{
    m_location_save = saveLocation;
    return;
}








///////////////////////////////////////////////////////////////////////////////
//
//              END Public member functions
//
///////////////////////////////////////////////////////////////////////////////






































///////////////////////////////////////////////////////////////////////////////
//
//              START Private member functions
//
///////////////////////////////////////////////////////////////////////////////






std::string Storage::makePath( unsigned long runNum , Utility::Dir_Struct kind )
{
    // check for which analysis mode we need the dir
    std::string tsubdir;
    switch( kind )
    {
        case Utility::Dir_Struct::INTER:
            tsubdir = "/intermediate";
            break;
        case Utility::Dir_Struct::PHYSICS:
            tsubdir = "/physics";
            break;
        case Utility::Dir_Struct::LIVE:
            tsubdir = "/live";
            break;
    }

    // get the current date in YYYY-MM-DD format
    std::string currDay{to_iso_extended_string(
            boost::gregorian::day_clock::local_day())};

    // create the final path
    std::string finalPathStr = 
        m_location_save +
        "/data/" +
        currDay +
        "/run-" +
        std::to_string(runNum) +
        "/raw" +
        tsubdir;

    boost::filesystem::path finalPath{finalPathStr};

    boost::filesystem::create_directories(finalPath);

    return finalPathStr;

}




///////////////////////////////////////////////////////////////////////////////
//
//              END Private member functions
//
///////////////////////////////////////////////////////////////////////////////

