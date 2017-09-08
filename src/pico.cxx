// =====================================================================================
// 
//       Filename:  pico.cxx
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  05.09.2017 18:18:50
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================


#include "pico.h"
#include "pico_channel.h"
#include "utility.h"
#include "clawsException.h"

#include <libps6000-1.4/PicoStatus.h>
#include <libps6000-1.4/ps6000Api.h>

#include <vector>
#include <string>



///////////////////////////////////////////////////////////////////////////////
//
//              START Constructor and Destructor
//
///////////////////////////////////////////////////////////////////////////////



Pico::Pico( Utility::Pico_Data_Pico* picoData ) :
    m_picoData( picoData ),
    m_location( &m_picoData->location ),
    m_channels( 
            new std::vector< Channel* >
                {
                    new Channel( 
                            PS6000_CHANNEL_A, 
                            &m_handle, 
                            m_picoData, 
                            m_picoData->dataIntermediate
                            ),
                    new Channel( 
                            PS6000_CHANNEL_B, 
                            &m_handle, 
                            m_picoData, 
                            m_picoData->dataIntermediate
                            ),
                    new Channel( 
                            PS6000_CHANNEL_C, 
                            &m_handle, 
                            m_picoData, 
                            m_picoData->dataIntermediate
                            ),
                    new Channel( 
                            PS6000_CHANNEL_D, 
                            &m_handle, 
                            m_picoData, 
                            m_picoData->dataIntermediate
                            ),
                }
    )
{
    // initialize pico
    init();
};









///////////////////////////////////////////////////////////////////////////////






Pico::~Pico()
{
    for ( auto& tmp : *m_channels )
    {
        delete tmp;
    }
    delete m_channels;
}




///////////////////////////////////////////////////////////////////////////////
//
//              END Constructor and Destructor
//
///////////////////////////////////////////////////////////////////////////////

































///////////////////////////////////////////////////////////////////////////////
//
//              START public member functions
//
///////////////////////////////////////////////////////////////////////////////





void Pico::init()
{
    if(!m_serial)
    {
        m_status = ps6000OpenUnit(&m_handle, nullptr);
    }
    else
    {
        m_status = ps6000OpenUnit(&m_handle, m_serial);
    }

    switch( m_handle )
    {
        case -1:
            throw PicoException(std::to_string(*m_serial) + " fails to open!");
            break;
        case 0:
            throw PicoException(std::to_string(*m_serial) + " not found!");
            break;
        default:
            break;
    }
}






///////////////////////////////////////////////////////////////////////////////







void Pico::loadConfig( Utility::ClawsGain mode )
{
    // first, load channel settings
    for ( auto& tmp : *m_channels )
    {
        tmp->setGainMode( mode );
        tmp->loadConfig();
    }

    Utility::Pico_Data_HL_Gain* data = getGainData( mode );


    // loading aquisition settings
    m_timebase                  = data->timebase;
    m_oversample                = data->oversample;
    m_preTrigger                = data->preTrigger;
    m_postTrigger               = data->postTrigger;
    m_downSamplingRatioMode     = data->downSampleRatioMode;
    m_downSampleRatio           = data->downSampleRatio;



    // loading simple trigger settings
    m_st_enabled                = data->trigger->enabled;
    m_st_source                 = data->trigger->source;
    m_st_threshold              = data->trigger->threshold;
    m_st_direction              = data->trigger->direction;
    m_st_delay                  = data->trigger->delay;
    m_st_autoTriggerTime_ms     = data->trigger->autoTriggerTime;

    //! \todo Add Advanced Trigger Settings!
}







///////////////////////////////////////////////////////////////////////////////








void Pico::setPico()
{

}








///////////////////////////////////////////////////////////////////////////////






Channel* Pico::getCh( PS6000_CHANNEL& cha )
{
    switch( cha )
    {
        case PS6000_CHANNEL_A:
            return m_channels->at(0);
        case PS6000_CHANNEL_B:
            return m_channels->at(1);
        case PS6000_CHANNEL_C:
            return m_channels->at(2);
        case PS6000_CHANNEL_D:
            return m_channels->at(3);
        default:
            return nullptr;
    }
}


Channel* Pico::getCh( int& cha )
{
    switch( cha )
    {
        case 1:
            return m_channels->at(0);
        case 2:
            return m_channels->at(1);
        case 3:
            return m_channels->at(2);
        case 4:
            return m_channels->at(3);
        default:
            return nullptr;
    }
}







///////////////////////////////////////////////////////////////////////////////







void runBlock();







///////////////////////////////////////////////////////////////////////////////







void stop();







///////////////////////////////////////////////////////////////////////////////







void close();








///////////////////////////////////////////////////////////////////////////////








//! Overloaded operator<< to print out information.
std::ostream& operator<<( std::ostream& out, Pico* picoscope );



















///////////////////////////////////////////////////////////////////////////////
//
//              END public member functions
//
///////////////////////////////////////////////////////////////////////////////
























///////////////////////////////////////////////////////////////////////////////
//
//              START private member functions
//
///////////////////////////////////////////////////////////////////////////////





Utility::Pico_Data_HL_Gain* Pico::getGainData( Utility::ClawsGain& mode )
{

    Utility::Pico_Data_HL_Gain* output;

    switch( mode )
    {
        case Utility::INTER:
            output = m_picoData->dataIntermediate;
            break;
        case Utility::LOW_GAIN:
            output = m_picoData->dataLowGain;
            break;
        case Utility::HIGH_GAIN:
            output = m_picoData->dataHighGain;
            break;
        default:
            throw PicoException("Wrong ClawsGain parameter entered!");
    }

    return output;
}







///////////////////////////////////////////////////////////////////////////////







// Sets the Trigger settings.
void    setTrigger();








///////////////////////////////////////////////////////////////////////////////









// Sets the aquisition settings.
void    setAqui();

















///////////////////////////////////////////////////////////////////////////////
//
//              END private member functions
//
///////////////////////////////////////////////////////////////////////////////
