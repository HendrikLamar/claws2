// =====================================================================================
// 
//       Filename:  database.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  24.07.2017 11:43:22
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

#include "readini.h"
#include "utility.h"


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////



class Database : public ReadIni
{


    public:
        Database();
        virtual ~Database();

        void            setStop( bool switcher );
        bool            getStop();

    private:
        bool            m_stopSwitch;


};


#endif // DATABASE_H
