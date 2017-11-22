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




ProcessData::ProcessData( std::shared_ptr<std::vector< std::shared_ptr< Pico > > > vPicos, std::shared_ptr<unsigned long> runNum ) :
    m_picos( vPicos ),
    m_runNum( runNum )
{

    m_picos_hist = std::make_shared< std::vector< std::shared_ptr< Utility::Pico_Hist_Pico > > >();
    m_save = std::make_shared<Storage>(m_picos_hist, m_runNum );
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
void ProcessData::sync( unsigned int& subRunNum, std::shared_ptr<Pico> tpico )
{

    makeTH1I( subRunNum, tpico );

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








void ProcessData::makeTH1I( 
        unsigned int& subRunNum,
        std::shared_ptr<Pico> tpico )
{
    // create mutex to lock variables for certain times
    std::mutex tmp_mutex_pico;

    // the first four entries indicate if the channels of the first pico are enabled
    // or not, the second four for pico #2, etc...
    std::shared_ptr< std::vector< int16_t > >  picoChannelEnabled{
        std::make_shared<std::vector< int16_t > >()};

    std::shared_ptr< TH1I >     hist;
    std::string                 channel;
    std::string                 location;
    std::string                 name;
    std::string                 title;

        
    // create instance to hold all the data for one pico
    std::shared_ptr< Utility::Pico_Hist_Pico >  hist_pico{
        std::make_shared<Utility::Pico_Hist_Pico>(tpico->getLocation())};
    location = tpico->getLocation();

    for( unsigned int kk = 0; kk < 4; ++kk )
    {
        picoChannelEnabled->push_back(tpico->getCh(kk)->getEnabled());

        if( tpico->getCh(kk)->getEnabled() )
        {

            // since the channel is enabled, create instance to hold the data for the channel
            std::shared_ptr<Utility::Pico_Hist_Channel> cha{
                std::make_shared<Utility::Pico_Hist_Channel>(tpico->getCh(kk)->getChNo())};

            // create proper name
            switch( tpico->getCh(kk)->getChNo() )
            {
                case PS6000_CHANNEL_A:
                    channel = "1";
                    break;
                case PS6000_CHANNEL_B:
                    channel = "2";
                    break;
                case PS6000_CHANNEL_C:
                    channel = "3";
                    break;
                case PS6000_CHANNEL_D:
                    channel = "4";
                    break;
                default:
                    channel = "XY";
            
            }
            name = location + "_" + channel;
            title = location+ "+" + channel + "_" + std::to_string(subRunNum);

            
            // create histogramm instance
            hist = std::make_shared< TH1I >(
                    name.c_str(),
                    title.c_str(),
                    tpico->getCh(kk)->getBuffer()->size(), 
                    0, 
                    tpico->getCh(kk)->getBuffer()->size()
                    );

            // copy the data
            for( unsigned tt = 0; tt < tpico->getCh(kk)->getBuffer()->size() ; ++tt )
            {
                hist->SetBinContent( tt-1, tpico->getCh(kk)->getBuffer()->at(tt) );
            }

            // set the data to the channel data instance
            cha->set( hist );

            // add the channel instance to the pico container
            hist_pico->add( cha );
        }
    }

    // append the pico to the m_picos_hist vector
    // this action must be secured with a mutex which is freed after
    // going out of scope
    {
        std::lock_guard<std::mutex> guard(tmp_mutex_pico);
        m_picos_hist->push_back( hist_pico );
    }


    return;

}








///////////////////////////////////////////////////////////////////////////////








///////////////////////////////////////////////////////////////////////////////
//
//              END Private Member Functions
//
///////////////////////////////////////////////////////////////////////////////
