// =====================================================================================
// 
//       Filename:  clawsRun.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01.09.2017 09:12:41
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================



#include "clawsRun.h"
#include "pico.h"
#include "n6700.h"
#include "database.h"



///////////////////////////////////////////////////////////////////////////////


ClawsRun::ClawsRun() :
    m_picos( nullptr ),
    m_psu( nullptr ),
    m_database ( new Database() )
{

}


ClawsRun::~ClawsRun()
{

}


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////




Database*   ClawsRun::getDatabase()
{
    return m_database;
}




///////////////////////////////////////////////////////////////////////////////



void        ClawsRun::initialize()
{
    m_database->Pico_init( m_picos );
//    m_database->Pico_readSettings
}











