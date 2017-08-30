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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

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


    N6700_readPSUConf();
    N6700_readChSet();

    



    ///////////////////////////////////////////////////////////////////////////
    //          Pico
    ///////////////////////////////////////////////////////////////////////////

    Pico_init();


//    Pico_readSettings( Utility::MERKEL_HG );

}
catch(...)
{
    std::cerr << "Database construction failed!\n";
    throw;
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
                            m_initReader->getInitstruct().PowerSupply, 
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

    std::string ip = root + "Server";

    m_n6700_connect.ip = m_initReader->getKey< std::string > (
            m_initReader->getInitstruct().PowerSupply, ip);



    std::string port = root + "Port";

    m_n6700_connect.port = m_initReader->getKey< std::string > (
            m_initReader->getInitstruct().PowerSupply, port);



    std::string identity = root + "Identity";

    m_n6700_connect.id = m_initReader->getKey< std::string > (
            m_initReader->getInitstruct().PowerSupply, identity);




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
    if ( m_picoData )
    {
        // delete data behind the pointer and invoke new vector
        delete m_picoData;
        m_picoData = new std::vector< Utility::Pico_Data_Pico >;
    }
    else m_picoData = new std::vector< Utility::Pico_Data_Pico >;


    // check if the pico pointer is empty. If yes allocate new, if no delete first.
    if ( m_picos )
    {
        // close picos properly before deleting the pointer
        Pico_close();

        // delete data behind the pointer and invoke new vector
        delete m_picos;
        m_picos = new std::vector< Pico >;
    }
    else m_picos = new std::vector< Pico >;
    

    std::vector< std::pair< std::string, std::string > > serialLocation;
    std::string sName = "Pico_Initializer.pico_";
    std::string nameSerial = "_serial";
    std::string nameLocation = "_location";


    std::vector<int> initCounter;       //! Counts which serials could be initialized.

    // read all serials from the ini file. Four at maximum.
    for ( int i = 0 ; i < 4 ; ++i )
    {
        // create key
        std::string keySerial = sName + std::to_string(i+1) + nameSerial;
        std::string keyLocation= sName + std::to_string(i+1) + nameLocation;
        std::string tmpSerial;
        std::string tmpLocation;

        // try if there are all serial given in the picoInit.ini file.
        // If not, just continue!
        try
        {

            boost::property_tree::ptree ptree;
            boost::property_tree::ini_parser::read_ini(
                    m_initReader->getInitstruct().initPico, ptree);

            tmpSerial = ptree.get< std::string >( keySerial );
            tmpLocation = ptree.get< std::string >( keyLocation );

/*             // try reading serial
 *             tmpSerial = m_initReader->getKey< std::string >
 *                 (m_initReader->getInitstruct().initPico, keySerial);
 * 
 *             // try reading location
 *             tmpLocation = m_initReader->getKey< std::string >
 *                 (m_initReader->getInitstruct().initPico, keyLocation);
 */
        }
        catch( boost::property_tree::ptree_error excep )
        {
            continue;
        }

        serialLocation.push_back( std::make_pair( tmpSerial, tmpLocation ) );
        initCounter.push_back(1);
    };



    if ( serialLocation.size() > 0 )
    {
        
        for ( unsigned int ii = 0; ii < serialLocation.size(); ++ii )
        {
            try
            {
                Utility::Pico_Data_Pico pico( 
                        serialLocation.at(ii).first, 
                        serialLocation.at(ii).second);

                m_picoData->push_back(pico);
            }
            catch( PicoException excep )
            {
                std::cout << "For " << serialLocation.at(ii).first;
                std::cout <<  ":\n" << excep.what() << "\n";
                initCounter.at(ii) = 0;
                continue;
            }


            try
            {
            m_picos->push_back(
                    Pico( m_picoData->at(ii).serial, &m_picoData->at(ii).location )
                    );
            }
            catch( PicoException error )
            {
                initCounter.at(ii) = 0;
            };

        }
    }

    if ( serialLocation.size() > 0)
    {
        int sumI{0};
        for ( unsigned int bb = 0; bb < serialLocation.size(); ++bb )
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
        std::cout << "Serials found:\t\t" << serialLocation.size();
        std::cout << "\nPicos intialized:\t" << sumI << "\n";
        std::cout << "--------------------------------------------------------\n";

        // Return to the user how many and which Picos have been found 
        // and could be initialized.
        for ( unsigned int ii = 0; ii < serialLocation.size(); ++ii)
        {
            if ( initCounter.at(ii) == 1)
            {
                std::cout << "#" << ii << "\t" << serialLocation.at(ii).first;
                std::cout << "\t" << serialLocation.at(ii).second;
                std::cout << "\tinitialized!\n";
            }
            else
            {
                std::cout << "#" << ii << "\t" << serialLocation.at(ii).first;
                std::cout << "\t" << serialLocation.at(ii).second;
                std::cout << "\tnot found!\n";
            }
        }
    }
    else std::cout << "\n\tSorry no serials found! Does the ini-file exist?\n";

    std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    std::cout << std::endl;

    return;
}






