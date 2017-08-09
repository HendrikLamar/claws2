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
    void CallBackBlock(
            int16_t         handle,          \
            PICO_STATUS     t_status,        \
            void            *pParameter);
    ////////////////////////////////////////////////////////////////////////

    enum CLAWS_TRIGGER_MODE{
        
        CLAWS_TRIGGER_SIMPLE,
        CLAWS_TRIGGER_ADVANCED,
    };
    

    //! Struct for a N6700 channel.
    //! This might be useful in later version of the code where a change without
    //! ini-file editing is wanted.
    struct K_Channel 
    {
        std::string         volt;
        std::string         curr;
        std::string         curr_limit;
        std::string         active;
    };

}
#endif //UTILITY_H
