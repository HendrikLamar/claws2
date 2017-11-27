// =====================================================================================
// 
//       Filename:  clawsRun.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01.09.2017 09:12:41
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================



#include "pico.h"
#include "n6700.h"
#include "database.h"
#include "processData.h"
#include "clawsRun.h"
#include "clawsException.h"
//#include "utime.h"

#include <boost/property_tree/exceptions.hpp>

#include <numeric>
#include <iostream>
#include <exception>
#include <memory>
#include <utility>
#include <thread>
#include <mutex>

#include <TROOT.h>

///////////////////////////////////////////////////////////////////////////////


ClawsRun::ClawsRun() :
    m_database ( std::make_shared<Database>() ),
    m_picos( std::make_shared<std::vector<std::shared_ptr<Pico>>>() ),
    m_psu( nullptr )
{

}


ClawsRun::~ClawsRun()
{

    Pico_close();
    
    // check if pointing to nullptr
    m_picos.reset();

    // check if pointing to nullptr
    delete m_psu;
    m_psu = nullptr;

    m_database.reset();

}


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////



















///////////////////////////////////////////////////////////////////////////////
//
//              START Public Member Functions 
//
///////////////////////////////////////////////////////////////////////////////






std::shared_ptr<Database> ClawsRun::getDatabase()
{
    return m_database;
}






///////////////////////////////////////////////////////////////////////////////





void        ClawsRun::initialize()
{

//    Pico_init_bySerial();
/*     Pico_init_byNullptr();
 *     m_database->setNoOfPicosInitialized(m_picos->size());
 */

    PSU_init();


    try
    {
        loadConfig();
    }
    catch( ... )
    {
        std::cout << "\nUnknown error. Config could not be loaded...\n";
    }



    return;
}






///////////////////////////////////////////////////////////////////////////////






void    ClawsRun::run()
{
    std::cout << "sendConf...\n";
    m_psu->sendConf( Utility::Claws_Gain::HIGH_GAIN );
    std::cout << "start...\n";
    m_psu->start();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::cout << "stop...\n";
    m_psu->stop();

//    std::cout << "sendConf...\n";
//    m_psu->sendConf( Utility::Claws_Gain::LOW_GAIN );
//    std::cout << "start...\n";
//    m_psu->start();
//
//    std::this_thread::sleep_for(std::chrono::seconds(10));
//
//    std::cout << "stoping...\n";
//    m_psu->stop();
/*     for( int i = 0; i < 1; ++i )
 *     {
 *         m_database->Claws_incrCounter();
 *         m_database->Claws_rwCounter('w');
 * 
 *         auto time1{std::chrono::system_clock::now()};
 *         try
 *         {
 *             Pico_runRapid();
 *         }
 *         catch( ClawsException& excep )
 *         {
 *             std::cout << excep.what() << std::endl;
 *         }
 *         catch( std::exception& excep )
 *         {
 *             std::cout << excep.what() << std::endl;
 *         }
 *         auto time2{std::chrono::system_clock::now()};
 *         auto diff{std::chrono::duration_cast<std::chrono::seconds>(time2 - time1)};
 *         std::cout << "Inter: " << diff.count() << "sec\n";
 *         
 *         try
 *         {
 * //            Pico_runBlock( Utility::Claws_Gain::HL_GAIN );
 *         }
 *         catch( ClawsException& excep )
 *         {
 *             std::cout << excep.what() << std::endl;
 *         }
 *         catch( std::exception& excep )
 *         {
 *             std::cout << excep.what() << std::endl;
 *         }
 *         auto time3{std::chrono::system_clock::now()};
 *         diff = std::chrono::duration_cast<std::chrono::seconds>(time3 - time2);
 *         std::cout << "Physics: " << diff.count() << "sec\n";
 *     }
 * 
 */
    return;

}







///////////////////////////////////////////////////////////////////////////////





