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
                    new Channel( PS6000_CHANNEL_A, &m_handle, m_picoData, m_picoData->dataIntermediate)
                }
            )
{

    Channel::
}




///////////////////////////////////////////////////////////////////////////////







///////////////////////////////////////////////////////////////////////////////
//
//              END Constructor and Destructor
//
///////////////////////////////////////////////////////////////////////////////
