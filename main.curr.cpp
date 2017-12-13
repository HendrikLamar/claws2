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


    
/*     unsigned int starter = 0;
 *     unsigned int ender = 0;
 *     unsigned int stepSize = 4;
 * 
 *     ender += stepSize;
 *     std::vector< int > w1{0,1,2,3,4,5,6,7,8,9,10};
 *     std::vector< int > w2{w1.begin()+starter, w1.begin()+ender};
 *     starter = ender;
 *     ender += stepSize;
 * 
 *     std::vector< int > w3;
 *     if( ender > w1.size() )
 *     {
 *         std::vector< int > dummy{w1.begin()+starter, w1.end()};
 *         w3 = dummy;
 *     }
 *     else 
 *     {
 *         std::vector< int > dummy{w1.begin()+starter, w1.begin()+ender};
 *         w3 = dummy;
 *     };
 * 
 *     std::cout << "w1: " << std::flush;
 *     for( auto& tmp : w1 )
 *     {
 *         std::cout << tmp << " ";
 *     }
 *     std::cout << "\n";
 * 
 *     std::cout << "w2: " << std::flush; 
 *     for( auto& tmp : w2 )
 *     {
 *         std::cout << tmp << " ";
 *     }
 *     std::cout << "\n";
 * 
 *     std::cout << "w3: " << std::flush;
 *     for( auto& tmp : w3 )
 *     {
 *         std::cout << tmp << " ";
 *     }
 *     std::cout << "\n";
 */



/*     unsigned int loop = 5;
 *     unsigned int samples = 5;
 * 
 *     std::shared_ptr< std::vector< std::shared_ptr< 
 *         std::vector< int16_t >>>> m_buffer_inter_data;
 * 
 *     m_buffer_inter_data = std::make_shared< std::vector<
 *         std::shared_ptr< std::vector< int16_t>>>>();
 * //    m_buffer_inter_data->reserve(loop);
 *     m_buffer_inter_data->resize(
 *             loop, 
 *             std::make_shared< std::vector< int16_t >>(samples+3, 2));
 * 
 * //    for( unsigned int i = 0; i < loop; ++i )
 * //    {
 * //        std::shared_ptr< std::vector< int16_t >> tmp{
 * //            std::make_shared< std::vector< int16_t>>()};
 * //        tmp->reserve(samples);
 * //        tmp->resize(samples, 0);
 * //        m_buffer_inter_data->push_back(tmp);
 * //    }
 * 
 *     std::cout << "Buffer capacity: " << m_buffer_inter_data->capacity() << std::endl;
 *     std::cout << "Buffer Size: " << m_buffer_inter_data->size() << "\n" << std::endl;
 * 
 * //    for( auto& tmp1 : *m_buffer_inter_data )
 * //    {
 * //        for( auto& tmp2 : *tmp1 )
 * //        {
 * //            tmp2 = 2;
 * //        }
 * //    }
 * 
 *     std::cout << "Buffer capacity: " << m_buffer_inter_data->capacity() << std::endl;
 *     std::cout << "Buffer Size: " << m_buffer_inter_data->size() << "\n" << std::endl;
 * 
 *     m_buffer_inter_data->resize(
 *             loop+1, 
 *             std::make_shared< std::vector< int16_t >>(samples, 0));
 *     m_buffer_inter_data->shrink_to_fit();
 * 
 *     std::cout << "Buffer capacity: " << m_buffer_inter_data->capacity() << std::endl;
 *     std::cout << "Buffer Size: " << m_buffer_inter_data->at(0)->size() << std::endl;
 * 
 *     int count1{0};
 *     int count2{0};
 *     for( auto& tmp1 : *m_buffer_inter_data )
 *     {
 * //        for( int16_t& tmp2 : *tmp1 )
 *         for( unsigned int i = 0; i < tmp1->size(); ++i )
 *         {
 *             int16_t tmp2 = tmp1->at(i);
 *             std::cout << count1 << ":" << count2 << " -> " << tmp2 <<  "\n";
 *             ++count2;
 *         }
 *         count2 = 0;
 *         ++count1;
 *     }
 */


/*     boost::gregorian::date current_date(boost::gregorian::day_clock::local_day());
 * 
 *     std::cout << to_iso_extended_string(current_date) << std::endl;
 * 
 *     boost::filesystem::path dir{boost::filesystem::canonical(
 *             boost::filesystem::current_path())};
 * 
 *     std::cout << dir.string() << std::endl;
 * 
 *     dir += boost::filesystem::path("/abcd/efz");
 * 
 *     if( boost::filesystem::create_directories(dir))
 *     {
 *         std::cout << "success!\n";
 *     }
 * 
 *     std::cout << sizeof(unsigned long) << std::endl;
 * 
 * 
 * 
 * 
 * 
 *     std::string m_location_save{"/mnt/claws_disk_7"};
 *     unsigned long runNum{4001443};
 *     std::string tsubdir{"/int"};
 * 
 *     // get the current date in YYYY-MM-DD format
 *     std::string curDay{to_iso_extended_string(
 *             boost::gregorian::day_clock::local_day())};
 * 
 *     // create the final path
 *     std::string finalPathStr = 
 *         m_location_save +
 *         "/data/" +
 *         curDay +
 *         "/run-" +
 *         std::to_string(runNum) +
 *         "/raw" +
 *         tsubdir;
 * 
 *     boost::filesystem::path finalPath{finalPathStr};
 * 
 * 
 *     boost::filesystem::create_directories(finalPath);
 * 
 */
































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
 *     int8_t pstring[10];
 *     int16_t stringL = 9;
 *     int16_t stringR;
 *     PICO_INFO info = 4;
 * 
 *     pstatus = ps6000GetUnitInfo(handle_1, pstring, stringL, &stringR, info);
 *     std::cout << pstring << std::endl;
 *     std::cout << stringR << std::endl;
 * 
 *     std::string str_serial = "";
 *     for( int tt = 0; tt < stringR-1; ++tt )
 *     {
 *         str_serial += pstring[tt];
 *     }
 *     std::cout << "Transformed string: " << str_serial << std::endl;
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
