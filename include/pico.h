// =====================================================================================
// 
//       Filename:  pico.h
// 
//    Description:  Header file for pico-class in pico.cpp
// 
//        Version:  1.1
//        Created:  05.05.2017 15:18:56
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

#include <iostream>
#include <vector>
#include <string>

#include "utility.h"

/*! The Pico class to steer and handle one Picoscope */
class Pico
{
    private: 

        // Information of the entire Picoscope
        PICO_STATUS         m_status;

        int16_t             m_handle;
        int8_t              *m_serial;
        std::string         m_alias;

        uint16_t            m_noChannelsEnabled         {0};

        ////////////////////////////////////////////////////////////////////////
        /*
         * Start - Timebase and aquisition variables
         */

        uint32_t        m_timebase                      {2};
//        uint32_t        m_noSamples; -> equals pretrigger+posttrigger
        float           m_timeInterval_ns               {0};
        int16_t         m_oversample                    {1};
        uint32_t        m_maxSamples                    {0};
        uint32_t        m_segmentIndex                  {0};

        // reserve a vector of that size per channel when initializing the pico
        uint32_t        m_bufferSizeReserve             {10000000};
        uint32_t        m_bufferSize                    {10000};

        uint32_t        m_preTrigger                    {1000};
        uint32_t        m_postTrigger                   {9000};

        int32_t         m_timeIndisposedMs              {0};

        enPS6000RatioMode   m_downSamplingRatioMode     {PS6000_RATIO_MODE_NONE};
        uint32_t        m_downSampleRatio               {1};


        // FIXED! no change needed. (indicates the startpoint for data collection
        // in samples intervals from the start of the buffer)
        uint32_t            m_startIndex                {0};

        // on input the number of samples required (pre+postrigger), on output 
        // the number of samples actually retrieved.
        uint32_t            m_noOfSamples;

        // on exit, a set of bitflags indicating weather an overvoltage has
        // occured in one of the channels. Bit 0 denots ChannelA.
        int16_t             m_overflow                    {0};
        

        /*
         * End - Timebase and aquisition variables
         */
        ////////////////////////////////////////////////////////////////////////



        ////////////////////////////////////////////////////////////////////////
        // default trigger settings
        int16_t                     m_tEnabled          {true};
        PS6000_CHANNEL              m_tSource           {PS6000_TRIGGER_AUX};
        int16_t                     m_tThreshold        {-1024};
        PS6000_THRESHOLD_DIRECTION  m_tDirection        {PS6000_FALLING};
        uint32_t                    m_tDelay            {0}; // in ms
        int16_t                     m_tAutoTrigger_ms   {10000};
        //
        ////////////////////////////////////////////////////////////////////////

        
        ////////////////////////////////////////////////////////////////////////
        /*
         * Start - PrivateMemberFunctions!
         */

        // prints a dedicated error message
        void    printError(const char* message);          
        // turns of unneeded stuff like ETS
        void    turnOffUnnecessary();                      
        // opens the pico unit
        void    openUnit();  
//        void openUnit(int16_t *handle, int8_t *serial);  
        // checks the m_status value
        void    checkStatus();         
        //! Sets the m_bufferSize and m_noOfSamples according to the pre+postriggers.
        void    setBufferANDnoOfSamples();

        /*
         * End - PrivateMemberFunctions!
         */
        ////////////////////////////////////////////////////////////////////////
    

        /*! The Channel class to handle a channel on a Pico*/
        class Channel
        {
            private:
                // the pico to which the channel belongs
                Pico                        *m_pico;
        
                // single channel only setting        
                const PS6000_CHANNEL        m_channel; 
                PS6000_COUPLING             m_coupling          {PS6000_DC_50R};
                int16_t                     m_enabled           {false};
                float                       m_analogueOffset    {0};
                PS6000_RANGE                m_range             {PS6000_1V};
                PS6000_BANDWIDTH_LIMITER    m_bandwidth         {PS6000_BW_FULL};
        
                std::vector<int16_t>*       m_buffer;
        

            public:
                ///////////////////////////////////////////////////////////////////////
                /*
                 *! Contructor
                 */
                Channel(PS6000_CHANNEL channel, Pico *picoscope);
                ///////////////////////////////////////////////////////////////////////
                
                ~Channel(){};
        

    
                ///////////////////////////////////////////////////////////////////////
                /*
                 * ReturnFunctions - Return Information about this instance.
                 */
        

                //! Returns the corresponding handle value
                int16_t                     returnHandle() const;
                //! Returns the corresponding serial number of the pico
                int8_t*                     returnSerial() const;
                //! Returns the steered channel.
                PS6000_CHANNEL              returnChannel() const;
                //! Return is the channel is enabled or not.
                int16_t                     returnEnabled() const;
                //! Returns the set offset.
                float                       returnOffset() const;
                //! Returns the set coupling.
                PS6000_COUPLING             returnCoupling() const;
                //! Returns the range.
                PS6000_RANGE                returnRange() const;
                //! Returns the bandwidth.
                PS6000_BANDWIDTH_LIMITER    returnBandwidth() const;
                

                //! Returns the channel buffer.
                std::vector<int16_t>*       getBuffer();
                ///////////////////////////////////////////////////////////////////////
        
        
        
