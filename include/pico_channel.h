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
        
        //! Const pointer to a variable set of pico data.
        Utility::Pico_Data_Pico* const      m_picoData; 

        


        ///////////////////////////////////////////////////////////////////////
        //
        //                  END const member variables
        //
        ///////////////////////////////////////////////////////////////////////












        ///////////////////////////////////////////////////////////////////////
        //
        //                 START settings 
        //
        ///////////////////////////////////////////////////////////////////////
        

        //! Pointer to the current run mode data. The pointer changes a lot, 
        //! e.g. to intermediate, high gain, low gain.
        Utility::Pico_Data_HL_Gain*         m_channelData;


        // single channel settings
        PS6000_COUPLING                 m_coupling;
        int16_t                         m_enabled;
        float                           m_analogueOffset;
        PS6000_RANGE                    m_range;
        PS6000_BANDWIDTH_LIMITER        m_bandwidth{PS6000_BW_FULL};

        
        //! This vector stores all the data coming from the pico.
        std::vector< int16_t >*         m_dataBuffer;
        uint32_t                        m_dataBufferSize;

        void        calcDataBufferSize();

        // reserve a data buffer of that size per channel when initializing the pico
        // 500 000 000 is 2GS divided by 4 channels
        uint32_t        m_bufferSizeReserve{500000000};
        


        ///////////////////////////////////////////////////////////////////////
        //
        //                 END settings 
        //
        ///////////////////////////////////////////////////////////////////////
        




    public:
        


        ///////////////////////////////////////////////////////////////////////
        /*
         *! Constructor
         */
        Channel(PS6000_CHANNEL channel, 
                int16_t* handle, 
                Utility::Pico_Data_Pico* picoData,
                Utility::Pico_Data_HL_Gain* channelData
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
        std::vector< int16_t >*     getBuffer();

        //! Sets the run mode. This function needs to be called before loadConfig(),
        //! setDataBuffer() and setChannel().
        //! Possible arguments defined in Utility::ClawsGain.
        void    setGainMode( Utility::ClawsGain& gain );


        //! Loads the settings stored in the database to the channel.
        void    loadConfig();

        //! Tells the Pico where to store the data for this channel.
        PICO_STATUS     setDataBuffer();

        //! Configures the channel with the current loaded data. You might want
        //! to update the data first with loadConfig().
        PICO_STATUS     setChannel();




        ///////////////////////////////////////////////////////////////////////
        //
        //                END public member functions 
        //
        ///////////////////////////////////////////////////////////////////////
    

};

#endif // PICO_CHANNEL_H


