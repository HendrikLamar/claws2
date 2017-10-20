// =====================================================================================
// 
//       Filename:  processData.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06.10.2017 07:05:31
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#ifndef PROCESSDATA_H
#define PROCESSDATA_H

#include "storage.h"
#include "utility.h"
#include "pico.h"

#include <TH1I.h>

#include <libps6000-1.4/ps6000Api.h>
#include <libps6000-1.4/PicoStatus.h>

#include <vector>
#include <utility>
#include <map>
#include <memory>




//! Compines the responisbility to save and analyze the data.
class ProcessData : public std::enable_shared_from_this< ProcessData >
{

    private:

        std::shared_ptr<Storage>    m_save;
        std::vector< Pico* >*       m_picos;

        // translates the vector data to Root::TH1I
        void    makeTH1I();
        
        // very complicated data structure to store
        std::shared_ptr < std::vector< std::shared_ptr< Utility::Pico_Hist_Pico > > > m_picos_hist;

    public:

        ProcessData( std::vector< Pico* >* vPicos );
        ~ProcessData();

        //! Returns
        std::shared_ptr< Storage >  save();

        //! Syncs the histogram with the latest picoscope data.
        void    sync();
//        std::shared_ptr<ProcessData> sync();

        //! Clears the TH1I vector.
        void clear();

        friend class Storage;

};

#endif //PROCESSDATA_H
