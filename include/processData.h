// =====================================================================================
// 
//       Filename:  processData.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06.10.2017 07:05:31
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#ifndef PROCESSDATA_H
#define PROCESSDATA_H

#include "storage.h"
#include "utility.h"
#include "storage.h"

#include <TH1I.h>

#include <libps6000-1.4/ps6000Api.h>
#include <libps6000-1.4/PicoStatus.h>

#include <vector>
#include <utility>




class ProcessData
{

    private:
        friend Storage;
        
        pico_data_raw*      m_data_raw;
        pico_data_hist*     m_data_hist;

        Utility::Pico_Data_Pico*    m_config_data;
        Utility::Claws_Gain         m_current_gain;
        Storage*                    m_save;

        // declares where the current data should be saved
        Storage::subdir      m_current_dataset;

    public:

        ProcessData();
        ProcessData( Utility::Pico_Data_Pico* config );
        ~ProcessData();

        Storage* save();

        void setData(   
                Utility::Claws_Gain gain, //!< Tell if it is intermediate or physics data
                pico_data_raw* data //!< Vector with data to analyze and store.
                );

        void setConfig( Utility::Pico_Data_Pico* config );


};

#endif //PROCESSDATA_H
