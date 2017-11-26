// =====================================================================================
// 
//       Filename:  storage.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03.10.2017 04:34:28
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================


#ifndef STORAGE_H
#define STORAGE_H

#include <TH1I.h>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/filesystem.hpp>


#include "utility.h"
//#include "processData.h"

class Storage
{
    private:

        std::shared_ptr < std::vector< std::shared_ptr< 
            Utility::Pico_Hist_Pico > > > m_picos_hist;

        std::shared_ptr<unsigned long> m_runNum;

        // backup save location
        std::string     m_location_save = "/dev/null";

        // creates the path where to save the data
        std::string     makePath( unsigned long runNum, Utility::Dir_Struct kind );

        void save(
                unsigned long& runNum,
                unsigned int& subRunNum, 
                std::string& fpath,
                Utility::Claws_Gain gain );



    public:
        Storage( std::shared_ptr < std::vector< std::shared_ptr< Utility::Pico_Hist_Pico > > > picos_hist, std::shared_ptr<unsigned long> runNum );

        ~Storage();


        //! Stores intermediate data and saves the belonging settings.ini file.
        void intermediate( 
                unsigned int& subRunNum);    //!< Defined in ClawsConfig loops_physics
        
        //! Stores physics data and saves the belonging settings.ini file.
        void physics( 
                unsigned int& subRunNum);    //!< Defined in ClawsConfig loops_physics

        //! Set the location where the data should be saved.
        void setSaveLocation( std::string saveLocation );
};




#endif // STORAGE_H
