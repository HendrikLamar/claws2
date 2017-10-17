// =====================================================================================
// 
//       Filename:  main_shr_ptr.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  17.10.2017 23:01:53
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include <memory>
#include <iostream>

int main()
{

    std::shared_ptr< int > ptr1{std::make_shared< int >(1)};

    std::shared_ptr< int > ptr2{ptr1};

    ptr1 = std::make_shared< int >(5);

    std::cout << "Ptr1: " << *ptr1 << std::endl;
    std::cout << "Ptr2: " << *ptr2 << std::endl;



    return 0;
}
