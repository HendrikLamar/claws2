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

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>


int main()
{

    MyState* mystate;

    try
    {
        mystate = new MyState();
        mystate->run();
    }
    catch(...)
    {
        std::cout << "Unknown error occured. Exiting..." << std::endl;
    }


    delete mystate;
    mystate = nullptr;
    

    return 0;
};
