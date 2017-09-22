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
    m_picoData( nullptr ),
    m_stopSwitch( false ),
    m_initReader( new ReadIni() ),
    m_steeringData( new Utility::Steering_Data() ),
    m_N6700_Channels( new N6700_Channels() )
{
    ///////////////////////////////////////////////////////////////////////////
    //          General 
    ///////////////////////////////////////////////////////////////////////////

    try
    {

        readSteeringFile();

    }
    catch( boost::property_tree::ptree_error& excep )
    {
        std::cout << excep.what() << std::endl;
        throw;
    }



    ///////////////////////////////////////////////////////////////////////////
    //          Powersupply
    ///////////////////////////////////////////////////////////////////////////


    



    ///////////////////////////////////////////////////////////////////////////
    //          Pico
    ///////////////////////////////////////////////////////////////////////////

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

    if( m_picoData )
    {
        delete m_picoData;
    }
    m_picoData = nullptr;
    
    delete m_steeringData;
    m_steeringData = nullptr;
};



///////////////////////////////////////////////////////////////////////////////
//          START Database only stuff
///////////////////////////////////////////////////////////////////////////////


void Database::setStop(bool switcher)
{
    m_stopSwitch = switcher;

    return;
}







///////////////////////////////////////////////////////////////////////////////






bool Database::getStop()
{
    return m_stopSwitch;
}






///////////////////////////////////////////////////////////////////////////////




ReadIni* Database::getInitReader()
{
    return m_initReader;
}






///////////////////////////////////////////////////////////////////////////////






Utility::Steering_Data* Database::getSteeringData()
{
    return m_steeringData;
}








///////////////////////////////////////////////////////////////////////////////






void Database::readSteeringFile()
{
    std::string root = "General.";


    // read in High Gain Mode
    std::string highGain = root + "RunMode_HighGain";
    std::string tmp = 
        m_initReader->getKey< std::string >(m_initReader->getInitstruct().SteeringFile, highGain);
    m_steeringData->runMode_HighGain = Utility::Pico_StringToEnum_runMode( tmp );


    // read in Low Gain Mode
    std::string lowGain = root + "RunMode_LowGain";
    tmp = m_initReader->getKey< std::string >(m_initReader->getInitstruct().SteeringFile, lowGain);
    m_steeringData->runMode_LowGain = Utility::Pico_StringToEnum_runMode( tmp );

    // read in Trigger Mode
    std::string triggerM = root + "TriggerMode";
    tmp = m_initReader->getKey< std::string >(m_initReader->getInitstruct().SteeringFile, triggerM);
    m_steeringData->triggerMode = Utility::Pico_StringToEnum_trigger( tmp );

    // read in loopsPhysics
    std::string loopsPhysics = root + "LoopsPhysics";
    m_steeringData->loopsPhysics = m_initReader->getKey<int>(
            m_initReader->getInitstruct().SteeringFile, loopsPhysics);

    // read in loopsIntermediate
    std::string loopsIntermediate = root + "LoopsIntermediate";
    m_steeringData->loopsIntermediate = m_initReader->getKey<int>(
            m_initReader->getInitstruct().SteeringFile, loopsIntermediate);

    // read in save path # 1
    std::string savePath_1 = root + "Save_Path_1";
    m_steeringData->savePath_1 = m_initReader->getKey<std::string>(
            m_initReader->getInitstruct().SteeringFile, savePath_1);
    
    // read in save path # 2
    std::string savePath_2 = root + "Save_Path_2";
    m_steeringData->savePath_2 = m_initReader->getKey<std::string>(
            m_initReader->getInitstruct().SteeringFile, savePath_2);

    // read in save path # 3
    std::string savePath_3 = root + "Save_Path_3";
    m_steeringData->savePath_3 = m_initReader->getKey<std::string>(
            m_initReader->getInitstruct().SteeringFile, savePath_3);

    return;
}












///////////////////////////////////////////////////////////////////////////////
//          END Database only stuff
///////////////////////////////////////////////////////////////////////////////






















///////////////////////////////////////////////////////////////////////////////
//          START Database PowerSupply (KPS) stuff
///////////////////////////////////////////////////////////////////////////////


// reads in high AND low gain mode everytime it is called.
void Database::N6700_readChSettings() 
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




void Database::N6700_readConnectSettings()
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


void Database::Pico_readSettings( Utility::Pico_RunMode mode )
{
    
    // since we have four picos, it loops fouer times
    for ( int i = 0; i < m_MaxNoOfPicos ; ++i )
//    for ( int i = 0; i < 2 ; ++i )
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
        catch( PicoException excep )
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
        catch( PicoException excep )
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
        catch( PicoException excep )
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
    // returns the corresponding high or low gain data structure according to the
    // run mode
    Utility::Pico_Data_HL_Gain* tmpDataStruct{
        Pico_getHLGainStruct( mode, picoNo )};

    ///////////////////////////////////////////////////////////////////////////

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
    rKey = headBegin + m_picoData->at(picoNo)->location + headEnd;

    // put channels in vector for better accessibility in loop
    std::vector< Utility::Pico_Data_Channel* >* channels = tmpDataStruct->channels;

    // loop through the channels
    for ( unsigned int kk = 0; kk < channelList.size(); ++kk )
    {
        // create loop specific root path (key)
        iKey = rKey + channelList.at(kk) + ".";
        
        // reading 'enabled'
        fKey = iKey + "enabled";
        channels->at(kk)->enabled = ptree.get< bool > ( fKey );
        
        // reading coupling
        fKey = iKey + "coupling";
        tmp = ptree.get< std::string > ( fKey );
        channels->at(kk)->coupling = Utility::Pico_StringToEnum_coupling( tmp ); 
        
        // reading range
        fKey = iKey + "range";
        tmp = ptree.get< std::string > ( fKey );
        channels->at(kk)->range = 
            Utility::Pico_StringToEnum_range( tmp );

        // reading analogue offset
        fKey = iKey + "analogueOffset";
        channels->at(kk)->analogueOffset = ptree.get< float >( fKey );

    }

    return;
}




