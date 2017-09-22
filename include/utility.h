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

//#include "pico.h"
#include "clawsException.h"

#include <map>
#include <vector>
#include <string>
#include <iostream>
//#include <typeinfo>

#include <libps6000-1.4/ps6000Api.h>
#include <libps6000-1.4/PicoStatus.h>

namespace Utility{

    ///////////////////////////////////////////////////////////////////////////
    /*
     * Pico Ready?
    */
/*     extern bool pReady;
 *     // I am not sure whats going on here.
 *     void CallBackBlock
 *     (
 *             int16_t         handle,          \
 *             PICO_STATUS     t_status,        \
 *             void            *pParameter
 *     );
 */

    
    //! Needed by Pico:runBlock() and Pico::runRapid() implementation.
    class CallBackPico
    {
        public:

            CallBackPico(){};
            ~CallBackPico(){};

            bool    pReady{false};

            void callBackBlock(
                        int16_t         handle,
                        PICO_STATUS     tstatus,
                        void*           pParameter
                    );
    };




    ///////////////////////////////////////////////////////////////////////////





    //! Rewrites a std::string to an int8_t array which is given as input parameter.
    void stringToInt8_t( int8_t* tarray, std::string txt );


    ///////////////////////////////////////////////////////////////////////////
    

    

    





    ///////////////////////////////////////////////////////////////////////////


    struct N6700_connect
    {
        std::string ip;
        std::string id;
        std::string port; 
    };












///////////////////////////////////////////////////////////////////////////////
//
//                  START Pico related stuff! 
//
///////////////////////////////////////////////////////////////////////////////





    
    ///////////////////////////////////////////////////////////////////////////
    //                  Pico Data Structures
    //


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

        friend std::ostream& operator<<(
                std::ostream& out, Utility::Pico_Data_Channel& data );
    };






///////////////////////////////////////////////////////////////////////////////








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

        friend std::ostream& operator<<(
                std::ostream& out, Utility::Pico_Data_Trigger_Simple& data );
    };

    //! Data structure for an entire picoscope. All the data need to be read-in.
    //! Supported input parameters:
    //!      - Ch1-4:
    //!          - see Utility::Pico_Data_Channel
    //!      - serial:
    //!          - the serial of the pico (written on the osci)
    //!      - location:
    //!           - Upper_Forward
    //!           - Upper_Backward
    //!           - Lower_Forward
    //!           - Lower_Backward
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
    struct Pico_Data_HL_Gain
    {
        //! Constructor takes care about the channels.
        Pico_Data_HL_Gain() : 
            channels( new std::vector< Utility::Pico_Data_Channel* >
                        {
                            new Utility::Pico_Data_Channel(PS6000_CHANNEL_A),
                            new Utility::Pico_Data_Channel(PS6000_CHANNEL_B),
                            new Utility::Pico_Data_Channel(PS6000_CHANNEL_C),
                            new Utility::Pico_Data_Channel(PS6000_CHANNEL_D)
                        }
                    ),
            trigger( new Utility::Pico_Data_Trigger_Simple() )
        {};

        //! Destructor takes care about the channels.
        ~Pico_Data_HL_Gain()
        {
            for ( auto& tmp: *channels )
            {
                // delete if empty
                if( tmp )
                {
                    delete tmp;
                }
                tmp = nullptr;
            }

            // delete if empty
            if( channels )
            {
                delete channels;
            }
            channels = nullptr;

            // delete if not empty
            if( trigger )
            {
                delete trigger;
            }
            trigger = nullptr;

        };


        std::vector< Utility::Pico_Data_Channel* >* channels;


        Pico_Data_Trigger_Simple* trigger;


        uint32_t                preTrigger;
        uint32_t                postTrigger;

        uint32_t                timebase;
        int16_t                 oversample;

        enPS6000RatioMode       downSampleRatioMode;
        uint32_t                downSampleRatio;

        friend std::ostream& operator<<(
                std::ostream& out, Utility::Pico_Data_HL_Gain& data );

    };



