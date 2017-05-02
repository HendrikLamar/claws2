// =====================================================================================
// 
//       Filename:  main.cpp
// 
//    Description:  Main File for the CLAWS DAQ2 
// 
//        Version:  1.0
//        Created:  25.04.2017 16:51:44
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================


#include "pico.h"
#include <iostream>


int main(){

    std::cout << "Welcome to the CLAWS DAQ2!\n\n";

    int16_t handle;

    // initialize pico
    Pico pico1(handle);
    
    // set channel
    pico1.changeCh(PS6000_CHANNEL_A).setChEnabled(true);
    pico1.printCh();
    pico1.changeCh(PS6000_CHANNEL_B).setChEnabled(true);
    pico1.printCh();
    pico1.changeCh(PS6000_CHANNEL_C).setChEnabled(true);
    pico1.printCh();
    pico1.changeCh(PS6000_CHANNEL_D).setChEnabled(false);
    pico1.printCh();

    uint32_t    timebase{2};
    uint32_t    noSamples{1000};
    float       timeInterval{-1.f};
    uint32_t    maxSamples;

    uint32_t    counter{0};
    while(timeInterval < 0.f){
        ++counter;
        pico1.getTimebase(timebase, noSamples, &timeInterval, &maxSamples);
    }
    std::cout << "\nThe chosen time interval is: " << timeInterval << "\n";
    std::cout << "The number of samples we can collect: " << maxSamples << "\n\n";
    std::cout << "Get timebase took " << counter << " loops to get the right value!"\
        << "\n";

    pico1.setSimpleTrigger(true, PS6000_TRIGGER_AUX, -1600, PS6000_FALLING, 0, 1000);

    std::cout << "Okay, closing unit now!\n";
    pico1.closeUnit();
    std::cout << "Pico closed!" << std::endl;

}

