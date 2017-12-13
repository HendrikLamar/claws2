// =====================================================================================
// 
//
//       Filename:  client_put.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  21.10.2017 16:15:10
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include "cadef.h"
#include <unistd.h>


//#include <chrono>
//#include <thread>
#include <iostream>

int main()
{

    std::cout << "Here 1\n";

// Kanalinstance
chid  mychid;
    std::cout << "Here 2\n";


// beim start:

    std::cout << "Here 3\n";
  SEVCHK(ca_context_create(ca_disable_preemptive_callback),"ca_context_create");
    std::cout << "Here 4\n";
  SEVCHK(ca_create_channel("MTEST:RAND",NULL,NULL,10,&mychid),"ca_create_channel failure");
    std::cout << "Here 5\n";
  SEVCHK(ca_pend_io(5.0),"ca_pend_io failure");
    std::cout << "Here 6\n";

// bei jedem update


  double data=0;
  for( int i = 0; i < 10; ++i )
  {
    data = i;
    std::cout << "Here 7\n";
    SEVCHK(ca_put(DBR_DOUBLE,mychid,(void*)&data),"ca_set failure");
    std::cout << "Here 8\n";
    SEVCHK(ca_pend_io(5.0),"ca_pend_io failure");
    std::cout << "Here 9\n";

    sleep(1);
//    std::this_thread::sleep_for(std::chrono::seconds(1));
  ;}

  return 0;
}
