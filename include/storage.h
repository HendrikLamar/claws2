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

class Storage
{
    private:

        enum class subdir 
        {
            INTER,
            PHYSICS,
            LIVE
        };

        std::string     m_location_pico;
        std::string     m_location_save;
        void            makePath( unsigned long runNum, subdir kind );


        TH1I*   m_hist_inter;
        TH1I*   m


    public:

        Storage( 
                std::string picoLocation,   //!< the location of the pico
                std::string saveLocation    //!< root path of the save location (hdd)
                );

        ~Storage();


        void intermediate( 
                unsigned long runNum,       //!< Specified in runNumber.ini
                unsigned int subRunNum);    //!< Defined in ClawsConfig loops_physics
        
        void physics( 
                unsigned long runNum,       //!< Specified in runNumber.ini
                unsigned int subRunNum);    //!< Defined in ClawsConfig loops_physics

};




#endif // STORAGE_H
