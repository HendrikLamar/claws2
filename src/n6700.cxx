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
    
/*         const std::vector< std::string > Channels{"Ch1", "Ch2", "Ch3", "Ch4"};
 * 
 *         //! Vector containing all possible settings per channel.
 *         const std::vector< std::string > Settings{"Active", "CurrLimit", "Volt"};
 * 
 *         //! Vector containing all possible modes (e.g. High_Gain, Low_Gain).
 *         const std::vector< std::string > Gains{"High_Gain", "Low_Gain"};
 */
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

//    std::string rootVolt = 



    


}



void N6700::turnChannelsOnOff( bool tmp )
{

    // first check which channels need to be turned on, usually its every channel
    std::string strtmp;
    
    if ( m_database->N6700_getChannels().getKey("High_Gain", "Ch1", "Active") == "true" )
    {
        strtmp += "1";
    }
    else if ( m_database->N6700_getChannels().getKey("High_Gain", "Ch2", "Active") == "true" )
    {
        strtmp += "2";
    }
    else if ( m_database->N6700_getChannels().getKey("High_Gain", "Ch3", "Active") == "true" )
    {
        strtmp += "3";
    }
    else if ( m_database->N6700_getChannels().getKey("High_Gain", "Ch4", "Active") == "true" )
    {
        strtmp += "4";
    }



    // second, add a ',' between each channel number
    //
    std::string tcmd;

    for ( std::string::iterator itB = strtmp.begin(); 
            itB != strtmp.end(); ++itB )
    {
       if ( itB == strtmp.end() - 1 )
       {
            tcmd += *itB;
            break;
       }
        
       tcmd += *itB;
       tcmd += ",";

    }



    // third, assemply the final command and send it to the PSU
    //
    std::string cmd = "OUTP ON, (@" + tcmd + ")";

    if ( tmp )
    {
        setCommand(cmd);
    }

    // if channels should be turned off, turn all of them off
    //
    else setCommand("OUTP OFF, (@1:4)");

    return;
}



std::vector< double > N6700::getVolt()
{
    setCommand("MEAS:VOLT? (@1:4)");
    
    std::string tmp = getAnswer();

    std::vector< double > output = splitStringbyComma( tmp );


    return output;

}



std::vector< double > N6700::getCurr()
{
    setCommand("MEAS:CURR? (@1:4)");
    
    std::string tmp = getAnswer();

    std::vector< double > output = splitStringbyComma( tmp );


    return output;

}
