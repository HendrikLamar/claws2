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

#include <libps6000-1.4/PicoStatus.h>
#include <libps6000-1.4/ps6000Api.h>
#include <vector>

/*! The Pico class to steer and handle one Picoscope */
class Pico
{
    private: 

        // Information of the entire Picoscope
        PICO_STATUS m_status;

        int16_t   m_handle;             
//        int16_t         m_serialLength      {127};   
//        int8_t          m_serials;
        const int8_t    m_serial;              
//        bool            m_psOpen            {false};

        // timebase and acquisition variables
        // b -> Block Mode
        // r -> Rapid Block Mode
        uint32_t        m_timebase          {2}; // means 0.8ns
        uint32_t        m_bNoSamples;            
        uint32_t        m_rNoSamples;            
        float           m_timeInterval_ns;
        int16_t         m_oversample        {0}; // we cant use it
        uint32_t        m_bMaxSamples;
        uint32_t        m_rMaxSamples;
        uint32_t        m_bSegmentIndex      {0}; // since we have only one block
        uint32_t        m_rSegmentIndex;          // I have not idea so far.
        uint32_t        m_bBufferSize       {1000000}; // to reserve vectors for the data with this size
        uint32_t        m_preTrigger;
        uint32_t        m_postTrigger;

        enPS6000RatioMode   PS6000_RATIO_MODE_NONE;



        /////////////////////////////////////////////////////////////////////
        // default channel values
        //
        // only default values - each channel has its own struct!
        //
        PS6000_CHANNEL  m_channel;    //used to partially edit the channel named here
        PS6000_COUPLING m_coupling              {PS6000_DC_50R};
        int16_t         m_enabled               {false};
        float           m_analogueOffset        {0};
        PS6000_RANGE    m_range                 {PS6000_50MV};
        PS6000_BANDWIDTH_LIMITER m_bandwidth    {PS6000_BW_FULL};

        // struct for one channel
        struct Channel{
            PS6000_CHANNEL              channel;
            int16_t                     enabled; 
            float                       analogueOffset;
            PS6000_COUPLING             coupling; 
            PS6000_RANGE                range;
            PS6000_BANDWIDTH_LIMITER    bandwidth;

            std::vector<int16_t>*       buffer;     // channel buffer
        };

        Channel m_channelA;
        Channel m_channelB;
        Channel m_channelC;
        Channel m_channelD;
        Channel m_channelExt;
        ////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////
        // default trigger settings
        int16_t                     m_tEnabled          {false};
        PS6000_CHANNEL              m_tSource           {PS6000_TRIGGER_AUX}; 
        int16_t                     m_tThreshold        {-1000};
        PS6000_THRESHOLD_DIRECTION  m_tDirection        {PS6000_FALLING};
        uint16_t                    m_tDelay            {0};
        int16_t                     m_tAutoTrigger_ms   {1500};
        ////////////////////////////////////////////////////////////////////

        
        void checkStatus();         // checks the m_status value
        void printError(const char* message);          // prints a dedicated error message

        void turnOffUnnecessary();                      // turns of unneeded stuff like ETS
        void openUnit(int16_t &handle, int8_t serial);  // opens the pico unit
    
        
        Channel& assignToChannel(); 

    public:
        
        ////////////////////////////////////////////////////////////////////
        // Constructor
        Pico(int16_t &handle, int8_t serial = 0);

        // Destructor
        ~Pico(){
//            closeUnit();
        };
        ////////////////////////////////////////////////////////////////////





        void    loadDefaultCh(Channel &s_channel, PS6000_CHANNEL i_channel);
        void    loadDefaultCh(Channel &s_channel);

        
        ////////////////////////////////////////////////////////////////////
        // Changes only often needed settings. 
        void    setCh(int16_t enabled, PS6000_RANGE range,\
                    float analogueOffset);

        void    setChEnabled(int16_t enabled);
        void    setChRange(PS6000_RANGE range);
        void    setChOffset(float analogueOffset);
        ////////////////////////////////////////////////////////////////////
        

        // Set simples trigger.
        void    setSimpleTrigger(int16_t enabled, PS6000_CHANNEL source, \
                    int16_t threshold, PS6000_THRESHOLD_DIRECTION direction,\
                    uint16_t delay, int16_t autoTriggerTime_ms);

        // Defines the data buffer to use per channel.
        void    setDataBuffer();

        // Gets the right channel to change settings.
        Pico&                       changeCh(int channel);
        Pico&                       changeCh(PS6000_CHANNEL channel);



        ////////////////////////////////////////////////////////////////////
        // Get each setting of the before defined channel.
        PS6000_CHANNEL              getSChannel();
        int16_t                     getSEnabled();
        float                       getSOffset();
        PS6000_COUPLING             getSCoupling();
        PS6000_RANGE                getSRange();
        PS6000_BANDWIDTH_LIMITER    getSBandwidth();

        ////////////////////////////////////////////////////////////////////
        // Get the timebase from the Picoscope
        uint32_t getTimebase( 
                uint32_t    timebase,   \
                uint32_t    noSamples,  \
                float       *timeInterval_ns
                );


        // prints data of the current channel
        void                        printCh();
        ////////////////////////////////////////////////////////////////////
        
        // Close the picoscope.
        void                        closeUnit();

};





#endif

