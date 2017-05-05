// =====================================================================================
// 
//       Filename:  pico.cxx
// 
//    Description:  Contains the pico class definitions
// 
//        Version:  1.0
//        Created:  21.04.2017 14:01:03
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================


#include "pico.h"
#include "channel.h"
#include "utility.h"

#include <libps6000-1.4/ps6000Api.h>
#include <libps6000-1.4/PicoStatus.h>

#include <iostream>
#include <cassert>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////
/*
 * Pico Contructor.
 */

Pico::Pico(int16_t *handle, int8_t *serial)
    : m_handle(handle), m_serial(serial)
{
    Pico::openUnit();
    Pico::turnOffUnnecessary();

    // Initialize the channels and load defaults
    m_channelA = new Channel(m_handle, PS6000_CHANNEL_A, m_bufferSize);
    m_channelB = new Channel(m_handle, PS6000_CHANNEL_B, m_bufferSize);
    m_channelC = new Channel(m_handle, PS6000_CHANNEL_C, m_bufferSize);
    m_channelD = new Channel(m_handle, PS6000_CHANNEL_D, m_bufferSize);

    std::cout << "openUnit done!\n";
}
////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////
/*
 * Start - PrivateMemberFunctions!
 */
void Pico::printError(const char* message){
    std::cout << "\n########### START PICO ERROR ##########\n";
    std::cout << "\nError code: " << m_status << "\n";
    std::cout << message << "\n";
    std::cout << "\n########### END  PICO  ERROR ##########\n";

}

void Pico::turnOffUnnecessary(){

    m_status = ps6000SetEts(*m_handle,PS6000_ETS_OFF,0,0,nullptr);
    Pico::checkStatus();
}

void Pico::openUnit(){
    
    if(!m_serial){
        m_status = ps6000OpenUnit(m_handle,nullptr);
    }
    
    else{
        m_status = ps6000OpenUnit(m_handle,m_serial);
    }

    switch(*m_handle){

        case -1:
            std::cout << "Scope fails to open!\n";
            break;
        case 0:
            std::cout << "No scope is found!\n";
            break;
        default:
            std::cout << "Found a Pico: " << m_serial << "\n";
            sleep(1);
            break;
    }
}
/*
 * End - PrivateMemberFunctions!
 */
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
/*
 * Start - Return/Get Functions. 
 */

//! Returns the Channel to work on further on with class Channel methods.
Channel*    Pico::getCh(PS6000_CHANNEL channel){
               switch(channel){
                    case PS6000_CHANNEL_A:
                        return m_channelA;

                    case PS6000_CHANNEL_B:
                        return m_channelB;

                    case PS6000_CHANNEL_C:
                        return m_channelC;

                    case PS6000_CHANNEL_D:
                        return m_channelD;

                    default:
                        std::cout << "Can't find your channel. Please try again!\n";
                        return nullptr;
               } 
}
Channel*    Pico::getCh(int channel){
                PS6000_CHANNEL t_channel = static_cast<PS6000_CHANNEL>(channel);         

                switch(t_channel){
                    case PS6000_CHANNEL_A:
                        return m_channelA;

                    case PS6000_CHANNEL_B:
                        return m_channelB;

                    case PS6000_CHANNEL_C:
                        return m_channelC;

                    case PS6000_CHANNEL_D:
                        return m_channelD;

                    default:
                        std::cout << "Can't find your channel. Please try again!\n";
                        return nullptr;
               } 
}

//! Returns the current timebase (Default: 2).
uint32_t        Pico::returnTimebase() const{
                    return m_timebase;
}
//! Returns the number of the current preTriggers.
uint32_t        Pico::returnPreTrigger() const{
                    return m_preTrigger;
}
//! Returns the number of the current postTriggers.
uint32_t        Pico::returnPostTrigger() const{
                    return m_postTrigger;
}
//! Returns preTrigger + postTrigger
uint32_t        Pico::returnNoSamples() const{
                    uint32_t t_noSamples = m_preTrigger + m_postTrigger;
                    return t_noSamples;
}
//! Returns the current sampling interval in ns.
float           Pico::returnTimeInterval() const{
                    return m_timeInterval_ns;
}
//! Returns the current oversample mode (Default: 0).
int16_t         Pico::returnOversample() const{
                    return m_oversample;
}
//! Returns the max possible numer of samples to be taken.
//! This depens on the number of channels turned on. So first prepare
//! the Pico.
uint32_t        Pico::returnMaxSamples() const{
                    return m_maxSamples;
}
//! Returns the current segment index we use.
uint32_t        Pico::returnSegmentIndex() const{
                    return m_segmentIndex;
}
//! Returns the current buffer size.
uint32_t        Pico::returnBufferSize() const{
                    return m_bufferSize;
}
//! Returns the current ratio mode (Default: 0 = RATIO_MODE_NONE).
enPS6000RatioMode       Pico::returnRatioMode() const{
                    return m_ratioMode;
}

