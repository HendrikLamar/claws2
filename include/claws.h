// =====================================================================================
// 
//       Filename:  claws.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08.05.2017 17:31:55
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#ifndef CLAWS_H
#define CLAWS_H

#include "pico.h"

//! Contains all the different claws running schemes.
// Saving data and pushing to epics is also included.
namespace Claws{

    //! This is the physics run mode. Data analyzation, saving and pushing to
    // epics is included here. 
    void runPhysics(Pico *picoscope);
}

#endif
