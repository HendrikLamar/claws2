// =====================================================================================
// 
//       Filename:  statechart.h
// 
//    Description:  Implementation of a state machine using the boost.Statechart
//                  library. The state machine is the steering wheel of the
//                  ClawsDAQ handling all the inputs and starting/stopping data
//                  collection and so on and forth.
//
//                  Header-only library!
// 
//        Version:  1.0
//        Created:  28.07.2017 13:00:47
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#ifndef STATECHART_H
#define STATECHART_H

#include <boost/statechart/event.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list.hpp>

#include "database.h"
#include "n6700.h"

#include <iostream>
#include <thread>

namespace ClawsStatemachine{

namespace sc = boost::statechart;
namespace mpl = boost::mpl;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/* ClawsDAQ State Machine
 *                          
 *                          Active 
 *  ---------------------------------------------------------------------
 *  |
 *  |           ------------
 *  |   O----> |   Idle     |
 *  |          |            |
 *  |           ------------
 *  |
 *  |--------------------------------------------------------------------
 *  |
 *  |           ----------------
 *  |   0----> | SystemIdle     |
 *             |                |              
 *             |                |
 *             -----------------
 *  |
 */


/* !Events accessible with keyboard inputs:
 *
 * i = Initialize Pico, PowerSupply, Epics & co.
 * l = Load config file (default: Merkel
 * c = Do a system check
 * s = Show current status
 * 1 = Start/Stop measurement
 * 0 = Force Stop the measurement
 * h = Print help
 * q = Quit
 *
 */


///////////////////////////////////////////////////////////////////////////////
//                  FORWARD DECLARATION OF ALL THE STATES
///////////////////////////////////////////////////////////////////////////////

/*! Claws state machine is named ClawsDAQ. The other are states.
 *
 */

/// Forward declaration of the state machine
struct ClawsDAQ;

/// Outmost state
struct Active;
/// Initial state of the menu
struct MenuIdle;
/// Status state
struct Status;
/// Config state
struct Config;
/// Quit state
struct Quit;

/// Idle & initial state of the data taking
struct SystemIdle;
/// Active data taking state
struct SystemRun;
///////////////////////////////////////////////////////////////////////////////
//          END FORWARD DECLARATION
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/*! Events accessible with keyboard input. Not every keyboard input its own
 *  state in the statemachine.
 */

/// Changes into the status state
struct EvStatus : sc::event< EvStatus > {};
/// Changes into the load config state
struct EvLoadConfig: sc::event< EvLoadConfig > {};
/// Start and stops the data taking
struct EvStartStop : sc::event< EvStartStop > {};
/// Checks and - in case - waits for the data taking to stop
struct EvQuit : sc::event< EvQuit > {};

///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
//              STATE MACHINE DECLARATIONS
//



//! State machine. Initializes the Database and contains all the states.
struct ClawsDAQ : sc::state_machine< ClawsDAQ, Active >
{
    protected:
        Database*   m_database;

    public:
        ClawsDAQ() : m_database(new Database)
        {};
        virtual ~ClawsDAQ() 
        {
            delete m_database;
            m_database = nullptr;

            std::cout << 
    "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
            std::cout << "\t\t ClawsDAQ says Goodbye!\n";
            std::cout << "\n\t\t ROOOOOOOOAAAAAAAR!\n";
            std::cout << 
    "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
        };
        

        /// Returns the database 
        Database * getDatabase()
        {
            return m_database;
        }


/*         /// 
 *         char getKey()
 *         {
 *             std::cout << "[" << m_database->getInputCounter() << "]> "; 
 *             char key;
 *             std::cin >> key;
 *             return key;
 *         };
 */
};


///////////////////////////////////////////////////////////////////////////////


//! Outermost state of ClawsDAQ. Contains and handles all the inner states, also
//! concurrent ones.
struct Active : sc::simple_state< Active, ClawsDAQ,
    mpl::list< MenuIdle, SystemIdle > >
{
/*     protected:
 * 
 *         /// Global database holding ALL the pathes, values, data arrays.
 *         /// To be accessed by all the substates if needed.
 *         Database *          m_database;
 */

    public:

//        Active() :  m_database(new Database)
        Active() 
        {
            // \todo Functionality to be added!
            std::cout << "Entering Active\n";
        };
        virtual ~Active() 
        {
            std::cout << "Exiting Active\n";
        };
/*         virtual ~Active() 
 *         {
 *             delete m_database;
 *             m_database = nullptr;
 * 
 *             std::cout << 
 *     "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
 *             std::cout << "\t\t ClawsDAQ says Goodbye!\n";
 *             std::cout << "\n\t\t ROOOOOOOOAAAAAAAR!\n";
 *             std::cout << 
 *     "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
 *         };
 */


/*         /// Returns the database 
 *         Database * getDatabase()
 *         {
 *             return m_database;
 *         }
 */
        


};


///////////////////////////////////////////////////////////////////////////////




//! Initial state of concurrency level <0>. Is the idle state of level <0>.
struct MenuIdle : sc::simple_state< MenuIdle, Active::orthogonal<0> >
{
    public:
        // define possible transitions from that state
        typedef mpl::list<
            sc::transition< EvLoadConfig, Config >,
            sc::transition< EvStatus, Status >,
//            sc::custom_reaction< EvQuit > 
            sc::transition< EvQuit, Quit >
                > reactions;


/*         // Forward declaration of the custom_reaction. Implementation in .cpp-file
 *         sc::result react( const EvQuit & );
 */

