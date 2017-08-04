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
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/mpl/list.hpp>

#include <iostream>

namespace sc = boost::statechart;

struct EvStatus : sc::event< EvStatus > {};
struct EvConfig : sc::event< EvConfig > {};

struct ClawsDAQ;
struct Active;
struct Idle;
struct Status;
struct Config;

struct SystemRun;
struct SystemStop;

struct EvStartStop : sc::event< EvStartStop > {};

struct ClawsDAQ : sc::state_machine< ClawsDAQ, Active > {};

///////////////////////////////////////////////////////////////////////////////

class Data
{
    public:
      unsigned int m_counter;  

      Data(): m_counter(0)
        {};
      ~Data(){};
};



struct Active : sc::simple_state< Active, ClawsDAQ, 
    boost::mpl::list< Idle, SystemStop > >
{
    public:
//        Data *  m_database;

//        Active():m_database(new Data)
        unsigned int m_counter;
        Active():m_counter(0)
        {
            std::cout << "Hello World!\n";
            std::cout << "I am Active!\n";
        }
        ~Active(){
            std::cout << "Exiting Active!\n";
        }

        unsigned int getCounter() const {
            return m_counter;
        }
        unsigned int & getCounter() {
            return m_counter;
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

struct Status : sc::simple_state< Status, Active::orthogonal<0> >
{
    public:
        typedef boost::mpl::list<
            sc::transition< EvStatus, Idle >,
            sc::transition< EvConfig, Config >
                > reactions;


        Status()
        {
            std::cout << "Current counter: " << 
                context< Active >().getCounter() << "\n";
        }
        ~Status(){
        }
};

struct Config : sc::simple_state< Config, Active::orthogonal<0> >
{
    public:
        typedef boost::mpl::list<
            sc::transition< EvConfig, Idle >,
            sc::transition< EvStatus, Status>
                > reactions;

        Config()
        {
            std::cout << "I am Config!\n";
        }
        ~Config(){
            std::cout << "Exiting Config!\n";
        }

};

struct SystemStop : sc::simple_state< SystemStop, Active::orthogonal<1> >
{
    public:
        typedef sc::transition< EvStartStop, SystemRun > reactions;

        SystemStop()
        {
            std::cout << "I am SystemStop!\n";
        }
        ~SystemStop(){
            std::cout << "Exiting SystemStop!\n";
        }
};

struct SystemRun : sc::simple_state< SystemRun, Active::orthogonal<1> >
{
    public:
        typedef sc::transition< EvStartStop, SystemStop > reactions;

        SystemRun()
        {
            std::cout << "I am SystemRun!\n";
        }
        ~SystemRun(){
            std::cout << "Exiting SystemRun!\n";
        }
};

char getKey(){

    std::cout << "> ";
    char key;
    std::cin >> key;
    return key;
}



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

