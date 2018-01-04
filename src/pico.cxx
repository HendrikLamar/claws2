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
#include <chrono>
#include <thread>
#include <memory>
#include <typeinfo>



///////////////////////////////////////////////////////////////////////////////
//
//              START Constructor and Destructor
//
///////////////////////////////////////////////////////////////////////////////



/* Pico::Pico( Utility::Pico_Conf_Pico* picoData ) :
 *     m_data_pico( picoData ),
 *     m_serial( m_data_pico->val_serial ),
 *     m_location( &m_data_pico->val_location ),
 *     m_data_current( new Utility::Pico_Conf_HL_Gain( Utility::Claws_Gain::INTERMEDIATE ) ),
 *     m_channels( 
 *             new std::vector< Channel* >
 *                 {
 *                     new Channel( 
 *                             PS6000_CHANNEL_A, 
 *                             &m_handle, 
 *                             m_data_current
 *                             ),
 *                     new Channel( 
 *                             PS6000_CHANNEL_B, 
 *                             &m_handle, 
 *                             m_data_current
 *                             ),
 *                     new Channel( 
 *                             PS6000_CHANNEL_C, 
 *                             &m_handle, 
 *                             m_data_current
 *                             ),
 *                     new Channel( 
 *                             PS6000_CHANNEL_D, 
 *                             &m_handle, 
 *                             m_data_current
 *                             ),
 *                 }
 *                )
 * {
 *     // initialize pico
 *     init();
 *     turnOffUnneeded();
 * 
 * }
 */







///////////////////////////////////////////////////////////////////////////////







Pico::Pico( std::shared_ptr<Utility::Pico_Conf_Pico> picoData, int16_t handle ) :
    m_data_pico( picoData ),
    m_handle( handle ),
    m_serial( m_data_pico->val_serial ),
    m_location( &m_data_pico->val_location ),
    m_data_current( new Utility::Pico_Conf_HL_Gain( Utility::Pico_RunMode::PHYSICS) ),
    m_data_analyzed( std::make_shared<
            Utility::Pico_Data_Analyzed_Pico>(getLocation())),
    m_channels( 
            std::make_shared<std::vector< std::shared_ptr<Channel> > >() )
{
    m_channels->push_back(std::make_shared<Channel>( 
                                        PS6000_CHANNEL_A, 
                                        &m_handle, 
                                        m_data_current
                                        ));
    m_channels->push_back(std::make_shared<Channel>( 
                                        PS6000_CHANNEL_B, 
                                        &m_handle, 
                                        m_data_current
                                        ));
    m_channels->push_back(std::make_shared<Channel>( 
                                        PS6000_CHANNEL_C, 
                                        &m_handle, 
                                        m_data_current
                                        ));
    m_channels->push_back(std::make_shared<Channel>( 
                                        PS6000_CHANNEL_D, 
                                        &m_handle, 
                                        m_data_current
                                        ));
    // no initialization needed since the handle is known
    
    pingUnit();
};







///////////////////////////////////////////////////////////////////////////////







