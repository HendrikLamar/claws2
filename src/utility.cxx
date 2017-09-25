// =====================================================================================
// 
//       Filename:  utility.cxx
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03.05.2017 16:36:13
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include "clawsException.h"
#include "utility.h"

#include <iostream>

#include <libps6000-1.4/ps6000Api.h>
#include <libps6000-1.4/PicoStatus.h>

namespace Utility{

    ////////////////////////////////////////////////////////////////////////
    /*
     * Pico Ready?
    */
/*     bool pReady = false;
 *     // I am not sure whats going on here.
 *     void CallBackBlock(
 *             int16_t         handle,          \
 *             PICO_STATUS     t_status,        \
 *             void            *pParameter){
 * 
 *         if(handle){};       // dirty: don't want 'variable unused' warning
 *         if(pParameter){};
 * 
 * 
 *         if(t_status != PICO_CANCELLED){
 *             pReady = true;
 *             std::cout << "Pico is done with aquiring!" << std::endl;
 *         }
 *     };
 */

    

    //! It is absolutly not clear to me why this function is implemented like that,
    //! but thats a copy of the pico example.
    void CallBackPico::callBackBlock(
                int16_t         handle,
                PICO_STATUS     tstatus,
                void*           pParameter
            )
    {
        if( handle ) {};        // dirty: otherwise 'variable unused'
        if(pParameter){};       // dirty: otherwise 'variable unused'

        if( tstatus != PICO_CANCELLED)
        {
            pReady = true;
        }
    };





    ///////////////////////////////////////////////////////////////////////////

    void stringToInt8_t( int8_t* tarray, std::string txt )
    {
        int tlength = txt.size();
        for ( std::string::iterator it = txt.begin(); it < txt.end(); ++it)
        {
            // the argument of tarray is the iterator reformed as int
            tarray[tlength - ( txt.end() - it ) ] = *it;

            // put an end character at the end of the array
            if ( txt.end() - it == 1 )
            {
                tarray[ tlength ] = '\0';
            }
        }
    };







///////////////////////////////////////////////////////////////////////////////



    ///////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //                  START Pico related stuff! 
    //
    //
    //
    ///////////////////////////////////////////////////////////////////////////



    std::ostream& operator<<( std::ostream& out, Utility::Pico_Data_Channel& data )
    {
        PS6000_CHANNEL tmp_channel = data.channel;

        out << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
        out << "\n\t\t" << Utility::Pico_EnumToString_channel( tmp_channel );
        out << "\n";
        out << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
        out << "\n";

        try
        {
            out << "Coupling: " << Utility::Pico_EnumToString_coupling( data.coupling );
            out << "\t\tEnabled: " << data.enabled;
            out << "\nAnaOffset: " << data.analogueOffset << " V";
            out << "\t\tRange: " << Utility::Pico_EnumToString_range( data.range );
            out << "\n";
            out << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
        }
        catch( UtilityException& excep )
        {
            std::cout << excep.what() << std::endl;
        }

        out << "\n";

        return out;
    }






    ///////////////////////////////////////////////////////////////////////////




    std::ostream& operator<<( std::ostream& out, Utility::Pico_Data_Trigger_Simple& data )
    {

        out << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
        out << "\n\t\tTrigger Simple\n";
        out << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
        out << "\n";

        try
        {
            out << "Enabled: " << data.enabled;
            out << "\t\tSource: " << Utility::Pico_EnumToString_channel( data.source );
            out << "\t\tThreshold: " << data.threshold;
            out << "\nDirection: " << Utility::Pico_EnumToString_thresDir( data.direction );
            out << "\t\tDelay: " << data.delay << " Samples";
            out << "\t\tAutoTriggerTime: " << data.autoTriggerTime << " ms";
            out << "\n";
            out << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
        }
        catch( UtilityException& excep )
        {
            std::cout << excep.what() << std::endl;
        }

        out << "\n";

        return out;
    }






    ///////////////////////////////////////////////////////////////////////////



