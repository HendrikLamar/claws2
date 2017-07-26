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



#include "statemachine.h"
#include "database.h"

#include <thread>


int main()
{

    MyState mystate;
    mystate.doMenu();

//    mystate.doMenu();

//    std::thread mythread(&MyState::doMenu, mystate);

//    mythread.join();

    return 0;
}
