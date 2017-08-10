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
#include "n6700.h"


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////



class Database : 
    public ReadIni,
    public KPS_Channels
{


    public:
        Database();
        virtual ~Database();

        void            setStop( bool switcher );
        bool            getStop();

    private:
        bool            m_stopSwitch;

        ///////////////////////////////////////////////////////////////////////
        //                  Power Supply
        //   All settings for the powersupply are send as strings, anyway. Therefore
        //   we can read them in as strings.
        ///////////////////////////////////////////////////////////////////////
        
        KPS_Channels            m_n6700_channels;



        //! Void function which reads in the powersupply settings for low and
        //! high gain mode.
        void KPS_readIni( Utility::ClawsGain HIGH_LOW_GAIN );
        


};


#endif // DATABASE_H
