// =====================================================================================
// 
//       Filename:  main.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  24.08.2017 15:31:26
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include <string>
#include <iostream>


int main()
{

    std::string stri = "test";

    int llength = stri.end() - stri.begin();
//    ++llength;
    unsigned char foo[llength];
    int ii = 0;
    for ( std::string::iterator it = stri.begin(); it < stri.end(); ++it)
    {
        std::cout << llength - (stri.end() - it) << std::endl;
        foo[ii] = *it;
        ++ii;
    }


    std::cout << "Orig: " << stri << std::endl;
    std::cout << "Copy: " << foo << std::endl;


    return 0;
}
