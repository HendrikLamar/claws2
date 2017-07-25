// =====================================================================================
// 
//       Filename:  database.cxx
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  25.07.2017 15:42:55
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include "database.h"

#include <iostream>
#include <string>


Database::Database()
    :m_counterA(0),
    m_counterB(0),
    m_currentState(State::IDLE),
    m_stopSwitch(false)
{}

Database::~Database(){}


void Database::incrementCounterA()
{
    ++m_counterA;
}


void Database::incrementCounterB()
{
    ++m_counterB;
}

unsigned long Database::getCounterA()
{
    return m_counterA;
}

unsigned long Database::getCounterB()
{
    return m_counterB;
}

std::string Database::getState()
{
    std::string retArg;
    switch(m_currentState){
        case State::IDLE:
            retArg = "IDLE";
            break;
        case State::STATE_A:
            retArg = "STATE_A";
            break;
        case State::STATE_B:
            retArg = "STATE_B";
            break;
        default:
            std::cout << "No State available!\n";

    }
    return retArg;
}

void Database::setState(State state)
{
    m_currentState = state;
}

void Database::setStop(bool switcher)
{
    m_stopSwitch = switcher;
}

bool Database::getStop()
{
    return m_stopSwitch;
}

