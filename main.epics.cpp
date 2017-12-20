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
#include <string>
#include <vector>

int main()
{


// Kanalinstance
chid  var1;
chid  var2;
chid  var3;

chid ch1;
chid ch2;
chid ch3;

// beim start:

    std::string system_prefix{"BEAST:"};
    std::string host_prefix{"CLAWS:"};
    std::string var{"RC:STATUS"};
    std::string nvar2{"GRAPH"};
    std::string nvar3{"GRAPH2D"};

    std::string nch1{"CH1"};
    std::string nch2{"CH2"};
    std::string nch3{"CH3"};

  SEVCHK(ca_context_create(ca_disable_preemptive_callback),"ca_context_create");

  std::string tmpVar{system_prefix + host_prefix + var};
  SEVCHK(ca_create_channel(tmpVar.c_str(),NULL,NULL,10,&var1),"ca_create_channel failure");
  tmpVar = system_prefix + host_prefix + nvar2;
  SEVCHK(ca_create_channel(tmpVar.c_str(),NULL,NULL,10,&var2),"ca_create_channel failure");
  tmpVar = system_prefix + host_prefix + nvar3;
  SEVCHK(ca_create_channel(tmpVar.c_str(),NULL,NULL,10,&var3),"ca_create_channel failure");

  tmpVar = system_prefix + host_prefix + nch1;
  SEVCHK(ca_create_channel(tmpVar.c_str(),NULL,NULL,10,&ch1),"ca_create_channel failure");
  tmpVar = system_prefix + host_prefix + nch2;
  SEVCHK(ca_create_channel(tmpVar.c_str(),NULL,NULL,10,&ch2),"ca_create_channel failure");
  tmpVar = system_prefix + host_prefix + nch3;
  SEVCHK(ca_create_channel(tmpVar.c_str(),NULL,NULL,10,&ch3),"ca_create_channel failure");

  SEVCHK(ca_pend_io(5.0),"ca_pend_io failure");

// bei jedem update


  std::vector< std::string > data{"IDLE", "INTERMEDIATE","PHYSICS","OFFLINE"};
  std::vector< int > data2{1,5,1,2,2,2,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,8,8,8,9,9,9};
  SEVCHK(ca_array_put(DBF_INT,data2.size(),var2,(void*)&data2.at(0)),"ca_set failure");
  SEVCHK(ca_pend_io(5.0),"ca_pend_io failure");



//  std::vector< float > data3{0.1,0.1,0.1,0.2,0.0,0.5};
  std::vector< float > data3{0.1,0,1.,0.5,0.3,0};
  SEVCHK(ca_array_put(DBF_FLOAT,data3.size(),var3,(void*)&data3.at(0)),"ca_set failure");
  SEVCHK(ca_pend_io(5.0),"ca_pend_io failure");

  float dch1 = 0.1;
  float dch2 = 0.5;
  float dch3 = 0.9;
  
    SEVCHK(ca_put(DBF_FLOAT,ch1,(void*)&dch1),"ca_set failure");
    SEVCHK(ca_put(DBF_FLOAT,ch2,(void*)&dch2),"ca_set failure");
    SEVCHK(ca_put(DBF_FLOAT,ch3,(void*)&dch3),"ca_set failure");
    SEVCHK(ca_pend_io(5.0),"ca_pend_io failure");

  for( auto& tmp : data )
  {
    SEVCHK(ca_put(DBR_STRING,var1,(void*)tmp.c_str()),"ca_set failure");
    SEVCHK(ca_pend_io(5.0),"ca_pend_io failure");

    sleep(3);
//    std::this_thread::sleep_for(std::chrono::seconds(1));
  ;}

  return 0;
}
