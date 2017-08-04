// =====================================================================================
// 
//       Filename:  main.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  24.07.2017 15:33:05
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================


#include "statechart.h"
#include <iostream>
#include <thread>
#include <chrono>


int main()
{
    ClawsStatemachine::ClawsDAQ* daq = new ClawsStatemachine::ClawsDAQ();
    daq->initiate();


    std::cout << "1<CR> = StartStop\n";
    std::cout << "c<CR> = Config\n";
    std::cout << "s<CR> = Status\n";
    std::cout << "q<CR> = Exit\n";

    char key = daq->getKey();

    while ( true )
    {
        switch ( key )
        {
            case '1':
                daq->process_event( ClawsStatemachine::EvStartStop() );
                break;
            case 'c':
                daq->process_event( ClawsStatemachine::EvLoadConfig() );
                break;
            case 's':
                daq->process_event( ClawsStatemachine::EvStatus() );
                break;
            case 'q':
                daq->process_event( ClawsStatemachine::EvQuit() );
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                daq->process_event( ClawsStatemachine::EvQuit() );
                break;
            default:
                std::cout << "Unknown command. Try h<CR>!\n";
        };
        
        if ( daq->getDatabase()->getStop() )
        {
            break;
        }
        else key = daq->getKey();

    };

    delete daq;
    daq = nullptr;

    return 0;
};
