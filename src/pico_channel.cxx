// =====================================================================================
// 
//       Filename:  pico_channel.cxx
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  18.08.2017 14:43:13
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include <vector>

#include "pico_channel.h"
#include "utility.h"
#include "clawsException.h"



///////////////////////////////////////////////////////////////////////////////
//
//              START Constructor and Destructor
//
///////////////////////////////////////////////////////////////////////////////



Channel::Channel
    (
        PS6000_CHANNEL channel,
        int16_t* handle, 
        std::shared_ptr<Utility::Pico_Conf_HL_Gain> const dataHLGain
    ) :
    m_channel( channel ),
    m_handle( handle ),
    m_data( dataHLGain )
{
    loadConfig();

    // create data buffer and reserve maximum needed space on disk/RAM
    m_buffer_block_data = std::make_shared<std::vector< int16_t >>();
    m_buffer_block_data->reserve( m_buffer_block_sizeReserve );

    // set intermediate data buffer for rapid block mode
    m_buffer_rapid = std::make_shared<std::vector<
        std::shared_ptr< std::vector< int16_t >>>>();

    for( unsigned int i = 0; i < 10; ++i )
    {
        std::shared_ptr<std::vector<int16_t>> tmp =
            std::make_shared<std::vector<int16_t>>(m_buffer_rapid_sizeReserver);
        m_buffer_rapid->push_back(tmp);
    }
}




///////////////////////////////////////////////////////////////////////////////




Channel::~Channel()
{
}



///////////////////////////////////////////////////////////////////////////////
//
//              END Constructor and Destructor
//
///////////////////////////////////////////////////////////////////////////////





















///////////////////////////////////////////////////////////////////////////////
//
//             START Public member functions
//
///////////////////////////////////////////////////////////////////////////////




std::shared_ptr<std::vector< int16_t >>     Channel::getBufferBlock()
{
    return m_buffer_block_data;
}





///////////////////////////////////////////////////////////////////////////////




std::shared_ptr< std::vector< 
    std::shared_ptr< std::vector< int16_t >>>> Channel::getBufferRapid()
{
    return m_buffer_rapid;
}








///////////////////////////////////////////////////////////////////////////////






unsigned int Channel::getBufferSize()
{
    return m_buffer_block_size;
}





///////////////////////////////////////////////////////////////////////////////













/* void Channel::setGainMode( Utility::Claws_Gain& gain )
 * {
 *     switch ( gain )
 *     {
 *         case Utility::INTER:
 *             m_channelData = m_picoData->dataIntermediate;
 *             break;
 *         case Utility::HIGH_GAIN:
 *             m_channelData = m_picoData->dataHighGain;
 *             break;
 *         case Utility::LOW_GAIN:
 *             m_channelData = m_picoData->dataLowGain;
 *             break;
 *         default:
 *             throw ChannelException("Wrong gain mode defined!");
 *     }
 * 
 *     return;
 * }
 */






///////////////////////////////////////////////////////////////////////////////






void    Channel::loadConfig()
{

//    switch( m_data->gain )
//    {
////        case Utility::Claws_Gain::INTERMEDIATE:
////            m_buffer_current_data = m_buffer_inter_data;
////            m_buffer_current_size = &m_buffer_inter_size;
//        default:
//            m_buffer_current_data = m_buffer_block_data;
//            m_buffer_current_size = &m_buffer_block_size;
//    }

    // first, find the correct channel in the database
    for ( auto& tmp : *m_data->channels )
    {
        if ( tmp->channel == m_channel )
        {
            m_data_channel = tmp;
            break;
        }
    }


    calcDataBufferSize( );
}






///////////////////////////////////////////////////////////////////////////////




PICO_STATUS Channel::setDataBufferBlock()
{

    PICO_STATUS output; 

    if( m_buffer_block_data->size() != m_buffer_block_size )
    {
        m_buffer_block_data->resize( m_buffer_block_size );
        m_buffer_block_data->shrink_to_fit();
    }

    output = ps6000SetDataBuffer(
            *m_handle,
            m_channel,
            &m_buffer_block_data->at(0),
            m_buffer_block_size,
            m_data->val_downSampleRatioMode
            );


    return output;

}








///////////////////////////////////////////////////////////////////////////////







