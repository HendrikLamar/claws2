// =====================================================================================
// 
//       Filename:  readini.cxx
// 
//    Description:  The Ini manager.
// 
//        Version:  1.0
//        Created:  08.05.2017 17:50:24
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <iostream>
#include <string>

#include "readini.h"

///////////////////////////////////////////////////////////////////////////////



ReadIni::ReadIni() :
    m_pathIniDir(boost::filesystem::canonical(
            boost::filesystem::current_path()).string() + "/ini_files/")
{

    try
    {
        initialize();
    }
    catch( boost::property_tree::ptree_error excep) 
    {
        std::cout << "Ptree exception: " << excep.what() << std::endl;
    }
};


///////////////////////////////////////////////////////////////////////////////


ReadIni::~ReadIni()
{};


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////


void ReadIni::initialize(){

    // relativ path to the initializer file 
    std::string fileInitializer             {"/initializer.ini"};

    // absolute path to the initializer file with complex conversion from
    // boost::path to string type
    std::string pathInitializer = m_pathIniDir.string() + fileInitializer;

    // create and parse the property tree
    boost::property_tree::ptree ptree;
    boost::property_tree::ini_parser::read_ini(pathInitializer.c_str(), ptree);
    
    // uniform initilization of the Initstruct structure
    m_initstruct = {\
//                ptree.get<std::string>("Initializer.ConfigFile_Path"),            

        m_pathIniDir.string() + ptree.get<std::string>("Path.ClawsDAQ_config"),        \
        m_pathIniDir.string() + ptree.get<std::string>("Path.intermediate"),      \
        m_pathIniDir.string() + ptree.get<std::string>("Path.constant_config"),      \
        m_pathIniDir.string() + ptree.get<std::string>("Path.physics_Obermaier"), \
        m_pathIniDir.string() + ptree.get<std::string>("Path.physics_Merkel"),    \
        m_pathIniDir.string() + ptree.get<std::string>("Path.physics_Schiffer"),  \
        m_pathIniDir.string() + ptree.get<std::string>("Path.physics_Klum"),      \
        m_pathIniDir.string() + ptree.get<std::string>("Path.physics_Garrn"),     \
        m_pathIniDir.string() + ptree.get<std::string>("Path.powerSupply"),     \
        m_pathIniDir.string() + ptree.get<std::string>("Path.picoInit"),
        m_pathIniDir.string() + ptree.get<std::string>("Path.runNumber")
    };

    
};


ReadIni::Initstruct ReadIni::getInitstruct() const
{
    return m_initstruct;
}


