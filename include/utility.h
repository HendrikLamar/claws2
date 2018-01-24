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
#include <memory>
#include <atomic>
//#include <typeinfo>

#include <libps6000-1.4/ps6000Api.h>
#include <libps6000-1.4/PicoStatus.h>

#include <TH1I.h>




namespace Utility{

    ///////////////////////////////////////////////////////////////////////////





    //! Rewrites a std::string to an int8_t array which is given as input parameter.
    void stringToInt8_t( int8_t* tarray, std::string txt );


    ///////////////////////////////////////////////////////////////////////////
    


    //! Searches for a picoscope and returns its serial number as a string.
    //! On exit, handle holds the unique handle value for the picoscope.
    std::string Pico_preInitializer( int16_t* handle );

    //! Closes the pico if it is not needed.
    void        Pico_preClose( int16_t handle );
    

    std::string coutColor(std::string text, std::string color, 
            std::string other = "none");
    





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
    //                      Pico Enums
    //



    //! Enum to specify which run mode should be loaded in Database::Pico_readConfig().
    enum class Pico_RunMode
    {
        INTERMEDIATE,
        PHYSICS
    };
    




    ////////////////////////////////////////////////////////////////////////




    enum class Pico_Trigger_Mode{
        
        TRIGGER_SIMPLE,
        TRIGGER_ADVANCED 
    };
    



    ///////////////////////////////////////////////////////////////////////////





    enum class Claws_Gain
    {
        HIGH_GAIN,
        LOW_GAIN
    };



    ///////////////////////////////////////////////////////////////////////////
    





    enum class Collection_Mode
    {
        BLOCK,
        RAPID,
        INTERMEDIATE
    };








///////////////////////////////////////////////////////////////////////////////




    enum class Dir_Struct
    {
        INTER,
        PHYSICS,
        LIVE
    };

















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

    //! String to enum for Utility::Pico_Trigger_Mode
    Utility::Pico_Trigger_Mode  Pico_StringToEnum_trigger( std::string& enumerator );

    //! String to enum for Utility::Collection_Mode
    Utility::Collection_Mode    Pico_StringToEnum_collection( std::string& enumerator );

    //! Transforms a integer channel number (0-3) to a PS6000_CHANNEL enum.
    PS6000_CHANNEL              Pico_intToEnum_channel( int ch );


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
    std::string     Pico_EnumToString_channel( PS6000_CHANNEL var );

    //! Enum to string for PS6000_THRESHOLD_DIRECTION
    std::string     Pico_EnumToString_thresDir( PS6000_THRESHOLD_DIRECTION& var );

    //! Enum to string for Utility::Pico_runMode
    std::string     Pico_EnumToString_runMode( Utility::Pico_RunMode& mode );

    //! Enum to string for Utility::Pico_Trigger_Mode
    std::string     Pico_EnumToString_trigger( Utility::Pico_Trigger_Mode& mode );

    //! Enum to string for Utility::Collection_Mode
    std::string     Pico_EnumToString_collection( Utility::Collection_Mode& mode );

    //
    //                  END Pico_EnumToString functions
    ///////////////////////////////////////////////////////////////////////////






    //
    //                      END Pico Enums
    ///////////////////////////////////////////////////////////////////////////































    ///////////////////////////////////////////////////////////////////////////
    //                  START Data Structures
    //



    //! Struct to store the data of one channel as a TH1I histogramm.
    struct Pico_Hist_Channel
    {
        Pico_Hist_Channel( PS6000_CHANNEL cha );
        Pico_Hist_Channel( int cha );   //!> Goes from 0 to 3.
        Pico_Hist_Channel( PS6000_CHANNEL cha, std::shared_ptr<TH1I> hist );
        Pico_Hist_Channel( int cha, std::shared_ptr<TH1I> hist );
        ~Pico_Hist_Channel();


        ///////////////////////////////////////////////////////////////////////

        //! Sets a set of data if the channel was created empty or redefine.
        void set( std::shared_ptr<TH1I> hist );

        //! Returns the TH1I of the channel.
        std::shared_ptr<TH1I>    get();

        //! Returns the channel number.
        PS6000_CHANNEL getID();


