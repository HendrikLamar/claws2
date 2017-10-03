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



#include "clawsRun.h"
#include "pico.h"
#include "n6700.h"
#include "database.h"

#include <boost/property_tree/exceptions.hpp>

#include <iostream>
#include <exception>


///////////////////////////////////////////////////////////////////////////////


ClawsRun::ClawsRun() :
    m_database ( new Database() ),
    m_picos( nullptr ),
    m_psu( nullptr )
{

}


ClawsRun::~ClawsRun()
{

    Pico_close();
    
    // check if pointing to nullptr
    if( m_picos )
    {
        delete m_picos;
        m_picos = nullptr;
    }

    // check if pointing to nullptr
    if( m_psu )
    {
        delete m_psu;
        m_psu = nullptr;
    }

    delete m_database;
    m_database = nullptr;

}


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////



















///////////////////////////////////////////////////////////////////////////////
//
//              START Public Member Functions 
//
///////////////////////////////////////////////////////////////////////////////






Database*   ClawsRun::getDatabase()
{
    return m_database;
}






///////////////////////////////////////////////////////////////////////////////





void        ClawsRun::initialize()
{

//    Pico_init_bySerial();
    Pico_init_byNullptr();
    m_database->setNoOfPicosInitialized(m_picos->size());

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

    Pico_runInter();

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
        m_database->readSteeringFile();
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
        std::cout << "done!\n";
    }
    catch( UtilityException& excep )
    {
        std::cout << "\n" << excep.what() << std::endl;
    }




    try
    {
        std::cout << "Reading Pico high gain settings...";
        // load pico configs for the set run mode...
        m_database->Pico_readSettings( m_database->getSteeringData()->runMode_HighGain );
        std::cout << "done!\n";
    }
    catch( UtilityException& excep )
    {
        std::cout << "\n" << excep.what() << std::endl;
    }


    try
    {
        std::cout << "Reading low gain settings...";
        m_database->Pico_readSettings( m_database->getSteeringData()->runMode_LowGain );
        std::cout << "done!\n";
    }
    catch( UtilityException& excep )
    {
        std::cout << "\n" << excep.what() << std::endl;
    }

    try
    {
        std::cout << "Reading intermediate settings...";
        // ...and for the intermediate mode
        m_database->Pico_readSettings( Utility::Pico_RunMode::INTERMEDIATE );
        std::cout << "done!\n";
    }
    catch( UtilityException& excep )
    {
        std::cout << "\n" << excep.what() << std::endl;
    }


    return;
}





///////////////////////////////////////////////////////////////////////////////