Pico::~Pico()
{
    m_channels.reset();

    m_data_current.reset();

    close();
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




    
void Pico::pingUnit()
{
    m_status = ps6000PingUnit( m_handle );
    checkStatus();

    return;
}
    




////////////////////////////////////////////////////////////////////////////////
    



void Pico::setConfig( Utility::Pico_RunMode mode )
{

    m_noChannelsEnabled = 0;

    switch( mode )
    {
        case Utility::Pico_RunMode::INTERMEDIATE:
            *m_data_current = *m_data_pico->data_inter;
            break;
        case Utility::Pico_RunMode::PHYSICS:
            *m_data_current = *m_data_pico->data_physics;
            break;
        default:
            throw PicoException("Wrong gain input!");
    }

    // load channel settingso
    for( auto& tmp : *m_channels )
    {
        tmp->loadConfig();
        tmp->getEnabled() ? ++m_noChannelsEnabled : false;
    }


    m_buffer_data_size = m_data_current->val_preTrigger + m_data_current->val_postTrigger;


    return;
}







///////////////////////////////////////////////////////////////////////////////








std::shared_ptr< Utility::Pico_Conf_HL_Gain > Pico::getConfig()
{
    return m_data_current;
}







///////////////////////////////////////////////////////////////////////////////







std::shared_ptr< Utility::Pico_Conf_Analysis > Pico::getConfigAnalysis()
{
    return m_data_pico->conf_analysis;
}







///////////////////////////////////////////////////////////////////////////////







std::shared_ptr< Utility::Pico_Data_Analyzed_Pico > Pico::getAnalyzed()
{
    return m_data_analyzed;
}







///////////////////////////////////////////////////////////////////////////////






std::shared_ptr<Channel> Pico::getCh( PS6000_CHANNEL cha )
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


std::shared_ptr<Channel> Pico::getCh( int cha )
{
    switch( cha )
    {
        case 0:
            return m_channels->at(0);
        case 1:
            return m_channels->at(1);
        case 2:
            return m_channels->at(2);
        case 3:
            return m_channels->at(3);
        default:
            throw PicoException("Error. Channels count from 0 to 3, dumbass!");
            return nullptr;
    }
}








///////////////////////////////////////////////////////////////////////////////







std::string Pico::getLocation()
{
    std::string     tlocation = *m_location;

    return tlocation;
}









///////////////////////////////////////////////////////////////////////////////








void Pico::setReadyBlock( )
{
    setChannels();

    // divide local pico memory in N pieces
    setMemorySegments(1);

    setNoOfCaptures(1);
    
    getTimebase();

    setTrigger();

    // set data buffer for each channel to define where the data should be stored
    for( auto& tmp : *m_channels )
    {
        tmp->setDataBufferBlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }


    return;

}








///////////////////////////////////////////////////////////////////////////////








void Pico::setReadyRapid()
{
    setChannels();

//    setMemorySegments(100);
    setMemorySegments(m_data_current->loops_inter);

//    setNoOfCaptures(30);
    setNoOfCaptures(m_data_current->loops_inter);

    getTimebase();

    setTrigger();

    for( auto& tmp : *m_channels )
    {
        tmp->setDataBufferRapidBlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }


    return;
}











///////////////////////////////////////////////////////////////////////////////













void Pico::runBlock()
{

    // Make the pico ready! Afterwards wait until the trigger is fired and
    // the data is collected.
    m_status = ps6000RunBlock(
                m_handle,
                m_data_current->val_preTrigger,
                m_data_current->val_postTrigger,
                m_data_current->val_timebase,
                m_data_current->val_oversample,
                &m_timeIndisposedMS,
                m_startIndex,
                nullptr,
                nullptr
            );

    checkStatus();


    // Now, check if data taking is done!
    int16_t     ready = 0;
//    int16_t*    ready = nullptr;


// Wait until data collection is done the ready pointer changes its value // when this is the case. System can not be triggered for a long time, thats why it
    // can take some time!
    // \todo Here a stop switch would be imlementable!
    while( !ready )
    {
        m_status = ps6000IsReady( m_handle, &ready);
        checkStatus();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    getValuesBlock();

    return;
}








///////////////////////////////////////////////////////////////////////////////









void Pico::runRapid()
{

    // Make the pico ready! Afterwards wait until the trigger is fired and
    // the data is collected.
    m_status = ps6000RunBlock(
                m_handle,
                m_data_current->val_preTrigger,
                m_data_current->val_postTrigger,
                m_data_current->val_timebase,
                m_data_current->val_oversample,
                &m_timeIndisposedMS,
                m_startIndex,
                nullptr,
                nullptr
            );

    checkStatus();


    // Now, check if data taking is done!
    int16_t     ready = 0;
//    int16_t*    ready = nullptr;



    // Wait until data collection is done the ready pointer changes its value
    // when this is the case. System can not be triggered for a long time, thats why it
    // can take some time!
    // \todo Here a stop switch would be imlementable!
    while( !ready )
    {
        m_status = ps6000IsReady( m_handle, &ready);
        checkStatus();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    getValuesRapid( m_startIndex, m_data_current->loops_inter );

    return;
}









///////////////////////////////////////////////////////////////////////////////








void Pico::runIntermediate()
{
    // trigger once on every channel to get the data
    for( int i = 0; i < 4; ++i )
    {
        if( getCh(i)->getEnabled() < 1 ) continue;

//        // first, run setReadyRapid in a modified way again
//        // turn all channels of only the needed one
//        for( auto& tmp : *m_channels )
//        {
//            if( tmp->getChNo() == m_channels->at( i )->getChNo() )
//            {
//                tmp->setChannel( false, true );
//            }
//            else tmp->setChannel( false, false );
//        }
//        setMemorySegments( m_data_current->loops_inter );
//        std::cout << "setNoOfCaptures...\n";
//        setNoOfCaptures( m_data_current->loops_inter );
//        std::cout << "getTimebase...\n";
//        getTimebase();
//        std::cout << "setTrigger...\n";
        setTrigger_Simple( i+1 );
//        std::cout << "beforeDataBuffer...\n";

        for( auto& tmp : *m_channels )
        {
            if( tmp->getChNo() == m_channels->at( i )->getChNo() )
            {
                tmp->setDataBufferIntermediate( true );
            }
            else tmp->setDataBufferIntermediate( false );
        }



//        std::cout << "ps6000RunBlock...\n";
        m_status = ps6000RunBlock(
                    m_handle,
                    m_data_current->val_preTrigger,
                    m_data_current->val_postTrigger,
                    m_data_current->val_timebase,
                    m_data_current->val_oversample,
//                    &m_timeIndisposedMS,
                    nullptr,
                    m_startIndex,
                    nullptr,
                    nullptr
                );

        checkStatus();


        // Now, check if data taking is done!
        int16_t     ready = 0;


        // Wait until data collection is done the ready pointer changes its value
        // when this is the case. System can not be triggered for a long time, thats why it
        // can take some time!
        // \todo Here a stop switch would be imlementable!
        while( !ready )
        {
            m_status = ps6000IsReady( m_handle, &ready);
            checkStatus();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

            unsigned int stopIndex{m_data_current->loops_inter-1};
            getValuesRapid( m_startIndex, stopIndex );


        // DEBUG
        // print all the channel data in each loop
/*         for( auto& tmp : *m_channels )
 *         {
 *             std::cout << "Channel #"<< tmp->getChNo() << "\n";
 *             for( auto& tmp1 : *tmp->getBufferRapid() )
 *             {
 *                 for( auto& tmp2 : *tmp1 )
 *                 {
 *                     std::cout << tmp2 << "  " << std::flush;
 *                 }
 *                 std::cout << "\n";
 *             }
 *         }
 */
    }



    return;
}









///////////////////////////////////////////////////////////////////////////////







void Pico::stop()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    m_status = ps6000Stop(m_handle);
    checkStatus();

    return;
}






///////////////////////////////////////////////////////////////////////////////







void Pico::close()
{
    stop();
    // Wait a bit before closing. Had problems in past without.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    m_status = ps6000CloseUnit(m_handle);

    checkStatus(); 

    return;
}








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

    return;
}






///////////////////////////////////////////////////////////////////////////////






// Sets the Trigger settings.
void    Pico::setTrigger( )
{

    switch( m_data_current->mode_trigger )
    {
        case Utility::Pico_Trigger_Mode::TRIGGER_ADVANCED:
            //! \todo Claws advanced trigger to be implemented!
            setTrigger_Advanced();
            break;
        case Utility::Pico_Trigger_Mode::TRIGGER_SIMPLE:
            // applying the simple trigger
            setTrigger_Simple();
            break;
        default:
            throw PicoException("Wrong Trigger Mode! Can't apply trigger settings.");

    }

    return;
}







///////////////////////////////////////////////////////////////////////////////







void Pico::setTrigger_Simple( int cha  )
{
/*     std::cout << "Trigger is " << m_data_current->data_trigger->enabled << "\n";
 *     std::cout << "Threshold: " << m_data_current->data_trigger->threshold << "\n";
 *     std::cout << "Direction: " << Utility::Pico_EnumToString_thresDir(m_data_current->data_trigger->direction) << "\n";
 *     std::cout << "AutoTrigger: " << m_data_current->data_trigger->autoTriggerTime << "\n";
 */



    /*
     * If cha is specified. The corresponding trigger is enabled on the 
     * corresponding channel.
     */
    switch( cha )
    {
        case 0:
//            std::cout << "setTriggerSimple." << ;
            m_status = ps6000SetSimpleTrigger
                (
                 m_handle,
                 m_data_current->data_trigger->enabled,
                 m_data_current->data_trigger->source,
                 m_data_current->data_trigger->threshold,
                 m_data_current->data_trigger->direction,
                 m_data_current->data_trigger->delay,
                 m_data_current->data_trigger->autoTriggerTime);
            break;
        case 1:
            m_status = ps6000SetSimpleTrigger
                (
                 m_handle,
                 1,
                 PS6000_CHANNEL_A,
                 m_data_current->data_trigger->threshold,
                 m_data_current->data_trigger->direction,
                 m_data_current->data_trigger->delay,
                 m_data_current->data_trigger->autoTriggerTime);
            break;
        case 2:
            m_status = ps6000SetSimpleTrigger
                (
                 m_handle,
                 1,
                 PS6000_CHANNEL_B,
                 m_data_current->data_trigger->threshold,
                 m_data_current->data_trigger->direction,
                 m_data_current->data_trigger->delay,
                 m_data_current->data_trigger->autoTriggerTime);
            break;
        case 3:
            m_status = ps6000SetSimpleTrigger
                (
                 m_handle,
                 1,
                 PS6000_CHANNEL_C,
                 m_data_current->data_trigger->threshold,
                 m_data_current->data_trigger->direction,
                 m_data_current->data_trigger->delay,
                 m_data_current->data_trigger->autoTriggerTime);
            break;
        case 4:
            m_status = ps6000SetSimpleTrigger
                (
                 m_handle,
                 1,
                 PS6000_CHANNEL_D,
                 m_data_current->data_trigger->threshold,
                 m_data_current->data_trigger->direction,
                 m_data_current->data_trigger->delay,
                 m_data_current->data_trigger->autoTriggerTime);
            break;
        default:
            throw PicoException("This channel is not available for the trigger");


    }
    checkStatus();

    return;
}








///////////////////////////////////////////////////////////////////////////////















void Pico::setTrigger_Advanced()
{

}










///////////////////////////////////////////////////////////////////////////////






void Pico::setChannels()
{
    for( auto& tmp : *m_channels )
    {

        // configure the channel with the fresh data and check if it
        // was successful. Data is loaded before
        m_status = tmp->setChannel();

        checkStatus();
    }
     return;
}










///////////////////////////////////////////////////////////////////////////////








void    Pico::getTimebase()
{
    // create a counter to check how often the pico is asked for the correct value
    uint32_t counter{0};
    uint32_t counterMax = 5000;

    // give the membervariable a non physicsal value to see if it changes
    m_timeInterval_ns = -1.f;

    // on exit, the maximum number of samples available.
    uint32_t maxSamples{0};

    while( m_timeInterval_ns < 0 && counter < counterMax )
    {
        m_status = ps6000GetTimebase2(
                        m_handle,
                        m_data_current->val_timebase,
                        m_buffer_data_size,
                        &m_timeInterval_ns,
                        0,
                        &maxSamples,
                        m_startIndex                        
                );


        checkStatus();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        ++counter;
    }



    // check if the while loop finished properly
    if( m_timeInterval_ns < 0 || counter > counterMax -1 )
    {
        std::cout << "TimeInterval: " << m_timeInterval_ns << std::endl;
        throw PicoException("Demanded sampling interval not available!");
    }

    // check if the maxSamples variable is large enough
    // since it is equal for all channels, this caluclation should be fine
    if( maxSamples < ( m_noChannelsEnabled *  m_buffer_data_size ) )
    {
        
        throw PicoException("Demanded buffer is too large! Pico cannot allocate it!");
    }

    checkStatus();

    return;
}







///////////////////////////////////////////////////////////////////////////////






void    Pico::getValuesBlock()
{

    

    uint32_t noOfSamplesReturned = m_buffer_data_size;


    m_status = ps6000GetValues(
                    m_handle,
                    m_startIndex,
                    &noOfSamplesReturned,
                    m_data_current->val_downSampleRatio,
                    m_data_current->val_downSampleRatioMode,
                    m_segmentIndex,
                    &m_overflow
            );

    checkStatus();

    if( noOfSamplesReturned != m_buffer_data_size )
    {
        throw PicoException("# of samples returned unequal to # of acquired samples!");
    }

    return;
}








///////////////////////////////////////////////////////////////////////////////








void    Pico::getValuesRapid( uint32_t& startIndex, uint32_t& lastIndex )
{

    uint32_t noOfSamplesReturned = m_buffer_data_size;

    int16_t overflow[lastIndex+1]{0};
    m_status = ps6000GetValuesBulk(
                    m_handle,
                    &noOfSamplesReturned,
                    startIndex,
                    lastIndex,
                    m_data_current->val_downSampleRatio,
                    m_data_current->val_downSampleRatioMode,
                    overflow
            );

    checkStatus();

    if( noOfSamplesReturned != m_buffer_data_size )
    {
        throw PicoException(
                "# of samples returned unequal to # of acquired samples!");
    };

    return;
}








///////////////////////////////////////////////////////////////////////////////





void    Pico::setMemorySegments( uint32_t nSegments )
{
    m_status = ps6000MemorySegments(
                    m_handle,
                    nSegments,
                    &m_noMemoryMaxSamples
            );

    checkStatus();

    return;
}








///////////////////////////////////////////////////////////////////////////////








void    Pico::setNoOfCaptures( uint32_t nCaptures )
{
    m_status = ps6000SetNoOfCaptures(
                    m_handle,
                    nCaptures
                    );
    checkStatus();

    return;
}









///////////////////////////////////////////////////////////////////////////////







void Pico::checkStatus()
{
    std::string output;
    switch (m_status){
        case 0:
            break;
        case 1:
            output = "An attempt has been made to open more than\
                    <API>_MAX_UNITS";
            break;
        case 2:
            output = "Not enough memory could be allocated on the host\
                    machine.";
            break;
        case 3:
            output = "No Pico Technology device could be found.";
            break;
        case 4:
            output = "Unable to download firmware.";
            break;
        case 5:
            output = "The driver is busy opening a device.";
            break;
        case 6:
            output = "An unspecified failure occurred.";
            break;
        case 7:
            output = "The PicoScope is not responding to commands from the PC.";
            break;
        case 8:
            output = "The configuration information in the PicoScope\
                    is corrupt or missing.";
            break;
        case 9:
            output = "The picopp.sys file is too old to be used with\
                    the device driver.";
            break;
        case 10:
            output = "The EEPROM has become corrupt, so the device \
                    will use a default setting.";
        case 11:
            output = "The operating system on the PC is not supported \
                    by this driver.";
            break;
        case 12:
            output = "There is no device with the handle value passed.";
            break;
        case 13:
            output = "A parameter value is not valid.";
            break;
        case 14:
            output = "The timebase is not supported or is invalid.";
            break;
        case 15:
            output = "The voltage range is not supported or is invalid.";
            break;
        case 16:
            output = "The channel number is not valid on this device or no channels have been set.";
            break;
        case 17:
            output = "The channel set for a trigger is not available on this device.";
            break;
        case 18:
            output = "The channel set for a condition is not available on this device.";
            break;
        case 19:
            output = "The device does not have a signal generator.";
            break;
        case 20:
            output = "Streaming has failed to start or has stopped without user request.";
            break;
        case 21:
            output = "Block failed to start - a parameter may have been set wrongly.";
            break;
        case 22:
            output = "A parameter that was required is NULL.";
            break;
        case 23:
            output = "The current functionality is not available while using ETS capture mode.";
            break;
        case 24:
            output = "No data is available from a run block call.";
            break;
        case 25:
            output = "The buffer passed for the information was too small.";
            break;
        case 26:
            output = "ETS is not supported on this device.";
            break;
        case 27:
            output = "The auto trigger time is less than the time it\
                    will take to collect the pre-trigger data.";
            break;
        case 28:
            output = "The collection of data has stalled as unread \
                    data would be overwritten.";
            break;
        case 29:
            output = "Number of samples requested is more than \
                    available in the current memory segment.";
            break;
        case 30:
            output = "Not possible to create number of segments requested.";
            break;
        case 31:
            output = "A null pointer has been passed in the trigger \
                    function or one of the parameters is out of range.";
            break;
        case 32:
            output = "One or more of the hold-off parameters are out of range.";
            break;
        case 33:
            output = "One or more of the source details are incorrect.";
            break;
        case 34:
            output = "One or more of the conditions are incorrect.";
            break;
        case 35:
            output = " The driver's thread is currently in the \
                    <API>Ready callback function and therefore the action cannot be carried out.";
            break;
        case 36:
            output = "Data is unavailable because a run has not been completed.";
            break;
        case 37:
            output = "The memory segment index is out of range.";
            break;
        case 38:
            output = "The device is busy so data cannot be returned yet.";
            break;
        case 39:
            output = "The start time to get stored data is out of range.";
            break;
        case 40:
            output = "The information number requested is not a valid number.";
            break;
        case 41:
            output = "The handle is invalid so no information is \
                    available about the device. Only PICO_DRIVER_VERSION \
                    is available.";
            break;
        case 42:
            output = "The sample interval selected for streaming is out of range.";
            break;
        case 43:
            output = "ETS is set but no trigger has been set. A trigger setting is required for ETS.";
            break;
        case 44:
            output = "Driver cannot allocate memory.";
            break;
        case 45:
            output = "Incorrect parameter passed to the signal generator.";
            break;
        case 46:
            output = "Conflict between the shots and sweeps parameters sent to the signal generator.";
            break;
        case 47:
            output = "A software trigger has been sent but the trigger source is not a software trigger.";
            break;
        case 48:
            output = "An <API>SetTrigger call has found a conflict \
                    between the trigger source and the AUX output enable.";
            break;
        case 49:
            output = "ETS mode is being used and AUX is set as an input.";
            break;
        case 50:
            output = "Attempt to set different EXT input thresholds \
                    set for signal generator and oscilloscope trigger.";
            break;
        case 51:
            output = "An <API>SetTrigger... function has set AUX as an \
                    output and the signal generator is using it as a trigger.";
            break;
        case 52:
            output = "The combined peak to peak voltage and the analog \
                    offset voltage exceed the maximum voltage the signal \
                    generator can produce.";
            break;
        case 53:
            output = "NULL pointer passed as delay parameter.";
            break;
        case 54:
            output = "The buffers for overview data have not been set \
                    while streaming.";
            break;
        case 55:
            output = "The analog offset voltage is out of range.";
            break;
        case 56:
            output = "The analog peak-to-peak voltage is out of range.";
            break;
        case 57:
            output = "A block collection has been cancelled.";
            break;
        case 58:
            output = "The segment index is not currently being used.";
            break;
        case 59:
            output = "The wrong GetValues function has been called for the collection mode in use.";
            break;
        case 60:
            output = "PICO_GET_VALUES_INTERRUPTED";
            break;
        case 61:
            output = "The function is not available.";
            break;
        case 62:
            output = "The aggregation ratio requested is out of range.";
            break;
        case 63:
            output = "Device is in an invalid state.";
            break;
        case 64:
            output = "The number of segments allocated is fewer than the number of captures requested.";
            break;
        case 65:
            output = "A driver function has already been called and \
                    not yet finished. Only one call to the driver can be made \
                    at any one time.";
            break;
        case 66:
            output = "Not used. PICO_RESERVED";
            break;
        case 67:
            output = "An invalid coupling type was specified in <API>SetChannel.";
            break;
        case 68:
            output = "An attempt was made to get data before a data buffer was defined.";
            break;
        case 69:
            output = "The selected downsampling mode (used for data reduction) is not allowed.";
            break;
        case 70:
            output = "Aggregation was requested in rapid block mode.";
            break;
        case 71:
            output = "An invalid parameter was passed to <API>SetTriggerChannelProperties.";
            break;
        case 72:
            output = "The driver was unable to contact the oscilloscope.";
            break;
        case 73:
            output = "Resistance-measuring mode is not allowed in conjunction with the specified probe.";
            break;
        case 74:
            output = "The device was unexpectedly powered down.";
            break;
        case 75:
            output = "A problem occurred in <API>SetSigGenBuiltIn or <API>SetSigGenArbitrary.";
            break;
        case 76:
            output = "FPGA not successfully set up.";
            break;
        case 77:
            output = "PICO_POWER_MANAGER";
            break;
        case 78:
            output = "An impossible analog offset value was specified in <API>SetChannel.";
            break;
        case 79:
            output = "There is an error within the device hardware.";
            break;
        case 80:
            output = "There is an error within the device hardware.";
            break;
        case 81:
            output = "Unable to configure the signal generator.";
            break;
        case 82:
            output = "The FPGA cannot be initialized, so unit cannot be opened.";
            break;
        case 83:
            output = "The frequency for the external clock is not \
                    within 15\% of the nominal value.";
            break;
        case 84:
            output = "The FPGA could not lock the clock signal.";
            break;
        case 85:
            output = "You are trying to configure the AUX input as \
                    both a trigger and a reference clock.";
            break;
        case 86:
            output = "You are trying to congfigure the AUX input as \
                    both a pulse width qualifier and a reference clock.";
            break;
        case 87:
            output = "The requested scaling file cannot be opened.";
            break;
        case 88:
            output = "The frequency of the memory is reporting incorrectly.";
            break;
        case 89:
            output = "The I2C that is being actioned is not responding to requests.";
            break;
        case 90:
            output = "There are no captures available and therefore no data can be returned.";
            break;
        case 91:
            output = "The number of trigger channels is greater than 4,\
                    except for a PS4824 where 8 channels are allowed for \
                    rising/falling/rising_or_falling trigger directions.";
            break;
        case 92:
            output = "When more than 4 trigger channels are set on a \
                    PS4824 and the direction is out of range.";
            break;
        case 93:
            output = "When more than 4 trigger channels are set and \
                    their trigger condition states are not <API>_CONDITION_TRUE.";
            break;
        case 94:
            output = "The capture mode the device is currently running in does not support the current request.";
            break;
        case 95:
            output = "PICO_GET_DATA_ACTIVE";
            break;

        // Stopping here, but there are more cases! Puh!
        default:
            output = "Error code: " + std::to_string(m_status);
            break;
    }

    // throw exception only when string is longer than zero
    if ( output.size() != 0 )
    {
        throw PicoException(output);
    }


    return;
}








///////////////////////////////////////////////////////////////////////////////







void Pico::turnOffUnneeded()
{
    m_status = ps6000SetEts(
            m_handle,
            PS6000_ETS_OFF,
            0,
            0,
            nullptr
            );

    checkStatus();

    return;
}









///////////////////////////////////////////////////////////////////////////////
//
//              END private member functions
//
///////////////////////////////////////////////////////////////////////////////