        private:
            PS6000_CHANNEL  channel;

            void set( 
                    int cha, 
                    std::shared_ptr<TH1I> hist );

            std::shared_ptr< TH1I > data;



    };















    ///////////////////////////////////////////////////////////////////////////
    











    struct Pico_Hist_Pico
    {

        Pico_Hist_Pico( std::string loc );

        void add( std::shared_ptr<Pico_Hist_Channel> hist );

        std::shared_ptr<TH1I>  get( PS6000_CHANNEL ch );

        std::shared_ptr<TH1I>  get( int ch );

        std::string getLoc();

        int         getSize();

        ///////////////////////////////////////////////////////////////////////

        private:
            std::shared_ptr< std::vector< std::shared_ptr<Pico_Hist_Channel> > > data;
            std::string location;

    };








///////////////////////////////////////////////////////////////////////////////








    struct Pico_Data_Analyzed_Channel
    {
        Pico_Data_Analyzed_Channel( int cha ) :
            channel( Utility::Pico_intToEnum_channel(cha)) {};
        Pico_Data_Analyzed_Channel( PS6000_CHANNEL cha ) :
            channel( cha ) {};
        virtual ~Pico_Data_Analyzed_Channel(){};

        PS6000_CHANNEL getID();

        //! Normalized 1pe integral. Normalized against vertical range and
        //! preamp.
        double          cal_1pe_normalized;

        //! Gives the fraction of the number of 1pe wf in the acquired data.
        //! This gives hints about the radiation damagage of the SiPM.
        double          cal_1peVStotal_fraction;

        //! Absolute number of MIPs in the waveform.
        unsigned int    data_mip_absolute;

        //! Holds the reconstructed waveform.
        std::shared_ptr< std::vector< unsigned int > > data_wf_reconstructed;

        //! Holds the reconstructed and downsampled waveform which is suitable for
        //! epics.
        std::shared_ptr< std::vector< unsigned long > > 
            data_wf_reconstructedANDdownsampled;

        //! Bin size value of reconstructed and downsampled data.
        float           info_value_binSize_reconstructedANDdownsampled;

        //! Unit of the bins of reconstructed and downsampled waveform.
        std::string     info_unit_binSize_reconstructedANDdownsampled;

        //! CDF of the waveform.
        std::shared_ptr< std::vector< double > > data_cdf;

        //! Bin size value of the cdf.
        float           info_value_binSize_cdf;

        //! Unit of the cdf bins.
        std::string     info_unit_binSize_cdf;

        private:
            PS6000_CHANNEL      channel;
    };









///////////////////////////////////////////////////////////////////////////////









    struct Pico_Data_Analyzed_Pico
    {

        Pico_Data_Analyzed_Pico( std::string loc ) :
            data( std::make_shared< std::vector< std::shared_ptr< 
                    Pico_Data_Analyzed_Channel > > >() ),
            location( loc )
        {
            data->push_back( 
                    std::make_shared<Utility::Pico_Data_Analyzed_Channel>(
                        PS6000_CHANNEL_A) );
            data->push_back( 
                    std::make_shared<Utility::Pico_Data_Analyzed_Channel>(
                        PS6000_CHANNEL_B) );
            data->push_back( 
                    std::make_shared<Utility::Pico_Data_Analyzed_Channel>(
                        PS6000_CHANNEL_C) );
            data->push_back( 
                    std::make_shared<Utility::Pico_Data_Analyzed_Channel>(
                        PS6000_CHANNEL_D) );
        }
        virtual ~Pico_Data_Analyzed_Pico(){};

        //! Returns the channel data.
        std::shared_ptr< Pico_Data_Analyzed_Channel > get( PS6000_CHANNEL ch );

        //! Returns the channel data.
        std::shared_ptr< Pico_Data_Analyzed_Channel > get( int ch );

        //! Returns the pico location.
        std::string getLocation();

        private:
            std::shared_ptr< std::vector< std::shared_ptr< 
                Pico_Data_Analyzed_Channel> > > data;
            std::string location;

    };







///////////////////////////////////////////////////////////////////////////////








    struct Pico_Conf_Analysis
    {
        // intermediate
        float       inter_factor1pe;