void Database::Pico_close()
{
    
    // check if m_picos is empty
    if ( m_picos && (m_picos->size() > 0) ) 
    {
        // closing all pico instances
        for ( Pico pico : *m_picos )
        {
            pico.closeUnit();
        }
    }

    return;
}




void Database::Pico_readSettings( Utility::Pico_RunMode mode )
{
    
    // since we have four picos, it loops fouer times
    for ( int i = 0; i < 4; ++i )
    {

        ///////////////////////////////////////////////////////////////////////
        // catch exceptions for each settings separatly and inform the user.
        

        // reading in channel settings
        try
        {
            Pico_readChannelsSettings( mode, i );
        }
        catch( boost::property_tree::ptree_error excep )
        {
            std::cout << "\nChannel settings couldn't be read-in for Pico# ";
            std::cout << i+1 << "\n";
            std::cout << "Please check the ini-file arguments and Claws docs.\n";
            std::cout << excep.what() << "\n" << std::endl;
            continue;
        }



        // reading in aquisition settings
        try
        {
            Pico_readAquisitionSettings( mode, i );
        }
        catch( boost::property_tree::ptree_error excep )
        {
            std::cout << "\nAquisition settings couldn't be read-in for Pico# ";
            std::cout << i+1 << "\n";
            std::cout << "Please check the ini-file arguments and Claws docs.\n";
            std::cout << excep.what() << "\n" << std::endl;
            continue;
        }



        // reading in simple trigger settings
        try
        {
            Pico_readTriggerSimpleSettings( mode, i );
        }
        catch( boost::property_tree::ptree_error excep )
        {
            std::cout << "\nSimple Trigger settings couldn't be read-in for Pico# ";
            std::cout << i+1 << "\n";
            std::cout << "Please check the ini-file arguments and Claws docs.\n";
            std::cout << excep.what() << "\n" << std::endl;
            continue;
        }



        // reading in advanced trigger settings
        //! \todo This function needs to be filled with some code!
//        try
//        {
//            Pico_readTriggerAdvSettings( mode, i );
//        }
//        catch( boost::property_tree::ptree_error excep )
//        {
//            std::cout << "\nAdvanced Trigger settings couldn't be read-in for Pico# ";
//            std::cout << i+1 << "\n";
//            std::cout << "Please check the ini-file arguments and Claws docs.\n";
//            std::cout << excep.what() << "\n" << std::endl;
//            continue;
//        }

    }


    return;
}




void Database::Pico_readChannelsSettings( Utility::Pico_RunMode mode, int picoNo )
{
    std::string headBegin{"Pico_"};
    std::string headEnd{"_Channel_"};
    std::vector< std::string > channelList{
        "A",
        "B",
        "C",
        "D"
    };

    // preparation variables for the loop read-in
    std::string pathToIniFile{Pico_returnPathToRunMode(mode)};

    boost::property_tree::ptree ptree;
    boost::property_tree::ini_parser::read_ini(pathToIniFile.c_str(), ptree);

    std::string rKey;      // root path
    std::string iKey;      // intermediate path
    std::string fKey;      // final path

    std::string tmp;
    rKey = headBegin + std::to_string(picoNo+1) + headEnd;

    // put channels in vector for better accessibility in loop
    std::vector< Utility::Pico_Data_Channel > channels{
        m_picoData->at(picoNo).Ch1,
        m_picoData->at(picoNo).Ch2,
        m_picoData->at(picoNo).Ch3,
        m_picoData->at(picoNo).Ch4
    };

    // loop through the channels
    for ( unsigned int kk = 0; kk < channelList.size(); ++kk )
    {
        // create loop specific root path (key)
        iKey = rKey + channelList.at(kk) + ".";
        
        // reading 'enabled'
        fKey = iKey + "enabled";
        channels.at(kk).enabled = ptree.get< bool > ( fKey );
        
        // reading coupling
        fKey = iKey + "coupling";
        tmp = ptree.get< std::string > ( fKey );
        channels.at(kk).coupling = 
            Utility::Pico_StringToEnum_coupling( tmp ); 
        
        // reading range
        fKey = iKey + "range";
        tmp = ptree.get< std::string > ( fKey );
        channels.at(kk).range = 
            Utility::Pico_StringToEnum_range( tmp );

        // reading analogue offset
        fKey = iKey + "analogueOffset";
        channels.at(kk).analogueOffset = ptree.get< float >( fKey );

    }

    return;
}




