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
    //
    //                          END 
    //
    //                  Pico_StringToEnum functions
    //
    ///////////////////////////////////////////////////////////////////////////
    
}
