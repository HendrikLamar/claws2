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

#include <numeric>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////
//
//                  START Con-Destructor



Analysis::Analysis()
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

    int16_t maxVal{
        static_cast<int16_t>(
                tpico->getConfig()->average_1pe_height * 
                tpico->getConfigAnalysis()->inter_factor1pe)};

    for( int i = 0; i < 4; ++i )
    {
        if( !tpico->getCh(i)->getEnabled() ) continue;
        // vector to store all the calculated integrals
        std::unique_ptr< std::vector< long> > integrals{
            new std::vector< long>()};

        // get channel i's data
        auto tmpdata = tpico->getCh(i)->getBufferRapid();

        // loop through each waveform and calculate
        double maxCount{static_cast<double>(tmpdata->at(0)->size())};
        for( auto& tmp : *tmpdata )
        {
            // checks if it is an 1pe or higher
            // looks only in the second half of the waveform where the
            // waveform is expected
            if( maxVal < *(std::max_element(
                            tmp->begin()+tmp->size()/2.,tmp->end())) )
            {
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


        tpico->getAnalyzed()->get(i)->cal_1pe_normalized = mean;
        tpico->getAnalyzed()->get(i)->cal_1peVStotal_fraction =
                maxCount/static_cast<double>(tmpdata->at(0)->size());


    }

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
