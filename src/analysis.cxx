// =====================================================================================
// 
//       Filename:  analysis.cxx
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  20.12.2017 22:30:45
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================


#include "analysis.h"
#include "clawsException.h"

#include <thread>
#include <chrono>
#include <numeric>
#include <algorithm>

#include <boost/algorithm/string.hpp>

///////////////////////////////////////////////////////////////////////////////
//
//                  START Con-Destructor

                //std::shared_ptr< std::vector<std::pair<std::string,chid>>> epicsVars = nullptr);


Analysis::Analysis(std::shared_ptr< std::vector<std::pair<std::string,chid>>> epicsVars) :
    m_epicsVars{epicsVars}
{}



Analysis::~Analysis()
{

}



//                  END Con-Destructor
//
///////////////////////////////////////////////////////////////////////////////


















///////////////////////////////////////////////////////////////////////////////
//                  START Public Member Functions
//



void Analysis::intermediate( std::shared_ptr<Pico> tpico )
{
    std::string pico_location{boost::to_upper_copy(tpico->getLocation())};

    double maxVal{
                tpico->getConfig()->average_1pe_height * 
                tpico->getConfigAnalysis()->inter_factor1pe};

    for( int i = 0; i < 4; ++i )
    {
        if( !tpico->getCh(i)->getEnabled() ) continue;
        // vector to store all the calculated integrals
        std::unique_ptr< std::vector< long> > integrals{
            new std::vector< long>()};

        // get channel i's data
        auto tmpdata = tpico->getCh(i)->getBufferRapid();

        // loop through each waveform and calculate
        double maxCount{static_cast<double>(tmpdata->size())};
        for( auto& tmp : *tmpdata )
        {
            // checks if it is an 1pe or higher
            // looks only in the second half of the waveform where the
            // waveform is expected
            if( maxVal > *(std::min_element(
                            tmp->begin()+tmp->size()/2.,tmp->end())) )
            {
//                std::cout << *(std::min_element(
//                                tmp->begin()+tmp->size()/2.,tmp->end()))
//                    << std::endl;
//                   
                --maxCount;

                continue;
            }

            integrals->push_back( 
                    std::accumulate(
                        tmp->begin()+tmp->size()/2.,
                        tmp->end(),
                        0L)
                    -
                    std::accumulate(
                            tmp->begin(),
                            tmp->begin()+tmp->size()/2.,
                            0L));
        }

        double mean{std::accumulate(integrals->begin(),integrals->end(),0.0)/
            static_cast<double>(integrals->size())};

        // lambda function to transform pico enum to value for normalization
        auto enumToDouble = [](PS6000_RANGE range)->double
        {
            switch( range )
            {
                case PS6000_50MV:
                    return 50.;
                case PS6000_100MV:
                    return 100.;
                case PS6000_200MV:
                    return 200.;
                case PS6000_500MV:
                    return 500.;
                case PS6000_1V:
                    return 1000.;
                case PS6000_2V:
                    return 2000.;
                case PS6000_5V:
                    return 5000.;
                default:
                    throw ProcessDataException("Inter range not covered");
            }
        };

        // normalize also by preamp factor 10
        mean /= (10. * enumToDouble(tpico->getConfig()->channels->at(i)->range));


        // save analyzed data 
        tpico->getAnalyzed()->get(i)->cal_1pe_normalized = mean;
        tpico->getAnalyzed()->get(i)->cal_1peVStotal_fraction =
                maxCount/static_cast<double>(tmpdata->size());




        // epics starts here
        std::string epics_1pe_ratio{"RATE:1PEVSTOTAL:" + 
            pico_location + "_" + std::to_string(i+1)};
/*         std::string epics_1pe_normalized{"RATE:1PEVSTOTAL:" + 
 *             pico_location + "_" + std::to_string(i+1)};
 *         int breakCounter{0};
 */


        if( !m_epicsVars ) 
        {
            std::cout << "is Empty\n";
            continue;
        }
        for( auto& tmp : *m_epicsVars ) 
        {
            if( tmp.first.find(epics_1pe_ratio) != std::string::npos )
            {
                ca_put(DBR_DOUBLE,tmp.second,
                        &tpico->getAnalyzed()->get(i)->cal_1peVStotal_fraction);

                break;
            }
/*                 ++breakCounter;
 *                 if( breakCounter > 1) break;
 *             }
 *             else if( tmp.first.find(epics_1pe_normalized) != std::string::npos )
 *             {
 *                 ca_put(DBR_DOUBLE, tmp.second,
 *                         &tpico->getAnalyzed()->get(i)->cal_1pe_normalized);
 * 
 *                 ++breakCounter;
 *                 if( breakCounter > 1) break;
 *             }
 */

        }
    }
    ca_poll();

    return;
}







///////////////////////////////////////////////////////////////////////////////








void Analysis::physics()
{




    return;
}







//                  END Public Member Functions
//
///////////////////////////////////////////////////////////////////////////////
