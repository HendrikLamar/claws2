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
            
            std::string SteeringFile;

            std::string intermediate;
            std::string constant_config;

            std::string Obermaier_HG;
            std::string Merkel_HG;
            std::string Schiffer_LG;
            std::string Klum_LG;
            std::string Garrn;

            std::string PowerSupply;
            std::string initPico;

            std::string runNumber;
        };



        //! Reads in the very first initializer and return a struct containing all
        // the variables.
        void            initialize();

        Initstruct      getInitstruct() const;



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
