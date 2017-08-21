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

#include "pico.h"
#include "database.h"

/*! The Channel class to handle a channel on a Pico*/
class Channel
{

    private:

        //! Holds the database to call the settings.
        const Database*                       m_database;

        //! This vector stores all the data coming from the pico.
        std::vector< int16_t >*         m_dataBuffer;

        //! Defines which channel number the current channel has at the pico.
        const PS6000_CHANNEL            m_channel;


        ///////////////////////////////////////////////////////////////////////
};

#endif // PICO_CHANNEL_H