/*
 * End - Return/Get Functions. 
 */
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
/*
 * Start - Set Pico related stuff like Trigger and so on...
 */
void Pico::setSimpleTrigger(
        int16_t enabled,                        \
        PS6000_CHANNEL source,                  \
        int16_t threshold,                      \
        PS6000_THRESHOLD_DIRECTION direction,   \
        uint16_t delay,                         \
        int16_t autoTriggerTime_ms)
{

    m_tEnabled =            enabled;
    m_tSource =             source;
    m_tThreshold =          threshold;
    m_tDirection =          direction;
    m_tDelay =              delay;
    m_tAutoTrigger_ms =     autoTriggerTime_ms;

    m_status = ps6000SetSimpleTrigger(*m_handle, m_tEnabled, m_tSource,\
            m_tThreshold, m_tDirection, m_tDelay, m_tAutoTrigger_ms);

    Pico::checkStatus();
}

void Pico::getTimebase( 
        uint32_t    timebase,           \
        uint32_t    noSamples,          \
        float       *timeInterval_ns,  \
        uint32_t    *maxSamples
        ){

    m_status = ps6000GetTimebase2(
            *m_handle,               \
            timebase,               \
            noSamples,              \
            timeInterval_ns,       \
            m_oversample,           \
            maxSamples,            \
            m_segmentIndex);

    m_timebase =        timebase;
    m_noSamples =       noSamples;
    m_timeInterval_ns = *timeInterval_ns;
    m_maxSamples =      *maxSamples;

    Pico::checkStatus();

}

void Pico::runBlock(
        uint32_t    preTrigger,     \
        uint32_t    postTrigger,    \
        uint32_t    timebase
        ){
    m_preTrigger =      preTrigger;
    m_postTrigger =     postTrigger;
    m_timebase =        timebase;

    int32_t aqTime;

    m_status = ps6000RunBlock(
            *m_handle,              \
            m_preTrigger,           \
            m_postTrigger,          \
            m_timebase,             \
            m_oversample,           \
            &aqTime,                \
            m_segmentIndex,         \
            Utility::CallBackBlock,          \
            nullptr);

    // Waits until the pico is done with data taking!
    while(!Utility::pReady){
        usleep(10);
    }
    Utility::pReady = false;

    Pico::checkStatus();
}

void    Pico::getValues(
            uint32_t    *numberOfSamples,            \
            int16_t     *overflow
            ){
        m_status = ps6000GetValues(
                        *m_handle,                  \
                        0,                          \
                        numberOfSamples,            \
                        1,                          \
                        PS6000_RATIO_MODE_NONE,     \
                        m_segmentIndex,             \
                        overflow);
}

/*
 * End - Set Pico related stuff like Trigger and so on...
 */
////////////////////////////////////////////////////////////////////////


//! Closes the current Pico.
void        Pico::closeUnit(){
                sleep(1);
                m_status = ps6000CloseUnit(*m_handle); 
                Pico::checkStatus();
}

////////////////////////////////////////////////////////////////////////////////
/*
 * Start - Pico::checkStatus()
 */
