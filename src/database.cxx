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

    KPS_Channels        m_n6700_channels;
    
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


void Database::KPS_readIni( Utility::ClawsGain HIGH_LOW_GAIN )
{
    std::string gain;
    switch ( HIGH_LOW_GAIN )
    {

        case Utility::HIGH_GAIN:
            gain = "Low_Gain";
            break;

        case Utility::LOW_GAIN:
            gain = "Low_Gain";
            break;

        default:
            throw SCPIException("Wrong HIGH_LOW_GAIN input!");

    }

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
                        ReadIni::getKey< std::string >( m_initstruct.m_filePowerSupply, key));
            };
        };
    };
    
    return;
}





///////////////////////////////////////////////////////////////////////////////
//          END Database PowerSupply (KPS) stuff
///////////////////////////////////////////////////////////////////////////////
