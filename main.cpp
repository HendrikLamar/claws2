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

//#include "clawsRun.h"

#include <chrono>
#include <thread>
#include <libps6000-1.4/PicoStatus.h>
#include <libps6000-1.4/ps6000Api.h>

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


/* 
 *     PICO_STATUS pstatus;
 * 
 * //    int16_t counts;
 * //    int8_t  serials[100];
 * //    int16_t serialL;
 * //
 * //    pstatus = ps6000EnumerateUnits(&counts, serials, &serialL);
 * //
 * //    std::cout << "Counts: " << counts;
 * //    std::cout << "\tSerials: " << serials;
 * //    std::cout << "\tlength: " << serialL << std::endl;
 * 
 * 
 * 
 * 
 *     int16_t handle_1 = 0;
 *     int16_t handle_2 = 0;
 * 
 *     // 6404D
 * //    int8_t serial_1[]{'E','Q','2','3','2','/','0','0','4','\0'};
 * //    int8_t serial_2[]{'D','V','0','2','7','/','0','4','3','\0'};
 * //    int8_t serial_2[]{'C','X','7','4','3','/','0','2','4','\0'};
 * 
 *     // 6403
 *     int8_t serial_1[]{'A', 'Y', '1', '6', '6', '/', '0', '4', '7','\0'};
 *     int8_t serial_2[]{'A', 'Y', '1', '6', '6', '/', '0', '3', '2','\0'};
 * 
 *     pstatus = ps6000OpenUnit(&handle_1, serial_1);
 * 
 *     std::cout << "Status: " << pstatus << "\t" << "Handle: " << handle_1 << 
 *         std::endl;
 * 
 *     int8_t pstring[100];
 *     int16_t stringL = 9;
 *     int16_t stringR;
 *     PICO_INFO info = 4;
 * 
 *     pstatus = ps6000GetUnitInfo(handle_1, pstring, stringL, &stringR, info);
 *     std::cout << pstring << std::endl;
 *     std::cout << stringR << std::endl;
 * 
 * //    pstatus = ps6000OpenUnit(&handle_2, serial_2);
 *     pstatus = ps6000OpenUnit(&handle_2, serial_2);
 * 
 *     std::cout << "status: " << pstatus << "\t" << "handle: " << handle_2 << 
 *         std::endl;
 * 
 *     pstatus = ps6000GetUnitInfo(handle_2, pstring, stringL, &stringR, info);
 *     std::cout << pstring << std::endl;
 *     std::cout << stringR << std::endl;
 * 
 * 
 * 
 *     pstatus = ps6000SetChannel(handle_1, PS6000_CHANNEL_A, true, PS6000_DC_50R, 
 *             PS6000_50MV, 0, PS6000_BW_FULL);
 *     pstatus = ps6000SetChannel(handle_1, PS6000_CHANNEL_B, true, PS6000_DC_50R, 
 *             PS6000_50MV, 0, PS6000_BW_FULL);
 *     pstatus = ps6000SetChannel(handle_1, PS6000_CHANNEL_C, true, PS6000_DC_50R, 
 *             PS6000_50MV, 0, PS6000_BW_FULL);
 *     pstatus = ps6000SetChannel(handle_1, PS6000_CHANNEL_D, true, PS6000_DC_50R, 
 *             PS6000_50MV, 0, PS6000_BW_FULL);
 * 
 *     float       timeInterval_ns = 0;
 *     uint32_t    maxSamples = 200;
 *     pstatus = ps6000GetTimebase2(handle_1, 2, 200, &timeInterval_ns, 0, &maxSamples,0);
 * 
 *     std::cout << "Time interval: " << timeInterval_ns << " ns\n";
 * 
 * 
 * 
 * //
 * //    pstatus = ps6000OpenUnit(&handle_3, serial_3);
 * //
 * //    std::cout << "status: " << pstatus << "\t" << "handle: " << handle_3 << 
 * //        std::endl;
 * 
 * 
 * 
 *     std::cout << "Exiting handle_1\n";
 *     ps6000CloseUnit(handle_1);
 * 
 *     std::cout << "Exiting handle_2\n";
 *     ps6000CloseUnit(handle_2);
 * //    ps6000CloseUnit(handle_3);
 * 
 * 
 */
    
    return 0;
};
