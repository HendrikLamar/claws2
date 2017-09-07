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
        Utility::Pico_Data_Pico* picoData,
        Utility::Pico_Data_HL_Gain* channelData 
    ) :
    m_channel( channel ),
    m_handle( handle ),
    m_picoData( picoData ),
    m_channelData( channelData )
{
    // create data buffer and reserve maximum needed space on disk/RAM
    m_dataBuffer = new std::vector< int16_t >;
    m_dataBuffer->reserve( m_bufferSizeReserve );
}




///////////////////////////////////////////////////////////////////////////////




Channel::~Channel()
{
    if ( m_dataBuffer )
    {
        delete m_dataBuffer;
        m_dataBuffer = nullptr;
    }
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
    return m_dataBuffer;
}





///////////////////////////////////////////////////////////////////////////////






void Channel::setGainMode( Utility::ClawsGain& gain )
{
    switch ( gain )
    {
        case Utility::INTER:
            m_channelData = m_picoData->dataIntermediate;
            break;
        case Utility::HIGH_GAIN:
            m_channelData = m_picoData->dataHighGain;
            break;
        case Utility::LOW_GAIN:
            m_channelData = m_picoData->dataLowGain;
            break;
        default:
            throw ChannelException("Wrong gain mode defined!");
    }

    return;
}






///////////////////////////////////////////////////////////////////////////////






void    Channel::loadConfig()
{

    // holds the database data in this function
    Utility::Pico_Data_Channel* data;

    // first, find the correct channel in the database
    for ( auto& tmp : *m_channelData->channels )
    {
        if ( tmp->channel == m_channel )
        {
            data = tmp;
        }
    }


    // second, copy the data from the database to the member variables of the channel
    m_coupling          = data->coupling;
    m_enabled           = data->enabled;
    m_analogueOffset    = data->analogueOffset;
    m_range             = data->range;


    calcDataBufferSize();
}






///////////////////////////////////////////////////////////////////////////////




PICO_STATUS Channel::setDataBuffer()
{

    PICO_STATUS output; 

    // check if resize is needed
    if ( m_enabled ) 
    {
        if ( m_dataBuffer->size() != m_dataBufferSize )
        {
            m_dataBuffer->resize( m_dataBufferSize );
        }

    }
    
    // clear data buffer in any case
    m_dataBuffer->clear();

    output = ps6000SetDataBuffer
        (
            *m_handle,
            m_channel,
            &m_dataBuffer->at(0),
            m_dataBufferSize,
            m_channelData->downSampleRatioMode
        );
        

    return output;

}






///////////////////////////////////////////////////////////////////////////////






PICO_STATUS Channel::setChannel()
{
    PICO_STATUS output;

    output = ps6000SetChannel
        (
            *m_handle,
            m_channel,
            m_enabled,
            m_coupling,
            m_range,
            m_analogueOffset,
            m_bandwidth 
        );

    return output;
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
    m_dataBufferSize = m_channelData->preTrigger + m_channelData->postTrigger;

    return;
}












///////////////////////////////////////////////////////////////////////////////
//
//             END Private member functions
//
///////////////////////////////////////////////////////////////////////////////

















