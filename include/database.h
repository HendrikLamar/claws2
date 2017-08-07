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
#include "utility.h"

class Database
{


    public:
        Database();
        virtual ~Database();

        enum State{
            IDLE,
            STATE_A,
            STATE_B
        };


        std::string     getState();
        void            setState(State state);

        void            setStop( bool switcher );
        bool            getStop();

    private:
        State           m_currentState;
        bool            m_stopSwitch;




};


#endif // DATABASE_H
