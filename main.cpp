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


// Kanalinstance
chid  mychid;


// beim start:

  SEVCHK(ca_context_create(ca_disable_preemptive_callback),"ca_context_create");
  SEVCHK(ca_create_channel("MTEST:RAND",NULL,NULL,10,&mychid),"ca_create_channel failure");
    std::cout << "Here 5\n";
  SEVCHK(ca_pend_io(5.0),"ca_pend_io failure");
    std::cout << "Here 6\n";

// bei jedem update


  double data=0;
  for( int i = 0; i < 10; ++i )
  {
    data = i;
    std::cout << "Set: " << i << std::endl;
    SEVCHK(ca_put(DBR_DOUBLE,mychid,(void*)&data),"ca_set failure");
    SEVCHK(ca_pend_io(5.0),"ca_pend_io failure");

    sleep(3);
//    std::this_thread::sleep_for(std::chrono::seconds(1));
  ;}

  return 0;
}
