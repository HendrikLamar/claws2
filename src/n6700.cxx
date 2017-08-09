// =====================================================================================
// 
//       Filename:  n6700.cxx
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  09.08.2017 10:23:50
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


KN6700::KN6700(std::string ipAdress, std::string identity, unsigned short port) :
    SCPI(ipAdress, port),
    m_ID(identity)
{
    initSocket();
    checkDevice();
};


KN6700::~KN6700()
{
};


///////////////////////////////////////////////////////////////////////////////



void KN6700::checkDevice()
{
    openSocket();
    setCommand("*IDN?");
    if( m_ID != getAnswer() )
    {
        throw SCPIException("You connected to the wrong device! Is the IP and Identity parameter correct?");
    }
    closeSocket();
}


void KN6700::K_readIni()
{
    std::vector<std::string> keys { }
    
};
