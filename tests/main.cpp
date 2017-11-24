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

#include <cstring>
#include <string>
#include <iostream>
#include <vector>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/lexical_cast.hpp>


int main()
{

    std::string stri = "test";

    int llength = stri.end() - stri.begin();
//    ++llength;
    unsigned char foo[llength+1];
    int ii = 0;
    for ( std::string::iterator it = stri.begin(); it < stri.end(); ++it)
    {
        std::cout << llength - (stri.end() - it) << std::endl;
        foo[ii] = *it;
        ++ii;
    }


    std::cout << "Orig: " << stri << std::endl;
    std::cout << "Copy: " << foo << std::endl;

    int16_t boo = 25;
    int16_t* too;

    too = &boo;

    std::cout << "\nOrig: " << boo << std::endl;
    std::cout << "Copy: " << *too << std::endl;


    boo = 33;

    std::cout << "\nOrig: " << boo << std::endl;
    std::cout << "Copy: " << *too << std::endl;




    ///////////////////////////////////////////////////////////////////////////


    int     mint;
    int*    ptr = &mint;

    std::cout << "Uninit: " << *ptr << std::endl;

    mint = 5;

    std::cout << "Init: " << *ptr << std::endl;

    
    std::string output;

    output = "t";
    std::cout << output.size() << std::endl;



    return 0;
}
