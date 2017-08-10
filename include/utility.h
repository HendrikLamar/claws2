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
    void CallBackBlock(
            int16_t         handle,          \
            PICO_STATUS     t_status,        \
            void            *pParameter);
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

}
#endif //UTILITY_H