        // physics
        int         physics_signalCut;
        unsigned long   physics_minSignalLength;
        unsigned long   physics_preSamplesBeforeThreshold;
        unsigned long   physics_postSamplesAfterThreshold;
        int             physics_wfScanStepSize;
    };














    //
    //                   END Data Structures 
    ///////////////////////////////////////////////////////////////////////////







































    
    ///////////////////////////////////////////////////////////////////////////
    //                  Pico Setting Structures
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
    struct Pico_Conf_Channel
    {
        Pico_Conf_Channel( PS6000_CHANNEL tchannel ) : channel(tchannel) {};
        //! Holds the channel number in the pico. Not set by the ini-file.
        const PS6000_CHANNEL        channel;

        PS6000_COUPLING             coupling;
        int16_t                     enabled;
        float                       analogueOffset;
        PS6000_RANGE                range;
        PS6000_BANDWIDTH_LIMITER    bandwidth;

    };






///////////////////////////////////////////////////////////////////////////////







    //! Pico PS6000_TRIGGER_CONDITIONS structure
#pragma pack(push,1)
    struct tTriggerConditions
    {
        PS6000_TRIGGER_STATE    channelA;
        PS6000_TRIGGER_STATE    channelB;
        PS6000_TRIGGER_STATE    channelC;
        PS6000_TRIGGER_STATE    channelD;
        PS6000_TRIGGER_STATE    external;
        PS6000_TRIGGER_STATE    aux;
        PS6000_TRIGGER_STATE    pulseWidthQualifier;
    };
#pragma pack(pop)







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
    struct Pico_Conf_Trigger_Simple
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
    //!          - see Utility::Pico_Conf_Channel
    //!      - serial:
    //!          - the serial of the pico (written on the osci)
    //!      - location:
    //!           - Top_Forward
    //!           - Top_Backward
    //!           - Bottom_Forward
    //!           - Bottom_Backward
    //!      - triggerMode:
    //!          - TRIGGER_SIMPLE
    //!          - TRIGGER_ADVANCED
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
    struct Pico_Conf_HL_Gain
    {
        //! Constructor takes care about the channels.
        Pico_Conf_HL_Gain( Utility::Pico_RunMode tmode ) : 
            runMode( tmode ),
            channels( new std::vector< std::shared_ptr<Utility::Pico_Conf_Channel> >
                        {
                            std::make_shared<Utility::Pico_Conf_Channel>(PS6000_CHANNEL_A),
                            std::make_shared<Utility::Pico_Conf_Channel>(PS6000_CHANNEL_B),
                            std::make_shared<Utility::Pico_Conf_Channel>(PS6000_CHANNEL_C),
                            std::make_shared<Utility::Pico_Conf_Channel>(PS6000_CHANNEL_D)
                        }
                    ),
            data_trigger( new Utility::Pico_Conf_Trigger_Simple() )
        {};

        //! Destructor takes care about the channels.
        ~Pico_Conf_HL_Gain()
        {
            channels.reset();
            data_trigger.reset();
        };

        Utility::Claws_Gain     gain;
        Utility::Pico_RunMode   runMode;


        std::shared_ptr<std::vector< 
            std::shared_ptr<Utility::Pico_Conf_Channel> > > channels;


        std::shared_ptr<Pico_Conf_Trigger_Simple> data_trigger;

        Pico_Trigger_Mode       mode_trigger;

        uint32_t                val_preTrigger;
        uint32_t                val_postTrigger;

        uint32_t                val_timebase;
        int16_t                 val_oversample;

        enPS6000RatioMode       val_downSampleRatioMode;
        uint32_t                val_downSampleRatio;

        uint32_t                loops_inter;

        int                     average_1pe_height;

    };



///////////////////////////////////////////////////////////////////////////////



