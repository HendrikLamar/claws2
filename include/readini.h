// =====================================================================================
// 
//       Filename:  readini.h
// 
//    Description:  The Ini manager.
// 
//        Version:  1.0
//        Created:  09.05.2017 11:10:08
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#ifndef READINI_H 
#define READINI_H 

#include <boost/filesystem.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <string>

class ReadIni
{
    public:
        //! Standard Constructor with hardcoded path for ini-file
        ReadIni();

        //! Standard Destructor
        virtual ~ReadIni();




        //! Contains all the values from the initializer.ini file.
        struct Initstruct
        {
            
            std::string m_fileIntermediate;

            std::string m_filePhysicsObermaier;
            std::string m_filePhysicsMerkel;
            std::string m_filePhysicsSchiffer;
            std::string m_filePhysicsKlum;
            std::string m_filePhysicsGarrn;

            std::string m_filePowerSupply;
        };




        //! Reads in the very first initializer and return a struct containing all
        // the variables.
        Initstruct initialize();





        //! Returns the key you specify from the file you specify.
        //! Since it is a tempalte function it is specified header only.
        template < class T >
        T getKey( std::string file, std::string key )
        {
            boost::property_tree::ptree ptree;
            boost::property_tree::ini_parser::read_ini(file.c_str(), ptree);
        
            T output = ptree.get< T >( key );
        
            return output;
            
        };






    protected:
        //! Contains the current working directory.
        boost::filesystem::path m_pathIniDir;

        Initstruct              m_initstruct;



}; 


#endif // READINI_H
