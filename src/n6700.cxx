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

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

///////////////////////////////////////////////////////////////////////////////


//N6700::N6700(std::string ipAdress, std::string identity, unsigned short port) :
N6700::N6700(std::shared_ptr<Database> database) :
    SCPI(database->N6700_getConnect().ip, std::stoi(database->N6700_getConnect().port)),
    m_database(database),
    m_ID(m_database->N6700_getConnect().id),
    m_config_high(std::make_shared<Utility::PSU_Config>()),
    m_config_low(std::make_shared<Utility::PSU_Config>())
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
    std::cout << "\n";
    std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    std::cout << "\tPSU initialization...\n";
    std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

    //! \todo Check at which line the execution stops if the PSU is not available!
    openSocket();
    setCommand("*IDN?");
    if( m_ID != getAnswer() )
    {
        throw SCPIException("You connected to the wrong device! Is the IP and Identity parameter correct?");
    }
    else
    {
        std::cout << "\n" << m_ID << "\n";
        std::cout << "\t successful initialized!\n";
        std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
    }
    closeSocket();
}





///////////////////////////////////////////////////////////////////////////////






void N6700::loadConfig()
{

    auto loadHL = [](
            std::shared_ptr<Utility::PSU_Config> config_low,
            std::shared_ptr<Utility::PSU_Config> config_high,
            std::shared_ptr<Database> database,
            Utility::Claws_Gain tgain
            )
    {
        std::string gain;
        std::shared_ptr<Utility::PSU_Config> config;
        switch( tgain )
        {
            case Utility::Claws_Gain::LOW_GAIN:
                gain = "Low_Gain";
                config = config_low;
                break;
            default:
                gain = "High_Gain";
                config = config_high;
                break;
        }
        config->clear();

        std::vector< std::string > channels{"Ch1", "Ch2", "Ch3", "Ch4"};
        std::vector< std::string > attribute{"Power", "CurrLimit", "Volt"};

        boost::property_tree::ptree ptree;
        boost::property_tree::ini_parser::read_ini(
                database->getInitReader()->getInitstruct().PowerSupply.c_str(), 
                ptree);

        std::string fstring;

        int count1{0};
        int count2{0};
        for( auto tmp1 : channels )
        {
            std::shared_ptr<Utility::PSU_Channel> cha{
                std::make_shared<Utility::PSU_Channel>( tmp1, count1)};

            for( auto tmp2 : attribute )
            {
                fstring = gain + "." + tmp1 + tmp2;
                switch( count2 )
                {
                    case 0:
                        cha->powerOnOff = ptree.get<std::string>(fstring);
                        break;
                    case 1:
                        cha->limit_current = ptree.get<float>(fstring);
                        break;
                    case 2:
                        cha->limit_volt = ptree.get<float>(fstring);
                        break;
                    default:
                        throw SCPIException("Unknown PSU readin attribute");
                }

                ++count2;
            }
            config->setCh(cha);
            count2 = 0;
            ++count1;
        }
    };


    loadHL( m_config_low, m_config_high, 
            m_database, Utility::Claws_Gain::LOW_GAIN );
    loadHL( m_config_low, m_config_high, 
            m_database, Utility::Claws_Gain::HIGH_GAIN);

    return;
}







///////////////////////////////////////////////////////////////////////////////












void N6700::sendCommand( std::string cmd )
{
    openSocket();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    setCommand( cmd );
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    closeSocket();

    return;
}





///////////////////////////////////////////////////////////////////////////////





