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

        //! Defines which channel number the current channel has at the pico.
        const PS6000_CHANNEL                m_channel;

        //! Holds the pico handle value.
        const int16_t*                      m_handle;
        
//        //! Holds the database to call the settings.
//        const Database*                     m_database;

        //! Holds the pico settings struct.  
        const Utility::Pico_Data_Pico*      m_picoData; 

        //! Holds the channel settings struct
        const Utility::Pico_Data_HL_Gain*   m_channelData;


        /////////////////////////////////////////////////////////////////////// 
        

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
    public:
        ///////////////////////////////////////////////////////////////////////
        /*
         *! Contructor
         */
        Channel( 
                PS6000_CHANNEL channel, 
                int16_t* handle, 
                Utility::Pico_Data_Pico* picoData,
                Utility::Pico_Data_HL_Gain* channelData
               );
        ///////////////////////////////////////////////////////////////////////
        /*
         *! Destructor
         */
        ~Channel();
        

        //! Returns the channel buffer.
        std::vector< int16_t >*     getBuffer();

        //! Loads the settings stored in the database to the channel.
        void    loadConfig();

        //! Tells the Pico where to store the data for this channel.
        PICO_STATUS     setDataBuffer();

        //! Configures the channel with the loaded data.
        PICO_STATUS     setChannel();

    

};

#endif // PICO_CHANNEL_H