void Database::Pico_readAquisitionSettings( Utility::Pico_RunMode mode, int picoNo )
{

    std::string headBegin{"Pico_"};
    std::string headEnd{"_Aquisition"};

    // preparation variables for the loop read-in
    std::string pathToIniFile{Pico_returnPathToRunMode(mode)};

    boost::property_tree::ptree ptree;
    boost::property_tree::ini_parser::read_ini(pathToIniFile.c_str(), ptree);

    std::string rKey;      // root path
    std::string fKey;      // final path

    std::string tmp;
    rKey = headBegin + std::to_string(picoNo+1) + headEnd + ".";

    
    // reading preTrigger 
    fKey = rKey + "preTrigger";
    m_picoData->at(picoNo).preTrigger = ptree.get< unsigned int > ( fKey );
    
    // reading postTrigger
    fKey = rKey + "postTrigger";
    m_picoData->at(picoNo).postTrigger = ptree.get< unsigned int > ( fKey );
    
    // reading timebase
    fKey = rKey + "timebase";
    m_picoData->at(picoNo).timebase = ptree.get< unsigned int > ( fKey );

    // reading downSampleMode 
    fKey = rKey + "downSampleMode";
    tmp = ptree.get< std::string >( fKey );
    m_picoData->at(picoNo).downSampleRatioMode = 
        Utility::Pico_StringToEnum_ratio( tmp );

    // reading downSamlpeRatio
    fKey = rKey + "downSampleRatio";
    m_picoData->at(picoNo).downSampleRatio = ptree.get< unsigned int  >( fKey );

    return;
}




void Database::Pico_readTriggerSimpleSettings( Utility::Pico_RunMode mode, int picoNo )
{

    std::string headBegin{"Pico_"};
    std::string headEnd{"_Trigger_Simple"};

    // preparation variables for the loop read-in
    std::string pathToIniFile{Pico_returnPathToRunMode(mode)};

    boost::property_tree::ptree ptree;
    boost::property_tree::ini_parser::read_ini(pathToIniFile.c_str(), ptree);

    std::string rKey;      // root path
    std::string iKey;      // intermediate path
    std::string fKey;      // final path

    std::string tmp;
    rKey = headBegin + std::to_string(picoNo+1) + headEnd + ".";
    iKey = rKey;

    // reading enabled
    fKey = iKey + "enabled";
    m_picoData->at(picoNo).trigger.enabled = ptree.get< bool > ( fKey );
    
    // reading source
    fKey = iKey + "source";
    tmp = ptree.get< std::string > ( fKey );
    m_picoData->at(picoNo).trigger.source = 
        Utility::Pico_StringToEnum_channel( tmp );
    
    // reading threshold
    fKey = iKey + "threshold";
    m_picoData->at(picoNo).trigger.threshold = ptree.get< int > ( fKey );

    // reading direction
    fKey = iKey + "direction";
    tmp = ptree.get < std::string > ( fKey );
    m_picoData->at(picoNo).trigger.direction = 
        Utility::Pico_StringToEnum_thresDir( tmp );

    // reading delay 
    fKey = iKey + "delay";
    m_picoData->at(picoNo).trigger.delay = ptree.get < unsigned int > ( fKey );

    // reading autoTriggerTime
    fKey = iKey + "autoTriggerTime";
    m_picoData->at(picoNo).trigger.autoTriggerTime = ptree.get < int > ( fKey );

    return;
}




//! \todo Function is empty!
void Database::Pico_readTriggerAdvSettings( Utility::Pico_RunMode mode, int picoNo )
{

}


std::string Database::Pico_returnPathToRunMode( Utility::Pico_RunMode mode )
{

    std::string output;
    switch( mode )
    {
        case Utility::INTERMEDIATE:
            output = m_initReader->getInitstruct().Intermediate;
            break;
        case Utility::OBERMAIER_HG:
            output = m_initReader->getInitstruct().Obermaier_HG;
            break;
        case Utility::MERKEL_HG:
            output = m_initReader->getInitstruct().Merkel_HG;
            break;
        case Utility::SCHIFFER_LG:
            output = m_initReader->getInitstruct().Schiffer_LG;
            break;
        case Utility::KLUM_LG:
            output = m_initReader->getInitstruct().Klum_LG;
            break;
        case Utility::GARRN:
            output = m_initReader->getInitstruct().Garrn;
            break;
        default:
            throw PicoException("Wrong Pico_RunMode input!");
    };

    return output;
}

///////////////////////////////////////////////////////////////////////////////
//                      END Pico stuff
///////////////////////////////////////////////////////////////////////////////
