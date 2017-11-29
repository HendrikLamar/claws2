// =====================================================================================
// 
//       Filename:  pico_channel.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  18.08.2017 14:31:20
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================


#ifndef PICO_CHANNEL_H
#define PICO_CHANNEL_H


#include <libps6000-1.4/PicoStatus.h>
#include <libps6000-1.4/ps6000Api.h>

#include <vector>

#include "utility.h"
#include "database.h"

/*! The Channel class to handle a channel on a Pico*/
class Channel
{

    private:
        ///////////////////////////////////////////////////////////////////////
        //
        //                  START const member variables
        //
        ///////////////////////////////////////////////////////////////////////

        //! Defines which channel number the current channel has at the pico.
        //! This should never change!
        const PS6000_CHANNEL                m_channel;

        //! Const pointer to a variable handle value.
        //! The handle value is not yet defined in the moment of construction.
        //! Initialization follows later.
        int16_t* const                      m_handle;
        
        //! Pointer to the current run mode data. The pointer changes a lot, 
        //! e.g. to intermediate, high gain, low gain.
        std::shared_ptr<Utility::Pico_Conf_HL_Gain>         m_data;

        std::shared_ptr<Utility::Pico_Conf_Channel>         m_data_channel;


        ///////////////////////////////////////////////////////////////////////
        //
        //                  END const member variables
        //
        ///////////////////////////////////////////////////////////////////////












        ///////////////////////////////////////////////////////////////////////
        //
        //                 START internal variables 
        //
        ///////////////////////////////////////////////////////////////////////
        

/*         // single channel settings
 *         PS6000_COUPLING                 m_coupling;
 *         int16_t                         m_enabled;
 *         float                           m_analogueOffset;
 *         PS6000_RANGE                    m_range;
 *         PS6000_BANDWIDTH_LIMITER        m_bandwidth{PS6000_BW_FULL};
 */

        
        //! data vector to store data coming from physics block mode
        std::shared_ptr<std::vector< int16_t >> m_buffer_block_data;
        uint32_t                                m_buffer_block_size;
        // reserve a data buffer of that size per channel when initializing the pico
        // 500 000 000 is 2GS divided by 4 channels
        uint32_t        m_buffer_block_sizeReserve{500000000};


        // data vector to store data coming from intermediate mode
        std::shared_ptr< std::vector< std::shared_ptr< 
            std::vector< int16_t >>>> m_buffer_rapid;
        uint32_t m_buffer_rapid_size;
        uint32_t m_buffer_rapid_sizeReserver{500};

//        std::shared_ptr<std::vector< int16_t >> m_buffer_current_data;
//        uint32_t*                       m_buffer_current_size;
        // calculates the buffer size by adding pre and posttrigger. needed 
        // as input parameter for several pico functions
        void        calcDataBufferSize( );




        ///////////////////////////////////////////////////////////////////////
        //
        //                 END internal variables 
        //
        ///////////////////////////////////////////////////////////////////////
        




    public:


        ///////////////////////////////////////////////////////////////////////
        /*
         *! Constructor
         */
        Channel(PS6000_CHANNEL channel, 
                int16_t* handle, 
                std::shared_ptr<Utility::Pico_Conf_HL_Gain> dataHLGain
               );
        ///////////////////////////////////////////////////////////////////////
        /*
         *! Destructor
         */
        ~Channel();
        

        ///////////////////////////////////////////////////////////////////////
        //
        //                START public member functions 
        //
        ///////////////////////////////////////////////////////////////////////
        


        //! Returns the channel buffer.
        //! Don't use size() method on this. Use getBufferSize() instead!
        std::shared_ptr<std::vector< int16_t >> getBufferBlock();

        //! Returns the channel buffer for rapid mode.
        //! Don't use size() method on this. Use getBufferSize() instead!
        std::shared_ptr< std::vector< 
            std::shared_ptr< std::vector< int16_t >>>> getBufferRapid();

        //! Returns the size of the current data buffer.
        unsigned int                getBufferSize();

/*         //! Sets the run mode. This function needs to be called before loadConfig(),
 *         //! setDataBuffer() and setChannel().
 *         //! Possible arguments defined in Utility::ClawsGain.
 *         void                        setGainMode( Utility::Claws_Gain& gain );
 */


        //! Loads the settings stored in the database to the channel.
        void                        loadConfig();

        //! Tells the Pico where to store the block mode data for this channel.
        PICO_STATUS                 setDataBufferBlock();

        //! Tells the Pico where to store the rapid block mode for this channel.
        PICO_STATUS                 setDataBufferRapidBlock();

        //! Set the data buffer for this channel to fake or real buffer.
        //! Channel::setDataBufferRapidBlock() must be called before.
        //! true -> Yes, set data buffer.
        //! false -> Set fake buffer.
        PICO_STATUS                 setDataBufferIntermediate( bool yesno );

        //! Configures the channel with the current loaded data. You might want
        //! to update the data first with loadConfig().
        PICO_STATUS                 setChannel(
                                        bool followsSettings = true,
                                        bool ifNotIsEnabled = false );

        //! Returns true if channel is enabled.
        int16_t                     getEnabled();

        //! Returns channel number.
        PS6000_CHANNEL              getChNo();





        ///////////////////////////////////////////////////////////////////////
        //
        //                END public member functions 
        //
        ///////////////////////////////////////////////////////////////////////
    

};

#endif // PICO_CHANNEL_H


