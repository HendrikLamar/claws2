// =====================================================================================
// 
//       Filename:  analysis.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  19.12.2017 18:02:52
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================



#ifndef ANALYSIS_H
#define ANALYSIS_H


#include <pico.h>
//#include <utility.h>

#include <vector>


class Analysis
{
    private:




    public:

        //! Default constructor.
        Analysis();
        virtual ~Analysis();


        //! Calculates the average 1pe value and total 1pe fraction per channel.
        //! First, the waveform containing 1pe values are extracted. 
        //! If a value higher as 1.5 times the average waveform is found, 
        //! the waveform is rejected.
        //! Afterwards, the integral of the second half is subtracted by the integral
        //! of the first (pedestal) part which yields the pedestal subtracted
        //! integral. This is done waveform by waveform.
        //! As the last step, the arithmetic mean of all  pedestal subtracted 
        //! integral is calculated.
        void intermediate( std::shared_ptr<Pico> tpico );

        void physics();
};







#endif // ANALYSIS_H
