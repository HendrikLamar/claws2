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

#include <map>
#include <memory>
#include <algorithm>
#include <mutex>
#include <thread>
#include <utility>

#include <TH1I.h>
#include <TGraph.h>



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






void ProcessData::syncBlock( 
        unsigned int& subRunNum,
        std::shared_ptr<Pico> tpico )
{

    sync( subRunNum, tpico, false );


    return;
}








///////////////////////////////////////////////////////////////////////////////








void ProcessData::syncSaveRapid( unsigned int& subRunNum )
{


    auto channelEnumToStringNo = [](PS6000_CHANNEL cha)->std::string
    {
        std::string output;
        switch( cha )
        {
            case PS6000_CHANNEL_A:
                output = "1";
                break;
            case PS6000_CHANNEL_B:
                output = "2";
                break;
            case PS6000_CHANNEL_C:
                output = "3";
                break;
            case PS6000_CHANNEL_D:
                output = "4";
                break;
            default:
                output = "XY";
        }

        return output;
    };


    // array to hold the intermediate data for easier access by loopNo
    //
    std::shared_ptr< std::vector<       // size of loops_inter
        std::shared_ptr< std::vector <  // size of all channels
            std::shared_ptr< std::pair< std::string, 
                std::shared_ptr< std::vector <  // size of acquired samples-> contains
                                            // the data for one waveform
                int16_t > > > > > > > > data_intermediate{
                    std::make_shared< std::vector<
                        std::shared_ptr< std::vector<
                            std::shared_ptr< std::pair< std::string, 
                                std::shared_ptr< std::vector< int16_t>>>>>>>>()};

//    std::cout << "SubRunNum: " << subRunNum << std::endl;

    for( unsigned int noWaveform = 0; noWaveform < subRunNum; ++noWaveform )
    {
        std::shared_ptr< std::vector<
            std::shared_ptr< std::pair< std::string, 
                std::shared_ptr< std::vector<int16_t>>>>>> data_waveform{
                    std::make_shared< std::vector<
                        std::shared_ptr< std::pair< std::string, 
                            std::shared_ptr<std::vector<int16_t>>>>>>()};

        for( auto& tmp1 : *m_picos )
        {
            for( auto i = 0; i < 4; ++i )
            {
                std::string name = tmp1->getLocation() + "_";
                name += channelEnumToStringNo(tmp1->getCh(i)->getChNo());
                std::shared_ptr< std::pair< std::string, 
                    std::shared_ptr< std::vector<int16_t> > > > data_channel{
                        std::make_shared<std::pair< 
                            std::string, std::shared_ptr<
                                std::vector<int16_t>>>>(
                                name, 
                                tmp1->getCh(i)->getBufferRapid()->at(noWaveform))};

//                std::cout << tmp1->getCh(i)->getBufferRapid()->at(noWaveform)<< "\n";

/*                 std::cout << name << std::endl;
 *                 for(auto& tmp3 : *tmp1->getCh(i)->getBufferRapid()->at(noWaveform))
 *                 {
 *                     std::cout << tmp3 << "  " << std::flush;
 *                 }
 *                 std::cout << "\n";
 */
                data_waveform->push_back(data_channel);
            }
        }
        data_intermediate->push_back(data_waveform);
    }

//    std::cout << "Size of data_intermediate: " << data_intermediate->size() << "\n";
//    std::cout << "Size of data_waveform: " << data_intermediate->at(0)->size();
//    std::cout << "\n";
//    std::cout << "Size for data_channel: " << 
//        data_intermediate->at(0)->at(0)->second->size() << "\n";
//    std::cout << "SomeName: " << data_intermediate->at(0)->at(0)->first << "\n";

/*     unsigned int counter1{0};
 *     for( auto& tmp1 : *data_intermediate)
 *     {
 *         std::cout << "\t\t\t\t#" << counter1 << "\n";
 *         for( auto& tmp2 : *tmp1 )
 *         {
 *             std::cout << tmp2->first << ": " << std::flush;
 *             for( auto& tmp3 : *tmp2->second )
 *             {
 *                 std::cout << tmp3 << " " << std::flush;
 *             }
 *             std::cout << "\n";
 *         }
 *         std::cout << "\n";
 *         ++counter1;
 *     }
 */


    auto work = [this](
        std::shared_ptr< std::vector<       // size of loops_inter
            std::shared_ptr< std::vector <  // size of all channels
                std::shared_ptr< std::pair< std::string, 
                    std::shared_ptr< std::vector <  // size of acquired samples-> contains
                                                // the data for one waveform
                    int16_t > > > > > > > > data_intermediate,
        unsigned int startIndex,
        unsigned int lastIndex)
    {

        std::string title{""};
        for( auto& tmp : *data_intermediate )
        {
            // holds the data in TH1I format for one time waveform taken
            std::shared_ptr< std::vector< 
                    std::shared_ptr< TH1I >>> hist_channel{
                        std::make_shared< std::vector< 
                            std::shared_ptr<TH1I>>>()};

            // rearrange each channel data into a TH1I
            for( auto& tmp2 : *tmp )
            {
                title = tmp2->first + "_" + std::to_string(startIndex);
                std::shared_ptr<TH1I> hist = 
                    std::make_shared<TH1I>(
                            tmp2->first.c_str(), 
                            title.c_str(),
                            tmp2->second->size(),
                            0,
                            tmp2->second->size());

                // add the data to the TH1I
                for( unsigned int ii = 0; ii < tmp2->second->size(); ++ii )
                {
                    hist->SetBinContent(ii+1, tmp2->second->at(ii));
                };

                // add the TH1I to the vector holding all channel data
                hist_channel->push_back( hist );
            }

            save()->intermediate(startIndex, hist_channel);

            // last step, increment the subRunNumber
            ++startIndex;
        }

        if( startIndex != lastIndex )
        {
            std::cout << "startIndex != lastIndex\n";
        }

        return;
    };

    work(data_intermediate, 0, data_intermediate->size());


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
        std::shared_ptr<Pico> tpico,
        bool isRapid )
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

            // resets the histogram
            hist->Reset("M");

            std::shared_ptr< std::vector<int16_t> > data;
            if( isRapid )
            {
                data = channel->getBufferRapid()->at(subRunNum);
            }
            else data = channel->getBufferBlock();

            // copy the data
            for( unsigned tt = 0; tt < data->size() ; ++tt )
            {
                hist->SetBinContent( tt-1, data->at(tt) );
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
