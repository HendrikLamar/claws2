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

    std::cout << m_initReader->getInitstruct().PowerSupply << std::endl;

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

    std::cout << "File :" << m_initReader->getInitstruct().PowerSupply << "\n";

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
                (m_initReader->getInitstruct().initPico, key);
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




void Database::Pico_readConfig( Utility::Pico_RunMode mode )
{

    // open file according to runmode
    std::string iniPath;





    // header string for each point
    std::string headS{"Pico_"};
    std::vector< std::string > headE{
        "_Aquisition",
        "_Trigger_Simple",
        "_Trigger_Advanced"
    };


    // Settings aquisition
    std::vector< std::string > aquisitionSettings{
        "preTrigger",
        "postTrigger",
        "timebase",
        "downSampleMode",
        "downSampleRatio",
        "oversample"
    };


    // Settings trigger simple
    std::vector< std::string > triggerSSettings{
        "enabled",
        "source",
        "threshold",
        "direction",
        "delay",
        "autoTriggerTime"
    };





    // header channel with its settings
    std::vector< std::string > channel{
        "_Channel_A",
        "_Channel_B",
        "_Channel_C",
        "_Channel_D"
    };
    std::vector< std::string > chSettings{
        "enabled",
        "coupling",
        "range",
        "analogueOffset",
        "bandwidth"
    };


    // loop for four picos. four is hardcoded.
    //! \todo Number of read-in config cycles is hardcoded to four. No matter
    //! \todo how many picos are connected.
    //
    std::string rPath;      // root path
    std::string iPath;      // intermediate path
    std::string fPath;      // final path
    for ( int counter_1 = 0; counter_1 < 4; ++counter_1)
    {
        rPath = headS + std::to_string(counter_1+1);

        for ( auto tmp : headE )
        {
            iPath = rPath + tmp + ".";

            if ( tmp.compare(headE.at(0)) == 0 )
            {
                for ( auto subtmp : aquisitionSettings )
                {
                    fPath = iPath + subtmp;


                }
            }
        }


    }



}




void Database::Pico_readChannelsSettings( Utility::Pico_RunMode mode )
{
    std::string headBegin{"Pico_"};
    std::string headEnd{"_Channel_"};
    std::vector< std::string > channelList{
        "A",
        "B",
        "C",
        "D"
    };

    // single channel settings
    std::vector< std::string > chSettings{
        "enabled",
        "coupling",
        "range",
        "analogueOffset",
        "bandwidth"
    };


    // preparation variables for the loop read-in
    std::string pathToIniFile{Pico_returnPathToRunMode(mode)};

    boost::property_tree::ptree ptree;
    boost::property_tree::ini_parser::read_ini(pathToIniFile.c_str(), ptree);

    std::string rKey;      // root path
    std::string iKey;      // intermediate path
    std::string fKey;      // final path

    std::string tmp;
    // the loop goes to four because of four picos are used
    for ( int ii = 0; ii < 4; ++ii )
    {
        rKey = headBegin + std::to_string(ii+1) + headEnd;

        // put channels in vector for better accessibility in loop
        std::vector< Utility::Pico_Data_Channel > channels{
            m_picoData->at(ii).Ch1,
            m_picoData->at(ii).Ch2,
            m_picoData->at(ii).Ch3,
            m_picoData->at(ii).Ch4
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
    }
}




void Database::Pico_readAquisitionSettings( Utility::Pico_RunMode mode, int picoNo )
{

}




void Database::Pico_readTriggerSimpleSettings( Utility::Pico_RunMode mode, int picoNo )
{

}




void Database::Pico_readTrigerAdvSettings( Utility::Pico_RunMode mode, int picoNo )
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
