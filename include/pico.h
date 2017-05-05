// =====================================================================================
// 
//       Filename:  pico.h
// 
//    Description:  Header file for pico-class in pico.cpp
// 
//        Version:  1.0
//        Created:  25.04.2017 13:49:56
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

#include "channel.h"

#include <libps6000-1.4/PicoStatus.h>
#include <libps6000-1.4/ps6000Api.h>
#include <vector>

/*! The Pico class to steer and handle one Picoscope */
class Pico
{
    private: 

        // Information of the entire Picoscope
        PICO_STATUS     m_status;

        int16_t         *m_handle;
        int8_t          *m_serial;

        ////////////////////////////////////////////////////////////////////////
        /*
         * Start - Timebase and aquisition variables
         */

        uint32_t        m_timebase          {2}; // means 0.8ns
        float           m_timeInterval_ns;
        int16_t         m_oversample        {0}; // we cant use it
        uint32_t        m_maxSamples;
        uint32_t        m_segmentIndex      {0}; // since we have only one block
        uint32_t        m_bufferSize        {10000000};  // to reserve vectors for 
                                                        // the data with this size
        uint32_t        m_preTrigger;
        uint32_t        m_postTrigger;
        uint32_t        m_noSamples;

        enPS6000RatioMode   m_ratioMode     {PS6000_RATIO_MODE_NONE};

        /*
         * End - Timebase and aquisition variables
         */
        ////////////////////////////////////////////////////////////////////////



        ////////////////////////////////////////////////////////////////////////
        /*
         * Channels from the class Channel
        */
        Channel *m_channelA;
        Channel *m_channelB;
        Channel *m_channelC;
        Channel *m_channelD;
        //
        ////////////////////////////////////////////////////////////////////////



        ////////////////////////////////////////////////////////////////////////
        // default trigger settings
        int16_t                     m_tEnabled          {false};
        PS6000_CHANNEL              m_tSource           {PS6000_TRIGGER_AUX}; 
        int16_t                     m_tThreshold        {-1000};
        PS6000_THRESHOLD_DIRECTION  m_tDirection        {PS6000_FALLING};
        uint16_t                    m_tDelay            {0};
        int16_t                     m_tAutoTrigger_ms   {1500};
        //
        ////////////////////////////////////////////////////////////////////////

        
        ////////////////////////////////////////////////////////////////////////
        /*
         * Start - PrivateMemberFunctions!
         */

        // prints a dedicated error message
        void printError(const char* message);          
        // turns of unneeded stuff like ETS
        void turnOffUnnecessary();                      
        // opens the pico unit
        void openUnit();  
//        void openUnit(int16_t *handle, int8_t *serial);  
        // checks the m_status value
        void checkStatus();         
        
        /*
         * End - PrivateMemberFunctions!
         */
        ////////////////////////////////////////////////////////////////////////
    

    public:
        
        ////////////////////////////////////////////////////////////////////////
        //! Constructor
        Pico(int16_t *handle, int8_t *serial);

        // Destructor
        ~Pico(){
//            closeUnit();
        };
        ////////////////////////////////////////////////////////////////////////



        ////////////////////////////////////////////////////////////////////////
        /*
         * Start - Return/Get Functions. 
         */

        //! Returns the Channel to work on further on with class Channel methods.
        Channel*        getCh(PS6000_CHANNEL channel);
        Channel*        getCh(int channel);

        //! Returns the current timebase (Default: 2).
        uint32_t        returnTimebase() const;
        //! Returns the number of the current preTriggers.
        uint32_t        returnPreTrigger() const;
        //! Returns the number of the current postTriggers.
        uint32_t        returnPostTrigger() const;
        //! Returns preTrigger + postTrigger
        uint32_t        returnNoSamples() const;
        //! Returns the current sampling interval in ns.
        float           returnTimeInterval() const;
        //! Returns the current oversample mode (Default: 0).
        int16_t         returnOversample() const;
        //! Returns the max possible numer of samples to be taken.
        //! This depens on the number of channels turned on. So first prepare
        //! the Pico.
        uint32_t        returnMaxSamples() const;
        //! Returns the current segment index we use.
        uint32_t        returnSegmentIndex() const;
        //! Returns the current buffer size.
        uint32_t        returnBufferSize() const;
        //! Returns the current ratio mode (Default: 0 = RATIO_MODE_NONE).
        enPS6000RatioMode       returnRatioMode() const;

        /*
         * End - Return/Get Functions. 
         */
        ////////////////////////////////////////////////////////////////////////
        

        ////////////////////////////////////////////////////////////////////////
        /*
         * Start - Set Pico related stuff like Trigger and so on...
         */

        //! Sets a simple trigger.
        void    setSimpleTrigger(\
                    int16_t                     enabled,            \
                    PS6000_CHANNEL              source,             \
                    int16_t                     threshold,          \
                    PS6000_THRESHOLD_DIRECTION  direction,          \
                    uint16_t                    delay,              \
                    int16_t                     autoTriggerTime_ms
                    );
        //! Puts the timebase from the Picoscope and returns the TimeInterval in
        //! nanoseconds as well as the maximum number of samples to take.
        void    getTimebase( 
                   uint32_t    timebase,           \
                   uint32_t    noSamples,          \
                   float       *timeInterval_ns,   \
                   uint32_t    *maxSamples
                   );

        //! This function starts collecting data in block mode.
        void    runBlock(
                    uint32_t    preTrigger,         \
                    uint32_t    postTrigger,        \
                    uint32_t    timebase
                    );

        //! Retrieves the aquired data, outputs the number of collected samples
        //! and saves the number of overflows.
        void    getValues(
                    uint32_t    *numberOfSamples,            \
                    int16_t     *overflow
                    ); 
        /*
         * End - Set Pico related stuff like Trigger and so on...
         */
        ////////////////////////////////////////////////////////////////////////


        //! Closes the picoscope.
        void    closeUnit();

};





#endif

