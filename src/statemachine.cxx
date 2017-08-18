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
#include "statechart.h"
#include "database.h"
#include "utility.h"
#include "scpi.h"
#include "clawsException.h"

#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>



MyState::MyState() :
    m_inputCounter(0),
    m_daq(new ClawsStatemachine::ClawsDAQ())
{
    // check if the help vectors are of different lenght
    assert(m_nickname.size() == m_description.size() );
    // create a vector of pairs to create the 'printHelp()' function
    for(unsigned int i = 0; i < m_nickname.size(); ++i){
        m_tuple.push_back(std::make_pair(m_nickname.at(i), m_description.at(i)));
    }

    m_daq->initiate();
}


///////////////////////////////////////////////////////////////////////////////

MyState::~MyState()
{
    delete m_daq;
    m_daq = nullptr;
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

char MyState::getKey()
{
    std::cout << "claws[" << m_inputCounter << "]> ";
    ++m_inputCounter;
    char key;
    std::cin >> key;
    return key;
}


///////////////////////////////////////////////////////////////////////////////

void MyState::run()
{
    printWelcome();
    char key = getKey();
    while ( true )
    {
        switch(key)
        {

            case 'i':
                std::cout << "Not available yet :(\n";
                break;

            case 'l':
                std::cout << "Not available yet :(\n";
                break;

            case 'c':
                std::cout << "Not available yet :(\n";
                std::cout << "Probably obsolete soon.\n";
                break;

            case 's':
                m_daq->process_event( ClawsStatemachine::EvStatus() );
                break;

            case '1':
                m_daq->process_event( ClawsStatemachine::EvStartStop() );
                m_daq->process_event( ClawsStatemachine::EvStartStop() );
                break;

            case '0':
                break;

            case 'q':
                // This structure is needed to check if all the systems are
                // in the correct state. If everything is alright, leave the 
                // state again and stop in the the while loop. See the next
                // if-statement.
                m_daq->process_event( ClawsStatemachine::EvQuit() );
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                m_daq->process_event( ClawsStatemachine::EvQuit() );
                break;

            case 'h':
                printHelp();
                break;

            default:
                std::cout << "Invalid input! Try 'h'!\n";
        };

        // checks if the system shall shut down or not.
        if ( m_daq->getDatabase()->getStop() )
        {
            break;
        }
        else key = getKey();
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////
