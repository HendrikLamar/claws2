// =====================================================================================
// 
//       Filename:  channel.cxx
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02.05.2017 13:42:27
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include "channel.h"
#include "pico.h"

#include <libps6000-1.4/PicoStatus.h>
#include <libps6000-1.4/ps6000Api.h>

#include <vector>

////////////////////////////////////////////////////////////////////////////////
//
//          CHANNEL CONSTRUCTOR
//
////////////////////////////////////////////////////////////////////////////////

Channel::Channel(int16_t *handle, PS6000_CHANNEL channel, uint32_t bufferSize)
    : m_handle(handle), m_channel(channel), m_bufferSize(bufferSize){

        m_buffer = new std::vector<int16_t>;
        m_buffer->resize(m_bufferSize);
        setDataBuffer();
    }
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*
 * Start - Hidden member function!
 */
PICO_STATUS     Channel::m_setCh(){
                    
                    PICO_STATUS     t_status;

                    t_status = ps6000SetChannel(\
                            *m_handle,           \
                            m_channel,          \
                            m_enabled,          \
                            m_coupling,         \
                            m_range,            \
                            m_analogueOffset,   \
                            m_bandwidth);

                    return t_status;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/*
 * Start - ReturnFunctions!
 */
//int16_t                    Channel::returnHandle() const{
//                                return m_handle;
//                            }

PS6000_CHANNEL              Channel::returnChannel() const{
                                return m_channel;
                            }

int16_t                     Channel::returnEnabled() const{
                                return m_enabled;
                            }

float                       Channel::returnOffset() const{
                                return m_analogueOffset;
                            }

PS6000_COUPLING             Channel::returnCoupling() const{
                                return m_coupling;
                            }

PS6000_RANGE                Channel::returnRange() const{
                                return m_range;
                            }

PS6000_BANDWIDTH_LIMITER    Channel::returnBandwidth() const{
                                return m_bandwidth;
                            }

//! Returns the channel buffer.
std::vector<int16_t>*       Channel::getBuffer(){
                                return m_buffer;
                            }

/*
 * End - ReturnFunctions!
 */
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
/*
 * Start - SetFunctions!
 */
PICO_STATUS     Channel::setEnabled(int16_t enabled){
                    PICO_STATUS t_status;

                    m_enabled = enabled;
                    
                    t_status = m_setCh();

                    return t_status;
}

PICO_STATUS     Channel::setOffset(float analogueOffset){
                    PICO_STATUS t_status;

                    m_analogueOffset = analogueOffset;

                    t_status = m_setCh();

                    return t_status;
}

PICO_STATUS     Channel::setCoupling(PS6000_COUPLING coupling){
                    PICO_STATUS t_status;

                    m_coupling = coupling;

                    t_status = m_setCh();

                    return t_status;
}

PICO_STATUS     Channel::setRange(PS6000_RANGE range){
                    PICO_STATUS t_status;

                    m_range = range;

                    t_status = m_setCh();

                    return t_status;
}
                    

PICO_STATUS     Channel::setBandwidth(PS6000_BANDWIDTH_LIMITER bandwidth){
                    PICO_STATUS t_status;

                    m_bandwidth = bandwidth;

                    t_status = m_setCh();

                    return t_status;
}

PICO_STATUS     Channel::setChannel(\
                        int16_t                     enabled,            \
                        PS6000_RANGE                range,              \
                        float                       analogueOffset
                        ){
                    PICO_STATUS t_status;

                    m_enabled =         enabled;
                    m_range =           range;
                    m_analogueOffset =  analogueOffset;

                    t_status = m_setCh();

                    return t_status;
}

PICO_STATUS     Channel::setChannel(\
                        int16_t                     enabled,            \
                        PS6000_RANGE                range,              \
                        float                       analogueOffset,     \
                        PS6000_COUPLING             coupling,           \
                        PS6000_BANDWIDTH_LIMITER    bandwidth
                        ){
                    PICO_STATUS t_status;

                    m_enabled =         enabled;
                    m_range =           range;
                    m_analogueOffset =  analogueOffset;
                    m_coupling =        coupling;
                    m_bandwidth =       bandwidth;

                    t_status = m_setCh();

                    return t_status;
}

PICO_STATUS     Channel::setDataBuffer(){
                    PICO_STATUS     t_status;

                    t_status = ps6000SetDataBuffer(
                                    *m_handle,              \
                                    m_channel,              \
                                    &m_buffer->at(0),        \
                                    m_bufferSize,           \
                                    PS6000_RATIO_MODE_NONE);
                    return t_status;
}

/*
 * End - SetFunctions!
 */
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/*
 * Start - Print!
 */
std::ostream& operator<< (std::ostream &out, const Channel *channel){
     
    out << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"\
    << "Printing channel information...\n\n"\
    << "Pico:\t\t" << 5 << "\n"\
    << "Channel:\t\t" << channel->returnChannel() << "\n"\
    << "Enabled:\t\t" << channel->returnEnabled() << "\n"\
    << "Ana. Offset:\t\t" << channel->returnOffset() << "\n"\
    << "Coupling:\t\t" << channel->returnCoupling() << "\n"\
    << "Range:\t\t\t"   << channel->returnRange() << "\n"\
    << "Bandwidth:\t\t" << channel->returnBandwidth() << "\n"\
    << "++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";

    return out;
}

/*
 * End - Print!
 */
////////////////////////////////////////////////////////////////////////////////


