        MenuIdle() 
        {
            std::cout << "Entering MenuIdle\n";
        };
        virtual ~MenuIdle() 
        {
            std::cout << "Exiting MenuIdle\n";
        };

};


///////////////////////////////////////////////////////////////////////////////




//! Displays the current status of the running system. Is in concurrency level <0>.
//! A submenu might follow.
struct Status : sc::state< Status, Active::orthogonal<0> >
{
    public:
        typedef mpl::list<
            sc::transition< EvStatus, MenuIdle >,
            sc::transition< EvLoadConfig, Config >
                > reactions;

        Status( my_context ctx ) : my_base( ctx )
        {
            // \todo Add functionality!
            std::cout << "Entering Status\n";
        };

        virtual ~Status() 
        {
            std::cout << "Exiting Status\n";
        };
};


///////////////////////////////////////////////////////////////////////////////




//! Changes configuration file(s). Is concurrency level <0>.
struct Config : sc::state< Config, Active::orthogonal<0> >
{
    public:
        typedef mpl::list<
            sc::transition< EvLoadConfig, MenuIdle >,
            sc::transition< EvStatus, Status >
                > reactions;

        Config( my_context ctx ) : my_base( ctx )
        {
            // \todo Add functionality!
            std::cout << "Entering Config\n";
        };
        virtual ~Config() 
        {
            std::cout << "Exiting Config\n";
        };
};


///////////////////////////////////////////////////////////////////////////////




//! Inital state of concurrency level <1> and is the idle state for
//! data taking.
struct SystemIdle : sc::state< SystemIdle, Active::orthogonal<1> >
{
    public:
        typedef sc::transition< EvStartStop, SystemRun > reactions;

        SystemIdle( my_context ctx ) : my_base( ctx ) 
        {
            std::cout << "Entering SystemIdle\n";
        };
        virtual ~SystemIdle() 
        {
            std::cout << "Exiting SystemIdle\n";
        };
};


///////////////////////////////////////////////////////////////////////////////




//! Data taking state. Is concurrency level <1> and starts a second thread
//! to handle the data taking.
struct SystemRun : sc::state< SystemRun, Active::orthogonal<1> >
{
    private:
        std::thread         m_thread;

    public:
        typedef sc::transition< EvStartStop, SystemIdle> reactions;
        
        SystemRun( my_context ctx ) : my_base( ctx )
        {
            // \todo Add functionality! Run function in m_thread!
            std::cout << "Entering SystemRun\n";

            // Initialize PSU with the database as parameter.
            N6700 psu(context< ClawsDAQ >().getDatabase());
            m_thread = std::thread(&N6700::run, psu);
        };
        virtual ~SystemRun()
        {
            m_thread.join();
            std::cout << "Exiting SystemRun\n";
        };

};


///////////////////////////////////////////////////////////////////////////////




//! Quit state. Checks if all the other states are stopped and/or stops
//! them in case. Is in concurrency level <0>.
struct Quit : sc::state< Quit, Active::orthogonal<0> >
{
    public:
        typedef sc::transition< EvQuit, MenuIdle > reactions ;

        /// Do nothing here! Enter and leave this state immediately.
        /// The Idle-State checks before if data taking is off so that the
        /// system can be shut down confidently.
        Quit( my_context ctx ) : my_base( ctx )
        {
            context< ClawsDAQ >().getDatabase()->setStop(true);
            std::cout << "Entering Quit\n";
        };
        virtual ~Quit() 
        {
            std::cout << "Exiting Quit\n";
        };

};

///////////////////////////////////////////////////////////////////////////////



//
//              END STATE MACHINE DECLARATIONS
///////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////
// 



}; // End NAMESPACE ClawsStatemachine

#endif // STATECHART_H