void            ClawsRun::printData()
{

    Utility::Steering_Data tmp = *m_database->getSteeringData();
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

    
        std::cout << "\n";
        std::cout << "Pico initialization...\n";
        std::cout << "--------------------------------------------------------\n";

        // check if the picoData pointer is empty. If yes allocate new, 
        // if no delete first.
        if ( m_database->m_picoData )
        {
            // delete data behind the pointer and invoke new vector
            delete m_database->m_picoData;
            m_database->m_picoData = new std::vector< Utility::Pico_Data_Pico* >;
        }
        else m_database->m_picoData = new std::vector< Utility::Pico_Data_Pico* >;
    
    
        // check if the pico pointer is empty. If yes allocate new, if no delete first.
        if ( m_picos )
        {
            // close picos properly before deleting the pointer
            Pico_close( );
    
            // delete data behind the pointer and invoke new vector
            delete m_picos;
            m_picos = new std::vector< Pico* >;
        }
        else m_picos = new std::vector< Pico* >;
        
    
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
                        m_database->getInitReader()->getInitstruct().initPico, ptree);
    
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
                    Utility::Pico_Data_Pico *pico = new Utility::Pico_Data_Pico( 
                            serialLocation.at(ii).first, 
                            serialLocation.at(ii).second);
    
                    m_database->m_picoData->push_back(pico);
                }
                catch( PicoException& excep )
                {
                    std::cout << "For " << serialLocation.at(ii).first;
                    std::cout <<  ":\n" << excep.what() << "\n";
                    initCounter.at(ii) = 0;
                    continue;
                }
    
    
                try
                {
                    m_picos->push_back(
                            new Pico( m_database->m_picoData->at(ii) )
                            );
                }
                catch( PicoException& error )
                {
                    initCounter.at(ii) = 0;

                    // delete the data from the m_picoData vector which is not needed
                    delete m_database->m_picoData->at(ii);
                    m_database->m_picoData->at(ii) = nullptr;
                };
    
            }
            
            // make m_picoData the same size as clawsRun::m_picos,
            // otherwise going through both vector leads to problems,
            // e.g. data for m_pico->at(0) is at m_picoData->at(1)
            std::vector< Utility::Pico_Data_Pico* >* tmp = 
                new std::vector< Utility::Pico_Data_Pico* >;
            for( unsigned int tt = 0; tt < m_database->m_picoData->size(); ++tt )
            {
                if( m_database->m_picoData->at( tt ) )
                {
                    tmp->push_back(m_database->m_picoData->at( tt ) );
                    m_database->m_picoData->at( tt ) = nullptr;
                }
            }

            m_database->m_picoData = tmp;
            tmp = nullptr;
        }


    
        int sumI{0};
        if ( serialLocation.size() > 0)
        {
            for ( unsigned int bb = 0; bb < serialLocation.size(); ++bb )
            {
                if ( initCounter.at(bb) == 1)
                {
                    ++sumI;
                }
            }
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
    
        std::cout << "--------------------------------------------------------\n";
        std::cout << std::endl;
    
        return;
    }
    
    







    ///////////////////////////////////////////////////////////////////////////
    











    void ClawsRun::Pico_init_byNullptr( )
    {

    
        std::cout << "\n";
        std::cout << "Pico initialization...\n";
        std::cout << "--------------------------------------------------------\n";

        // check if the picoData pointer is empty. If yes allocate new, 
        // if no delete first.
        if ( m_database->m_picoData )
        {
            // delete data behind the pointer and invoke new vector
            delete m_database->m_picoData;
            m_database->m_picoData = new std::vector< Utility::Pico_Data_Pico* >;
        }
        else m_database->m_picoData = new std::vector< Utility::Pico_Data_Pico* >;
    
    
        // check if the pico pointer is empty. If yes allocate new, if no delete first.
        if ( m_picos )
        {
            // close picos properly before deleting the pointer
            Pico_close( );
    
            // delete data behind the pointer and invoke new vector
            delete m_picos;
            m_picos = new std::vector< Pico* >;
        }
        else m_picos = new std::vector< Pico* >;


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
                        m_database->getInitReader()->getInitstruct().initPico, ptree);
    
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
                int16_t tmp_handle;
                std::string tmp_serial;
                try
                {
                    tmp_serial = Utility::Pico_preInitializer( &tmp_handle );
                }
                catch( PicoException& excep )
                {
                    initCounter.at(ii) = 0;
                    continue;
                }


                // search for correct location to the given serial
                for( unsigned int tt = 0; tt < serialLocation.size(); ++tt )
                {
                    if( tmp_serial.compare( serialLocation.at(tt).first ) == 0 )
                    {
                        
                        // first, create data struct to pass it to the pico in
                        // the second step
                        try
                        {
                            Utility::Pico_Data_Pico *pico = new Utility::Pico_Data_Pico( 
                                    serialLocation.at(tt).first, 
                                    serialLocation.at(tt).second);
    
                            m_database->m_picoData->push_back(pico);
                        }
                        catch( PicoException& excep )
                        {
                            std::cout << "For " << serialLocation.at(tt).first;
                            std::cout <<  ":\n" << excep.what() << "\n";
                            initCounter.at(ii) = 0;
                            continue;
                        }
    
    
                        // second, create a pico instance with the data struct
                        // created before and the handle value
                        try
                        {
                            m_picos->push_back(
                                    new Pico( m_database->m_picoData->at(ii), tmp_handle )
                                    );
                        }
                        catch( PicoException& error )
                        {
                            initCounter.at(ii) = 0;

                            // delete the data from the m_picoData vector which is not needed
                            delete m_database->m_picoData->at(ii);
                            m_database->m_picoData->at(ii) = nullptr;
                        };
    
                    }
                }
            }
            
//            // make m_picoData the same size as clawsRun::m_picos,
//            // otherwise going through both vector leads to problems,
//            // e.g. data for m_pico->at(0) is at m_picoData->at(1)
//            std::vector< Utility::Pico_Data_Pico* >* tmp = 
//                new std::vector< Utility::Pico_Data_Pico* >;
//            for( unsigned int bb = 0; bb < m_database->m_picoData->size(); ++bb )
//            {
//                if( m_database->m_picoData->at( bb ) )
//                {
//                    tmp->push_back(m_database->m_picoData->at( bb ) );
//                    m_database->m_picoData->at( bb ) = nullptr;
//                }
//            }
//
//            m_database->m_picoData = tmp;
//            tmp = nullptr;
        }


    
        int sumI{0};
        if ( serialLocation.size() > 0)
        {
            for ( unsigned int bb = 0; bb < serialLocation.size(); ++bb )
            {
                if ( initCounter.at(bb) == 1)
                {
                    ++sumI;
                }
            }
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
            for ( Pico* tpico : *m_picos )
            {
                delete tpico;
                tpico = nullptr;
            }

        }
    
        return;
    }
    
    






    
    ///////////////////////////////////////////////////////////////////////////







    void ClawsRun::Pico_runInter()
    {
        //! \todo Extend for multiple Picos!
        for( unsigned int ii = 0; ii < m_picos->size(); ++ii)
        {
        
            try
            {
//                m_picos->at(ii)->loadConfig( Utility::INTER );
                m_picos->at(ii)->setReadyBlock();
                m_picos->at(ii)->runBlock();

                std::vector< int16_t >* data = m_picos->at(ii)->getCh(ii)->getBuffer();


                for( auto& tmp : *data )
                {
                    std::cout << tmp << "\n";
                }

                m_picos->at(ii)->stop();
            }
            catch( ChannelException& excep )
            {
                std::cout << excep.what() << std::endl;
            }
            catch( PicoException& excep )
            {
                std::cout << excep.what() << std::endl;
            }
            catch( ClawsException& excep )
            {
                std::cout << excep.what() << std::endl;
            }
        }

        return;
    }







    ///////////////////////////////////////////////////////////////////////////













    









