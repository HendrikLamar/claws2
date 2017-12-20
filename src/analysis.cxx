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

#include <numeric>


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

    for( int i = 0; i < 4; ++i )
    {
        if( !tpico->getCh(i)->getEnabled() ) continue;
        // vector to store all the calculated integrals
        std::unique_ptr< std::vector< long> > integrals{
            new std::vector< long>()};

        // get channel i's data
        auto tmpdata = tpico->getCh(i)->getBufferRapid();

        // loop through each waveform and calculate
        for( auto& tmp : *tmpdata )
        {
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



    }

    return;
}





//                  END Public Member Functions
//
///////////////////////////////////////////////////////////////////////////////
