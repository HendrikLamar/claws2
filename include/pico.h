// =====================================================================================
//
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



/*! Pico class
 * 
 * Use it the following:
 *  1. init() -> is done when instance is created.
 *  2. loadConfig( Utility::Claws_Gain& mode )
 *  3. setReadyBlock() / setReadyRapid()
 *  4. runBlock() / runRapid()
 *  6. stop()
 *  7. close()
 *
 *  Steps 3 & 4 might be repeated. 
 *
 *
 */ 
class Pico
{
    private:

        // Holds information about the current status of the scope
        PICO_STATUS                             m_status;

        ////////////////////////////////////////////////////////////////////////
        // 
        //                  START Pico unique variables
        //
        ////////////////////////////////////////////////////////////////////////
        

        //! Const pointer to a variable set of pico data.
        Utility::Pico_Data_Pico* const          m_picoData;

        //! Handle value is defined during the initialization of the pico.
        int16_t                                 m_handle;

        // This is not const since I get crazy when trying.
        int8_t*                                 m_serial;

        // The location is const since it can't be changed and is known at initialization.
        const std::string* const                m_location;

        
        // Vector holding the four channels of the pico
        const std::vector< Channel* >* const    m_channels;



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



        // needed to be read-in!
        Utility::Pico_Trigger_Mode         m_triggerMode;

        // needed to be read-in!
        // signifies in how many memory segments we need for rapid block mode
        uint32_t                            m_noOfRapidBlockCollections;













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

        uint32_t            m_noChannelsEnabled;    // holds the number of enabled channels


        uint32_t            m_noMemorySegments{1};  // holds the number of memory segments
                                                    // the pico memory is divided in
                                                    // Default: 1 (by picoscope)
                                                
        uint32_t            m_noMemoryMaxSamples;   // holds the max number of samples
                                                    // available in each memory segment

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


        //! Initializes the picoscope.
        void init();

        // Returns a Utility::Pico_Data_HL_Gain pointer to the correct data set
        // for the demanded gain mode.
        Utility::Pico_Data_HL_Gain*  getGainData( Utility::Claws_Gain& mode );

        // Sets the Trigger settings.
        void    setTrigger();

        // Configures the channels.
        void    setChannels();

        // checks if the pico return the correct time interval
        void    getTimebase();

        // Acquires the data from the pico after data taking.
        void    getValuesBlock();

        void    getValuesRapid();

        // Only needed when rapid block mode is used. Otherwise its 1 by default.
        void    setMemorySegments( uint32_t nSegments );

        // Checks m_status for possible error and throws a PicoException() if it is
        // the case. Should be called after every ps6000-function.
        void    checkStatus();

        // Turns off unneeded things. Currently: ETSmode
        void    turnOffUnneeded();

        ////////////////////////////////////////////////////////////////////////
        //
        //         END private member functions 
        //
        ////////////////////////////////////////////////////////////////////////











    public:
        ///////////////////////////////////////////////////////////////////////
        /*
         *! Constructor to be called if pico is initialized by serial.
         */
        Pico( Utility::Pico_Data_Pico* picoData );

        //! Constructor to be called when pico is initialized before which means
        //! that its unique handle is known.
        Pico( Utility::Pico_Data_Pico* picoData, int16_t handle );
        ///////////////////////////////////////////////////////////////////////
        /*
         *! Destructor
         */
        ~Pico();

        //! This function can be use to check that the already opened device is
        //! communicating.
        void pingUnit();

        //! Returns the channel.
        Channel* getCh( PS6000_CHANNEL cha );
        Channel* getCh( int cha );

        //! Loads the settings from the Database and stores it in member variables
        void loadConfig( Utility::Claws_Gain mode );

        //! Prepares the Pico to get ready for block mode. Should be called before
        //! block mode.
        void setReadyBlock();

        void setReadyRapid();

        //! Starts one time data taking in block mode. Should be placed in a loop.
        //! Settings should to be loaded
        //! before. PicoExceptions might be thrown. When runBlock() ends, the 
        //! acquired data is Can be retrieved by calling Pico_Channel::getBuffer() 
        //! Data can be processed afterwards.
        void runBlock();

        //! Starts the rapid block mode.
        void runRapid();

        //! Stops the data taking properly. If this is called before a trigger event
        //! occurs, the data array might contain non valid data.
        void stop();

        //! Shuts down the Picoscope.
        void close();


        //! Overloaded operator<< to print out information.
        friend std::ostream& operator<<(
                std::ostream& out, Pico* picoscope );
};




#endif // PICO_H
