// =====================================================================================
// 
//       Filename:  pico.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  04.09.2017 10:27:32
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================


#ifndef PICO_H
#define PICO_H

#include <libps6000-1.4/PicoStatus.h>
#include <libps6000-1.4/ps6000Api.h>

#include "pico_channel.h"
#include "utility.h"

#include <vector>
#include <string>


//! The Pico class to steer and handle one Picoscope
class Pico
{
    private:

        // Holds information about the current status of the scope
        PICO_STATUS         m_status;

        // Pico unique variables
        int16_t             m_handle;
        int8_t*             m_serial;
        std::string*        m_location;

        
        // vector holding the four channels
        std::vector< Channel* >*    m_channels;


        ////////////////////////////////////////////////////////////////////////
        //
        //          START Readin variables
        //
        ////////////////////////////////////////////////////////////////////////
        

        // aquisition settings
        uint32_t            m_timebase;
        int16_t             m_oversample;
        uint32_t            m_preTrigger;
        uint32_t            m_postTrigger;
        enPS6000RatioMode   m_downSamplingRatioMode;
        uint32_t            m_downSampleRatio;




        // simple trigger settings
        int16_t                     m_st_enabled;
        PS6000_CHANNEL              m_st_source;
        int16_t                     m_st_threshold;
        PS6000_THRESHOLD_DIRECTION  m_st_direction;
        uint32_t                    m_st_delay;
        int16_t                     m_st_autoTriggerTime_ms;




        ////////////////////////////////////////////////////////////////////////
        //
        //          END Readin variables
        //
        ////////////////////////////////////////////////////////////////////////











        


        ////////////////////////////////////////////////////////////////////////
        //
        //          START  Internal variables  
        //
        ////////////////////////////////////////////////////////////////////////



        // block mode (not rapid block mode) settings with start value
        // rapid block mode might be needed for faster calibration
        uint32_t            m_startIndex{0};    // startpoint of the buffer for data
                                                // collection
        

        uint32_t            m_noOfSamplesTotal;     // sum of pre- and posttriggers
                                                    // written every time new 
                                                    // data is loaded


        float               m_timeInterval_ns;      // sampling interval written by
                                                    // the pico

        int32_t             m_timeIndisposedMS;     // time differenz between
                                                    // pico ready and data taking

        int16_t             m_overflow;             // on exit, each bit indicates if
                                                    // an overflow has occurred or not
                                                    // bit 0 = ch1, bit 1 = ch2 ...



        ////////////////////////////////////////////////////////////////////////
        //
        //          END Internal variables  
        //
        ////////////////////////////////////////////////////////////////////////


};




#endif // PICO_H
