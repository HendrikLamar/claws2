// =====================================================================================
// 
//       Filename:  utility.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03.05.2017 16:25:43
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#ifndef UTILITY_H
#define UTILITY_H

#include "clawsException.h"

#include <map>
#include <string>

#include <libps6000-1.4/ps6000Api.h>
#include <libps6000-1.4/PicoStatus.h>

namespace Utility{

    ////////////////////////////////////////////////////////////////////////
    /*
     * Pico Ready?
    */
    extern bool pReady;
    // I am not sure whats going on here.
    void CallBackBlock
    (
            int16_t         handle,          \
            PICO_STATUS     t_status,        \
            void            *pParameter
    );

    

    ////////////////////////////////////////////////////////////////////////


    //! Rewrites a std::string to an int8_t array which is given as input parameter.
    void stringToInt8_t( int8_t* tarray, std::string txt );


    ////////////////////////////////////////////////////////////////////////


    struct N6700_connect
    {
        std::string ip;
        std::string id;
        std::string port; 
    };



    ////////////////////////////////////////////////////////////////////////
    //
    //          Pico Data Structs to be read-in
    //
    ////////////////////////////////////////////////////////////////////////


    //! Data structure for on channel of a picoscope. This data needs to be read-in.
    //! Supported input parameters:
    //!      - enabled:
    //!          - true
    //!          - false
    //!      - coupling:
    //!          - PS6000_AC
    //!          - PS6000_DC_1M
    //!          - PS6000_DC_50R
    //!      - analogouOffset:
    //!          - Floating point given in volts.
    //!      - range:
    //!          - PS6000_50MV
    //!          - PS6000_100MV
    //!          - PS6000_200MV
    //!          - PS6000_500MV
    //!          - PS6000_1V
    //!          - PS6000_2V
    //!          - PS6000_5V
    //!          - PS6000_10V
    //!          - PS6000_20V
    //!      - bandwidth:
    //!          - PS6000_BW_FULL
    //!          - PS6000_BW_25MHZ
    //
    struct Pico_Data_Channel
    {
        Pico_Data_Channel( PS6000_CHANNEL tchannel ) : channel(tchannel) {};
        //! Holds the channel number in the pico. Not set by the ini-file.
        const PS6000_CHANNEL        channel;

        PS6000_COUPLING             coupling;
        int16_t                     enabled;
        float                       analogueOffset;
        PS6000_RANGE                range;
        PS6000_BANDWIDTH_LIMITER    bandwidth;
    };



    //! Data structure holding the trigger settings.
    //! Supported input parameters:
    //!          - enabled:
    //!              - true
    //!              - false
    //!          - source:
    //!              - PS6000_CHANNEL_A
    //!              - PS6000_CHANNEL_B
    //!              - PS6000_CHANNEL_C
    //!              - PS6000_CHANNEL_D
    //!              - PS6000_EXTERNAL
    //!              - PS6000_TRIGGER_AUX
    //!          - threshold:
    //!              - Integer value between +-32512 in steps of 256
    //!          - direction (only mentioning most probable ones):
    //!              - PS6000_RISING        // using upper threshold
    //!              - PS6000_FALLING       // using upper threshold
    //!          - delay:
    //!              - Integer given in milliseconds.
    //!          - autoTriggerTime:
    //!              - Integer given in milliseconds.
    //!              - 0 = no auto trigger
    struct Pico_Data_Trigger_Simple
    {
        int16_t                     enabled;
        PS6000_CHANNEL              source;
        int16_t                     threshold;
        PS6000_THRESHOLD_DIRECTION  direction;
        uint32_t                    delay;
        int16_t                     autoTriggerTime;
    };

    //! Data structure for an entire picoscope. All the data need to be read-in.
    //! Supported input parameters:
    //!      - Ch1-4:
    //!          - see Pico_DATA_CHANNEL
    //!      - serial:
    //!          - the serial of the pico (written on the osci)
    //!      - pre-/postTrigger:
    //!          - integer, the sum of both should not exceed 2 GS
    //!      - timebase:
    //!          - 0 -> 200 ps   // only available if less than 3 channels enabled per pico
    //!          - 1 -> 400 ps   // only available if less than 3 channels enabled per pico
    //!          - 2 -> 800 ps
    //!          - 3 -> 1.6 ns
    //!          - 4 -> 3.2 ns
    //!          - ...more are not implemented.
    //!      - oversample:
    //!          - 0 to 256 ( recommended: 0)
    //!      - downSampleRatioMode:
    //!          - PS6000_RATIO_MODE_NONE
    //!      - downSampleRatio:
    //!          - 1
    struct Pico_Data_Pico
    {
        Pico_Data_Pico( std::string tserial ) : 
            Ch1( PS6000_CHANNEL_A ),
            Ch2( PS6000_CHANNEL_B ),
            Ch3( PS6000_CHANNEL_C ),
            Ch4( PS6000_CHANNEL_D )
        {
            // check if the serial is longer than 100 chars.
            if ( tserial.size() > 100 )
            {
                throw PicoException("The serial cannot be longer than 100 chars. Sorry!");
            }

            Utility::stringToInt8_t( serial, tserial );
        };

        Pico_Data_Channel         Ch1;
        Pico_Data_Channel         Ch2;
        Pico_Data_Channel         Ch3;
        Pico_Data_Channel         Ch4;

        Pico_Data_Trigger_Simple  trigger;


        int8_t                  serial[100];

        uint32_t                preTrigger;
        uint32_t                postTrigger;

        uint32_t                timebase;
        int16_t                 oversample;

        enPS6000RatioMode       downSampleRatioMode;
        uint32_t                downSampleRatio;


    };





    ////////////////////////////////////////////////////////////////////////





    //! Enum to specify which run mode should be loaded in Database::Pico_readConfig().
    enum Pico_RunMode
    {
        OBERMAIER_HG,
        MERKEL_HG,
        SCHIFFER_LG,
        KLUM_LG,
        GARRN
    };
    




    ////////////////////////////////////////////////////////////////////////




    enum CLAWS_TRIGGER_MODE{
        
        CLAWS_TRIGGER_SIMPLE,
        CLAWS_TRIGGER_ADVANCED,
    };
    



    ////////////////////////////////////////////////////////////////////////





    enum ClawsGain
    {
        HIGH_GAIN,
        LOW_GAIN
    };





} // END Namespace Utility


#endif //UTILITY_H
