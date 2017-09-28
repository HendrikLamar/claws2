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
        Utility::Pico_Data_HL_Gain* const dataHLGain,
        Utility::Pico_Data_Inter*   const dataInter
    ) :
    m_channel( channel ),
    m_handle( handle ),
    m_data_HLGain( dataHLGain ),
    m_data_inter( dataInter )
{
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




std::vector< int16_t >*     Channel::getBuffer( Utility::Collection_Mode mode )
{
    switch( mode )
    {
        case Utility::Collection_Mode::BLOCK:
            return m_buffer_block_data;
        case Utility::Collection_Mode::INTERMEDIATE:
            return m_buffer_inter_data;
        case Utility::Collection_Mode::RAPID:
            return nullptr;

        default:
            throw ChannelException("This Collection Mode does not exist!");
    }

    return nullptr;
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

    // first, find the correct channel in the database
    for ( auto& tmp : *m_data_HLGain->channels )
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




PICO_STATUS Channel::setDataBuffer( Utility::Collection_Mode mode )
{

    PICO_STATUS output; 

    switch( mode )
    {
        case Utility::Collection_Mode::BLOCK:
            if( getEnabled( Utility::Collection_Mode::BLOCK ) )
            {
                if( m_buffer_block_data->size() != m_buffer_block_size )
                {
                    m_buffer_block_data->resize( m_buffer_block_size );
                }
            }

            output = ps6000SetDataBuffer(
                    *m_handle,
                    m_channel,
                    &m_buffer_block_data->at(0),
                    m_buffer_block_size,
                    m_data_HLGain->downSampleRatioMode
                    );

            return output;

        case Utility::Collection_Mode::INTERMEDIATE:
            if( getEnabled( Utility::Collection_Mode::INTERMEDIATE ) )
            {
                if( m_buffer_inter_data->size() != m_buffer_inter_size )
                {
                    m_buffer_inter_data->resize( m_buffer_inter_size );
                }
            }

            output = ps6000SetDataBuffer(
                    *m_handle,
                    m_channel,
                    &m_buffer_inter_data->at(0),
                    m_buffer_inter_size,
                    m_data_inter->downSampleRatioMode 
                    );
 
            return output;

        case Utility::Collection_Mode::RAPID:
            throw ChannelException("Rapid Block mode not available yet!");

        default:
            throw ChannelException("Wrong Collection Mode input!");
    }
    

    return PICO_CANCELLED;

}






///////////////////////////////////////////////////////////////////////////////






PICO_STATUS Channel::setChannel( Utility::Collection_Mode mode )
{
    PICO_STATUS output;

    switch( mode )
    {
        case Utility::Collection_Mode::INTERMEDIATE:
            output = ps6000SetChannel(
                    *m_handle,
                    m_channel,
                    getEnabled( mode ),
                    m_data_inter->coupling,
                    m_data_inter->range,
                    m_data_inter->analogueOffset,
                    m_data_inter->bandwidth
                    );
            return output;
        case Utility::Collection_Mode::BLOCK:
            output = ps6000SetChannel(
                    *m_handle,
                    m_channel,
                    getEnabled( mode ),
                    m_data_channel->coupling,
                    m_data_channel->range,
                    m_data_channel->analogueOffset,
                    m_data_channel->bandwidth
                    );
            return output;

        case Utility::Collection_Mode::RAPID:
            throw ChannelException("Rapid Block mode not available yet!");
            break;

        default:
            throw ChannelException("Wrong Collection Mode input!");
    }

    return PICO_CANCELLED;
}







///////////////////////////////////////////////////////////////////////////////






int16_t Channel::getEnabled( Utility::Collection_Mode mode )
{
    int16_t output = 0;

    int counter = 0;    // cannot initialized in case statement

    switch( mode )
    {
        case Utility::Collection_Mode::INTERMEDIATE:

            for( char&c : m_data_inter->channels_to_calibrate)
            {
                if( counter == static_cast<int>(m_channel) )
                {
                    if( c == '1' )
                    {
                        output = 1;
                        break;
                    };
                    ++counter;
                };
            };
            break;

        case Utility::Collection_Mode::BLOCK:
            output = m_data_channel->enabled;
            break;

        case Utility::Collection_Mode::RAPID:
            throw ChannelException("Rapid Block mode not available yet!");
            break;

        default:
            throw ChannelException("Wrong Collection Mode input!");
    }

    return PICO_CANCELLED;

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
    m_buffer_block_size = m_data_HLGain->preTrigger + m_data_HLGain->postTrigger;

    m_buffer_inter_size = m_data_inter->preTrigger = m_data_inter->postTrigger;

    return;
}












///////////////////////////////////////////////////////////////////////////////
//
//             END Private member functions
//
///////////////////////////////////////////////////////////////////////////////

















