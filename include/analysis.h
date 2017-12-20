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
#include <utility.h>

#include <vector>


class Analysis
{
    private:




    public:

        //! Default constructor.
        Analysis();
        virtual ~Analysis();


        void intermediate( std::shared_ptr<Pico> tpico );

        void physics();
};







#endif // ANALYSIS_H
