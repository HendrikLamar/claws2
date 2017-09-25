// =====================================================================================
// 
//       Filename:  clawsRun.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01.09.2017 08:10:47
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#ifndef CLAWSRUN_H
#define CLAWSRUN_H


#include <vector>

#include "utility.h"
#include "pico.h"
#include "n6700.h"
#include "database.h"




class ClawsRun
{
    private:

        Database*                       m_database;


        ///////////////////////////////////////////////////////////////////////
        //
        //              START Pico Stuff
        //
        ///////////////////////////////////////////////////////////////////////
        
        std::vector< Pico* >*           m_picos;


        void Pico_init( );

        void Pico_close();

        void Pico_runInter();

        void Pico_runBlock();

        ///////////////////////////////////////////////////////////////////////
        //
        //              END Pico Stuff
        //
        ///////////////////////////////////////////////////////////////////////













        ///////////////////////////////////////////////////////////////////////
        //
        //              START PSU Stuff
        //
        ///////////////////////////////////////////////////////////////////////
        
        N6700*                          m_psu;

        void PSU_init( );



        ///////////////////////////////////////////////////////////////////////
        //
        //              END PSU Stuff
        //
        ///////////////////////////////////////////////////////////////////////















        ///////////////////////////////////////////////////////////////////////
        //
        //              START General 
        //
        ///////////////////////////////////////////////////////////////////////






        ///////////////////////////////////////////////////////////////////////
        //
        //              END General 
        //
        ///////////////////////////////////////////////////////////////////////


    public:



        ///////////////////////////////////////////////////////////////////////
        
        ClawsRun();
        virtual ~ClawsRun();
    

        ///////////////////////////////////////////////////////////////////////


        //! Returns the Database().
        Database*       getDatabase();

        //! Initializes picos, psu and epics.
        void            initialize();

        //! Starts the pico runmode in an infinit loop until it is stopped by
        //! the (epics) user.
        void            run();

        //! (Re)Loads the configurations.
        void            loadConfig();

        //! Prints the current loaded data.
        void            printData();
};










#endif // CLAWSRUN_H
