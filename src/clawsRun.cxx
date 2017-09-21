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

    Pico_init();

    PSU_init();


    return;
}






///////////////////////////////////////////////////////////////////////////////






void            ClawsRun::runBlockMode()
{

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

    


    void ClawsRun::Pico_init( )
    {

    
        std::cout << "\n";
        std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
        std::cout << "\tPico initialization...\n";
        std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

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
                            new Pico( m_database->m_picoData->at(ii) )
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
    
    







    ///////////////////////////////////////////////////////////////////////////
    





    
    
    
    void ClawsRun::Pico_close( )
    {
        
        // check if m_picos is empty
        if ( m_picos && (m_picos->size() > 0) ) 
        {
            // closing all pico instances
            for ( Pico* tpico : *m_picos )
            {
                try
                {
                    tpico->close();
                }
                catch( PicoException& excep )
                {
                    throw;
                }
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