    std::ostream& operator<<( std::ostream& out, Utility::Pico_Data_HL_Gain& data )
    {

        try
        {
            out << "Pretrigger: " << data.preTrigger;
            out << "\t\tPosttrigger: " << data.postTrigger;
            out << "\t\tTimebase: " << data.timebase;
            out << "\nOversample: " << data.oversample;
            out << "\t\tDownSampleRatioMode: " << data.downSampleRatioMode;
            out << "\tDownSampleRatio: " << data.downSampleRatio;
            out << "\nTriggerMode: " << Utility::Pico_EnumToString_trigger(data.triggerMode);
            out << "\n";

            for( auto& tmp : *data.channels )
            {
                out << *tmp;
            }

            out << *(data.trigger);
        }
        catch( UtilityException& excep )
        {
            std::cout << excep.what() << std::endl;
        }

        out << "\n";

        return out;
    }





    ///////////////////////////////////////////////////////////////////////////




    std::ostream& operator<<( std::ostream& out, Utility::Pico_Data_Pico& data )
    {
        out << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
        out << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
        out << "\n\t\tPico\t" << data.location << "\n";
        out << "\t\tSerial: " << data.serial;
        out << "\n";
        out << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
        out << "\n";

        try
        {
            out << "\tLow Gain Data\n";
            out << "-----------------------------------------------------------\n";
            out << *data.dataLowGain;
            out << "\n\n";

            out << "\tHigh Gain Data\n";
            out << "-----------------------------------------------------------\n";
            out << *data.dataHighGain;
            out << "\n\n";

            out << "\tIntermediate Data\n";
            out << "-----------------------------------------------------------\n";
            out << *data.dataIntermediate;
        }
        catch( UtilityException& excep )
        {
            std::cout << excep.what() << std::endl;
        }

        out << "\n\n";

        return out;
    }












    ///////////////////////////////////////////////////////////////////////////
    //
    //                          START
    //
    //                  Pico_StringToEnum functions
    //
    ///////////////////////////////////////////////////////////////////////////


    PS6000_COUPLING     Pico_StringToEnum_coupling( std::string& enumerator )
    {
        PS6000_COUPLING output;

        if ( enumerator.compare("PS6000_AC") == 0 )
        {
            return  PS6000_AC;
        }
        else if ( enumerator.compare("PS6000_DC_1M") == 0 )
        {
            return  PS6000_DC_1M;
        }
        else if ( enumerator.compare("PS6000_DC_50R") == 0 )
        {
            return  PS6000_DC_50R;
        }
        
        throw PicoException("Wrong coupling input!");

        return output;
    };




    ///////////////////////////////////////////////////////////////////////////
    


    PS6000_RANGE        Pico_StringToEnum_range( std::string& enumerator )
    {
        PS6000_RANGE output;

        if( enumerator.compare("PS6000_50MV") == 0 )
        {
            return  PS6000_50MV;
        }
        if( enumerator.compare("PS6000_100MV") == 0 )
        {
            return  PS6000_100MV;
        }
        if( enumerator.compare("PS6000_200MV") == 0 )
        {
            return  PS6000_200MV;
        }
        if( enumerator.compare("PS6000_500MV") == 0 )
        {
            return  PS6000_500MV;
        }
        if( enumerator.compare("PS6000_1V") == 0 )
        {
            return  PS6000_1V;
        }
        if( enumerator.compare("PS6000_2V") == 0 )
        {
            return  PS6000_2V;
        }
        if( enumerator.compare("PS6000_5V") == 0 )
        {
            return  PS6000_5V;
        }
        if( enumerator.compare("PS6000_10V") == 0 )
        {
            return  PS6000_10V;
        }
        if( enumerator.compare("PS6000_20V") == 0 )
        {
            return  PS6000_20V;
        }
        
        throw PicoException("Wrong range input!");

        return output;

    };



    ///////////////////////////////////////////////////////////////////////////



    PS6000_BANDWIDTH_LIMITER    Pico_StringToEnum_bandwidth( std::string& enumerator )
    {

        PS6000_BANDWIDTH_LIMITER output;

        if ( enumerator.compare("PS6000_BW_FULL") == 0 )
        {
            return  PS6000_BW_FULL;
        }
        if ( enumerator.compare("PS6000_BW_25MHZ") == 0 )
        {
            return  PS6000_BW_25MHZ;
        }
        
        throw PicoException("Wrong bandwidth input!");
        

        return output;
    };



