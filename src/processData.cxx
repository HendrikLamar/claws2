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
#include "pico_channel.h"

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

    makePicoHist();
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







void ProcessData::makePicoHist()
{

    for( auto& tpico : *m_picos )
    {
        // create instance to hold all the data for one pico
        std::shared_ptr< Utility::Pico_Hist_Pico >  hist_pico{
            std::make_shared<Utility::Pico_Hist_Pico>(tpico->getLocation())};

        for(int kk = 0; kk < 4; ++kk )
        {

            if( tpico->getCh(kk)->getEnabled() )
            {

                // since the channel is enabled, create instance to hold the 
                // data for the channel
                std::shared_ptr< TH1I >     hist;
                std::string                 channel;
                std::string                 location;
                std::string                 name;
                std::string                 title;
            
                location = tpico->getLocation();
            
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
            
                // create histogramm instance
                hist = std::make_shared< TH1I >(
                        name.c_str(),
                        name.c_str(),
                        tpico->getCh(kk)->getBufferSize(), 
                        0, 
                        tpico->getCh(kk)->getBufferSize()
                        );
            

                std::shared_ptr<Utility::Pico_Hist_Channel> cha{
                    std::make_shared<Utility::Pico_Hist_Channel>(
                            tpico->getCh(kk)->getChNo(), hist)};


                // add the channel instance to the pico container
                hist_pico->add( cha );
            }
        }
        m_picos_hist->push_back(hist_pico);
    }
}








///////////////////////////////////////////////////////////////////////////////








void ProcessData::sync( 
        unsigned int& subRunNum,
        std::shared_ptr<Pico> tpico )
{
    // create mutex to lock variables for certain times
//    std::mutex tmp_mutex_pico;
    std::shared_ptr<Utility::Pico_Hist_Pico> hist_pico;
    for( auto& tmp : *m_picos_hist )
    {
        if( tmp->getLoc().compare( tpico->getLocation() ) == 0 )
        {
            hist_pico = tmp;
        }
    }

    std::string location{tpico->getLocation()};

    for( int i = 0; i < 4; ++i )
    {
        std::shared_ptr<Channel> channel{tpico->getCh(i)};
        std::shared_ptr<TH1I>   hist{hist_pico->get(i)};
        std::string                 name;
        std::string                 title;
        std::string                 channelNo;

        if( channel->getEnabled() )
        {
            // create proper name
            switch( channel->getChNo() )
            {
                case PS6000_CHANNEL_A:
                    channelNo = "1";
                    break;
                case PS6000_CHANNEL_B:
                    channelNo = "2";
                    break;
                case PS6000_CHANNEL_C:
                    channelNo = "3";
                    break;
                case PS6000_CHANNEL_D:
                    channelNo = "4";
                    break;
                default:
                    channelNo = "XY";
            
            }
            title = location + "_" + channelNo + "_" + std::to_string(subRunNum);
            hist->SetTitle(title.c_str());

            // copy the data
            for( unsigned tt = 0; tt < channel->getBufferBlock()->size() ; ++tt )
            {
                hist->SetBinContent( tt-1, channel->getBufferBlock()->at(tt) );
            }
        }
    }

    return;
}








///////////////////////////////////////////////////////////////////////////////








///////////////////////////////////////////////////////////////////////////////
//
//              END Private Member Functions
//
///////////////////////////////////////////////////////////////////////////////
