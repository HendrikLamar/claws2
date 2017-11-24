// =====================================================================================
// 
//       Filename:  statemachine.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  24.07.2017 08:24:31
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "statechart.h"
#include "database.h"

#include <vector>
#include <string>
#include <thread>


class MyState
{

    public:
        MyState();
        ~MyState();

        // prints the help screen
        void printHelp();
        // prints a welcome screen
        void printWelcome();
        // demands user input
        char getKey();

        // check the user input and react accordingly
        void run();

    private:
        // counter for the getKey() function
        unsigned int        m_inputCounter;

        // holds the statemachine instance
        ClawsStatemachine::ClawsDAQ*    m_daq;

        // member variable of the vector contain the possible switches for the menu
        std::vector<char>               m_nickname{
            'i',
            'l',
            's',
            '1',
            '0',
            'q',
            'h'
        }; // 7 inputs

        // member variable of the vector containing the information for the menu
        std::vector<std::string>        m_description{
            "Initialize Pico & PowerSupply",
            "ReLoad config files",
            "Show current Status",
            "Start/Stop Measurement",
            "Force Stop",
            "Quit",
            "Print help!"
        }; // 7 inputs

        // member vector containing the pairs for creating the help function
        std::vector< std::pair<char, std::string> >   m_tuple;

};


#endif //STATEMACHINE_H
