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

        ////////////////////////////////////////////////////////////////////////
        // 
        //                  START Pico unique variables
        //
        ////////////////////////////////////////////////////////////////////////
        

        //! Const pointer to a variable set of pico data.
        Utility::Pico_Data_Pico* const      m_picoData;

        //! Handle value is defined during the initialization of the pico.
        int16_t             m_handle;

        // This is not const since I get crazy when trying.
        int8_t*             m_serial;

        // The location is const since it can't be changed and is known at initialization.
        const std::string* const            m_location;

        
        // Vector holding the four channels of the pico
        const std::vector< Channel* >* const      m_channels;



        ////////////////////////////////////////////////////////////////////////
        // 
        //                  END Pico unique variables
        //
        ////////////////////////////////////////////////////////////////////////



















        ////////////////////////////////////////////////////////////////////////
        //
        //          START Steering variables
        //
        ////////////////////////////////////////////////////////////////////////



        Utility::Claws_Trigger_Mode         m_triggerMode;














        ////////////////////////////////////////////////////////////////////////
        //
        //          END Steering variables
        //
        ////////////////////////////////////////////////////////////////////////

















        ////////////////////////////////////////////////////////////////////////
        //
        //          START Readin settings
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
        //          END Readin settings
        //
        ////////////////////////////////////////////////////////////////////////











        


        ////////////////////////////////////////////////////////////////////////
        //
        //          START  Picoscope internal variables  
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
        //          END Picoscope internal variables 
        //
        ////////////////////////////////////////////////////////////////////////


















        ////////////////////////////////////////////////////////////////////////
        //
        //         START private member functions 
        //
        ////////////////////////////////////////////////////////////////////////
        


        // Returns a Utility::Pico_Data_HL_Gain pointer to the correct data set
        // for the demanded gain mode.
        Utility::Pico_Data_HL_Gain*  getGainData( Utility::Claws_Gain& mode );

        // Sets the Trigger settings.
        void    setTrigger();

        // Sets the aquisition settings.
        void    setAqui();

        void    checkStatus();


        ////////////////////////////////////////////////////////////////////////
        //
        //         END private member functions 
        //
        ////////////////////////////////////////////////////////////////////////


    public:
        ///////////////////////////////////////////////////////////////////////
        /*
         *! Contructor
         */
        Pico( Utility::Pico_Data_Pico* picoData );
        ///////////////////////////////////////////////////////////////////////
        /*
         *! Destructor
         */
        ~Pico();

        //! Initializes the picoscope.
        void init();

        //! Loads the settings from the Database and stores it in member variables
        void loadConfig( Utility::Claws_Gain& mode );

        void setPico();

        //! Returns the channel.
        Channel* getCh( PS6000_CHANNEL& cha );
        Channel* getCh( int& cha );

        void runBlock();

        void stop();

        void close();


        //! Overloaded operator<< to print out information.
        friend std::ostream& operator<<(
                std::ostream& out, Pico* picoscope );
};




#endif // PICO_H
