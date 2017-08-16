// =====================================================================================
// 
//       Filename:  main.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  24.07.2017 15:33:05
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================


//#include "statemachine.h"
//#include "database.h"

#include <iostream>
#include <string>


int main()
{

/*     Database database;
 * 
 *     database.N6700_readPSConf();
 *     database.N6700_readChSet();
 */


    std::string cmd = "S";
    std::string fcmd;

    std::string::iterator it = cmd.begin();
    std::string::iterator itB = cmd.begin();
    std::string::iterator itE = cmd.end();

//    if ( cmd.size() > 1 )
    {
        for ( itB; itB != itE; ++itB )
        {
            std::cout << itB - it << std::endl;
           if ( itB == itE - 1 )
           {
                fcmd += *itB;
                break;
           }
            
           fcmd += *itB;
           fcmd += ",";

        }
    }


    std::cout << "Before: " << cmd << "\tAfter: " << fcmd << std::endl;

/*     std::cout << itE - itB << std::endl;
 * //    std::cout << itB << "\n" << itE << "\n";
 *     std::cout << cmd.size() << std::endl;
 */


    return 0;
};