void            ClawsRun::loadConfig()
{
    std::cout << "\nStart loading Config...\n\n";

    


    try
    {
        std::cout << "Reading steering file...";
        // load general of database
        m_database->Claws_readConfig();
        std::cout << "done!\n";
    }
    catch( UtilityException& excep )
    {
        std::cout << "\n" << excep.what() << std::endl;
    }

    try
    {
        std::cout << "Reading PSU connection settings...";
        // load PSU configs
        m_database->N6700_readConnectSettings();
        m_psu->loadConfig();
        std::cout << "done!\n";
    }
    catch( UtilityException& excep )
    {
        std::cout << "\n" << excep.what() << std::endl;
    }




/*     try
 *     {
 *         std::cout << "Reading Pico high gain settings...";
 *         // load pico configs for the set run mode...
 *         m_database->Pico_readSettings( m_database->Claws_getConfig()->runMode_HighGain );
 *         std::cout << "done!\n";
 *     }
 *     catch( UtilityException& excep )
 *     {
 *         std::cout << "\n" << excep.what() << std::endl;
 *     }
 * 
 * 
 *     try
 *     {
 *         std::cout << "Reading low gain settings...";
 *         m_database->Pico_readSettings( m_database->Claws_getConfig()->runMode_LowGain );
 *         std::cout << "done!\n";
 *     }
 *     catch( UtilityException& excep )
 *     {
 *         std::cout << "\n" << excep.what() << std::endl;
 *     }
 * 
 *     try
 *     {
 *         std::cout << "Reading intermediate settings...";
 *         // ...and for the intermediate mode
 *         m_database->Pico_readSettings( Utility::Pico_RunMode::INTERMEDIATE );
 *         std::cout << "done!\n";
 *     }
 *     catch( UtilityException& excep )
 *     {
 *         std::cout << "\n" << excep.what() << std::endl;
 *     }
 * 
 */

    return;
}





///////////////////////////////////////////////////////////////////////////////






void            ClawsRun::printData()
{

    Utility::Steering_Data tmp = *m_database->Claws_getConfig();
    std::cout << tmp << std::endl;


    for( auto& tmp_pico : *m_database->m_picoData )
    {
        // m_picoData has size four all the time. herefore print only
        // entries in which the pointer is not empty
        if( tmp_pico )
        {
            std::cout << *tmp_pico << std::endl;
        }
    }


    return;
}




///////////////////////////////////////////////////////////////////////////////
//
//              END Public Member Functions 
//
///////////////////////////////////////////////////////////////////////////////























