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
    m_buffer_block_data = new std::vector< int16_t >;
    m_buffer_block_data->reserve( m_buffer_block_sizeReserve );

    m_buffer_inter_data = new std::vector< int16_t >;
    m_buffer_inter_data->reserve( m_buffer_inter_sizeReserver );
}




///////////////////////////////////////////////////////////////////////////////




Channel::~Channel()
{
    delete m_buffer_block_data;
    m_buffer_block_data = nullptr;

    delete m_buffer_inter_data;
    m_buffer_inter_data = nullptr;
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




std::vector< int16_t >*     Channel::getBuffer()
{
    return m_buffer_current_data;
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

    switch( m_data->gain )
    {
        case Utility::Claws_Gain::INTERMEDIATE:
            m_buffer_current_data = m_buffer_inter_data;
            m_buffer_current_size = &m_buffer_inter_size;
        default:
            m_buffer_current_data = m_buffer_block_data;
            m_buffer_current_size = &m_buffer_block_size;
    }

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




PICO_STATUS Channel::setDataBuffer()
{

    PICO_STATUS output; 

    if( m_buffer_current_data->size() != *m_buffer_current_size )
    {
        m_buffer_current_data->resize( *m_buffer_current_size );
    }

    output = ps6000SetDataBuffer(
            *m_handle,
            m_channel,
            &m_buffer_current_data->at(0),
            *m_buffer_current_size,
            m_data->val_downSampleRatioMode
            );


    return output;

}






///////////////////////////////////////////////////////////////////////////////






PICO_STATUS Channel::setChannel()
{
    PICO_STATUS output;

            output = ps6000SetChannel(
                    *m_handle,
                    m_channel,
                    getEnabled(),
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

    return;
}












///////////////////////////////////////////////////////////////////////////////
//
//             END Private member functions
//
///////////////////////////////////////////////////////////////////////////////

















