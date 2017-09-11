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
#include "n6700_channels.h"

#include <string>

#include <iostream>
#include <thread>
#include <chrono>

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
    else
    {
        std::cout << "\nPSU: "  << m_ID << "\n";
        std::cout << " successful initialized!\n";
    }
    closeSocket();
}





///////////////////////////////////////////////////////////////////////////////





void N6700::sendCommand( std::string cmd )
{
    openSocket();
    setCommand( cmd );
    closeSocket();
}





///////////////////////////////////////////////////////////////////////////////





void N6700::setConf( Utility::Claws_Gain HIGH_LOW_GAIN )
{
    std::string gain;

    switch( HIGH_LOW_GAIN )
    {
        case Utility::INTER:
            gain = "High_Gain";
            break;
        case Utility::LOW_GAIN:
            gain = "Low_Gain";
            break;

        case Utility::HIGH_GAIN:
            gain = "High_Gain";
            break;
    }
    
    std::string cmd;

    std::string rSetVolt = ":VOLT:LEV ";
    std::string spacer = ";";
    std::string sCh = "(@"; 
    std::string eCh = ")";
    

    N6700_Channels  tmp_channels = m_database->N6700_getChannels();

    for ( unsigned long ii = 0; ii < tmp_channels.Channels.size(); ++ii )
    {
        cmd += rSetVolt + tmp_channels.getKey(gain, tmp_channels.Channels.at(ii), "Volt")
            + ", " + sCh + std::to_string(ii+1) + eCh;

        // if it is not the last part, add a semicolon as separation pattern
        if ( ii != tmp_channels.Channels.size() - 1 )
        {
            cmd += spacer;
        }
    }

    sendCommand( cmd );


    return;

}






///////////////////////////////////////////////////////////////////////////////






void N6700::turnChannelsOnOff( bool tmp )
{

    // first check which channels need to be turned on, usually its every channel
    std::string strtmp;
    
    if ( m_database->N6700_getChannels().getKey("High_Gain", "Ch1", "Active") == "true" )
    {
        strtmp += "1";
    }
    if ( m_database->N6700_getChannels().getKey("High_Gain", "Ch2", "Active") == "true" )
    {
        strtmp += "2";
    }
    if ( m_database->N6700_getChannels().getKey("High_Gain", "Ch3", "Active") == "true" )
    {
        strtmp += "3";
    }
    if ( m_database->N6700_getChannels().getKey("High_Gain", "Ch4", "Active") == "true" )
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
        sendCommand(cmd);
    }

    // if channels should be turned off, turn all of them off
    //
    else sendCommand("OUTP OFF, (@1:4)");

    return;
}






///////////////////////////////////////////////////////////////////////////////






std::vector< double > N6700::getVolt()
{
    openSocket();
    setCommand("MEAS:VOLT? (@1:4)");
    
    std::string tmp = getAnswer();

    closeSocket();

    std::vector< double > output = splitStringbyComma( tmp );


    return output;

}






///////////////////////////////////////////////////////////////////////////////






std::vector< double > N6700::getCurr()
{
    openSocket();

    setCommand("MEAS:CURR? (@1:4)");
    
    std::string tmp = getAnswer();

    closeSocket();

    std::vector< double > output = splitStringbyComma( tmp );


    return output;

}




///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
//
// the current implementation of run is only for testing cases. There is no proof
// that the wait function in milliseconds are especially needed. 
//
///////////////////////////////////////////////////////////////////////////////
void N6700::run()
{
    try{
        setConf( Utility::LOW_GAIN );
    }
    catch(SCPIException error)
    {
        std::cerr << error.what() << std::endl;
        return;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    turnChannelsOnOff( true );
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::vector <double> tmpVolt = getVolt();

    for ( unsigned long ii = 0; ii < tmpVolt.size(); ++ii)
    {
        std::cout << m_database->N6700_getChannels().Channels.at(ii) << ": " << tmpVolt.at(ii) << "\n";
    }

    std::this_thread::sleep_for(std::chrono::seconds(15));
    turnChannelsOnOff( false );
    std::this_thread::sleep_for(std::chrono::milliseconds(50));


    setConf( Utility::HIGH_GAIN );

    turnChannelsOnOff( true );

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    tmpVolt = getVolt();
    for ( unsigned long ii = 0; ii < tmpVolt.size(); ++ii)
    {
        std::cout << m_database->N6700_getChannels().Channels.at(ii) << ": " << tmpVolt.at(ii) << "\n";
    }

    std::this_thread::sleep_for(std::chrono::seconds(15));

    turnChannelsOnOff( false );
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    tmpVolt = getVolt();
    for ( unsigned long ii = 0; ii < tmpVolt.size(); ++ii)
    {
        std::cout << m_database->N6700_getChannels().Channels.at(ii) << ": " << tmpVolt.at(ii) << "\n";
    }

    return;
}

