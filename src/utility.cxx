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
#include <memory>
#include <vector>
#include <string>

#include <libps6000-1.4/ps6000Api.h>
#include <libps6000-1.4/PicoStatus.h>

namespace Utility{




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

        return;
    };









///////////////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////////
    //                  START Hist Structures
    //

    
    ///////////////////////////////////////////////////////////////////////////
    //              Start Pico_Hist_Channel

    Pico_Hist_Channel::Pico_Hist_Channel( PS6000_CHANNEL cha )
    {
        channel = cha;
    };
    Pico_Hist_Channel::Pico_Hist_Channel( int cha )
    {
        channel = Utility::Pico_intToEnum_channel(cha);
    };
    Pico_Hist_Channel::Pico_Hist_Channel( PS6000_CHANNEL cha, std::shared_ptr<TH1I> hist )
    {
        set( cha, hist );
    };
    Pico_Hist_Channel::Pico_Hist_Channel( int cha, std::shared_ptr<TH1I> hist )
    {
        set( cha, hist );
    }
    Pico_Hist_Channel::~Pico_Hist_Channel()
    {};


    void    Pico_Hist_Channel::set( std::shared_ptr<TH1I> hist )
    {
        data = hist;

        return;
    }

    std::shared_ptr<TH1I>    Pico_Hist_Channel::get()
    {
        return data;
    }


    PS6000_CHANNEL Pico_Hist_Channel::getID()
    {
        return channel;
    }



    void    Pico_Hist_Channel::set( int cha, std::shared_ptr<TH1I> hist )
    {
        
        channel = Utility::Pico_intToEnum_channel(cha);
        data = hist;
        
        return;
    }

    //              END Pico_Hist_Channel
    ///////////////////////////////////////////////////////////////////////////





























    ///////////////////////////////////////////////////////////////////////////
    //              START Pico_Hist_Pico


    Pico_Hist_Pico::Pico_Hist_Pico( std::string loc ) :
        location( loc )
    {
        data = std::make_shared< std::vector< std::shared_ptr<
            Pico_Hist_Channel> > >();
    }

    void Pico_Hist_Pico::add( std::shared_ptr<Pico_Hist_Channel> hist )
    {
        bool isUnique{ true };
        for( std::shared_ptr<Pico_Hist_Channel> tmp : *data )
        {
            if( tmp->getID() == hist->getID() )
            {
                isUnique = false;
                break;
            }

        }

        if( isUnique )
        {
            data->push_back(hist);
        }

        return;

    }

    std::shared_ptr<TH1I>  Pico_Hist_Pico::get( PS6000_CHANNEL ch )
    {
        std::shared_ptr<TH1I> tdata = nullptr;
        for( std::shared_ptr<Pico_Hist_Channel> tmp : *data )
        {
            if( tmp->getID() == ch )
            {
                tdata = tmp->get();
                break;
            }
        }


        return tdata;

    }

    std::shared_ptr<TH1I>  Pico_Hist_Pico::get( int ch )
    {
        PS6000_CHANNEL tch = Utility::Pico_intToEnum_channel( ch );
        std::shared_ptr<TH1I> tdata;
        for( auto& tmp : *data )
        {
            if( tmp->getID() == tch )
            {
                tdata = tmp->get();
                break;
            }
        }

        return tdata;

    }

    std::string     Pico_Hist_Pico::getLoc()
    {
        return location;
    }


    int Pico_Hist_Pico::getSize()
    {
        return data->size();
    }




    //              END Pico_Hist_Pico
    ///////////////////////////////////////////////////////////////////////////
    











    ///////////////////////////////////////////////////////////////////////////
    //              START Pico_Data_Analyzed_Channel




    PS6000_CHANNEL Pico_Data_Analyzed_Channel::getID()
    {
        return channel;
    }



    //              END Pico_Data_Analyzed_Channel
    ///////////////////////////////////////////////////////////////////////////









    ///////////////////////////////////////////////////////////////////////////
    //              START Pico_Data_Analyzed_Pico




    std::shared_ptr< Pico_Data_Analyzed_Channel > 
        Pico_Data_Analyzed_Pico::get( PS6000_CHANNEL cha )
    {
        for( auto& tmp : *data )
        {
            if( tmp->getID() == cha )
            {
                return tmp;
            }
        }

        return nullptr;
    
    }



