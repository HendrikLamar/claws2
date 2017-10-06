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
#include "processData.h"

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

        // creates the path where to save the data
        std::string     makePath( unsigned long runNum, subdir kind );



        // gives new data to the instance
        void    setData( char ip, pico_data* data );




    public:
        Storage( Utility::Pico_Data_Pico data_pico );

        ~Storage();


        //! Stores intermediate data and saves the belonging settings.ini file.
        void intermediate( 
                unsigned long runNum,       //!< Specified in runNumber.ini
                unsigned int subRunNum);    //!< Defined in ClawsConfig loops_physics
        
        //! Stores physics data and saves the belonging settings.ini file.
        void physics( 
                unsigned long runNum,       //!< Specified in runNumber.ini
                unsigned int subRunNum);    //!< Defined in ClawsConfig loops_physics


        friend ProcessData;
};




#endif // STORAGE_H
