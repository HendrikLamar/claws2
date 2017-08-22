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
#include "n6700_channels.h"


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////



class Database
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
        void N6700_readPSUConf();

        //! Returns a nested pair-construct with high-/low gain settings for
        //! all four channels.
        N6700_Channels N6700_getChannels() const;

        //! Returns a struct with ip, id, and port for the n6700.
        Utility::N6700_connect N6700_getConnect() const;




        ///////////////////////////////////////////////////////////////////////
        //                 Picoscope 
        //
        ///////////////////////////////////////////////////////////////////////


        //! Picoscope initializer.
        //! Reads the serial numbers from initializer.ini and initializes them if
        //! available.
        void Pico_init();

        //! Reads in the pico settings.
        void Pico_readConfig( Utility::Pico_RunMode mode );



    private:
        bool                m_stopSwitch;

        ReadIni*            m_initReader;


        ///////////////////////////////////////////////////////////////////////
        //                  Power Supply
        //   All settings for the powersupply are send as strings. 
        ///////////////////////////////////////////////////////////////////////

        N6700_Channels*             m_N6700_Channels;
        
        Utility::N6700_connect      m_n6700_connect;




        ///////////////////////////////////////////////////////////////////////
        //                 Picoscope 
        //
        ///////////////////////////////////////////////////////////////////////

        
        std::vector< Utility::Pico_Data_Pico >*   m_picos;     


};


#endif // DATABASE_H
