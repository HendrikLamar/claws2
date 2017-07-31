// =====================================================================================
// 
//       Filename:  statechart.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  26.07.2017 15:47:42
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include <boost/statechart/event.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list.hpp>

#include <chrono>
#include <thread>

#include <iostream>

namespace sc = boost::statechart;

struct EvStatus : sc::event< EvStatus > {};
struct EvConfig : sc::event< EvConfig > {};

struct ClawsDAQ;
struct Active;
struct Idle;
struct Status;
struct Config;

struct SystemIdle;
struct SystemRun;

struct EvStartStop : sc::event< EvStartStop > {};

struct ClawsDAQ : sc::state_machine< ClawsDAQ, Active > {};

///////////////////////////////////////////////////////////////////////////////

char getKey(){

    std::cout << "> ";
    char key;
    std::cin >> key;
    return key;
}

class Data
{
    public:
      unsigned int m_counterA;  
      unsigned int m_counterB;  
      unsigned int m_counterC;  
      bool         m_stop;

      int           m_runA = 5;
      int           m_runB = 8;
      int           m_runC = 7;

      Data(): m_counterA(0),m_counterB(0),m_counterC(0), m_stop(true)
        {};
      ~Data(){};
};



struct Active : sc::simple_state< Active, ClawsDAQ, 
    boost::mpl::list< Idle, SystemIdle> >
{
    public:

        Data *  m_database;

        Active():m_database(new Data)
        {
            std::cout << "Hello World!\n";
            std::cout << "I am Active!\n";
        }
        ~Active(){
            std::cout << "Exiting Active!\n";
            delete m_database;
            m_database = nullptr;
        }

};

struct Idle : sc::simple_state< Idle, Active::orthogonal<0> >
{
    public:
        typedef boost::mpl::list<
            sc::transition< EvConfig, Config >,
            sc::transition< EvStatus, Status >
                > reactions;
//        typedef sc::transition< EvStatus, Status > reactions;

        Idle()
        {
            std::cout << "I am Idle!\n";
        }
        ~Idle(){
            std::cout << "Exiting Idle!\n";
        }
};

struct Status : sc::state< Status, Active::orthogonal<0> >
{
    public:
        typedef boost::mpl::list<
            sc::custom_reaction< EvStatus >,
//            sc::transition< EvStatus, Idle >,
            sc::transition< EvConfig, Config >
                > reactions;

        sc::result react( const EvStatus & ){
            if( true ){
                return forward_event();
            };
        };


        Status(my_context ctx) : my_base(ctx)
        {
            std::cout << "CounterA: " << context< Active >().m_database->m_counterA << std::endl;
            std::cout << "CounterB: " << context< Active >().m_database->m_counterB << std::endl;
            std::cout << "CounterC: " << context< Active >().m_database->m_counterC << std::endl;

            std::cout << "\nPress 's' to exit.\n";
        }
        ~Status(){
            std::cout << "Leavin Status!\n";
        }
        
};

struct Config : sc::state< Config, Active::orthogonal<0> >
{
    public:
        typedef boost::mpl::list<
            sc::transition< EvConfig, Idle >,
            sc::transition< EvStatus, Status>
                > reactions;

        Config(my_context ctx) : my_base(ctx)
        {
            std::cout << "l = load\n";
            std::cout << "s = save\n";
            std::cout << "\n";
            std::cout << "q = Exit\n";

            char key = getKey();
            while( true ){
                switch( key ){
                    case 'l':
                        std::cout << "Loading Data!\n";
                        break;
                    case 's':
                        std::cout << "Saving Data!\n";
                        break;
                    case 'q':
                       break; 
                    default:
                        std::cout << "Wrong input dude!\n";
                }
                key = getKey();
            }
        }
        ~Config(){
            std::cout << "Exiting Config!\n";
        }

};



struct SystemIdle : sc::simple_state< SystemIdle, Active::orthogonal<1> >
{
    public:
        typedef sc::transition< EvStartStop, SystemRun > reactions;

