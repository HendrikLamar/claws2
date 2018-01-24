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

#include <boost/filesystem.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <iostream>
#include <string>
#include <mutex>

#include "cadef.h"
///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////


Database::Database() try :
    m_picoData( nullptr ),
    m_stopSwitch( false ),
    m_initReader( std::make_shared<ReadIni>() ),
    m_steeringData( std::make_shared<Utility::Steering_Data>() ),
    m_N6700_Channels( std::make_shared<N6700_Channels>() ),
    m_epicsVars(std::make_shared<std::vector<std::pair<std::string,chid>>>()),
    m_epicsContext( nullptr ),
    m_runNumber(std::make_shared<unsigned long>())
{
    ///////////////////////////////////////////////////////////////////////////
    //          General 
    ///////////////////////////////////////////////////////////////////////////

    try
    {

        Claws_readConfig();

    }
    catch( boost::property_tree::ptree_error& excep )
    {
        std::cout << excep.what() << std::endl;
        throw;
    }
    catch( ClawsException& excep )
    {
        std::cout << excep.what() << std::endl;
    }



    try
    {
        Claws_rwCounter('r');
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
    m_initReader.reset();

    m_N6700_Channels.reset();

    m_picoData.reset();
    
    m_steeringData.reset();
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




std::shared_ptr<ReadIni> Database::getInitReader()
{
    return m_initReader;
}






///////////////////////////////////////////////////////////////////////////////






std::shared_ptr<Utility::Steering_Data> Database::Claws_getConfig()
{
    return m_steeringData;
}







///////////////////////////////////////////////////////////////////////////////

     

void Database::setNoOfPicosInitialized( unsigned int numb )
{
    m_MaxNoOfPicos = numb;

    return;
}








///////////////////////////////////////////////////////////////////////////////






void Database::Claws_readConfig()
{

    std::string tpath;
    std::string root = "General.";

    std::string tmp;

    // read in loopsPhysics
    tpath = root + "loops_physics";
    Claws_getConfig()->loops_Physics = m_initReader->getKey<unsigned int>(
            m_initReader->getInitstruct().ClawsConfig, tpath);

    // read in loopsIntermediate
    tpath = root + "loops_intermediate";
    Claws_getConfig()->loops_Intermediate = m_initReader->getKey<unsigned int>(
            m_initReader->getInitstruct().ClawsConfig, tpath );

    try
    {
        // read in isSaved 
        tpath  = root + "saveData_raw";
        tmp = m_initReader->getKey<std::string>(
                m_initReader->getInitstruct().ClawsConfig, tpath );
        tmp.compare("yes") == 0 ? Claws_getConfig()->isSaved_raw = true : Claws_getConfig()->isSaved_raw = false; 
    }
    catch( boost::property_tree::ptree_error& excep )
    {
        Claws_getConfig()->isSaved_raw = false;
        std::cout << "saveData_raw not found -> set to 'false'\n";
    }

    try
    {
        tpath  = root + "saveData_online";
        tmp = m_initReader->getKey<std::string>(
                m_initReader->getInitstruct().ClawsConfig, tpath );
        tmp.compare("yes") == 0 ? Claws_getConfig()->isSaved_online = true : Claws_getConfig()->isSaved_online = false; 
    }
    catch(  boost::property_tree::ptree_error& excep )
    {
        Claws_getConfig()->isSaved_online = false;
        std::cout << "saveData_online not found -> set to 'false'\n";
    }


    try
    {
        tpath  = root + "epics";
        tmp = m_initReader->getKey<std::string>(
                m_initReader->getInitstruct().ClawsConfig, tpath );
        tmp.compare("yes") == 0 ? Claws_getConfig()->useEpics = true : Claws_getConfig()->useEpics = false; 
    }
    catch( boost::property_tree::ptree_error& excep )
    {
        Claws_getConfig()->useEpics = false;
        std::cout << "epics not found -> set to 'false'\n";
    }

    // read in save path # 1
    tpath  = root + "path_saveData";
    Claws_getConfig()->path_saveData = m_initReader->getKey<std::string>(
            m_initReader->getInitstruct().ClawsConfig, tpath );
    

    
    root = "RunMode.";

    // read in intermedaite config file path
    tpath = root + "intermediate";
    Claws_getConfig()->path_config_intermediate = 
        m_initReader->getKey< std::string >(m_initReader->getInitstruct().ClawsConfig, tpath);

    // read in physics config file path
    tpath = root + "physics";
    Claws_getConfig()->path_config_physics = 
        m_initReader->getKey< std::string >(m_initReader->getInitstruct().ClawsConfig, tpath );




    return;
}










///////////////////////////////////////////////////////////////////////////////






std::shared_ptr< std::vector< std::pair<std::string,chid>>> 
    Database::Epics_getVars()
{
   return m_epicsVars; 
}








///////////////////////////////////////////////////////////////////////////////






ca_client_context* Database::Epics_getContext()
{
    return m_epicsContext;
}








///////////////////////////////////////////////////////////////////////////////








void Database::Epics_setContext( ca_client_context* context )
{
    m_epicsContext = context;
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

    if( mode == Utility::Pico_RunMode::INTERMEDIATE )
    {
        for( unsigned int i = 0; i < m_MaxNoOfPicos; ++i )
        {
            Pico_readIntermediateSettings( i );
        }

        return;
    }

    
    // since we have four picos, it loops fouer times
    for ( unsigned int i = 0; i < m_MaxNoOfPicos ; ++i )
//    for ( int i = 0; i < 2 ; ++i )
    {

        ///////////////////////////////////////////////////////////////////////
        // catch exceptions for each settings separatly and inform the user.
        

        // reading in channel settings
        try
        {
            Pico_readChannelsSettings( mode, i );
        }
        catch( boost::property_tree::ptree_error& excep )
        {
            std::cout << "\nChannel settings couldn't be read-in for Pico# ";
            std::cout << i+1 << "\n";
            std::cout << "Please check the ini-file arguments and Claws docs.\n";
            std::cout << excep.what() << "\n" << std::endl;
            continue;
        }
        catch( PicoException& excep )
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
        catch( boost::property_tree::ptree_error& excep )
        {
            std::cout << "\nAquisition settings couldn't be read-in for Pico# ";
            std::cout << i+1 << "\n";
            std::cout << "Please check the ini-file arguments and Claws docs.\n";
            std::cout << excep.what() << "\n" << std::endl;
            continue;
        }
        catch( PicoException& excep )
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
        catch( boost::property_tree::ptree_error& excep )
        {
            std::cout << "\nSimple Trigger settings couldn't be read-in for Pico# ";
            std::cout << i+1 << "\n";
            std::cout << "Please check the ini-file arguments and Claws docs.\n";
            std::cout << excep.what() << "\n" << std::endl;
            continue;
        }
        catch( PicoException& excep )
        {
            std::cout << "\nSimple Trigger settings couldn't be read-in for Pico# ";
            std::cout << i+1 << "\n";
            std::cout << "Please check the ini-file arguments and Claws docs.\n";
            std::cout << excep.what() << "\n" << std::endl;
            continue;
        }


        // reading in simple trigger settings
        try
        {
            Pico_readMiscSettings( i );
        }
        catch( boost::property_tree::ptree_error& excep )
        {
            std::cout << "\nAnalysis settings couldn't be read-in for Pico# ";
            std::cout << i+1 << "\n";
            std::cout << "Please check the ini-file arguments and Claws docs.\n";
            std::cout << excep.what() << "\n" << std::endl;
            continue;
        }
        catch( PicoException& excep )
        {
            std::cout << "\nAnalysis settings couldn't be read-in for Pico# ";
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
    std::shared_ptr<Utility::Pico_Conf_HL_Gain> tmpDataStruct{
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
    rKey = headBegin + m_picoData->at(picoNo)->val_location + headEnd;

    // put channels in vector for better accessibility in loop
    std::shared_ptr< std::vector< std::shared_ptr<Utility::Pico_Conf_Channel> > > channels = tmpDataStruct->channels;

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

        // read bandwith parameter
        fKey = iKey + "bandwidth";
        tmp = ptree.get< std::string > (fKey);
        channels->at(kk)->bandwidth = Utility::Pico_StringToEnum_bandwidth( tmp );

    }

    return;
}




void Database::Pico_readAquisitionSettings( Utility::Pico_RunMode mode, int picoNo )
{

    // returns the corresponding high or low gain data structure according to the
    // run mode
    std::shared_ptr<Utility::Pico_Conf_HL_Gain> tmpDataStruct{
        Pico_getHLGainStruct( mode, picoNo )};

    ///////////////////////////////////////////////////////////////////////////
    // preparation variables for the loop read-in
    std::string pathToIniFile{Pico_returnPathToRunMode(mode)};

    boost::property_tree::ptree ptree;
    boost::property_tree::ini_parser::read_ini(pathToIniFile.c_str(), ptree);
    
    std::string tmp;
    std::string rKey;      // root path
    std::string fKey;      // final path


    std::string headBegin{"Pico_"};
    std::string headEnd{"_Aquisition"};


    rKey = headBegin + m_picoData->at(picoNo)->val_location + headEnd + ".";

    
    // reading preTrigger 
    fKey = rKey + "preTrigger";
    tmpDataStruct->val_preTrigger = ptree.get< unsigned int > ( fKey );
    
    // reading postTrigger
    fKey = rKey + "postTrigger";
    tmpDataStruct->val_postTrigger = ptree.get< unsigned int > ( fKey );
    
    // reading timebase
    fKey = rKey + "timebase";
    tmpDataStruct->val_timebase = ptree.get< unsigned int > ( fKey );

    // reading downSampleMode 
    fKey = rKey + "downSampleMode";
    tmp = ptree.get< std::string >( fKey );
    tmpDataStruct->val_downSampleRatioMode = 
        Utility::Pico_StringToEnum_ratio( tmp );

    // reading downSamlpeRatio
    fKey = rKey + "downSampleRatio";
    tmpDataStruct->val_downSampleRatio = ptree.get< unsigned int  >( fKey );

    // reading oversample
    fKey = rKey + "oversample";
    tmpDataStruct->val_oversample = ptree.get< short >( fKey );

    // reading in trigger mode
    fKey = rKey + "triggerMode";
    tmp = ptree.get< std::string >( fKey );
    tmpDataStruct->mode_trigger= 
        Utility::Pico_StringToEnum_trigger( tmp );

    return;
}




void Database::Pico_readTriggerSimpleSettings( Utility::Pico_RunMode mode, int picoNo )
{
    // returns the corresponding high or low gain data structure according to the
    // run mode
    std::shared_ptr<Utility::Pico_Conf_HL_Gain> tmpDataStruct{
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

    
    std::string tmp = ptree.get< std::string >( "General.lowGain");
    Utility::Claws_Gain tmpGain{ tmp.compare("true")==0 ? Utility::Claws_Gain::LOW_GAIN : Utility::Claws_Gain::HIGH_GAIN };
    tmpDataStruct->gain = tmpGain;

    rKey = headBegin + m_picoData->at(picoNo)->val_location + headEnd + ".";
    iKey = rKey;

    // reading enabled
    fKey = iKey + "enabled";
    tmpDataStruct->data_trigger->enabled = ptree.get< bool > ( fKey );
    
    // reading source
    fKey = iKey + "source";
    tmp = ptree.get< std::string > ( fKey );
    tmpDataStruct->data_trigger->source = 
        Utility::Pico_StringToEnum_channel( tmp );
    
    // reading threshold
    fKey = iKey + "threshold";
    tmpDataStruct->data_trigger->threshold = ptree.get< int > ( fKey );

    // reading direction
    fKey = iKey + "direction";
    tmp = ptree.get < std::string > ( fKey );
    tmpDataStruct->data_trigger->direction = 
        Utility::Pico_StringToEnum_thresDir( tmp );

    // reading delay 
    fKey = iKey + "delay";
    tmpDataStruct->data_trigger->delay = ptree.get < unsigned int > ( fKey );

    // reading autoTriggerTime
    fKey = iKey + "autoTriggerTime";
    tmpDataStruct->data_trigger->autoTriggerTime = ptree.get < int > ( fKey );

    return;
}







///////////////////////////////////////////////////////////////////////////////











//! \todo Function is empty!
void Database::Pico_readTriggerAdvSettings( Utility::Pico_RunMode mode, int picoNo )
{

}







///////////////////////////////////////////////////////////////////////////////







void Database::Pico_readIntermediateSettings( int picoNo )
{

    std::shared_ptr<Utility::Pico_Conf_HL_Gain>   tmpDataStruct = 
        m_picoData->at(picoNo)->data_inter;

    ///////////////////////////////////////////////////////////////////////////


    // reade variable stuff
    //
    
    std::string headBegin{"Pico_"};
    std::string headEnd{"_Intermediate_Mode_Settings"};

    // preparation variables for the loop read-in
    std::string pathToIniFile{Pico_returnPathToRunMode(
            Utility::Pico_RunMode::INTERMEDIATE)};

    boost::property_tree::ptree ptree;
    boost::property_tree::ini_parser::read_ini(pathToIniFile.c_str(), ptree);

    std::string rKey;      // root path
    std::string iKey;      // intermediate path
    std::string fKey;      // final path

    std::string tmp;
    rKey = headBegin + m_picoData->at(picoNo)->val_location + headEnd + ".";
    iKey = rKey;

    fKey = iKey + "preTrigger";
    tmpDataStruct->val_preTrigger = ptree.get< unsigned int >( fKey );

    fKey = iKey + "postTrigger";
    tmpDataStruct->val_postTrigger = ptree.get< unsigned int >( fKey );

    fKey = iKey + "timebase";
    tmpDataStruct->val_timebase = ptree.get< unsigned int >( fKey );
    
    fKey = iKey + "threshold";
    tmpDataStruct->data_trigger->threshold =  ptree.get< int >( fKey );

    fKey = iKey + "direction";
    tmp = ptree.get< std::string >( fKey );
    tmpDataStruct->data_trigger->direction = Utility::Pico_StringToEnum_thresDir( tmp );

    fKey = iKey + "autoTriggerTime";
    tmpDataStruct->data_trigger->autoTriggerTime = ptree.get < int > ( fKey );

    tmpDataStruct->loops_inter = static_cast<uint32_t>(Claws_getConfig()->loops_Intermediate);

    // find out which channel is enabled
    fKey = iKey + "channels_to_calibrate";
    tmp = ptree.get< std::string >( fKey );
    int counter = 0;
    for( char &tchar : tmp )
    {
        if( tchar == '1' )
        {
            tmpDataStruct->channels->at(counter)->enabled = true;
        }
        else
        {
            tmpDataStruct->channels->at(counter)->enabled = false;
        }

        ++counter;
    }

    fKey = iKey + "average_1pe_height";
    tmpDataStruct->average_1pe_height = ptree.get< int >( fKey );





    // read constant stuff
    //
    headEnd = "_Intermediate_Mode";

    pathToIniFile = m_initReader->getInitstruct().constant_config;
    boost::property_tree::ini_parser::read_ini(pathToIniFile.c_str(), ptree);

    rKey = m_picoData->at(picoNo)->val_location + headEnd + ".";

    fKey = rKey + "downSampleMode";
    tmp = ptree.get< std::string >( fKey );
    tmpDataStruct->val_downSampleRatioMode = Utility::Pico_StringToEnum_ratio( tmp );

    fKey = rKey + "downSampleRatio";
    tmpDataStruct->val_downSampleRatio = ptree.get< unsigned int >( fKey );

    fKey = rKey + "oversample";
    tmpDataStruct->val_oversample = ptree.get< unsigned int >( fKey );

    fKey = rKey + "triggerMode";
    tmp = ptree.get< std::string >( fKey );
    tmpDataStruct->mode_trigger = Utility::Pico_StringToEnum_trigger( tmp );

    fKey = rKey + "delay";
    tmpDataStruct->data_trigger->delay = ptree.get< unsigned int >( fKey );


    // channel stuff is equal for all channels 
    PS6000_COUPLING             tmp_coupling;
    PS6000_RANGE                tmp_range;
    float                       tmp_anaOffset;
    PS6000_BANDWIDTH_LIMITER    tmp_bandwidth;

    fKey = rKey + "coupling";
    tmp = ptree.get< std::string >(fKey);
    tmp_coupling = Utility::Pico_StringToEnum_coupling( tmp );

    fKey = rKey + "range";
    tmp = ptree.get< std::string >( fKey );
    tmp_range = Utility::Pico_StringToEnum_range( tmp );

    fKey = rKey + "analogueOffset";
    tmp_anaOffset = ptree.get< float >( fKey );

    fKey = rKey + "bandwidth";
    tmp = ptree.get< std::string >( fKey );
    tmp_bandwidth = Utility::Pico_StringToEnum_bandwidth( tmp );

    for( auto& tcha : *tmpDataStruct->channels )
    {
        tcha->coupling = tmp_coupling;
        tcha->range = tmp_range;
        tcha->analogueOffset = tmp_anaOffset;
        tcha->bandwidth = tmp_bandwidth;
    }


    return;
}






///////////////////////////////////////////////////////////////////////////////






void Database::Pico_readMiscSettings( int picoNo )
{

    std::shared_ptr<Utility::Pico_Conf_Analysis>   tmp_conf = 
        m_picoData->at(picoNo)->conf_analysis;


    ///////////////////////////////////////////////////////////////////////////

    // PHYSICS
    //
    
    std::string headBegin{"Analysis"};

    // preparation variables for the loop read-in
    std::string pathToIniFile{Pico_returnPathToRunMode(
            Utility::Pico_RunMode::PHYSICS)};

    boost::property_tree::ptree ptree;
    boost::property_tree::ini_parser::read_ini(pathToIniFile.c_str(), ptree);

    std::string rKey;      // root path
    std::string iKey;      // intermediate path
    std::string fKey;      // final path

    std::string tmp;
    rKey = headBegin + ".";
    iKey = rKey;

    fKey = iKey + "signalCut";
    tmp_conf->physics_signalCut = ptree.get< int >( fKey );

    fKey = iKey + "minSignalLength";
    tmp_conf->physics_minSignalLength = 
        ptree.get< unsigned long >( fKey );

    fKey = iKey + "preSamplesBeforeThreshold";
    tmp_conf->physics_preSamplesBeforeThreshold
        = ptree.get< unsigned long >( fKey );

    fKey = iKey + "postSamplesAfterThreshold";
    tmp_conf->physics_postSamplesAfterThreshold
        = ptree.get< unsigned long >( fKey );

    fKey = iKey + "wfScanStepSize";
    tmp_conf->physics_wfScanStepSize
        = ptree.get< int >( fKey );






    // INTERMEDIATE
    //

    pathToIniFile = Pico_returnPathToRunMode(Utility::Pico_RunMode::INTERMEDIATE);
    boost::property_tree::ini_parser::read_ini(pathToIniFile.c_str(), ptree);

    fKey = iKey + "factor_1pe";
    tmp_conf->inter_factor1pe = ptree.get< float >( fKey );

    return;
}






///////////////////////////////////////////////////////////////////////////////


















std::string Database::Pico_returnPathToRunMode( Utility::Pico_RunMode mode )
{

    std::string tmp;
    switch( mode )
    {
        case Utility::Pico_RunMode::PHYSICS:
            tmp = Claws_getConfig()->path_config_physics;
            break;
        case Utility::Pico_RunMode::INTERMEDIATE:
            tmp = Claws_getConfig()->path_config_intermediate;
            break;
        default:
            throw PicoException("Wrong Pico_RunMode input!");
    };

    std::string output{boost::filesystem::canonical(
            boost::filesystem::current_path()).string() + "/ini_files/" +
            tmp};

    return output;
}







///////////////////////////////////////////////////////////////////////////////
















std::shared_ptr<Utility::Pico_Conf_HL_Gain> Database::Pico_getHLGainStruct(
                                        Utility::Pico_RunMode mode,
                                        int picoNo
                                        )
{
    switch( mode )
    {
        // return the high gain data structure
        case Utility::Pico_RunMode::PHYSICS:
            return m_picoData->at(picoNo)->data_physics;

        case Utility::Pico_RunMode::INTERMEDIATE:
            return m_picoData->at(picoNo)->data_inter;
        default:
            throw PicoException("Utility::Pico_RunMode does not exist!");
    }

} 




///////////////////////////////////////////////////////////////////////////////
//                      END Pico stuff
///////////////////////////////////////////////////////////////////////////////








































///////////////////////////////////////////////////////////////////////////////
//
//                START general clawsRun 
//
///////////////////////////////////////////////////////////////////////////////








std::shared_ptr<unsigned long> Database::Claws_getCounter()
{

    return m_runNumber;
}







///////////////////////////////////////////////////////////////////////////////








void            Database::Claws_incrCounter()
{
    ++(*m_runNumber);

    return;
}







///////////////////////////////////////////////////////////////////////////////








void Database::Claws_rwCounter( char rw, std::string file, std::string id )
{
    boost::property_tree::ptree ptree;
    
    if( !file.compare("") )
    {
        file = getInitReader()->getInitstruct().runNumber;
    }

    switch( rw )
    {
        case 'r':
            *m_runNumber = getInitReader()->getKey<unsigned int>(file, id);
            break;
        case 'w':
            ptree.put(id, *m_runNumber);
            boost::property_tree::ini_parser::write_ini(file, ptree);
            break;
        default:
            throw ClawsException("Mode not accessible!");

    }

    return;
}









///////////////////////////////////////////////////////////////////////////////
//
//                END general clawsRun 
//
///////////////////////////////////////////////////////////////////////////////
