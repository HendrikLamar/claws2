// =====================================================================================
// 
//       Filename:  processData.cxx
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06.10.2017 08:07:39
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include "utility.h"
#include "processData.h"
#include "storage.h"

#include <memory>

#include <TH1I.h>


///////////////////////////////////////////////////////////////////////////////
//
//              START Con- & Destructor
//
///////////////////////////////////////////////////////////////////////////////




ProcessData::ProcessData() :
    m_save( new Storage() )
{}






///////////////////////////////////////////////////////////////////////////////






ProcessData::~ProcessData()
{
    delete m_save;
    m_save = nullptr;

}







///////////////////////////////////////////////////////////////////////////////
//
//              END Con- & Destructor
//
///////////////////////////////////////////////////////////////////////////////
























































///////////////////////////////////////////////////////////////////////////////
//
//              START Public Member Functions 
//
///////////////////////////////////////////////////////////////////////////////





Storage* ProcessData::save()
{
    return m_save;
}







///////////////////////////////////////////////////////////////////////////////







ProcessData*    ProcessData::sync()
{

    return this;
}








///////////////////////////////////////////////////////////////////////////////


















///////////////////////////////////////////////////////////////////////////////
//
//              END Public Member Functions
//
///////////////////////////////////////////////////////////////////////////////


















































///////////////////////////////////////////////////////////////////////////////
//
//              START Private Member Functions 
//
///////////////////////////////////////////////////////////////////////////////








void ProcessData::makeTH1I()
{
    // the first four entries indicate if the channels of the first pico are enabled
    // or not, the second four for pico #2, etc...
    std::shared_ptr< std::vector< int16_t > >  picoChannelEnabled;

    TH1I* hist;

    for( unsigned int ii = 0; ii < m_picos->size(); ++ii )
    {
        for( unsigned int kk = 0; kk < 4; ++kk )
        {
            picoChannelEnabled->push_back(m_picos->at(ii)->getCh(kk)->getEnabled());
            if( m_picos->at(ii)->getCh(kk)->getEnabled() )
            {
                hist = new TH1I( "hist", "", 
                        m_picos->at(ii)->getCh(kk)->getBuffer()->size(), 
                        0, 
                        m_picos->at(ii)->getCh(kk)->getBuffer()->size()
                        );
            }
        }
    }



}








///////////////////////////////////////////////////////////////////////////////








///////////////////////////////////////////////////////////////////////////////
//
//              END Private Member Functions
//
///////////////////////////////////////////////////////////////////////////////
