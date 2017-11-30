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

    uint32_t nSegments = 200;
    uint32_t nCaptures = nSegments;

    uint32_t preTrigger = 20;
    uint32_t postTrigger = 20;
    uint32_t nSamples = preTrigger + postTrigger;

    uint32_t nLoops = nSegments;

    PICO_STATUS pstatus = PICO_OK;

    int16_t handle{0};

    std::cout << "pico init...\n";
    pstatus = ps6000OpenUnit(&handle, nullptr);

    if( pstatus != PICO_OK) std::cout << "Error: " << pstatus << "\n";


    std::cout << "setChannel...\n";
    pstatus = ps6000SetChannel(
            handle,
            PS6000_CHANNEL_A,
            1,
            PS6000_DC_50R,
            PS6000_50MV,
            0,
            PS6000_BW_FULL);

    if( pstatus != PICO_OK) std::cout << "Error: " << pstatus << "\n";

    std::cout << "setChannel...\n";
    pstatus = ps6000SetChannel(
            handle,
            PS6000_CHANNEL_B,
            1,
            PS6000_DC_50R,
            PS6000_50MV,
            0,
            PS6000_BW_FULL);

    if( pstatus != PICO_OK) std::cout << "Error: " << pstatus << "\n";

    std::cout << "setChannel...\n";
    pstatus = ps6000SetChannel(
            handle,
            PS6000_CHANNEL_C,
            1,
            PS6000_DC_50R,
            PS6000_50MV,
            0,
            PS6000_BW_FULL);

    if( pstatus != PICO_OK) std::cout << "Error: " << pstatus << "\n";

    std::cout << "setChannel...\n";
    pstatus = ps6000SetChannel(
            handle,
            PS6000_CHANNEL_D,
            1,
            PS6000_DC_50R,
            PS6000_50MV,
            0,
            PS6000_BW_FULL);

    if( pstatus != PICO_OK) std::cout << "Error: " << pstatus << "\n";



    std::cout << "memorySegments...\n";
    uint32_t nMaxSamples{0};
    pstatus = ps6000MemorySegments(
            handle,
            nSegments,
            &nMaxSamples);


    if( pstatus != PICO_OK) std::cout << "Error: " << pstatus << "\n";

    std::cout << "setNoOfCaptures...\n";
    pstatus = ps6000SetNoOfCaptures(
            handle,
            nCaptures);


    if( pstatus != PICO_OK) std::cout << "Error: " << pstatus << "\n";


    std::cout << "getTimebase2...\n";
    unsigned int counter{0};
    unsigned int counterMax{5000};
    float timeInterval_ns{-1};
    uint32_t maxSamples{0};
    while( timeInterval_ns < 0 && counter < counterMax )
    {
        pstatus = ps6000GetTimebase2(
                        handle,
                        2,
                        nSamples,
                        &timeInterval_ns,
                        0,
                        &maxSamples,
                        0);



        if( pstatus != PICO_OK) 
        {
            std::cout << "Error: " << pstatus << "\n";
            return 1;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        ++counter;
    }

    std::cout << "The timeinterval is " << timeInterval_ns << "ns.\n" 
        << "MaxSamples is " << maxSamples << ".\n";

    std::cout << "setSimpleTrigger...\n";
    pstatus = ps6000SetSimpleTrigger(
            handle,
            1,
            PS6000_CHANNEL_A,
            5000,
            PS6000_FALLING,
            0,
            1000);

    if( pstatus != PICO_OK) std::cout << "Error: " << pstatus << "\n";


    std::shared_ptr<std::vector< std::shared_ptr< std::vector< int16_t>>>> data1
        = std::make_shared<std::vector< std::shared_ptr< std::vector<int16_t>>>>();

    for( uint32_t ii = 0; ii < nLoops; ++ii )
    {
        data1->push_back( std::make_shared< std::vector<int16_t>>(nSamples,0));
    }

    std::shared_ptr<std::vector< std::shared_ptr< std::vector< int16_t>>>> data2
        = std::make_shared<std::vector< std::shared_ptr< std::vector<int16_t>>>>();

    for( uint32_t ii = 0; ii < nLoops; ++ii )
    {
        data2->push_back( std::make_shared< std::vector<int16_t>>(nSamples,0));
    }

    std::shared_ptr<std::vector< std::shared_ptr< std::vector< int16_t>>>> data3
        = std::make_shared<std::vector< std::shared_ptr< std::vector<int16_t>>>>();

    for( uint32_t ii = 0; ii < nLoops; ++ii )
    {
        data3->push_back( std::make_shared< std::vector<int16_t>>(nSamples,0));
    }

    std::shared_ptr<std::vector< std::shared_ptr< std::vector< int16_t>>>> data4
        = std::make_shared<std::vector< std::shared_ptr< std::vector<int16_t>>>>();

    for( uint32_t ii = 0; ii < nLoops; ++ii )
    {
        data4->push_back( std::make_shared< std::vector<int16_t>>(nSamples,0));
    }
    std::cout << "setDataBufferBulk...\n";
    uint32_t waveformNo{0};
    for( auto& tmp : *data1 )
    {
        pstatus = ps6000SetDataBufferBulk(
                handle,
                PS6000_CHANNEL_A,
                &tmp->at(0),
                nSamples,
                waveformNo,
                PS6000_RATIO_MODE_NONE);

        if( pstatus != PICO_OK) 
        {
            std::cout << "Error: " << pstatus << "\n";
            return 1;
        }

        ++waveformNo;
    }

    waveformNo = 0;
    for( auto& tmp : *data2 )
    {
        pstatus = ps6000SetDataBufferBulk(
                handle,
                PS6000_CHANNEL_B,
                &tmp->at(0),
                nSamples,
                waveformNo,
                PS6000_RATIO_MODE_NONE);

        if( pstatus != PICO_OK) 
        {
            std::cout << "Error: " << pstatus << "\n";
            return 1;
        }

        ++waveformNo;
    }

    waveformNo = 0;
    for( auto& tmp : *data3 )
    {
        pstatus = ps6000SetDataBufferBulk(
                handle,
                PS6000_CHANNEL_C,
                &tmp->at(0),
                nSamples,
                waveformNo,
                PS6000_RATIO_MODE_NONE);

        if( pstatus != PICO_OK) 
        {
            std::cout << "Error: " << pstatus << "\n";
            return 1;
        }

        ++waveformNo;
    }
    waveformNo = 0;
    for( auto& tmp : *data4 )
    {
        pstatus = ps6000SetDataBufferBulk(
                handle,
                PS6000_CHANNEL_D,
                &tmp->at(0),
                nSamples,
                waveformNo,
                PS6000_RATIO_MODE_NONE);

        if( pstatus != PICO_OK) 
        {
            std::cout << "Error: " << pstatus << "\n";
            return 1;
        }

        ++waveformNo;
    }


    std::cout << "runBlock...\n";
    pstatus = ps6000RunBlock(
            handle,
            preTrigger,
            postTrigger,
            2,
            1,
            nullptr,
            0,
            nullptr,
            nullptr);

    if( pstatus != PICO_OK) 
    {
        std::cout << "Error: " << pstatus << "\n";
        return 1;
    }

    int16_t ready = 0;
    while( !ready )
    {
        pstatus = ps6000IsReady( handle, &ready );
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "getValues...\n";

    int16_t overflow[nSegments+1]{};
    uint32_t noOfSamplesReturned = nSamples;
    pstatus = ps6000GetValuesBulk(
            handle,
            &noOfSamplesReturned,
            0,
            nLoops-1,
            1,
            PS6000_RATIO_MODE_NONE,
            overflow);




    uint32_t tmpcounter{0};
    for( unsigned int i = 0; i< nLoops; ++i )
    {
        std::cout << "#" << tmpcounter << "\n" << std::flush;
        for( unsigned int t = 0; t< nSamples; ++t )
        {
            std::cout << data1->at(i)->at(t) << " " << std::flush; 
        }
        std::cout << "\n";
        for( unsigned int t = 0; t< nSamples; ++t )
        {
            std::cout << data2->at(i)->at(t) << " " << std::flush; 
        }
        std::cout << "\n";
        for( unsigned int t = 0; t< nSamples; ++t )
        {
            std::cout << data3->at(i)->at(t) << " " << std::flush; 
        }
        std::cout << "\n";
        for( unsigned int t = 0; t< nSamples; ++t )
        {
            std::cout << data4->at(i)->at(t) << " " << std::flush; 
        }
        std::cout << "\n\n";

        ++tmpcounter;
    }






/*     ROOT::EnableThreadSafety();
 * //    gROOT->ProcessLine("gErrorIgnoreLevel = 6000");
 * 
 *     MyState* mystate;
 * 
 *     try
 *     {
 *         mystate = new MyState();
 *         mystate->run();
 *     }
 *     catch(...)
 *     {
 *         std::cout << "Unknown error occured. Exiting..." << std::endl;
 *     }
 * 
 * 
 *     delete mystate;
 *     mystate = nullptr;
 * 
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
