// =====================================================================================
// 
//       Filename:  roofi.cxx
// 
//    Description:  The ROOt File & Ini manager.
//                  Stores all the data. RAW and online computed.
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
    m_pathWorkingDir(boost::filesystem::current_path())
{

    try
    {
        m_initstruct = initialize();
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


ReadIni::Initstruct ReadIni::initialize(){

    // relativ path to the initializer file 
    std::string fileInitializer             {"/initializer.ini"};

    // absolute path to the initializer file with complex conversion from
    // boost::path to string type
    std::string pathInitializer = m_pathWorkingDir.string() + fileInitializer;

    // create and parse the property tree
    boost::property_tree::ptree ptree;
    boost::property_tree::ini_parser::read_ini(pathInitializer.c_str(), ptree);
    
    // uniform initilization of the Initstruct structure
    ReadIni::Initstruct initStruct{\
                ptree.get<std::string>("Initializer.ConfigFile_Path"),              \
                ptree.get<std::string>("Initializer.ConfigFile_Intermediate"),      \
                ptree.get<std::string>("Initializer.ConfigFile_Physics_Obermaier"), \
                ptree.get<std::string>("Initializer.ConfigFile_Physics_Merkel"),    \
                ptree.get<std::string>("Initializer.ConfigFile_Physics_Schiffer"),  \
                ptree.get<std::string>("Initializer.ConfigFile_Physics_Klum"),      \
                ptree.get<std::string>("Initializer.ConfigFile_Physics_Garrn"),     \
                ptree.get<std::string>("Initializer.ConfigFile_PowerSupply"),     \
    };
    
    return initStruct;
};







