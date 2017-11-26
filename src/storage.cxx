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

#include <TFile.h>
#include <TH1I.h>

#include <memory>





///////////////////////////////////////////////////////////////////////////////
//
//              START Constructor and destructor
//
///////////////////////////////////////////////////////////////////////////////





Storage::Storage(std::shared_ptr < std::vector< std::shared_ptr< Utility::Pico_Hist_Pico > > > picos_hist, std::shared_ptr<unsigned long> runNum ) :
    m_picos_hist( picos_hist ),
    m_runNum( runNum )
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






void Storage::intermediate( unsigned int& subRunNum)
{
    std::string tpath = makePath( *m_runNum, Utility::Dir_Struct::INTER );
    
    save( *m_runNum, subRunNum, tpath, Utility::Claws_Gain::INTERMEDIATE );


    return;
}





///////////////////////////////////////////////////////////////////////////////






void Storage::physics( unsigned int& subRunNum )
{

    std::string tpath = makePath( *m_runNum, Utility::Dir_Struct::PHYSICS );
    save( *m_runNum, subRunNum, tpath, Utility::Claws_Gain::HL_GAIN );


    return;
}








///////////////////////////////////////////////////////////////////////////////






void Storage::setSaveLocation( std::string saveLocation )
{
    boost::filesystem::path psaveLocation{saveLocation};
    
    // throw exception if this path does not exists
    if( !boost::filesystem::exists(psaveLocation) )
    {
        throw ProcessDataException("Save Location does not exist!");
    }
    
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







void Storage::save( 
        unsigned long& runNum, 
        unsigned int& subRunNum, 
        std::string& tpath,
        Utility::Claws_Gain gain )
{

    std::string tfileName;
    switch( gain )
    {
        case Utility::Claws_Gain::INTERMEDIATE:
            tfileName = "inter=";
            break;
        default:
            tfileName = "physics-";
    }

    
    tfileName += std::to_string(runNum);

    tfileName += "-";

    // extend with zeros to three digits
    if( subRunNum < 10 )
    {
        tfileName += "000" +
        std::to_string(subRunNum);
    }
    else if( subRunNum < 100 )
    {
        tfileName += "00" +
        std::to_string(subRunNum);
    }
    else if( subRunNum < 1000 )
    {
        tfileName += "0" +
        std::to_string(subRunNum);
    }
    else if( subRunNum < 10000 )
    {
        tfileName += "" +
        std::to_string(subRunNum);
    };

    tfileName += ".root";

    ////////////////////////////////////////////////////////////////////////////

    std::string fpath = tpath + "/" + tfileName;
    std::shared_ptr<TFile> file{std::make_shared<TFile>(fpath.c_str(), "recreate")}; 
    file->cd();

    for( auto& tmp : *m_picos_hist )
    {
        for( int ii = 0; ii < tmp->getSize(); ++ii )
        {
            tmp->get(ii)->Write();
        }
    }



//    file->SaveAs(fpath.c_str());
    file->Write();
    file->Close();



    ///////////////////////////////////////////////////////////////////////////

    return;
}









///////////////////////////////////////////////////////////////////////////////
//
//              END Private member functions
//
///////////////////////////////////////////////////////////////////////////////