/*     void Database::Pico_readSettings( 
 *             Utility::Pico_RunMode mode, 
 *             std::vector< Pico* >* vPicos 
 *             )
 *     {
 *         
 *         // since we have four picos, it loops fouer times
 *         for ( unsigned int i = 0; i < vPicos->size(); ++i )
 *         {
 *     
 *             ///////////////////////////////////////////////////////////////////////
 *             // catch exceptions for each settings separatly and inform the user.
 *             
 *     
 *             // reading in channel settings
 *             try
 *             {
 *                 Pico_readChannelsSettings( mode, i );
 *             }
 *             catch( boost::property_tree::ptree_error excep )
 *             {
 *                 std::cout << "\nChannel settings couldn't be read-in for Pico# ";
 *                 std::cout << i+1 << "\n";
 *                 std::cout << "Please check the ini-file arguments and Claws docs.\n";
 *                 std::cout << excep.what() << "\n" << std::endl;
 *                 continue;
 *             }
 *             catch( PicoException excep )
 *             {
 *                 std::cout << "\nChannel settings couldn't be read-in for Pico# ";
 *                 std::cout << i+1 << "\n";
 *                 std::cout << "Please check the ini-file arguments and Claws docs.\n";
 *                 std::cout << excep.what() << "\n" << std::endl;
 *                 continue;
 *             }
 *     
 *     
 *     
 *             // reading in aquisition settings
 *             try
 *             {
 *                 Pico_readAquisitionSettings( mode, i );
 *             }
 *             catch( boost::property_tree::ptree_error excep )
 *             {
 *                 std::cout << "\nAquisition settings couldn't be read-in for Pico# ";
 *                 std::cout << i+1 << "\n";
 *                 std::cout << "Please check the ini-file arguments and Claws docs.\n";
 *                 std::cout << excep.what() << "\n" << std::endl;
 *                 continue;
 *             }
 *             catch( PicoException excep )
 *             {
 *                 std::cout << "\nAquisition settings couldn't be read-in for Pico# ";
 *                 std::cout << i+1 << "\n";
 *                 std::cout << "Please check the ini-file arguments and Claws docs.\n";
 *                 std::cout << excep.what() << "\n" << std::endl;
 *                 continue;
 *             }
 *     
 *     
 *     
 *             // reading in simple trigger settings
 *             try
 *             {
 *                 Pico_readTriggerSimpleSettings( mode, i );
 *             }
 *             catch( boost::property_tree::ptree_error excep )
 *             {
 *                 std::cout << "\nSimple Trigger settings couldn't be read-in for Pico# ";
 *                 std::cout << i+1 << "\n";
 *                 std::cout << "Please check the ini-file arguments and Claws docs.\n";
 *                 std::cout << excep.what() << "\n" << std::endl;
 *                 continue;
 *             }
 *             catch( PicoException excep )
 *             {
 *                 std::cout << "\nSimple Trigger settings couldn't be read-in for Pico# ";
 *                 std::cout << i+1 << "\n";
 *                 std::cout << "Please check the ini-file arguments and Claws docs.\n";
 *                 std::cout << excep.what() << "\n" << std::endl;
 *                 continue;
 *             }
 *     
 *     
 *     
 *     
 *             // reading in advanced trigger settings
 *             //! \todo This function needs to be filled with some code!
 *     //        try
 *     //        {
 *     //            Pico_readTriggerAdvSettings( mode, i );
 *     //        }
 *     //        catch( boost::property_tree::ptree_error excep )
 *     //        {
 *     //            std::cout << "\nAdvanced Trigger settings couldn't be read-in for Pico# ";
 *     //            std::cout << i+1 << "\n";
 *     //            std::cout << "Please check the ini-file arguments and Claws docs.\n";
 *     //            std::cout << excep.what() << "\n" << std::endl;
 *     //            continue;
 *     //        }
 *     
 *         }
 *     
 *     
 *         return;
 *     }
 */


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