///////////////////////////////////////////////////////////////////////////////
//
//          START PRIVATE MEMBER FUNCTIONS
//
///////////////////////////////////////////////////////////////////////////////




    ///////////////////////////////////////////////////////////////////////////
    //
    //          START Pico Member Functions
    //
    ///////////////////////////////////////////////////////////////////////////

    


    void ClawsRun::Pico_init_bySerial( )
    {

/*     
 *         std::cout << "\n";
 *         std::cout << "Pico initialization...\n";
 *         std::cout << "--------------------------------------------------------\n";
 * 
 *         // check if the picoData pointer is empty. If yes allocate new, 
 *         // if no delete first.
 *         if ( m_database->m_picoData )
 *         {
 *             // delete data behind the pointer and invoke new vector
 *             delete m_database->m_picoData;
 *             m_database->m_picoData = new std::vector< Utility::Pico_Conf_Pico* >;
 *         }
 *         else m_database->m_picoData = new std::vector< Utility::Pico_Conf_Pico* >;
 *     
 *     
 *         // check if the pico pointer is empty. If yes allocate new, if no delete first.
 *         if ( m_picos )
 *         {
 *             // close picos properly before deleting the pointer
 *             Pico_close( );
 *     
 *             // delete data behind the pointer and invoke new vector
 *             delete m_picos;
 *             m_picos = new std::vector< Pico* >;
 *         }
 *         else m_picos = new std::vector< Pico* >;
 *         
 *     
 *         std::vector< std::pair< std::string, std::string > > serialLocation;
 *         std::string sName = "Pico_Initializer.pico_";
 *         std::string nameSerial = "_serial";
 *         std::string nameLocation = "_location";
 *     
 *     
 *         std::vector<int> isInitialized;       //! Counts which serials could be initialized.
 *     
 *         // read all serials from the ini file. Four at maximum.
 *         for ( int i = 0 ; i < 4 ; ++i )
 *         {
 *             // create key
 *             std::string keySerial = sName + std::to_string(i+1) + nameSerial;
 *             std::string keyLocation= sName + std::to_string(i+1) + nameLocation;
 *             std::string tmpSerial;
 *             std::string tmpLocation;
 *     
 *             // try if there are all serial given in the picoInit.ini file.
 *             // If not, just continue!
 *             try
 *             {
 *     
 *                 boost::property_tree::ptree ptree;
 *                 boost::property_tree::ini_parser::read_ini(
 *                         m_database->getInitReader()->getInitstruct().initPico, ptree);
 *     
 *                 tmpSerial = ptree.get< std::string >( keySerial );
 *                 tmpLocation = ptree.get< std::string >( keyLocation );
 *     
 *             }
 *             catch( boost::property_tree::ptree_error excep )
 *             {
 *                 continue;
 *             }
 * 
 *     
 *             serialLocation.push_back( std::make_pair( tmpSerial, tmpLocation ) );
 *             isInitialized.push_back(1);
 * 
 *         };
 *     
 *     
 *     
 *         if ( serialLocation.size() > 0 )
 *         {
 *             
 *             for ( unsigned int ii = 0; ii < serialLocation.size(); ++ii )
 *             {
 *                 try
 *                 {
 *                     Utility::Pico_Conf_Pico *pico = new Utility::Pico_Conf_Pico( 
 *                             serialLocation.at(ii).first, 
 *                             serialLocation.at(ii).second);
 *     
 *                     m_database->m_picoData->push_back(pico);
 *                 }
 *                 catch( PicoException& excep )
 *                 {
 *                     std::cout << "For " << serialLocation.at(ii).first;
 *                     std::cout <<  ":\n" << excep.what() << "\n";
 *                     isInitialized.at(ii) = 0;
 *                     continue;
 *                 }
 *     
 *     
 *                 try
 *                 {
 *                     m_picos->push_back(
 *                             new Pico( m_database->m_picoData->at(ii) )
 *                             );
 *                 }
 *                 catch( PicoException& error )
 *                 {
 *                     isInitialized.at(ii) = 0;
 * 
 *                     // delete the data from the m_picoData vector which is not needed
 *                     delete m_database->m_picoData->at(ii);
 *                     m_database->m_picoData->at(ii) = nullptr;
 *                 };
 *     
 *             }
 *             
 *             // make m_picoData the same size as clawsRun::m_picos,
 *             // otherwise going through both vector leads to problems,
 *             // e.g. data for m_pico->at(0) is at m_picoData->at(1)
 *             std::vector< Utility::Pico_Conf_Pico* >* tmp = 
 *                 new std::vector< Utility::Pico_Conf_Pico* >;
 *             for( unsigned int tt = 0; tt < m_database->m_picoData->size(); ++tt )
 *             {
 *                 if( m_database->m_picoData->at( tt ) )
 *                 {
 *                     tmp->push_back(m_database->m_picoData->at( tt ) );
 *                     m_database->m_picoData->at( tt ) = nullptr;
 *                 }
 *             }
 * 
 *             m_database->m_picoData = tmp;
 *             tmp = nullptr;
 *         }
 * 
 * 
 *     
 *         int sumI{0};
 *         if ( serialLocation.size() > 0)
 *         {
 *             for ( unsigned int bb = 0; bb < serialLocation.size(); ++bb )
 *             {
 *                 if ( isInitialized.at(bb) == 1)
 *                 {
 *                     ++sumI;
 *                 }
 *             }
 *             std::cout << "Serials found:\t\t" << serialLocation.size();
 *             std::cout << "\nPicos intialized:\t" << sumI << "\n";
 *             std::cout << "--------------------------------------------------------\n";
 *     
 *             // Return to the user how many and which Picos have been found 
 *             // and could be initialized.
 *             for ( unsigned int ii = 0; ii < serialLocation.size(); ++ii)
 *             {
 *                 if ( isInitialized.at(ii) == 1)
 *                 {
 *                     std::cout << "#" << ii << "\t" << serialLocation.at(ii).first;
 *                     std::cout << "\t" << serialLocation.at(ii).second;
 *                     std::cout << "\tinitialized!\n";
 *                 }
 *                 else
 *                 {
 *                     std::cout << "#" << ii << "\t" << serialLocation.at(ii).first;
 *                     std::cout << "\t" << serialLocation.at(ii).second;
 *                     std::cout << "\tnot found!\n";
 *                 }
 *             }
 *         }
 *         else std::cout << "\n\tSorry no serials found! Does the ini-file exist?\n";
 *     
 *         std::cout << "--------------------------------------------------------\n";
 *         std::cout << std::endl;
 *     
 */
        return;
    }
    
    







    ///////////////////////////////////////////////////////////////////////////
    











    void ClawsRun::Pico_init_byNullptr( )
    {

    
        std::cout << "\n";
        std::cout << "Pico initialization...\n";
        std::cout << "--------------------------------------------------------\n";

        // replace picoData storage
        m_database->m_picoData.reset(new std::vector< std::shared_ptr<Utility::Pico_Conf_Pico> >);
    
    
        // check if the pico pointer is empty. If yes allocate new, if no delete first.
        if ( m_picos )
        {
            // close picos properly before deleting the pointer
            Pico_close( );
    
            // delete data behind the pointer and invoke new vector
            m_picos.reset();
            m_picos = std::make_shared<
                std::vector< std::shared_ptr<Pico> > >();
        }
        else m_picos = std::make_shared<
            std::vector< std::shared_ptr<Pico> > >();


        std::vector< std::pair< std::string, std::string > > serialLocation;
        std::string sName = "Pico_Initializer.pico_";
        std::string nameSerial = "_serial";
        std::string nameLocation = "_location";
    

        std::vector< std::pair< int, std::string > > isInitialized;//! Counts which serials could be initialized.
    
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
                        m_database->getInitReader()->
                        getInitstruct().initPico, ptree);
    
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
            catch( boost::property_tree::ptree_error& excep )
            {
                continue;
            }

    
            serialLocation.push_back( std::make_pair( tmpSerial, tmpLocation ) );
            isInitialized.push_back(std::make_pair(0,tmpSerial));

        };


        // find picos for the known serials
        std::vector<int> isFound;
        if( serialLocation.size() > 0 )