///////////////////////////////////////////////////////////////////////////////




    std::shared_ptr< Pico_Data_Analyzed_Channel > 
        Pico_Data_Analyzed_Pico::get( int cha )
    {
        for( auto& tmp : *data )
        {
            if( tmp->getID() == Utility::Pico_intToEnum_channel(cha) )
            {
                return tmp;
            }
        }

        return nullptr;
    
    }

    //              END Pico_Data_Analyzed_Pico
    ///////////////////////////////////////////////////////////////////////////

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






    std::string Pico_preInitializer( int16_t* handle )
    {
//        PICO_STATUS tmp_status;
        std::string output = "";

        // first, open random pico
//        tmp_status = ps6000OpenUnit(handle, nullptr);
        ps6000OpenUnit(handle, nullptr);

        switch( *handle )
        {
            case -1:
                throw PicoException("Pico fails to open!");
                break;
            case 0:
                throw PicoException(" ");
                break;
            default:
                break;
        }


        // second, get serial of found pico and return
        int16_t stringL = 9;            // expected length of output
        int8_t pstring[stringL + 1];    // on exit, serial is written here
                                        // the length is #expectedCharacters plus
                                        // a ending character
        int16_t stringR;                // length of output array
        PICO_INFO info = 4;             // demandend info (4 = serial)

//        tmp_status = ps6000GetUnitInfo(*handle, pstring, stringL, &stringR, info);
        ps6000GetUnitInfo(*handle, pstring, stringL, &stringR, info);

        for( int tt = 0; tt < stringR-1; ++tt )
        {
            output += pstring[tt];
        }

        return output;

    }








///////////////////////////////////////////////////////////////////////////////







    void    Pico_preClose( int16_t handle )
    {
        ps6000CloseUnit(handle);

        return;
    }










///////////////////////////////////////////////////////////////////////////////




/*     std::string coutColor( std::string text, std::string color, std::string other)
 *     {
 *         std::string output;
 * 
 *         if( !color.compare("red") )
 *         {
 *             output = "\033[1;31mbold"
 *         }
 *     }
 */










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








    Utility::Pico_Trigger_Mode  Pico_StringToEnum_trigger( std::string& enumerator )
    {
        Utility::Pico_Trigger_Mode output;

        if (enumerator.compare("TRIGGER_SIMPLE") == 0 )
        {
            return Utility::Pico_Trigger_Mode::TRIGGER_SIMPLE;
        }
        if (enumerator.compare("TRIGGER_ADVANCED") == 0 )
        {
            return Utility::Pico_Trigger_Mode::TRIGGER_ADVANCED;
        }

        throw PicoException("Wrong trigger mode as input!");

        return output;

    }







///////////////////////////////////////////////////////////////////////////////




    Utility::Collection_Mode Pico_StringToEnum_collection( std::string& enumerator )
    {
        Utility::Collection_Mode output;

        if( enumerator.compare("BLOCK") == 0 )
        {
            return Utility::Collection_Mode::BLOCK;
        }
        if( enumerator.compare("RAPID") == 0 )
        {
            return Utility::Collection_Mode::RAPID;
        }

        throw PicoException("Wrong collection mode as input!");

        return output;
    }







///////////////////////////////////////////////////////////////////////////////





    PS6000_CHANNEL Pico_intToEnum_channel( int cha )
    {
        switch( cha )
        {
            case 0:
                return PS6000_CHANNEL_A;
            case 1:
                return PS6000_CHANNEL_B;
            case 2:
                return PS6000_CHANNEL_C;
            case 3:
                return PS6000_CHANNEL_D;
            default:
                throw PicoException("No known channel conversion available!");
        }

        return PS6000_CHANNEL_A;
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
    std::string     Pico_EnumToString_channel( PS6000_CHANNEL var )
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
        switch( static_cast<int>(mode) )
        {
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
        switch( static_cast<int>(mode) )
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







///////////////////////////////////////////////////////////////////////////////





    std::string     Pico_EnumToString_collection( Utility::Collection_Mode& mode )
    {
        std::string output;
        switch( static_cast<int>(mode) )
        {
            case 0:
                output = "BLOCK";
                break;
            case 1:
                output = "RAPID";
                break;
            default:
                throw UtilityException("Wrong Utility::Collection_Mode entered!");
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
    
    
    
    
    
    
    
    
    
    
    ///////////////////////////////////////////////////////////////////////////
    //
    //                  END General Data Structure 
    //
    ///////////////////////////////////////////////////////////////////////////





}