    ///////////////////////////////////////////////////////////////////////////



    PS6000_RATIO_MODE   Pico_StringToEnum_ratio( std::string& enumerator )
    {
        PS6000_RATIO_MODE   output;

        if ( enumerator.compare("PS6000_RATIO_MODE_NONE") == 0 )
        {
            return PS6000_RATIO_MODE_NONE;
        }
        else throw PicoException("Ratio mode not known!");

        return output;
    };



    ///////////////////////////////////////////////////////////////////////////



    PS6000_CHANNEL      Pico_StringToEnum_channel( std::string& enumerator )
    {

        PS6000_CHANNEL output;
        if ( enumerator.compare("PS6000_CHANNEL_A") == 0)
        {
            return  PS6000_CHANNEL_A;
        }
        if ( enumerator.compare("PS6000_CHANNEL_B") == 0)
        {
            return  PS6000_CHANNEL_B;
        }
        if ( enumerator.compare("PS6000_CHANNEL_C") == 0)
        {
            return  PS6000_CHANNEL_C;
        }
        if ( enumerator.compare("PS6000_CHANNEL_D") == 0)
        {
            return  PS6000_CHANNEL_D;
        }
        if ( enumerator.compare("PS6000_EXTERNAL") == 0)
        {
            return  PS6000_EXTERNAL;
        }
        if ( enumerator.compare("PS6000_TRIGGER_AUX") == 0)
        {
            return  PS6000_TRIGGER_AUX;
        }
        
        throw PicoException("Wrong channel input!");
        

        return output;
    };


    ///////////////////////////////////////////////////////////////////////////


    PS6000_THRESHOLD_DIRECTION  Pico_StringToEnum_thresDir(std::string& enumerator )
    {

        PS6000_THRESHOLD_DIRECTION  output;

        if ( enumerator.compare("PS6000_RISING") == 0 )
        {
            return  PS6000_RISING;
        }
        if ( enumerator.compare("PS6000_FALLING") == 0 )
        {
            return  PS6000_FALLING;
        }
        
        throw PicoException("Wrong threshold direction input!");

        return output;
    };





    ///////////////////////////////////////////////////////////////////////////





    Utility::Pico_RunMode Pico_StringToEnum_runMode( std::string& enumerator )
    {
        Utility::Pico_RunMode   output;

        if( enumerator.compare("MERKEL_HG") == 0 )
        {
            return Utility::MERKEL_HG;
        }
        if( enumerator.compare("OBERMAIER_HG") == 0 )
        {
            return Utility::OBERMAIER_HG;
        }
        if( enumerator.compare("SCHIFFER_LG") == 0 )
        {
            return Utility::SCHIFFER_LG;
        }
        if( enumerator.compare("KLUM_LG") == 0 )
        {
            return Utility::KLUM_LG;
        }
        if( enumerator.compare("GARRN") == 0 )
        {
            return Utility::GARRN;
        }
        if( enumerator.compare("INTERMEDIATE") == 0 )
        {
            return Utility::INTERMEDIATE;
        }

        throw PicoException("Wrong run mode as input!");

        return output;
    };





    ///////////////////////////////////////////////////////////////////////////






    Utility::Pico_Trigger_Mode  Pico_StringToEnum_trigger( std::string& enumerator )
    {
        Utility::Pico_Trigger_Mode output;

        if (enumerator.compare("TRIGGER_SIMPLE") == 0 )
        {
            return Utility::TRIGGER_SIMPLE;
        }
        if (enumerator.compare("TRIGGER_ADVANCED") == 0 )
        {
            return Utility::TRIGGER_ADVANCED;
        }

        throw PicoException("Wrong trigger mode as input!");

        return output;

    }






    ///////////////////////////////////////////////////////////////////////////
    //
    //                          END 
    //
    //                  Pico_StringToEnum functions
    //
    ///////////////////////////////////////////////////////////////////////////
    