                ///////////////////////////////////////////////////////////////////////
                /*
                 * SetFunctions - To set the channel preferences.
                 */
        

                //! Sets status of enabled.
                void     setEnabled(int16_t enabled);
                //! Sets the the coupling.
                void     setCoupling(PS6000_COUPLING coupling);
                //! Sets the offset.
                void     setOffset(float analogueOffset);
                //! Sets the range.
                void     setRange(PS6000_RANGE range);
                //! Sets the bandwidth.
                void     setBandwidth(PS6000_BANDWIDTH_LIMITER bandwidth);
        
                //! Sets all possible channel preferences.
                void     setChannel(\
                                    int16_t                     enabled,            \
                                    PS6000_RANGE                range,              \
                                    float                       analogueOffset,     \
                                    PS6000_COUPLING             coupling,           \
                                    PS6000_BANDWIDTH_LIMITER    bandwidth
                                    );
        
        
                //! Tells the Pico where to store the data for this channel. The values
                //! needed are already set before or come from the class Pico.
                void    setDataBuffer();
                
                //! Configures the channel with the given member variables.
                void    configureChannel();

                ///////////////////////////////////////////////////////////////////////
        };


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
        
        Pico() = delete;

    public:
        
        ////////////////////////////////////////////////////////////////////////
        //! Constructor
        Pico( int8_t *serial);

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
        //! Returns the number of samples retrieved in the actual run.
        uint32_t        returnNoOfSamples() const;
        //! Returns the current sampling interval in ns.
        float           returnTimeInterval() const;
        //! Returns the current oversample mode (Default: 0).
        int16_t         returnOversample() const;
        //! Returns the max possible numer of samples to be taken.
        // This depens on the number of channels turned on. So first prepare
        // the Pico.
        uint32_t        returnMaxSamples() const;
        //! Returns the current segment index we use.
        uint32_t        returnSegmentIndex() const;
        //! Returns the current buffer size.
        uint32_t        returnBufferSize() const;
        //! Returns the current ratio mode (Default: 0 = RATIO_MODE_NONE).
        enPS6000RatioMode       returnDownSampleRatioMode() const;
        //! Returns the down sampling ratio.
        uint32_t                returnDownSampleRatio() const;
        //! Returns the time in milliseconds that the scope will spend collecting
        // collecting samples. This does not include any auto trigger timeout.
        int32_t         returnTimeIndisposed() const;
        //! Returns a bitflag weather an overvoltage has occured on any of the
        // channels. The first bit is denoting Channel A.
        int16_t         returnOverflow() const;
        //! Returns the alias (aka Position) of the Pico in the setup.
        std::string     returnAlias() const;        

        /*
         * End - Return Functions. 
         */
        ////////////////////////////////////////////////////////////////////////
        

        ////////////////////////////////////////////////////////////////////////
        /*
         * Start - Set Pico related stuff like Trigger and so on...
         */

        //! Configures the trigger of the pico.
        void    configureTrigger(Utility::CLAWS_TRIGGER_MODE mode);

        //! Configure all four channel at once.
        // It sends the data to the pico, allocates the data buffer
        // for each channel and does a consistency check if the number
        // of total samples is allowed (compares with getTimebase())
        void    configureChannels();

        //! Sets the timebase of the pico.
        void    setTimebase(uint32_t timebase);
        //! Sets the pretrigger samples.
        void    setPreTrigger(uint32_t pretrigger);
        //! Sets the posttrigger samples.
        void    setPostTrigger(uint32_t posttrigger);

        //! Sets all Pico member variables at once (despite trigger settings).
        void    setVariables(   uint32_t        timebase,                       \
                                int16_t         oversample,                     \
                                uint32_t        segmentIndex,                   \
                                uint32_t        preTrigger,                     \
                                uint32_t        postTrigger,                    \
                                uint32_t        downSampleRatio,                \
                                PS6000_RATIO_MODE downSampleRatioMode
                                );
        
        //! Sets a simple trigger.
        void    setSimpleTrigger(\
                    int16_t                     enabled,            \
                    PS6000_CHANNEL              source,             \
                    int16_t                     threshold,          \
                    PS6000_THRESHOLD_DIRECTION  direction,          \
                    uint32_t                    delay,              \
                    int16_t                     autoTriggerTime_ms
                    );
        //! Puts the timebase from the Picoscope and returns the TimeInterval in
        // nanoseconds as well as the maximum number of samples to take.
        void    getTimebase();

        //! This function starts collecting data in block mode.
        void    runBlock();

        //! Retrieves the aquired data, outputs the number of collected samples
        // and saves the number of overflows.
        void    getValues(); 
        /*
         * End - Set Pico related stuff like Trigger and so on...
         */
        ////////////////////////////////////////////////////////////////////////

        //! Stops the picoscope from sampling data.
        void    stopUnit();

        //! Closes the connection to the picoscope.
        void    closeUnit();

        
        ///////////////////////////////////////////////////////////////////////
        /*
         * Function to print channel informations.
         */
        
        //! Overloading of the operator<< to just use 'cout << channelA << endl;'
        // printing the information needed.
        friend std::ostream& operator<< (
                std::ostream &out, const Pico::Channel *channel);
        
        ///////////////////////////////////////////////////////////////////////
};





#endif