//        for( unsigned int xx = 0; xx < serialLocation.size(); ++xx )
        {

            std::vector< std::pair< int16_t, std::string > >    handleSerial;

            // initialize all connected picos and save their serial and handle
            while( true )
            {
                try
                {
                    int16_t tmp_handle{-1};
                    std::string tmp_serial = Utility::Pico_preInitializer( &tmp_handle );
                    
                    handleSerial.push_back(std::make_pair(tmp_handle, tmp_serial));
                    isFound.push_back(0);
                }
                catch( PicoException& excep )
                {
                    break;
                }
            }


            // check if read-in serial and found serial are equal
            int tcounter = 0;
            for( auto tmp_pair : handleSerial)
            {
                for( auto tmp_var : serialLocation )
                {
                    if( tmp_pair.second.compare(tmp_var.first) == 0 )
                    {
                        try
                        {
                            std::shared_ptr<Utility::Pico_Conf_Pico> pico_conf{
                                std::make_shared<Utility::Pico_Conf_Pico>(
                                        tmp_var.first,
                                        tmp_var.second)};
                            m_database->m_picoData->push_back(pico_conf);

                            m_picos->push_back(
                                    std::make_shared<Pico>(
                                        pico_conf, tmp_pair.first));
                            isFound.at(tcounter) = 1;

                            // puts the initialized value to 1
                            for( auto& tmp : isInitialized )
                            {
                                if( !tmp.second.compare(tmp_pair.second) )
                                {
                                    tmp.first = 1;
                                }
                            }
                        }
                        catch( PicoException& excep )
                        {
                            std::cout << excep.what() << "\n";
                            continue;
                        }
                    }
                }

                ++tcounter;
            }


            // close remaining picos properly
            for( unsigned int xx; xx < isFound.size(); ++xx )
            {
                if( !isFound.at(xx) )
                {
                    Utility::Pico_preClose( handleSerial.at(xx).first );
                }
            }
        };



        int sumI{0};
        for( auto& tmp : isInitialized )
        {
            sumI += tmp.first;
        }
        if ( serialLocation.size() > 0)
        {
            std::cout << "Serials found:\t\t" << serialLocation.size();
            std::cout << "\nPicos intialized:\t" << sumI << "\n";
            std::cout << "--------------------------------------------------------\n";
    
            // Return to the user how many and which Picos have been found 
            // and could be initialized.
            for ( unsigned int ii = 0; ii < serialLocation.size(); ++ii)
            {
                if ( isInitialized.at(ii).first == 1)
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
    
        std::cout << "--------------------------------------------------------\n";
        std::cout << std::endl;

        return;
    }

    
    









    ///////////////////////////////////////////////////////////////////////////
    





    
    
    
    void ClawsRun::Pico_close( )
    {
        
        // check if m_picos is empty
        if ( m_picos && (m_picos->size() > 0) ) 
        {
            // closing all pico instances
            for ( auto& tpico : *m_picos )
            {
                tpico.reset();
            }

        }
    
        return;
    }
    
    






    
    ///////////////////////////////////////////////////////////////////////////







    void ClawsRun::Pico_runBlock( Utility::Claws_Gain gain )
    {

        Utility::Claws_Gain tgain;
        unsigned int tloops;
        bool isPhysics;

        switch( gain )
        {
            case Utility::Claws_Gain::INTERMEDIATE:
                tgain = Utility::Claws_Gain::INTERMEDIATE;
                tloops = m_database->Claws_getConfig()->loops_Intermediate;
                isPhysics = false;
                break;

            default:
                tgain = m_database->Claws_getConfig()->gain_current;
                tloops = m_database->Claws_getConfig()->loops_Physics;
                isPhysics = true;
        }


        // load each pico with a config and make it ready before running in the
        // loop
        for( auto& tmp : *m_picos )
        {
            tmp->setConfig( tgain );
            tmp->setReadyBlock();
        }

        // define work
        auto work = [](
                unsigned int subRunNum,
                std::shared_ptr<Pico> tpico,
                std::shared_ptr<ProcessData> dataProcessor )
                {
                    tpico->runBlock();
                    dataProcessor->sync(subRunNum, tpico);
                };


        // create a process data instance and give it the vector with the pointers
        // and the claws-global counter
        std::shared_ptr<ProcessData> dataProcessor{
            std::make_shared<ProcessData>( 
                    m_picos, m_database->Claws_getCounter() )};
        try
        {
            dataProcessor->save()->setSaveLocation(
                    m_database->Claws_getConfig()->path_saveData);
        }
        catch( ClawsException& excep )
        {
            std::cout << excep.what() << "\n";
            std::cout << "Stopping current run...\n";
            return;
        }


        // let each pico acquire data in its own thread
        std::vector<std::thread>    workers;
        ROOT::EnableThreadSafety();
        for( unsigned int counter1 = 0; counter1 < tloops; ++counter1 )
        {
            if( gain == Utility::Claws_Gain::INTERMEDIATE )
            {
                std::cout << "Intermediate #" << counter1 << "\n";
            }
            else std::cout << "Physics #" << counter1 << "\n";

            for( std::shared_ptr<Pico> tpico : *m_picos )
            {
                workers.emplace_back(
                                    work, 
                                    counter1,
                                    tpico,
                                    dataProcessor);
//                std::cout << "Thread started...\n";
            }

//            std::cout << "Waiting for threads...\n";
            for( auto& worker : workers )
            {
                if( worker.joinable() )
                {
                    worker.join(); 
//                    std::cout << "Thread ended...\n";
                }
            }
            workers.clear();

            if( isPhysics )
            {
                dataProcessor->save()->physics(counter1);
            }
            else dataProcessor->save()->intermediate(counter1);
//            std::cout << "Data saved\n";
//            dataProcessor.clear();
//            std::cout << "Data cleared.\n";

            
        }


        std::cout << "Going to stop picos from data taking...\n";
        // tell pico that data taking is done
        for( auto& tmp : *m_picos )
        {
            try
            {
                tmp->stop();
            }
            catch( PicoException& excep )
            {
                std::cout << excep.what() << std::endl;
            }
            catch( ClawsException& excep )
            {
                std::cout << excep.what() << std::endl;
            }
            catch( std::exception& excep )
            {
                std::cout << excep.what() << std::endl;
            }

            std::cout << "Pico closed\n";
        }

        return;
    }







    ///////////////////////////////////////////////////////////////////////////







    void ClawsRun::Pico_runRapid()
    {

        // ensures that the first case is used always, I might extend this method
        // at some point in future
        Utility::Claws_Gain gain{Utility::Claws_Gain::INTERMEDIATE};

        Utility::Claws_Gain tgain;
        unsigned int tloops;
        bool isPhysics;

        switch( gain )
        {
            case Utility::Claws_Gain::INTERMEDIATE:
                tgain = Utility::Claws_Gain::INTERMEDIATE;
                tloops = m_database->Claws_getConfig()->loops_Intermediate;
                isPhysics = false;

                break;

            default:
                tgain = m_database->Claws_getConfig()->gain_current;
                tloops = m_database->Claws_getConfig()->loops_Physics;
                isPhysics = true;
        }


        // load each pico with a config and make it ready before running in the
        // loop
        for( auto& tmp : *m_picos )
        {
            tmp->setConfig( tgain );
            tmp->setReadyRapid();
        }

        // define work
        auto work = [](
                std::shared_ptr<Pico> tpico,
                std::shared_ptr<ProcessData> dataProcessor )
                {
                    tpico->runRapid();
//                    dataProcessor->sync(subRunNum, tpico);
                };


        // create a process data instance and give it the vector with the pointers
        // and the claws-global counter
        std::shared_ptr<ProcessData> dataProcessor{
            std::make_shared<ProcessData>( 
                    m_picos, m_database->Claws_getCounter() )};
        try
        {
            dataProcessor->save()->setSaveLocation(
                    m_database->Claws_getConfig()->path_saveData);
        }
        catch( ClawsException& excep )
        {
            std::cout << excep.what() << "\n";
            std::cout << "Stopping current run...\n";
            return;
        }


        // let each pico acquire data in its own thread
        std::vector<std::thread>    workers;
        ROOT::EnableThreadSafety();
        for( std::shared_ptr<Pico> tpico : *m_picos )
        {
                workers.emplace_back(
                                    work, 
                                    tpico,
                                    dataProcessor);
//                std::cout << "Thread started...\n";
        };


        for( auto& worker : workers )
        {
            if( worker.joinable() )
            {
                worker.join(); 
//                std::cout << "Thread ended...\n";
            }
        }
        workers.clear();

        
        for( auto& tmp1 : *m_picos->at(0)->getCh(0)->getBufferRapid() )
        {
            for( auto& tmp2 : *tmp1 )
            {
                std::cout << tmp2 << "  " << std::flush;
            };
            std::cout << std::endl;
        };

//            if( isPhysics )
//            {
//                dataProcessor->save()->physics(counter1);
//            }
//            else dataProcessor->save()->intermediate(counter1);
//            std::cout << "Data saved\n";
//            dataProcessor.clear();
//            std::cout << "Data cleared.\n";

            

        std::cout << "Going to stop picos from data taking...\n";
        // tell pico that data taking is done
        for( auto& tmp : *m_picos )
        {
            try
            {
                tmp->stop();
            }
            catch( PicoException& excep )
            {
                std::cout << excep.what() << std::endl;
            }
            catch( ClawsException& excep )
            {
                std::cout << excep.what() << std::endl;
            }
            catch( std::exception& excep )
            {
                std::cout << excep.what() << std::endl;
            }

            std::cout << "Pico closed\n";
        }

        return;
    }
    










    ///////////////////////////////////////////////////////////////////////////
    //
    //          END Pico Member Functions
    //
    ///////////////////////////////////////////////////////////////////////////

















    ///////////////////////////////////////////////////////////////////////////
    //
    //          START PSU Member Functions
    //
    ///////////////////////////////////////////////////////////////////////////


    void ClawsRun::PSU_init()
    {

        // first read the data into the data base
        try
        {
            m_database->N6700_readConnectSettings();
        }
        catch( boost::property_tree::ptree_error& excep)
        {
            std::cout << "Property tree exception: ";
            std::cout << excep.what() << std::endl;
        }


        // then initialize psu
        try
        {
            m_psu = new N6700(m_database);
        }
        catch( SCPIException& excep)
        {
            std::cout << excep.what() << std::endl;
        }
        catch( std::exception& excep )
        {
            std::cout << "std::exception: ";
            std::cout << excep.what() << std::endl;
        }
    
        return;
    }




    ///////////////////////////////////////////////////////////////////////////
    //
    //          END PSU Member Functions
    //
    ///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
//          END PRIVATE MEMBER FUNCTIONS
//
///////////////////////////////////////////////////////////////////////////////

