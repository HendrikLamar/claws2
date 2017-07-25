// =====================================================================================
// 
//       Filename:  statemachine.cxx
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  24.07.2017 11:41:27
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include "statemachine.h"
#include "database.h"
#include "utility.h"

#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>



MyState::MyState(Database * database) :
    m_database(database)
{
    assert(m_nickname.size() == m_description.size() );

    for(unsigned int i = 0; i < m_nickname.size(); ++i){
        m_tuple.push_back(std::make_pair(m_nickname.at(i), m_description.at(i)));
    }


///////////////////////////////////////////////////////////////////////////////


}

MyState::~MyState()
{

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void MyState::printHelp()
{
    for(unsigned int ii = 0; ii < m_tuple.size(); ++ii){
        std::cout << "\t" << m_tuple.at(ii).first << "<CR> " << 
            m_tuple.at(ii).second << "\n";
    }

    std::cout << std::endl;
    
}

///////////////////////////////////////////////////////////////////////////////

void MyState::printWelcome()
{
    std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    std::cout << "Welcome to the Claws DAQ\n";
    std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    std::cout << "\n Try one of the following!\n";
    printHelp();
    std::cout << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

void MyState::printStatus()
{
    std::cout << m_database;
}

///////////////////////////////////////////////////////////////////////////////

char MyState::getKey()
{
    std::cout << "claws[" << m_inputCounter << "]> ";
    ++m_inputCounter;
    char key;
    std::cin >> key;
    return key;
}

///////////////////////////////////////////////////////////////////////////////

void MyState::showStatus()
{
    std::cout << "\n\tThe current state is: " << m_database->getState() << "\n";
    std::cout << "\tCounterA: " << m_database->getCounterA() << "\t"
        << "CounterB: " << m_database->getCounterB() << std::endl;
    return;
}

///////////////////////////////////////////////////////////////////////////////

void MyState::doMenu()
{
    std::thread secondThread;
    printWelcome();
    char key = getKey();
    while ( key != 'q' )
    {
        switch(key)
        {
            case 'i':
                std::cout << "Initializing Pico!\n";
                break;
            case 'l':
                std::cout << "Loading Config!\n";
                break;
            case 'c':
                std::cout << "Checking System!\n";
                break;
            case 's':
                showStatus();
                break;
            case '1':
                std::cout << "Starting Measurement!\n";
                m_database->setStop(false);
                secondThread = std::thread(&MyState::runPico, *this);
                break;
            case '2':
                std::cout << "Stopping Measurement!\n";
                m_database->setStop(true);
                secondThread.join();
                break;
            case '0':
                std::cout << "Force stopping Measurement!\n";
                break;
            case 'q':
                std::cout << "Quitting program, Goodbye!\n";
                break;
            case 'h':
                printHelp();
                break;
            default:
                std::cout << "Invalid input! Try 'h'!\n";
        };
        key = getKey();
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////

void MyState::runPico()
{
   while(m_database->getStop() != true)
   {
       m_database->setState(Database::STATE_A);
       m_database->incrementCounterA(); 
       std::this_thread::sleep_for(std::chrono::seconds(1));

       m_database->setState(Database::STATE_B);
       m_database->incrementCounterB(); 
       std::this_thread::sleep_for(std::chrono::seconds(3));

   }
   m_database->setState(Database::IDLE);
   return;
}
