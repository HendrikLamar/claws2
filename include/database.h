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
        




        ///////////////////////////////////////////////////////////////////////
        //                  Power Supply
        //   All settings for the powersupply are send as strings. 
        ///////////////////////////////////////////////////////////////////////
        

        //! Void function which reads in the powersupply settings for low and
        //! high gain mode.
        void N6700_readChSet();
        
        //! Reads in the n6700.ini file.
        void N6700_readPSConf();

        //! Returns a nested pair-construct with high-/low gain settings for
        //! all four channels.
        KPS_Channels N6700_getChannels();

        //! Returns a struct with ip, id, and port for the n6700.
        Utility::N6700_connect N6700_getConnect();


    private:
        bool            m_stopSwitch;

        ///////////////////////////////////////////////////////////////////////
        //                  Power Supply
        //   All settings for the powersupply are send as strings. 
        ///////////////////////////////////////////////////////////////////////
        
        KPS_Channels            m_n6700_channels;
         
        Utility::N6700_connect  m_n6700_connect;

};


#endif // DATABASE_H
