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

#include "pico.h"
#include "database.h"
#include "utility.h"
#include "n6700.h"
#include "clawsException.h"
#include "readini.h"

#include <boost/property_tree/exceptions.hpp>

#include <iostream>
#include <string>

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////


Database::Database() try :
    m_stopSwitch(false),
    m_initReader(new ReadIni()),
    m_N6700_Channels(new N6700_Channels()),
    m_picoData( nullptr ),
    m_picos( nullptr )

{
    ///////////////////////////////////////////////////////////////////////////
    //          Powersupply
    ///////////////////////////////////////////////////////////////////////////

//    Pico_init();

    N6700_readPSUConf();
    N6700_readChSet();

    std::cout << m_initReader->getInitstruct().m_filePowerSupply << std::endl;

}
catch(...)
{
    std::cerr << " Construction failed!\n";
}



Database::~Database()
{
    delete m_initReader;
    m_initReader = nullptr;

    delete m_N6700_Channels;
    m_N6700_Channels = nullptr;

    delete m_picoData;
    m_picoData = nullptr;
    
    delete m_picos;
    m_picos = nullptr;
};



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

    for ( size_t ii = 0; ii < m_N6700_Channels->Gains.size(); ++ii ) {
        for ( size_t jj = 0; jj < m_N6700_Channels->Channels.size(); ++jj ) {
            for ( size_t kk = 0; kk < m_N6700_Channels->Settings.size(); ++kk ) {

                // assembling of the key for every loop iteration
                std::string key = 
                    m_N6700_Channels->Gains.at(ii) + "." + 
                    m_N6700_Channels->Channels.at(jj) + 
                    m_N6700_Channels->Settings.at(kk);
                
                // reading the ini-file and storing it in
                m_N6700_Channels->setKey( 
                        m_N6700_Channels->Gains.at(ii), 
                        m_N6700_Channels->Channels.at(jj), 
                        m_N6700_Channels->Settings.at(kk), 
                        m_initReader->getKey< std::string >( 
                            m_initReader->getInitstruct().m_filePowerSupply, 
                            key)
                        );

//                std::cout << "Key: " << key << "\n" << m_initReader->getKey< std::string >( 
//                            m_initReader->getInitstruct()->m_filePowerSupply, 
//                            key) << std::endl;

            };
        };
    };
    
    return;
}




void Database::N6700_readPSUConf()
{
    std::string root = "Connect.";

    std::cout << "File :" << m_initReader->getInitstruct().m_filePowerSupply << "\n";

    std::string ip = root + "Server";

    m_n6700_connect.ip = m_initReader->getKey< std::string > (
            m_initReader->getInitstruct().m_filePowerSupply, ip);



    std::string port = root + "Port";

    m_n6700_connect.port = m_initReader->getKey< std::string > (
            m_initReader->getInitstruct().m_filePowerSupply, port);



    std::string identity = root + "Identity";

    m_n6700_connect.id = m_initReader->getKey< std::string > (
            m_initReader->getInitstruct().m_filePowerSupply, identity);




    return;

}




N6700_Channels Database::N6700_getChannels() const
{
    return *m_N6700_Channels;
}



Utility::N6700_connect Database::N6700_getConnect() const
{
    return m_n6700_connect;
}


///////////////////////////////////////////////////////////////////////////////
//          END Database PowerSupply (KPS) stuff
///////////////////////////////////////////////////////////////////////////////



















///////////////////////////////////////////////////////////////////////////////
//                      START Pico stuff
///////////////////////////////////////////////////////////////////////////////



void Database::Pico_init()
{
    // check if the picoData pointer is empty. If yes allocate new, if no delete first.
    if ( !m_picoData )
    {
        delete m_picoData;
        m_picoData = new std::vector< Utility::Pico_Data_Pico >;
    }
    else m_picoData = new std::vector< Utility::Pico_Data_Pico >;
    // check if the pico pointer is empty. If yes allocate new, if no delete first.
    if ( !m_picos )
    {
        delete m_picos;
        m_picos = new std::vector< Pico >;
    }
    else m_picos = new std::vector< Pico >;
    

    std::vector <std::string > serials;
    std::string sName = "Pico_Initializer.pico_";
    std::string eName = "_serial";


    std::vector<int> initCounter;       //! Counts which serials could be initialized.

    // read all serials from the ini file. Four at maximum.
    for ( int i = 0 ; i < 4 ; ++i )
    {
        // create key
        std::string key = sName + std::to_string(i+1) + eName;
        std::string tmp;

        // try if there are all serial given in the picoInit.ini file.
        // If not, just continue!
        try
        {
            tmp = m_initReader->getKey< std::string >
                (m_initReader->getInitstruct().m_initPico, key);
        }
        catch( boost::property_tree::ptree_error excep )
        {
            continue;
        }

        serials.push_back(tmp);
        initCounter.push_back(1);
    };



    if ( serials.size() > 0 )
    {
        
        for ( unsigned int ii = 0; ii < serials.size(); ++ii )
        {
            Utility::Pico_Data_Pico pico(serials.at(ii));
            m_picoData->push_back(pico);

            try
            {
            m_picos->push_back(
                    Pico( m_picoData->at(ii).serial )
                    );
            }
            catch( PicoException error )
            {
                initCounter.at(ii) = 0;
            };

        }
    }

    if ( serials.size() > 0)
    {
        int sumI{0};
        for ( unsigned int bb = 0; bb < serials.size(); ++bb )
        {
            if ( initCounter.at(bb) == 1)
            {
                ++sumI;
            }
        }

        std::cout << "\n";
        std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
        std::cout << "\tPico initialization\n";
        std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
        std::cout << "Serials found:\t\t" << serials.size();
        std::cout << "\nPicos intialized:\t" << sumI << "\n";
        std::cout << "--------------------------------------------------------\n";

        // Return to the user how many and which Picos have been found 
        // and could be initialized.
        for ( unsigned int ii = 0; ii < serials.size(); ++ii)
        {
            if ( initCounter.at(ii) == 1)
            {
                std::cout << "\t#" << ii << "\t" << serials.at(ii) << "\tinitialized!\n";
            }
            else std::cout << "\t#" << ii << "\t" << serials.at(ii) << "\tnot found!\n";
        }
    }
    else std::cout << "\n\tSorry no serials found! Does the ini-file exist?\n";

    std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    std::cout << std::endl;

    return;
}


/* void Database::Pico_readConfig( Utility::Pico_RunMode mode )
 * {
 *     
 * }
 */




///////////////////////////////////////////////////////////////////////////////
//                      END Pico stuff
///////////////////////////////////////////////////////////////////////////////