    ///////////////////////////////////////////////////////////////////////////
    //                  Pico_EnumToString functions
    //
    // The following functions are needed for readbility of std::cout and
    // saving of the ini files.
    //
    ///////////////////////////////////////////////////////////////////////////


    //! Enum to string for PS6000_COUPLING
    std::string     Pico_EnumToString_coupling( PS6000_COUPLING& var )
    {
        std::string output;
        switch( var )
        {
            case 0:
                output = "PS6000_AC";
                break;
            case 1:
                output = "PS6000_DC_1M";
                break;
            case 2:
                output = "PS6000_DC_50R";
                break;
            default:
                throw UtilityException("Wrong PS6000_COUPLING entered. Can't convert.");
        }
        
        return output;
    }





    ///////////////////////////////////////////////////////////////////////////





    //! Enum to string for PS6000_RANGE 
    std::string     Pico_EnumToString_range( PS6000_RANGE& var )
    {
        std::string output;
        switch( var )
        {
            case 0:
                output = "PS6000_10MV";
                break;
            case 1:
                output = "PS6000_20MV";
                break;
            case 2:
                output = "PS6000_50MV";
                break;
            case 3:
                output = "PS6000_100MV";
                break;
            case 4:
                output = "PS6000_200MV";
                break;
            case 5:
                output = "PS6000_500MV";
                break;
            case 6:
                output = "PS6000_1V";
                break;
            case 7:
                output = "PS6000_2V";
                break;
            case 8:
                output = "PS6000_5V";
                break;
            case 9:
                output = "PS6000_10V";
                break;
            case 10:
                output = "PS6000_50V";
                break;
            default:
                throw UtilityException("Wrong PS6000_RANGE entered. Can't convert.");
        }

        return output;
    }





    ///////////////////////////////////////////////////////////////////////////






    //! Enum to string for PS6000_BANDWIDTH_LIMITER_
    std::string     Pico_EnumToString_bandwidth( PS6000_BANDWIDTH_LIMITER& var )
    {
        std::string output;
        switch( var )
        {
            case 0:
                output = "PS6000_BW_FULL";
                break;
            case 1:
                output = "PS6000_BW_20MHZ";
                break;
            case 2:
                output = "PS6000_BW_25MHZ";
                break;
            default:
                throw UtilityException("Wrong PS6000_BANDWIDTH_LIMITER entered. Can't convert.");
        }

        return output;
    }





    ///////////////////////////////////////////////////////////////////////////






    //! Enum to string for PS6000_RATIO_MODE
    std::string     Pico_EnumToString_ratio( PS6000_RATIO_MODE& var )
    {
        std::string output;
        switch( var )
        {
            case 0:
                output = "PS6000_RATIO_MODE_NONE";
                break;
            case 1:
                output = "PS6000_RATIO_MODE_AGGREGATE";
                break;
            case 2:
                output = "PS6000_RATIO_MODE_AVERAGE";
                break;
            case 4:
                output = "PS6000_RATIO_MODE_DECIMATE";
                break;
            case 8:
                output = "PS6000_RATIO_MODE_DISTRIBUTION";
                break;
            default:
                throw UtilityException("Wrong PS6000_RATIO_MODE entered. Can't convert.");
        }

        return output;
    }





    ///////////////////////////////////////////////////////////////////////////






    //! Enum to string for PS6000_CHANNEL
    std::string     Pico_EnumToString_channel( PS6000_CHANNEL& var )
    {
        std::string output;
        switch( var ) 
        {
            case 0:
                output = "PS6000_CHANNEL_A";
                break;
            case 1:
                output = "PS6000_CHANNEL_B";
                break;
            case 2:
                output = "PS6000_CHANNEL_C";
                break;
            case 3:
                output = "PS6000_CHANNEL_D";
                break;
            case 4:
                output = "PS6000_EXTERNAL";
                break;
            case 5:
                output = "PS6000_MAX_CHANNELS";
                break;
            case 6:
                output = "PS6000_TRIGGER_AUX";
                break;
            default:
                throw UtilityException("Wrong PS6000_CHANNEL entered. Can't convert.");
        }

        return output;
    }





