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

    std::string::size_type sz;

    std::string sport = "5025";
    unsigned short port = std::stoi(sport, &sz);

    std::cout << port << "\t" << sport << "\t" << sz << std::endl;

    return 0;
};
