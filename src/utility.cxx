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
    bool pReady = false;
    // I am not sure whats going on here.
    void CallBackBlock(
            int16_t         handle,          \
            PICO_STATUS     t_status,        \
            void            *pParameter){

        if(handle){};       // dirty: don't want 'variable unused' warning
        if(pParameter){};


        if(t_status != PICO_CANCELLED){
            pReady = true;
            std::cout << "Pico is done with aquiring!" << std::endl;
        }
    };
    ////////////////////////////////////////////////////////////////////////

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
    
}
