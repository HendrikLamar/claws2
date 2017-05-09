// =====================================================================================
// 
//       Filename:  claws.cxx
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08.05.2017 15:19:03
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include "pico.h"
#include "utility.h"

namespace Claws {
    
    void runPhysics(Pico *picoscope){
        
        // load Physics run INI-File
        //! \todo Implement loading ini-Files!

        picoscope->configureChannels();
        picoscope->getTimebase();
        
        picoscope->configureTrigger(Utility::CLAWS_TRIGGER_SIMPLE);

        picoscope->runBlock();
        picoscope->getValues();

        //! \todo Only analyse the data here!

        //! \todo Save data here!
                
            
    }
}
