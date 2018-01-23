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
#include <memory>

//#include "clawsRun.h"

#include <chrono>
#include <thread>
#include <libps6000-1.4/PicoStatus.h>
#include <libps6000-1.4/ps6000Api.h>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/filesystem.hpp>

#include <TROOT.h>


int main()
{


    ROOT::EnableThreadSafety();
//    gROOT->ProcessLine("gErrorIgnoreLevel = 6000");

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


/* 
 * #include <thread>
 * #include <string>
 * #include <vector>
 * #include <chrono>
 * #include "cadef.h"
 * 
 * #include <map>
 * #include <memory>
 * #include <iostream>
 * 
 * int main()
 * {
 * 
 *     std::vector< std::string > tmpVars;
 *     tmpVars.push_back("RATE:1PEVSTOTAL:TOP_BACKWARD_1");
 *     tmpVars.push_back("RATE:1PEVSTOTAL:TOP_BACKWARD_2");
 *     tmpVars.push_back("RATE:1PEVSTOTAL:TOP_BACKWARD_3");
 *     tmpVars.push_back("RATE:1PEVSTOTAL:TOP_BACKWARD_4");
 * 
 * 
 *     std::vector< chid > ids{tmpVars.size()};
 * 
 *     // create epics client instance
 *     SEVCHK(ca_context_create(
 *                 ca_disable_preemptive_callback),"ca_context_create");
 * 
 *     // prefix used by IOC
 *     std::string prefix{"BEAST:CLAWS:"};
 * 
 *     // fill data structure holding the epics chids and push it to IOC
 *     std::shared_ptr< std::vector<std::pair< std::string, chid>>> epicsVars{std::make_shared<std::vector<std::pair<std::string,chid>>>()};
 *     for( unsigned int i = 0; i < tmpVars.size(); ++i )
 *     {
 *         epicsVars->push_back( std::make_pair(tmpVars.at(i), ids.at(i)) );
 *         SEVCHK(ca_create_channel(
 *                     std::string(prefix + epicsVars->at(i).first).c_str(),
 *                     NULL,
 *                     NULL,
 *                     10,
 *                     &epicsVars->at(i).second),
 *                 "ca_create_channel failure");
 *         SEVCHK(ca_pend_io(5.0),"ca_pend_io failure");
 * 
 *         std::this_thread::sleep_for(std::chrono::milliseconds(10));
 *     }
 * 
 * 
 * 
 *     std::string epics_root_rate{"RATE:1PEVSTOTAL:TOP_BACKWARD_" + std::to_string(1)};
 * 
 *     double cal_1peVStotal_fraction{0.1};
 *     if( !epicsVars ) 
 *     {
 *         std::cout << "is Empty\n";
 *         return 1;
 *     }
 * 
 *     for( int i = 0; i < 10; ++i )
 *     {
 *         for( auto& tmp : *epicsVars ) 
 *         {
 *             if( tmp.first.find(epics_root_rate) != std::string::npos )
 *             {
 *                 std::cout << tmp.first << "\tEpics starts: " << &tmp.second << std::endl;
 *                 SEVCHK(ca_put(DBR_DOUBLE,tmp.second,
 *                             &cal_1peVStotal_fraction),
 * //                        &dfraction),
 *                         "ca_set failure");
 *                 SEVCHK(ca_pend_io(5.0),"ca_pend_io failure");
 * 
 * //                std::this_thread::sleep_for(std::chrono::seconds(1));
 *                 break;
 *             }
 *         }
 * 
 *         cal_1peVStotal_fraction += 0.05;
 *         std::this_thread::sleep_for(std::chrono::seconds(1));
 *     }
 *     for( int i = 0; i<10; ++i )
 *     {
 * 
 *     }
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 *     return 0;
 * }
 */