void Database::Pico_readAquisitionSettings( Utility::Pico_RunMode mode, int picoNo )
{

    // returns the corresponding high or low gain data structure according to the
    // run mode
    Utility::Pico_Data_HL_Gain* tmpDataStruct{
        Pico_getHLGainStruct( mode, picoNo )};

    ///////////////////////////////////////////////////////////////////////////

    std::string headBegin{"Pico_"};
    std::string headEnd{"_Aquisition"};

    // preparation variables for the loop read-in
    std::string pathToIniFile{Pico_returnPathToRunMode(mode)};

    boost::property_tree::ptree ptree;
    boost::property_tree::ini_parser::read_ini(pathToIniFile.c_str(), ptree);

    std::string rKey;      // root path
    std::string fKey;      // final path

    std::string tmp;
    rKey = headBegin + m_picoData->at(picoNo)->location + headEnd + ".";

    
    // reading preTrigger 
    fKey = rKey + "preTrigger";
    tmpDataStruct->preTrigger = ptree.get< unsigned int > ( fKey );
    
    // reading postTrigger
    fKey = rKey + "postTrigger";
    tmpDataStruct->postTrigger = ptree.get< unsigned int > ( fKey );
    
    // reading timebase
    fKey = rKey + "timebase";
    tmpDataStruct->timebase = ptree.get< unsigned int > ( fKey );

    // reading downSampleMode 
    fKey = rKey + "downSampleMode";
    tmp = ptree.get< std::string >( fKey );
    tmpDataStruct->downSampleRatioMode = 
        Utility::Pico_StringToEnum_ratio( tmp );

    // reading downSamlpeRatio
    fKey = rKey + "downSampleRatio";
    tmpDataStruct->downSampleRatio = ptree.get< unsigned int  >( fKey );

    return;
}




void Database::Pico_readTriggerSimpleSettings( Utility::Pico_RunMode mode, int picoNo )
{
    // returns the corresponding high or low gain data structure according to the
    // run mode
    Utility::Pico_Data_HL_Gain* tmpDataStruct{
        Pico_getHLGainStruct( mode, picoNo )};

    ///////////////////////////////////////////////////////////////////////////


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
    rKey = headBegin + m_picoData->at(picoNo)->location + headEnd + ".";
    iKey = rKey;

    // reading enabled
    fKey = iKey + "enabled";
    tmpDataStruct->trigger->enabled = ptree.get< bool > ( fKey );
    
    // reading source
    fKey = iKey + "source";
    tmp = ptree.get< std::string > ( fKey );
    tmpDataStruct->trigger->source = 
        Utility::Pico_StringToEnum_channel( tmp );
    
    // reading threshold
    fKey = iKey + "threshold";
    tmpDataStruct->trigger->threshold = ptree.get< int > ( fKey );

    // reading direction
    fKey = iKey + "direction";
    tmp = ptree.get < std::string > ( fKey );
    tmpDataStruct->trigger->direction = 
        Utility::Pico_StringToEnum_thresDir( tmp );

    // reading delay 
    fKey = iKey + "delay";
    tmpDataStruct->trigger->delay = ptree.get < unsigned int > ( fKey );

    // reading autoTriggerTime
    fKey = iKey + "autoTriggerTime";
    tmpDataStruct->trigger->autoTriggerTime = ptree.get < int > ( fKey );

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









Utility::Pico_Data_HL_Gain*    Database::Pico_getHLGainStruct(
                                        Utility::Pico_RunMode mode,
                                        int picoNo
                                        )
{
    switch( mode )
    {
        // return the intermediate data structure
        case Utility::INTERMEDIATE:
            return m_picoData->at(picoNo)->dataIntermediate;

        // return the high gain data structure
        case Utility::MERKEL_HG:
            return m_picoData->at(picoNo)->dataHighGain;

        case Utility::OBERMAIER_HG:
            return m_picoData->at(picoNo)->dataHighGain;

        // return the low gain data structure
        case Utility::KLUM_LG:
            return m_picoData->at(picoNo)->dataLowGain;

        case Utility::SCHIFFER_LG:
            return m_picoData->at(picoNo)->dataLowGain;

        // if garrn is specified, use the high gain version as default
        case Utility::GARRN:
            return m_picoData->at(picoNo)->dataHighGain;            

        default:
            throw PicoException("Utility::Pico_RunMode does not exist!");
    }

} 




///////////////////////////////////////////////////////////////////////////////
//                      END Pico stuff
///////////////////////////////////////////////////////////////////////////////
