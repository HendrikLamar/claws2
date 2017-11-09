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
#include "pico.h"

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




ProcessData::ProcessData( std::shared_ptr<std::vector< std::shared_ptr< Pico > > >    vPicos ) :
    m_save( std::make_shared<Storage>() ),
    m_picos( vPicos )
{

    m_picos_hist = std::make_shared< std::vector< std::shared_ptr< Utility::Pico_Hist_Pico > > >();
}






///////////////////////////////////////////////////////////////////////////////






ProcessData::~ProcessData()
{
    m_picos_hist.reset();
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







//std::shared_ptr<ProcessData>    ProcessData::sync()
void ProcessData::sync()
{

    makeTH1I();

//    return shared_from_this();
    return;
}








///////////////////////////////////////////////////////////////////////////////








void    ProcessData::clear()
{
    m_picos_hist->clear();

    return;
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
    std::shared_ptr< std::vector< int16_t > >  picoChannelEnabled{
        std::make_shared<std::vector< int16_t > >()};

    std::shared_ptr< TH1I >     hist;

    for( unsigned int ii = 0; ii < m_picos->size(); ++ii )
    {
        
        // create instance to hold all the data for one pico
        std::shared_ptr< Utility::Pico_Hist_Pico >  tpico{
            std::make_shared<Utility::Pico_Hist_Pico>(m_picos->at(ii)->getLocation())};

        for( unsigned int kk = 0; kk < 4; ++kk )
        {
            picoChannelEnabled->push_back(m_picos->at(ii)->getCh(kk)->getEnabled());

            if( m_picos->at(ii)->getCh(kk)->getEnabled() )
            {

                // since the channel is enabled, create instance to hold the data for the channel
                std::shared_ptr<Utility::Pico_Hist_Channel> cha{
                    std::make_shared<Utility::Pico_Hist_Channel>(m_picos->at(ii)->getCh(kk)->getChNo())};

                // create histogramm instance
                hist = std::make_shared< TH1I >("hist", "", 
                        m_picos->at(ii)->getCh(kk)->getBuffer()->size(), 
                        0, 
                        m_picos->at(ii)->getCh(kk)->getBuffer()->size()
                        );

                // copy the data
                for( unsigned tt = 0; tt < m_picos->at(ii)->getCh(kk)->getBuffer()->size(); ++tt )
                {
                    hist->SetBinContent( tt, m_picos->at(ii)->getCh(kk)->getBuffer()->at(tt) );
                }

                // set the data to the channel data instance
                cha->set( hist );

                // add the channel instance to the pico container
                tpico->add( cha );
            }
        }

        // append the pico to the m_picos_hist vector
        m_picos_hist->push_back( tpico );

    }


    return;

}








///////////////////////////////////////////////////////////////////////////////








///////////////////////////////////////////////////////////////////////////////
//
//              END Private Member Functions
//
///////////////////////////////////////////////////////////////////////////////