PICO_STATUS Channel::setDataBufferRapidBlock()
{
    PICO_STATUS output;


    if( m_data->loops_inter > m_buffer_rapid->size() )
    {
        for( auto& tmp : *m_buffer_rapid )
        {
            tmp->resize(m_buffer_rapid_size);
        }
        unsigned int counter2{0};
//        std::cout << "Current size: " << m_buffer_rapid->size() << "\n";
//        std::cout << "Size to add: " << m_data->loops_inter-m_buffer_rapid->size();
//        std::cout << "\n";
        unsigned int noOfLoopsNeeded{static_cast<unsigned int>( 
                m_data->loops_inter - m_buffer_rapid->size())};
        for(  unsigned int i = 0; 
                i < noOfLoopsNeeded; ++i )
        {
            m_buffer_rapid->push_back(
                    std::make_shared< std::vector< int16_t>>(getBufferSize()));
            ++counter2;
        }
//        std::cout << "Loops ran: " << counter2 << "\n";
//        std::cout << "Size afterwards: " << m_buffer_rapid->size() << "\n";
//        std::cout << "Must be: " << m_data->loops_inter << "\n";
    }
    else 
    {
        m_buffer_rapid->resize( m_data->loops_inter );

        for( auto& tmp : *m_buffer_rapid )
        {
            tmp->resize(m_buffer_rapid_size);
        }
    };
//
//    std::cout << "setDataBufferBulk...\n";
//    std::cout << "Channel: " << Utility::Pico_EnumToString_channel(m_channel) << "\n";
//    std::cout << "Rapidsize: " << m_buffer_rapid_size  << "\n";
//    std::cout << "DownsampleMode: " << 
//        Utility::Pico_EnumToString_ratio(m_data->val_downSampleRatioMode) 
//        << std::endl;

    for( unsigned int waveformNo = 0; waveformNo < m_data->loops_inter; ++waveformNo )
    {
//        std::cout << "#" << waveformNo << "\t" << std::flush;
//        std::cout << "waveformadress: " << &m_buffer_rapid->at(waveformNo)->at(0) 
//            << "\n";
        output = ps6000SetDataBufferBulk(
                *m_handle,
                m_channel,
                &m_buffer_rapid->at(waveformNo)->at(0),
                m_buffer_rapid_size,
                waveformNo,
                m_data->val_downSampleRatioMode
                );
        if( output != PICO_OK ) break;
    }


    return output; 
}








///////////////////////////////////////////////////////////////////////////////








PICO_STATUS Channel::setDataBufferIntermediate( bool yesno )
{
    PICO_STATUS output = PICO_OK;
    if( yesno )
    {
        unsigned int counter1{0};
        for( unsigned int waveformNo = 0; 
                waveformNo < m_data->loops_inter; ++waveformNo )
        {

            output = ps6000SetDataBufferBulk(
                    *m_handle,
                    m_channel,
                    &m_buffer_rapid->at(waveformNo)->at(0),
                    m_buffer_rapid_size,
                    waveformNo,
                    m_data->val_downSampleRatioMode
                    );
            ++counter1;
            if( output != PICO_OK ) break;
        }
//        std::cout << "setDataBufferIntermediate loops: " << counter1 << "\n";
    }
    else
    {
        for( unsigned int waveformNo = 0; 
                waveformNo < m_data->loops_inter; ++waveformNo )
        {
            output = ps6000SetDataBufferBulk(
                    *m_handle,
                    m_channel,
                    nullptr,
                    0,
                    waveformNo,
                    m_data->val_downSampleRatioMode
                    );
            if( output != PICO_OK ) break;
        }

    }


    return output;
}









///////////////////////////////////////////////////////////////////////////////






PICO_STATUS Channel::setChannel( bool followsSettings, bool ifNotIsEnabled )
{
    PICO_STATUS output;

    int16_t isEnabled;

    if( followsSettings )
    {
        isEnabled = getEnabled();
    }
    else ifNotIsEnabled ? isEnabled = 1 : isEnabled = 0;


    
            output = ps6000SetChannel(
                    *m_handle,
                    m_channel,
                    isEnabled,
                    m_data_channel->coupling,
                    m_data_channel->range,
                    m_data_channel->analogueOffset,
                    m_data_channel->bandwidth
                    );


    return output;
}







///////////////////////////////////////////////////////////////////////////////






int16_t Channel::getEnabled()
{
    int16_t output = 0;

    output = m_data_channel->enabled;

    return output;

}








///////////////////////////////////////////////////////////////////////////////







PS6000_CHANNEL      Channel::getChNo()
{
    PS6000_CHANNEL tmpCh = m_channel;

    return tmpCh;
}









///////////////////////////////////////////////////////////////////////////////
//
//             END Public member functions
//
///////////////////////////////////////////////////////////////////////////////
































///////////////////////////////////////////////////////////////////////////////
//
//             START Private member functions
//
///////////////////////////////////////////////////////////////////////////////



void    Channel::calcDataBufferSize()
{
    m_buffer_block_size = m_data->val_preTrigger + m_data->val_postTrigger;
    m_buffer_rapid_size = m_data->val_preTrigger + m_data->val_postTrigger;

    return;
}












///////////////////////////////////////////////////////////////////////////////
//
//             END Private member functions
//
///////////////////////////////////////////////////////////////////////////////

