void Pico::checkStatus(){
    switch (m_status){
        case 0:
            break;
        case 1:
            Pico::printError("An attempt has been made to open more than\
                    <API>_MAX_UNITS");
            break;
        case 2:
            Pico::printError("Not enough memory could be allocated on the host\
                    machine.");
            break;
        case 3:
            Pico::printError("No Pico Technology device could be found.");
            break;
        case 4:
            Pico::printError("Unable to download firmware.");
            break;
        case 5:
            Pico::printError("The driver is busy opening a device.");
            break;
        case 6:
            Pico::printError("An unspecified failure occurred.");
            break;
        case 7:
            Pico::printError("The PicoScope is not responding to commands from the PC.");
            break;
        case 8:
            Pico::printError("The configuration information in the PicoScope\
                    is corrupt or missing.");
            break;
        case 9:
            Pico::printError("The picopp.sys file is too old to be used with\
                    the device driver.");
            break;
        case 10:
            Pico::printError("The EEPROM has become corrupt, so the device \
                    will use a default setting.");
        case 11:
            Pico::printError("The operating system on the PC is not supported \
                    by this driver.");
            break;
        case 12:
            Pico::printError("There is no device with the handle value passed.");
            break;
        case 13:
            Pico::printError("A parameter value is not valid.");
            break;
        case 14:
            Pico::printError("The timebase is not supported or is invalid.");
            break;
        case 15:
            Pico::printError("The voltage range is not supported or is invalid.");
            break;
        case 16:
            Pico::printError("The channel number is not valid on this device or no channels have been set.");
            break;
        case 17:
            Pico::printError("The channel set for a trigger is not available on this device.");
            break;
        case 18:
            Pico::printError("The channel set for a condition is not available on this device.");
            break;
        case 19:
            Pico::printError("The device does not have a signal generator.");
            break;
        case 20:
            Pico::printError("Streaming has failed to start or has stopped without user request.");
            break;
        case 21:
            Pico::printError("Block failed to start - a parameter may have been set wrongly.");
            break;
        case 22:
            Pico::printError("A parameter that was required is NULL.");
            break;
        case 23:
            Pico::printError("The current functionality is not available while using ETS capture mode.");
            break;
        case 24:
            Pico::printError("No data is available from a run block call.");
            break;
        case 25:
            Pico::printError("The buffer passed for the information was too small.");
            break;
        case 26:
            Pico::printError("ETS is not supported on this device.");
            break;
        case 27:
            Pico::printError("The auto trigger time is less than the time it\
                    will take to collect the pre-trigger data.");
            break;
        case 28:
            Pico::printError("The collection of data has stalled as unread \
                    data would be overwritten.");
            break;
        case 29:
            Pico::printError("Number of samples requested is more than \
                    available in the current memory segment.");
            break;
        case 30:
            Pico::printError("Not possible to create number of segments requested.");
            break;
        case 31:
            Pico::printError("A null pointer has been passed in the trigger \
                    function or one of the parameters is out of range.");
            break;
        case 32:
            Pico::printError("One or more of the hold-off parameters are out of range.");
            break;
        case 33:
            Pico::printError("One or more of the source details are incorrect.");
            break;
        case 34:
            Pico::printError("One or more of the conditions are incorrect.");
            break;
        case 35:
            Pico::printError(" The driver's thread is currently in the \
                    <API>Ready callback function and therefore the action cannot be carried out.");
            break;
        case 36:
            Pico::printError("Data is unavailable because a run has not been completed.");
            break;
        case 37:
            Pico::printError("The memory segment index is out of range.");
            break;
        case 38:
            Pico::printError("The device is busy so data cannot be returned yet.");
            break;
        case 39:
            Pico::printError("The start time to get stored data is out of range.");
            break;
        case 40:
            Pico::printError("The information number requested is not a valid number.");
            break;
        case 41:
            Pico::printError("The handle is invalid so no information is \
                    available about the device. Only PICO_DRIVER_VERSION \
                    is available.");
            break;
        case 42:
            Pico::printError("The sample interval selected for streaming is out of range.");
            break;
        case 43:
            Pico::printError("ETS is set but no trigger has been set. A trigger setting is required for ETS.");
            break;
        case 44:
            Pico::printError("Driver cannot allocate memory.");
            break;
        case 45:
            Pico::printError("Incorrect parameter passed to the signal generator.");
            break;
        case 46:
            Pico::printError("Conflict between the shots and sweeps parameters sent to the signal generator.");
            break;
        case 47:
            Pico::printError("A software trigger has been sent but the trigger source is not a software trigger.");
            break;
        case 48:
            Pico::printError("An <API>SetTrigger call has found a conflict \
                    between the trigger source and the AUX output enable.");
            break;
        case 49:
            Pico::printError("ETS mode is being used and AUX is set as an input.");
            break;
        case 50:
            Pico::printError("Attempt to set different EXT input thresholds \
                    set for signal generator and oscilloscope trigger.");
            break;
        case 51:
            Pico::printError("An <API>SetTrigger... function has set AUX as an \
                    output and the signal generator is using it as a trigger.");
            break;
        case 52:
            Pico::printError("The combined peak to peak voltage and the analog \
                    offset voltage exceed the maximum voltage the signal \
                    generator can produce.");
            break;
        case 53:
            Pico::printError("NULL pointer passed as delay parameter.");
            break;
        case 54:
            Pico::printError("The buffers for overview data have not been set \
                    while streaming.");
            break;
        case 55:
            Pico::printError("The analog offset voltage is out of range.");
            break;
        case 56:
            Pico::printError("The analog peak-to-peak voltage is out of range.");
            break;
        case 57:
            Pico::printError("A block collection has been cancelled.");
            break;
        case 58:
            Pico::printError("The segment index is not currently being used.");
            break;
        case 59:
            Pico::printError("The wrong GetValues function has been called for the collection mode in use.");
            break;
        case 60:
            Pico::printError("PICO_GET_VALUES_INTERRUPTED");
            break;
        case 61:
            Pico::printError("The function is not available.");
            break;
        case 62:
            Pico::printError("The aggregation ratio requested is out of range.");
            break;
        case 63:
            Pico::printError("Device is in an invalid state.");
            break;
        case 64:
            Pico::printError("The number of segments allocated is fewer than the number of captures requested.");
            break;
        case 65:
            Pico::printError("A driver function has already been called and \
                    not yet finished. Only one call to the driver can be made \
                    at any one time.");
            break;
        case 66:
            Pico::printError("Not used. PICO_RESERVED");
            break;
        case 67:
            Pico::printError("An invalid coupling type was specified in <API>SetChannel.");
            break;
        case 68:
            Pico::printError("An attempt was made to get data before a data buffer was defined.");
            break;
        case 69:
            Pico::printError("The selected downsampling mode (used for data reduction) is not allowed.");
            break;
        case 70:
            Pico::printError("Aggregation was requested in rapid block mode.");
            break;
        case 71:
            Pico::printError("An invalid parameter was passed to <API>SetTriggerChannelProperties.");
            break;
        case 72:
            Pico::printError("The driver was unable to contact the oscilloscope.");
            break;
        case 73:
            Pico::printError("Resistance-measuring mode is not allowed in conjunction with the specified probe.");
            break;
        case 74:
            Pico::printError("The device was unexpectedly powered down.");
            break;
        case 75:
            Pico::printError("A problem occurred in <API>SetSigGenBuiltIn or <API>SetSigGenArbitrary.");
            break;
        case 76:
            Pico::printError("FPGA not successfully set up.");
            break;
        case 77:
            Pico::printError("PICO_POWER_MANAGER");
            break;
        case 78:
            Pico::printError("An impossible analog offset value was specified in <API>SetChannel.");
            break;
        case 79:
            Pico::printError("There is an error within the device hardware.");
            break;
        case 80:
            Pico::printError("There is an error within the device hardware.");
            break;
        case 81:
            Pico::printError("Unable to configure the signal generator.");
            break;
        case 82:
            Pico::printError("The FPGA cannot be initialized, so unit cannot be opened.");
            break;
        case 83:
            Pico::printError("The frequency for the external clock is not \
                    within 15\% of the nominal value.");
            break;
        case 84:
            Pico::printError("The FPGA could not lock the clock signal.");
            break;
        case 85:
            Pico::printError("You are trying to configure the AUX input as \
                    both a trigger and a reference clock.");
            break;
        case 86:
            Pico::printError("You are trying to congfigure the AUX input as \
                    both a pulse width qualifier and a reference clock.");
            break;
        case 87:
            Pico::printError("The requested scaling file cannot be opened.");
            break;
        case 88:
            Pico::printError("The frequency of the memory is reporting incorrectly.");
            break;
        case 89:
            Pico::printError("The I2C that is being actioned is not responding to requests.");
            break;
        case 90:
            Pico::printError("There are no captures available and therefore no data can be returned.");
            break;
        case 91:
            Pico::printError("The number of trigger channels is greater than 4,\
                    except for a PS4824 where 8 channels are allowed for \
                    rising/falling/rising_or_falling trigger directions.");
            break;
        case 92:
            Pico::printError("When more than 4 trigger channels are set on a \
                    PS4824 and the direction is out of range.");
            break;
        case 93:
            Pico::printError("When more than 4 trigger channels are set and \
                    their trigger condition states are not <API>_CONDITION_TRUE.");
            break;
        case 94:
            Pico::printError("The capture mode the device is currently running in does not support the current request.");
            break;
        case 95:
            Pico::printError("PICO_GET_DATA_ACTIVE");
            break;

        // Stopping here, but there are more cases! Puh!
        default:
            std::cout << "Error code: " << m_status << std::endl;
            break;
    }
}
/*
 * End - Pico::checkStatus() 
 */
////////////////////////////////////////////////////////////////////////////////
