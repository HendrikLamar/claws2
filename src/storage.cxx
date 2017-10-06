// =====================================================================================
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





Storage::Storage( 
        std::string picoLocation, 
        std::string saveLocation
        ) :
    m_location_pico( picoLocation ),
    m_location_save( saveLocation )
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
    std::string tpath = makePath( runNum, subdir::INTER );

    m_hist_inter->SaveAs()
    
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






std::string Storage::makePath( unsigned long runNum , Storage::subdir kind )
{
    // check for which analysis mode we need the dir
    std::string tsubdir;
    switch( kind )
    {
        case subdir::INTER:
            tsubdir = "/inter";
            break;
        case subdir::PHYSICS:
            tsubdir = "/physics";
            break;
        case subdir::LIVE:
            tsubdir = "/live";
            break;
    }

    // get the current date in YYYY-MM-DD format
    std::string curDay{to_iso_extended_string(
            boost::gregorian::day_clock::local_day())};

    // create the final path
    std::string finalPathStr = 
        m_location_save +
        "/data/" +
        curDay +
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

