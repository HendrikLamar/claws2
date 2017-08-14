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
#include "utility.h"
#include "database.h"

#include <string>

///////////////////////////////////////////////////////////////////////////////


//N6700::N6700(std::string ipAdress, std::string identity, unsigned short port) :
N6700::N6700(Database* database) :
    SCPI(database->N6700_getConnect().ip, std::stoi(database->N6700_getConnect().port)),
    m_database(database),
    m_ID(m_database->N6700_getConnect().id)
{
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


void N6700::setConf( Utility::ClawsGain HIGH_LOW_GAIN )
{
    
    std::string gain;

    switch( HIGH_LOW_GAIN )
    {
        case Utility::LOW_GAIN:
            gain = "Low_Gain";
            break;

        case Utility::HIGH_GAIN:
            gain = "High_Gain";
            break;
    }

    


}



void N6700::setOutput( bool tmp )
{
    if ( tmp )
    {
        setCommand("OUTP ON, (@1:4)");
    }
    else setCommand("OUTP OFF, (@1:4)");

    return;
}
