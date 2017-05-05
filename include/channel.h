// =====================================================================================
// 
//       Filename:  channel.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02.05.2017 13:42:53
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#ifndef CHANNEL_H
#define CHANNEL_H

#include <libps6000-1.4/PicoStatus.h>
#include <libps6000-1.4/ps6000Api.h>

#include <iostream>
#include <vector>


/*! The Channel class to handle a channel on a Pico*/

class Channel
{
    private:
        // pico settings needed by channel
        const int16_t               *m_handle;
        int8_t                m_serial;
//        const int8_t                m_serial;

        // single channel only setting        
//        const PS6000_CHANNEL        m_channel;          // no initialization value needed
        const PS6000_CHANNEL              m_channel;          // no initialization value needed
        PS6000_COUPLING             m_coupling          {PS6000_DC_50R};
        int16_t                     m_enabled           {false};
        float                       m_analogueOffset    {0.f};
        PS6000_RANGE                m_range             {PS6000_1V};
        PS6000_BANDWIDTH_LIMITER    m_bandwidth         {PS6000_BW_FULL};

        const uint32_t              m_bufferSize;
        std::vector<int16_t>*       m_buffer;


        ///////////////////////////////////////////////////////////////////////
        /*
         *! Hidde set channel function for internal m_setChannel 
         */
        PICO_STATUS     m_setCh();
        ///////////////////////////////////////////////////////////////////////


    public:
        ///////////////////////////////////////////////////////////////////////
        /*
         *! Contructor
         */
        Channel(int16_t *handle, PS6000_CHANNEL channel, uint32_t bufferSize);
        ///////////////////////////////////////////////////////////////////////
        
        ~Channel(){};


        ///////////////////////////////////////////////////////////////////////
        /*
         * ReturnFunctions - Return Information about this instance.
         */

        //! Returns the corresponding handle value
        int16_t                     returnHandle() const;
        //! Returns the steered channel.
        PS6000_CHANNEL              returnChannel() const;
        //! Return is the channel is enabled or not.
        int16_t                     returnEnabled() const;
        //! Returns the set offset.
        float                       returnOffset() const;
        //! Returns the set coupling.
        PS6000_COUPLING             returnCoupling() const;
        //! Returns the range.
        PS6000_RANGE                returnRange() const;
        //! Returns the bandwidth.
        PS6000_BANDWIDTH_LIMITER    returnBandwidth() const;
        
        //! Returns the channel buffer.
        std::vector<int16_t>*       getBuffer();
        ///////////////////////////////////////////////////////////////////////



        ///////////////////////////////////////////////////////////////////////
        /*
         * SetFunctions - To set the channel preferences.
         */

        //! Sets status of enabled.
        PICO_STATUS     setEnabled(int16_t enabled);
        //! Sets the offset.
        PICO_STATUS     setOffset(float analogueOffset);
        //! Sets the the coupling.
        PICO_STATUS     setCoupling(PS6000_COUPLING coupling);
        //! Sets the range.
        PICO_STATUS     setRange(PS6000_RANGE range);
        //! Sets the bandwidth.
        PICO_STATUS     setBandwidth(PS6000_BANDWIDTH_LIMITER bandwidth);

        //! Sets several often needed channel preferences at once.
        PICO_STATUS     setChannel(\
                            int16_t                     enabled,            \
                            PS6000_RANGE                range,              \
                            float                       analogueOffset
                            );
        //! Sets all possible channel preferences.
        PICO_STATUS     setChannel(\
                            int16_t                     enabled,            \
                            PS6000_RANGE                range,              \
                            float                       analogueOffset,     \
                            PS6000_COUPLING             coupling,           \
                            PS6000_BANDWIDTH_LIMITER    bandwidth
                            );


        //! Tells the Pico where to store the data for this channel. The values
        //! needed are already set before or come from the class Pico.
        PICO_STATUS     setDataBuffer();
        
        ///////////////////////////////////////////////////////////////////////

        
        ///////////////////////////////////////////////////////////////////////
        /*
         * Function to print channel informations.
         */

        //! Overloading of the operator<< to just use 'cout << channelA << endl;'
        //! printing the information needed.
        friend std::ostream& operator<< (std::ostream &out, const Channel &channel);

        ///////////////////////////////////////////////////////////////////////
};

#endif