void N6700::sendConf( Utility::Claws_Gain HIGH_LOW_GAIN )
{

    switch( HIGH_LOW_GAIN )
    {
        case Utility::Claws_Gain::LOW_GAIN:
            m_config_current = m_config_low;
            break;
        default:
            m_config_current = m_config_high;
    }
    
    std::string cmd;

    std::string rSetVolt = ":VOLT:LEV ";
    std::string rSetCurr = ":CURR:LEV ";
    std::string spacer = ";";
    std::string sCh = "(@"; 
    std::string eCh = ")";
    

    // send voltage settings
    for( unsigned int i = 0; i < m_config_current->getNoOfCh(); ++i )
    {
        cmd += rSetVolt + std::to_string(m_config_current->getCh(i)->limit_volt) 
            + ", " + sCh + std::to_string(i+1) + eCh;
        if( i != m_config_current->getNoOfCh()-1 ) cmd += spacer;
    }
    sendCommand( cmd );


    // send current settings
    cmd = "";
    for( unsigned int i = 0; i < m_config_current->getNoOfCh(); ++i )
    {
        cmd += rSetCurr + 
            std::to_string(m_config_current->getCh(i)->limit_current) 
            + ", " + sCh + std::to_string(i+1) + eCh;
        if( i != m_config_current->getNoOfCh()-1 ) cmd += spacer;
    }
    sendCommand( cmd );


    return;

}





///////////////////////////////////////////////////////////////////////////////







void N6700::start()
{
    std::string tmpCmd{""};

    for( unsigned int i = 0; i < m_config_current->getNoOfCh(); ++i )
    {
        if( !(m_config_current->getCh(i)->powerOnOff.compare("ON")) )
        {
            tmpCmd += std::to_string(i+1);
        }
    }


    // second, add a ',' between each channel number
    //
    std::string tcmd{""};

    for ( std::string::iterator itB = tmpCmd.begin(); 
            itB != tmpCmd.end(); ++itB )
    {
       if ( itB == tmpCmd.end() - 1 )
       {
            tcmd += *itB;
            break;
       }
        
       tcmd += *itB;
       tcmd += ",";

    };

    std::string cmd = "OUTP ON, (@" + tcmd + ")";

    sendCommand(cmd);


    return;
}










///////////////////////////////////////////////////////////////////////////////







void N6700::stop()
{
    std::string cmd = "OUTP OFF, (@1:4)";

    sendCommand(cmd);


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


/* 
 * ///////////////////////////////////////////////////////////////////////////////
 * //
 * // the current implementation of run is only for testing cases. There is no proof
 * // that the wait function in milliseconds are especially needed. 
 * //
 * ///////////////////////////////////////////////////////////////////////////////
 * void N6700::run()
 * {
 *     try{
 *         setConf( Utility::Claws_Gain::LOW_GAIN );
 *     }
 *     catch(SCPIException error)
 *     {
 *         std::cerr << error.what() << std::endl;
 *         return;
 *     }
 *     std::this_thread::sleep_for(std::chrono::milliseconds(50));
 * 
 *     turnChannelsOnOff( true );
 *     std::this_thread::sleep_for(std::chrono::milliseconds(50));
 *     std::vector <double> tmpVolt = getVolt();
 * 
 *     for ( unsigned long ii = 0; ii < tmpVolt.size(); ++ii)
 *     {
 *         std::cout << m_database->N6700_getChannels().Channels.at(ii) << ": " << tmpVolt.at(ii) << "\n";
 *     }
 * 
 *     std::this_thread::sleep_for(std::chrono::seconds(15));
 *     turnChannelsOnOff( false );
 *     std::this_thread::sleep_for(std::chrono::milliseconds(50));
 * 
 * 
 *     setConf( Utility::Claws_Gain::HIGH_GAIN );
 * 
 *     turnChannelsOnOff( true );
 * 
 *     std::this_thread::sleep_for(std::chrono::milliseconds(50));
 *     tmpVolt = getVolt();
 *     for ( unsigned long ii = 0; ii < tmpVolt.size(); ++ii)
 *     {
 *         std::cout << m_database->N6700_getChannels().Channels.at(ii) << ": " << tmpVolt.at(ii) << "\n";
 *     }
 * 
 *     std::this_thread::sleep_for(std::chrono::seconds(15));
 * 
 *     turnChannelsOnOff( false );
 *     std::this_thread::sleep_for(std::chrono::milliseconds(50));
 *     tmpVolt = getVolt();
 *     for ( unsigned long ii = 0; ii < tmpVolt.size(); ++ii)
 *     {
 *         std::cout << m_database->N6700_getChannels().Channels.at(ii) << ": " << tmpVolt.at(ii) << "\n";
 *     }
 * 
 *     return;
 * }
 * 
 */