    struct Pico_Conf_Pico
    {
        Pico_Conf_Pico( std::string tserial, std::string tlocation ) :
            data_physics( new Utility::Pico_Conf_HL_Gain( 
                        Utility::Pico_RunMode::PHYSICS) ),
            data_inter( new Utility::Pico_Conf_HL_Gain( 
                        Utility::Pico_RunMode::INTERMEDIATE) ),
            conf_analysis( std::make_shared<Pico_Conf_Analysis>())
        {
            // check if the serial is longer than 100 chars.
            if ( tserial.size() > 100 )
            {
                throw PicoException("The serial cannot be longer than 100 chars. Sorry!");
            }

            // translate the string to the demanded int8_t type
            Utility::stringToInt8_t( val_serial, tserial );


            ///////////////////////////////////////////////////////////////////

            // check if the location is allowed!
            std::vector< std::string > possibleLocations{
                "Top_Forward",
                "Top_Backward",
                "Bottom_Forward",
                "Bottom_Backward"};

            // bool to check if the location is allowd
            bool dummybool = false;
            for ( auto &tmp : possibleLocations)
            {
                if ( tlocation.compare( tmp ) == 0 )
                {
                    dummybool = true;
                }
            }
            
            dummybool ? val_location = tlocation : throw PicoException(
                    "Location not valid!");


        };

        ~Pico_Conf_Pico()
        {
            // delete data pointers
            data_physics.reset();
            data_inter.reset();
        }


        ///////////////////////////////////////////////////////////////////////

        int8_t                  val_serial[100];
        std::string             val_location;

        std::shared_ptr<Pico_Conf_HL_Gain>      data_physics;

        std::shared_ptr<Pico_Conf_HL_Gain>      data_inter;

        std::shared_ptr<Pico_Conf_Analysis>     conf_analysis;

    };




    //
    //                      END Pico Setting Structures
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
//       Claws_Gain              gain_current;

//        Pico_RunMode            runMode_current;
//        Pico_RunMode            runMode_HighGain;
//        Pico_RunMode            runMode_LowGain;

        std::string             path_config_intermediate;
        std::string             path_config_physics;

        unsigned int            loops_Physics;
        unsigned int            loops_Intermediate;

        std::atomic<bool>       isSaved_raw;
        std::atomic<bool>       isSaved_online;

        bool                    useEpics;
        std::string             path_saveData;


    };










///////////////////////////////////////////////////////////////////////////////
//
//                  END General Data Structure 
//
///////////////////////////////////////////////////////////////////////////////













///////////////////////////////////////////////////////////////////////////////
//
//                  START PSU settings
//
///////////////////////////////////////////////////////////////////////////////



    struct PSU_Channel
    {
        PSU_Channel( std::string tname, int tchannelNo ) :
            name(tname),
            channelNo(tchannelNo)
        {};

        std::string     name;
        int             channelNo;

        std::string     powerOnOff;
        float           limit_volt;
        float           limit_current;
    };



    struct PSU_Config
    {
        public:
            //! Standard constructor.
            PSU_Config()
            {
                channels = std::make_shared<std::vector<
                    std::shared_ptr<PSU_Channel>>>();
            }

            ~PSU_Config(){};

            //! Returns the channel settings.
            std::shared_ptr<PSU_Channel> getCh( int i )
            {
                std::shared_ptr<PSU_Channel> output;
                for( auto& tmp : *channels )
                {
                    if( tmp->channelNo == i )
                    {
                        output = tmp;
                        break;
                    }
                }
                return output;
            }
            
            //! Returns the channel settings.
            std::shared_ptr<PSU_Channel> getCh( std::string name )
            {
                std::shared_ptr<PSU_Channel> output;
                for( auto& tmp : *channels )
                {
                    if( !(tmp->name.compare(name)) )
                    {
                        output = tmp;
                        break;
                    }
                }
                return output;
            }

            //! Sets a new channel.
            void setCh( std::shared_ptr<PSU_Channel> channel )
            {
                channels->push_back(channel);
                return;
            }

            //! Returns the amount of channels in this conifguration.
            unsigned int getNoOfCh()
            {
                return channels->size();
            }

            //! Remove all of the channels.
            void clear()
            {
                channels->clear();

                return;
            }

        private:
            std::shared_ptr<std::vector< std::shared_ptr<PSU_Channel>>> channels;
    };








///////////////////////////////////////////////////////////////////////////////
//
//                  START PSU settings
//
///////////////////////////////////////////////////////////////////////////////
} // END Namespace Utility


#endif //UTILITY_H

