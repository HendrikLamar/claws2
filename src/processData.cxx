// =====================================================================================
// 
//       Filename:  processData.cxx
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06.10.2017 08:07:39
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include "utility.h"
#include "processData.h"
#include "storage.h"

#include <memory>
#include <algorithm>
#include <mutex>
#include <thread>
#include <utility>

#include <TH1I.h>



///////////////////////////////////////////////////////////////////////////////
//
//              START Con- & Destructor
//
///////////////////////////////////////////////////////////////////////////////




ProcessData::ProcessData() :
    m_save( new Storage() )
{}






///////////////////////////////////////////////////////////////////////////////






ProcessData::~ProcessData()
{

}








///////////////////////////////////////////////////////////////////////////////
//
//              END Con- & Destructor
//
///////////////////////////////////////////////////////////////////////////////
























































///////////////////////////////////////////////////////////////////////////////
//
//              START Public Member Functions 
//
///////////////////////////////////////////////////////////////////////////////





std::shared_ptr< Storage >  ProcessData::save()
{
    return m_save;
}







///////////////////////////////////////////////////////////////////////////////







ProcessData*    ProcessData::sync()
{

    return this;
}








///////////////////////////////////////////////////////////////////////////////


















///////////////////////////////////////////////////////////////////////////////
//
//              END Public Member Functions
//
///////////////////////////////////////////////////////////////////////////////


















































///////////////////////////////////////////////////////////////////////////////
//
//              START Private Member Functions 
//
///////////////////////////////////////////////////////////////////////////////








void ProcessData::makeTH1I()
{
    // the first four entries indicate if the channels of the first pico are enabled
    // or not, the second four for pico #2, etc...
    std::shared_ptr< std::vector< int16_t > >  picoChannelEnabled;

    std::shared_ptr< TH1I >     hist;

    for( unsigned int ii = 0; ii < m_picos->size(); ++ii )
    {
        for( unsigned int kk = 0; kk < 4; ++kk )
        {
            picoChannelEnabled->push_back(m_picos->at(ii)->getCh(kk)->getEnabled());

            if( m_picos->at(ii)->getCh(kk)->getEnabled() )
            {
                hist = std::make_shared< TH1I >("hist", "", 
                        m_picos->at(ii)->getCh(kk)->getBuffer()->size(), 
                        0, 
                        m_picos->at(ii)->getCh(kk)->getBuffer()->size()
                        );

                for( unsigned tt = 0; tt < m_picos->at(ii)->getCh(kk)->getBuffer()->size(); ++tt )
                {
                    hist->SetBinContent( tt, m_picos->at(ii)->getCh(kk)->getBuffer()->at(tt) );
                }

                // very very long way (thanks to shr_ptr to construct a pair of pico_data_hist
                std::shared_ptr< std::pair< PS6000_CHANNEL, std::shared_ptr< TH1I > > > tipair{
                    std::make_shared< std::pair< PS6000_CHANNEL, std::shared_ptr< TH1I > > >
                        ( std::make_pair( m_picos->at(ii)->getCh(kk)->getChNo(), hist ) )};
//
//                // another wrong and not easy to read way to construct one unit of m_picos_hist
//                std::shared_ptr< std::pair< std::string, std::shared_ptr< std::vector< std::shared_ptr< std::pair< PS6000_CHANNEL, std::shared_ptr<TH1I> > > > > > > topair
//                {
//                    std::make_shared< std::pair< std::string, std::shared_ptr< std::vector< std::shared_ptr< std::pair< PS6000_CHANNEL, std::shared_ptr<TH1I> > > > > > >
//                    (
//                        std::make_pair( m_picos->at(ii)->getLocation(), tipair )
//                    )
//                };
//
//                m_picos_hist->push_back(topair);
            }
        }
    }



}








///////////////////////////////////////////////////////////////////////////////








///////////////////////////////////////////////////////////////////////////////
//
//              END Private Member Functions
//
///////////////////////////////////////////////////////////////////////////////
