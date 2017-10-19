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




class ProcessData : public std::enable_shared_from_this< ProcessData >
{

    private:

        enum class subdir 
        {
            INTER,
            PHYSICS,
            LIVE
        };

        
        std::vector< Pico* >*       m_picos;

        std::shared_ptr<Storage>    m_save;

        // translates the vector data to Root::TH1I
        void    makeTH1I();
        
        // very complicated data structure to store
        std::shared_ptr < std::vector< std::shared_ptr< Utility::Pico_Hist_Pico > > > m_picos_hist;


        // declares where the current data should be saved
        subdir      m_current_dataset;

    public:

        ProcessData();
        ~ProcessData();

        std::shared_ptr< Storage >  save();

        //! Syncs the histogram with the latest picoscope data.
        std::shared_ptr<ProcessData> sync();


        friend class Storage;

};

#endif //PROCESSDATA_H