        SystemIdle()
        {
            std::cout << "I am SystemIdle!\n";
        }
        ~SystemIdle(){
            std::cout << "Exiting SystemIdle!\n";
        }
};

struct SystemRun : sc::state< SystemRun, Active::orthogonal<1> >
{
    public:
        typedef sc::transition< EvStartStop, SystemIdle > reactions;
/*         typedef boost::mpl::list<
 *             sc::transition< EvStartStop, SystemIdle >,
 *             sc::transition< EvStateA, StateA >,
 *             sc::transition< EvStateB, StateB >,
 *             sc::transition< EvStateC, StateC >
 *             > reactions;
 */
        std::thread m_thread;

        SystemRun(my_context ctx) : 
            my_base(ctx)
        {
            std::cout << "SystemRun!\n";
//            context< ClawsDAQ >().process_event( EvStateA() );
        }

        ~SystemRun(){

            std::cout << "SystemStop!\n";
            context< Active >().m_database->m_stop = false;
//            m_thread.join();
        }

};

/* struct StateA : sc::state< StateA, SystemRun >
 * {
 *     public:
 *         typedef sc::transition< EvStateB, StateB > reactions;
 * 
 *         StateA(my_context ctx) : my_base(ctx)
 *         {
 *             incrementCounterA();
 *         };
 *         ~StateA(){};
 * 
 *         void incrementCounterA(){
 *             int mycount = 0;
 *             while( mycount < context< Active >().m_database->m_runA){
 *                 ++context< Active >().m_database->m_counterA;
 *                 std::this_thread::sleep_for(std::chrono::milliseconds(500));
 *                 ++mycount;
 *             };
 * //            context< ClawsDAQ >().process_event( EvStateB() );
 *         }
 * };
 * 
 * struct StateB : sc::state< StateB, SystemRun >
 * {
 *     public:
 *         typedef sc::transition< EvStateC, StateC > reactions;
 * 
 *         StateB(my_context ctx) : my_base(ctx)
 *         {
 *             incrementCounterB();
 *         };
 *         ~StateB(){};
 * 
 *         void incrementCounterB(){
 *             int mycount = 0;
 *             while( mycount < context< Active >().m_database->m_runB){
 *                 ++context< Active >().m_database->m_counterB;
 *                 std::this_thread::sleep_for(std::chrono::milliseconds(500));
 *                 ++mycount;
 *             };
 * //            context< ClawsDAQ >().process_event( EvStateC() );
 *         }
 * };
 * 
 * struct StateC : sc::state< StateC, SystemRun >
 * {
 *     public:
 *         typedef sc::transition< EvStartStop, SystemIdle> reactions;
 * 
 *         StateC(my_context ctx) : my_base(ctx)
 *         {
 *             incrementCounterC();
 *         };
 *         ~StateC(){};
 * 
 *         void incrementCounterC(){
 *             int mycount = 0;
 *             while( mycount < context< Active >().m_database->m_runC){
 *                 ++context< Active >().m_database->m_counterC;
 *                 std::this_thread::sleep_for(std::chrono::milliseconds(500));
 *                 ++mycount;
 *             };
 * //            context< ClawsDAQ >().process_event( EvStartStop() );
 *         }
 * };
 */





int main(){
    
    ClawsDAQ daq;
    daq.initiate();

    std::cout << "r = StartStop\n";
    std::cout << "c = Config\n";
    std::cout << "s = Status\n";
    std::cout << "e = Exit\n";

    char key = getKey();
    while( key != 'e' ){
        switch( key ){
            case 'r':
                daq.process_event( EvStartStop() );
                break;
            case 'c':
                daq.process_event( EvConfig() );
                daq.process_event( EvConfig() );
                break;
            case 's':
                daq.process_event( EvStatus() );
                break;
            default:
                std::cout << "Wrong input dude!\n";
        }
        key = getKey();
    }


    return 0;
}

