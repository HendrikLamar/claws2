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

    enum CLAWS_TRIGGER_MODE{
        
        CLAWS_TRIGGER_SIMPLE,
        CLAWS_TRIGGER_ADVANCED,
    };
    
    enum ClawsGain
    {
        HIGH_GAIN,
        LOW_GAIN
    };
    

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
    //!      - coupling:
    //!          - PS6000_AC
    //!          - PS6000_DC_1M
    //!          - PS6000_DC_50R
    //!      - enabled:
    //!          - true
    //!          - false
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
    struct PS_Data_Channel
    {
        //! Holds the channel number in the pico. Not set by the ini-file.
        const PS6000_CHANNEL        channel;

        PS6000_COUPLING             coupling;
        int16_t                     enabled;
        float                       analogueOffset;
        PS6000_RANGE                range;
        PS6000_BANDWIDTH_LIMITER    bandwidth;
    };


    //! Data structure for an entire picoscope. All the data need to be read-in.
    //! Supported input parameters:
    //!      - Ch1-4:
    //!          - see PS_DATA_CHANNEL
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
    struct PS_Data_Pico
    {
        PS_Data_Channel     Ch1;
        PS_Data_Channel     Ch2;
        PS_Data_Channel     Ch3;
        PS_Data_Channel     Ch4;



        const int8_t        serial;

        uint32_t            preTrigger;
        uint32_t            postTrigger;

        uint32_t            timebase;
        int16_t             oversample;

        enPS6000RatioMode   downSampleRatioMode;
        uint32_t            downSampleRatio;


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
    //!          - autoTriggerTime_ms:
    //!              - Integer given in milliseconds.
    struct PS_Data_Trigger_Simple
    {
        int16_t                     enabled;
        PS6000_CHANNEL              source;
        int16_t                     threshold;
        PS6000_THRESHOLD_DIRECTION  direction;
        uint32_t                    delay;
        int16_t                     autoTriggerTime_ms;
    };

}
#endif //UTILITY_H