///////////////////////////////////////////////////////////////////////////////



    struct Pico_Data_Pico
    {
        Pico_Data_Pico( std::string tserial, std::string tlocation ) :
            dataLowGain( new Utility::Pico_Data_HL_Gain() ),
            dataHighGain( new Utility:: Pico_Data_HL_Gain() ),
            dataIntermediate( new Utility::Pico_Data_HL_Gain() )
        {
            // check if the serial is longer than 100 chars.
            if ( tserial.size() > 100 )
            {
                throw PicoException("The serial cannot be longer than 100 chars. Sorry!");
            }

            // translate the string to the demanded int8_t type
            Utility::stringToInt8_t( serial, tserial );


            ///////////////////////////////////////////////////////////////////

            // check if the location is allowed!
            std::vector< std::string > possibleLocations{
                "Upper_Forward",
                "Upper_Backward",
                "Lower_Forward",
                "Lower_Backward"};

            // bool to check if the location is allowd
            bool dummybool = false;
            for ( auto &tmp : possibleLocations)
            {
                if ( tlocation.compare( tmp ) == 0 )
                {
                    dummybool = true;
                }
            }
            
            dummybool ? location = tlocation : throw PicoException(
                    "Location not valid!");


        };

        ~Pico_Data_Pico()
        {
            // delete data pointers
            if ( dataLowGain )
            {
                delete dataLowGain;
                dataLowGain = nullptr;
            }
            if ( dataHighGain )
            {
                delete dataHighGain;
                dataHighGain = nullptr;
            }
            if ( dataIntermediate )
            {
                delete dataIntermediate;
                dataIntermediate = nullptr;
            }
        }


        ///////////////////////////////////////////////////////////////////////

        int8_t                  serial[100];
        std::string             location;

        Pico_Data_HL_Gain*      dataLowGain;
        Pico_Data_HL_Gain*      dataHighGain;
        Pico_Data_HL_Gain*      dataIntermediate;

        friend std::ostream& operator<<(
                std::ostream& out, Utility::Pico_Data_Pico& data );
    };




    //
    //                      END Pico Data Structures
    ///////////////////////////////////////////////////////////////////////////





















    ///////////////////////////////////////////////////////////////////////////
    //                      Pico Enums
    //



    //! Enum to specify which run mode should be loaded in Database::Pico_readConfig().
    enum Pico_RunMode
    {
        INTERMEDIATE,

        MERKEL_HG,
        OBERMAIER_HG,
        SCHIFFER_LG,
        KLUM_LG,
        GARRN
    };
    




    ////////////////////////////////////////////////////////////////////////




    enum Pico_Trigger_Mode{
        
        TRIGGER_SIMPLE,
        TRIGGER_ADVANCED 
    };
    



    ///////////////////////////////////////////////////////////////////////////





    enum Claws_Gain
    {
        INTER,
        HIGH_GAIN,
        LOW_GAIN
    };

    //
    //                      END Pico Enums
    ///////////////////////////////////////////////////////////////////////////























    ///////////////////////////////////////////////////////////////////////////
    //                  Pico_StringToEnum functions
    //

    //! String to enum for PS6000_COUPLING
    PS6000_COUPLING     Pico_StringToEnum_coupling( std::string& enumerator );

    //! String to enum for PS600_RANGE
    PS6000_RANGE        Pico_StringToEnum_range( std::string& enumerator );

    //! String to enum for PS6000_BANDWIDTH_LIMITER
    PS6000_BANDWIDTH_LIMITER    Pico_StringToEnum_bandwidth( std::string& enumerator );

    //! String to enum for PS6000_RATIO_MODE
    PS6000_RATIO_MODE           Pico_StringToEnum_ratio( std::string& enumerator );

    //! String to enum for PS6000_CHANNEL
    PS6000_CHANNEL      Pico_StringToEnum_channel( std::string& enumerator );

    //! String to enum for PS6000_THRESHOLD_DIRECTION
    PS6000_THRESHOLD_DIRECTION  Pico_StringToEnum_thresDir(std::string& enumerator );

    //! String to enum for Utility::Pico_RunMode
    Utility::Pico_RunMode       Pico_StringToEnum_runMode( std::string& enumerator );

    //! String to enum for Utility::Pico_Trigger_Mode
    Utility::Pico_Trigger_Mode  Pico_StringToEnum_trigger( std::string& enumerator );


    //
    //                      END Pico_StringToEnum functions
    ///////////////////////////////////////////////////////////////////////////






    ///////////////////////////////////////////////////////////////////////////
    //                  Pico_EnumToString functions
    //
    // The following functions are needed for readbility of std::cout and
    // saving of the ini files.
    //

    //! Enum to string for PS6000_COUPLING
    std::string     Pico_EnumToString_coupling( PS6000_COUPLING& var );

    //! Enum to string for PS6000_RANGE 
    std::string     Pico_EnumToString_range( PS6000_RANGE& var );

    //! Enum to string for PS6000_BANDWIDTH_LIMITER_
    std::string     Pico_EnumToString_bandwidth( PS6000_BANDWIDTH_LIMITER& var );

    //! Enum to string for PS6000_RATIO_MODE
    std::string     Pico_EnumToString_ratio( PS6000_RATIO_MODE& var );

    //! Enum to string for PS6000_CHANNEL
    std::string     Pico_EnumToString_channel( PS6000_CHANNEL& var );

    //! Enum to string for PS6000_THRESHOLD_DIRECTION
    std::string     Pico_EnumToString_thresDir( PS6000_THRESHOLD_DIRECTION& var );

    //! Enum to string for Utility::Pico_runMode
    std::string     Pico_EnumToString_runMode( Utility::Pico_RunMode& mode );

    //! Enum to string for Utility::Pico_Trigger_Mode
    std::string     Pico_EnumToString_trigger( Utility::Pico_Trigger_Mode& mode );


    //
    //                  END Pico_EnumToString functions
    ///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
//                  END Pico related stuff! 
//
///////////////////////////////////////////////////////////////////////////////






























///////////////////////////////////////////////////////////////////////////////
//
//                  START General Data Structure 
//
///////////////////////////////////////////////////////////////////////////////



    
    struct Steering_Data
    {
        Pico_Trigger_Mode       triggerMode;
        Pico_RunMode            runMode_HighGain;
        Pico_RunMode            runMode_LowGain;


        int                     loopsPhysics;
        int                     loopsIntermediate;

        std::string             savePath_1;
        std::string             savePath_2;
        std::string             savePath_3;


        friend std::ostream& operator<<(
                std::ostream& out, Utility::Steering_Data& data );
    };










///////////////////////////////////////////////////////////////////////////////
//
//                  END General Data Structure 
//
///////////////////////////////////////////////////////////////////////////////


} // END Namespace Utility


#endif //UTILITY_H
