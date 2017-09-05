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



Channel::Channel( 
        PS6000_CHANNEL channel, 
        int16_t* handle, 
        Database* database,
        Utility::Pico_Data_Pico* picoData ) :
    m_channel( channel ),
    m_handle( handle ),
    m_database( database ),
    m_picoData( picoData )
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




void    Channel::loadConf( Utility::ClawsGain gain )
{

    // holds the database data in this function
    Utility::Pico_Data_HL_Gain* gdata;
    Utility::Pico_Data_Channel* data;



    switch( gain )
    {
        case Utility::INTER:
            gdata = m_picoData->dataIntermediate;
            break;
        case Utility::HIGH_GAIN:
            gdata = m_picoData->dataHighGain;
            break;
        case Utility::LOW_GAIN:
            gdata = m_picoData->dataLowGain;
            break;
        default:

    }




    // iterate through the pico data and 
    for ( auto& tmp : *m_database->m_picoData )
    {
        if( tmp->location.compare( *m_location ) == 0 )
        {

        }

    }
    if ( m_database->m_picoData)

}











///////////////////////////////////////////////////////////////////////////////
//
//             END Public member functions
//
///////////////////////////////////////////////////////////////////////////////






