    ///////////////////////////////////////////////////////////////////////////






    //! Enum to string for PS6000_THRESHOLD_DIRECTION
    std::string     Pico_EnumToString_thresDir( PS6000_THRESHOLD_DIRECTION& var )
    {
        std::string output;
        switch( var )
        {
            case 0:
                output = "PS6000_ABOVE";
                break;
            case 1:
                output = "PS6000_BELOW";
                break;
            case 2:
                output = "PS6000_RISING";
                break;
            case 3:
                output = "PS6000_FALLING";
                break;
            case 4:
                output = "PS6000_RISING_OR_FALLING";
                break;
            case 5:
                output = "PS6000_ABOVE_LOWER";
                break;
            case 6:
                output = "PS6000_BELOW_LOWER";
                break;
            case 7:
                output = "PS6000_RISING_LOWER";
                break;
            case 8:
                output = "PS6000_FALLING_LOWER";
                break;
            case 9:
                output = "PS6000_POSITIVE_RUNT";
                break;
            case 10:
                output = "PS6000_NEGATIVE_RUNT";
                break;
            default:
                throw UtilityException("Wrong PS6000_THRESHOLD_DIRECTION entered. Can't convert.");
        }

        return output;
    }





    ///////////////////////////////////////////////////////////////////////////






    std::string     Pico_EnumToString_runMode( Utility::Pico_RunMode& mode )
    {
        std::string output;
        switch( mode )
        {
            case 0:
                output = "INTERMEDIATE";
                break;
            case 1:
                output = "MERKEL_HG";
                break;
            case 2:
                output = "OBERMAIER_HG";
                break;
            case 3:
                output = "SCHIFFER_LG";
                break;
            case 4:
                output = "KLUM_LG";
                break;
            case 5:
                output = "GARRN";
                break;
            default:
                throw UtilityException("Wrong Utility::Pico_RunMode enum entered!");
        }

        return output;
    }




    ///////////////////////////////////////////////////////////////////////////





    std::string     Pico_EnumToString_trigger( Utility::Pico_Trigger_Mode& mode )
    {
        std::string output;
        switch( mode )
        {
            case 0:
                output = "TRIGGER_SIMPLE";
                break;
            case 1:
                output = "TRIGGER_ADVANCED";
                break;
            default:
                throw UtilityException("Wrong Utility::Pico_Trigger_Mode entered!");
        }

        return output;
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    //                  END Pico_EnumToString functions
    ///////////////////////////////////////////////////////////////////////////






    ///////////////////////////////////////////////////////////////////////////
    //
    //
    //                  END Pico related stuff! 
    //
    //
    ///////////////////////////////////////////////////////////////////////////




































    ///////////////////////////////////////////////////////////////////////////////
    //
    //                  START General Data Structure 
    //
    ///////////////////////////////////////////////////////////////////////////////
    
    
    //! Overloaded operator<< for Steering_Data.
    std::ostream& operator<<( std::ostream& out, Utility::Steering_Data& data )
    {

        try
        {
                out << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
                out << "\n\t\t\tSteering Data\n";
                out << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
                out << "\t\tHighGain: " << Pico_EnumToString_runMode( data.runMode_HighGain );
                out << "\t\tLowGain: " << Pico_EnumToString_runMode( data.runMode_LowGain );
                out << "\nLoopsPhysics: " << data.loopsPhysics;
                out << "\t\t\tLoopsInter: " << data.loopsIntermediate;
                out << "\t\tSavePath#1: " << data.savePath_1;
                out << "\nSavePath#2: " << data.savePath_2;
                out << "\t\tSavePath#3: " << data.savePath_3;
                out << "\n";

        }
        catch( UtilityException& excep )
        {
            std::cout << excep.what() << std::endl;
        }

        return out;
    }
    
    
    
    
    
    
    
    
    
    
    ///////////////////////////////////////////////////////////////////////////
    //
    //                  END General Data Structure 
    //
    ///////////////////////////////////////////////////////////////////////////





}
