// =====================================================================================
// 
//       Filename:  database.cxx
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  25.07.2017 15:42:55
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include "database.h"

#include <iostream>
#include <string>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


Database::Database() :
    m_stopSwitch(false)
{};



Database::~Database(){};



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Database::setStop(bool switcher)
{
    m_stopSwitch = switcher;
}

bool Database::getStop()
{
    return m_stopSwitch;
}
