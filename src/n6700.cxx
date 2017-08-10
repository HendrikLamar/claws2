// =====================================================================================
// 
//       Filename:  n6700.cxx
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  09.08.2017 14:13:30
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================


#include "n6700.h"
#include "clawsException.h"


///////////////////////////////////////////////////////////////////////////////


N6700::N6700(std::string ipAdress, std::string identity, unsigned short port) :
    SCPI(ipAdress, port),
    m_ID(identity)
{
    initSocket();
    checkDevice();
};


N6700::~N6700()
{
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////



void N6700::checkDevice()
{
    openSocket();
    setCommand("*IDN?");
    if( m_ID != getAnswer() )
    {
        throw SCPIException("You connected to the wrong device! Is the IP and Identity parameter correct?");
    }
    closeSocket();
}

