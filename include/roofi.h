// =====================================================================================
// 
//       Filename:  roofie.h
// 
//    Description:  The ROOt File & Ini manager.
//                  Stores all the data. RAW and online computed.
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

#ifndef ROOFI_H
#define ROOFI_H

#include <boost/filesystem.hpp>
#include <string>

namespace Roofi{

    //! Contains the current working directory.
    extern boost::filesystem::path pathWorkingDir;

    //! Contains all the values from the initializer.ini file.
    struct Initstruct{
        
        std::string pathConfigFile;
        std::string fileIntermediate;

        std::string filePhysicsObermaier;
        std::string filePhysicsMerkel;
        std::string filePhysicsSchiffer;
        std::string filePhysicsKlum;
        std::string filePhysicsGarrn;

        std::string Pico1Serial;
        std::string Pico1Alias;

/*         std::string Pico2Serial;
 *         std::string Pico2Alias;
 * 
 *         std::string Pico3Serial;
 *         std::string Pico3Alias;
 * 
 *         std::string Pico4Serial;
 *         std::string Pico4Alias;
 */
    };

    //! Reads in the very first initializer and return a struct containing all
    // the variables.
    Roofi::Initstruct start_readIni();

}

#endif
