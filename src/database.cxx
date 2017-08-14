// =====================================================================================
// 
//       Filename:  database.cxx
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  25.07.2017 15:42:55
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include "database.h"
#include "utility.h"
#include "n6700.h"
#include "clawsException.h"
#include "readini.h"

#include <iostream>
#include <string>

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////


Database::Database() :
    ReadIni(),
    m_stopSwitch(false)
{
    ///////////////////////////////////////////////////////////////////////////
    //          Powersupply
    ///////////////////////////////////////////////////////////////////////////

};



Database::~Database(){};



///////////////////////////////////////////////////////////////////////////////
//          START Database only stuff
///////////////////////////////////////////////////////////////////////////////


void Database::setStop(bool switcher)
{
    m_stopSwitch = switcher;
}


///////////////////////////////////////////////////////////////////////////////


bool Database::getStop()
{
    return m_stopSwitch;
}


///////////////////////////////////////////////////////////////////////////////
//          END Database only stuff
///////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
//          START Database PowerSupply (KPS) stuff
///////////////////////////////////////////////////////////////////////////////


// reads in high AND low gain mode everytime it is called.
void Database::N6700_readChSet() 
{

    for ( size_t ii = 0; ii < m_KPS_mode.size(); ++ii ) {
        for ( size_t jj = 0; jj < m_KPS_channels.size(); ++jj ) {
            for ( size_t kk = 0; kk < m_KPS_settings.size(); ++kk ) {

                // assembling of the key for every loop iteration
                std::string key = 
                    m_KPS_mode.at(ii) + "." + m_KPS_channels.at(jj) + 
                    m_KPS_settings.at(kk);
                
                // reading the 
                m_n6700_channels.setKey( 
                        m_KPS_mode.at(ii), 
                        m_KPS_channels.at(jj), 
                        m_KPS_settings.at(kk), 
                        ReadIni::getKey< std::string >( 
                            m_initstruct.m_filePowerSupply, 
                            key)
                        );

//                std::cout << "Key: " << key << "\n" << ReadIni::getKey< std::string >( 
//                            m_initstruct.m_filePowerSupply, 
//                            key) << std::endl;

            };
        };
    };
    
    return;
}




void Database::N6700_readPSConf()
{
    std::string root = "Connect.";


    std::string ip = root + "Server";

    m_n6700_connect.ip = ReadIni::getKey< std::string > (
            m_initstruct.m_filePowerSupply, ip);



    std::string port = root + "Port";

    m_n6700_connect.port = ReadIni::getKey< std::string > (
            m_initstruct.m_filePowerSupply, port);



    std::string identity = root + "Identity";

    m_n6700_connect.id = ReadIni::getKey< std::string > (
            m_initstruct.m_filePowerSupply, identity);




    return;

}




KPS_Channels Database::N6700_getChannels()
{
    return m_n6700_channels;
}



Utility::N6700_connect Database::N6700_getConnect()
{
    return m_n6700_connect;
}


///////////////////////////////////////////////////////////////////////////////
//          END Database PowerSupply (KPS) stuff
///////////////////////////////////////////////////////////////////////////////
