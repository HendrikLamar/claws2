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

#include "database.h"

#include <vector>
#include <string>
#include <thread>


class MyState
{
    private:
        unsigned int    m_inputCounter         =       0;
        Database*       m_database;



        std::vector<char>               m_nickname{
            'i',
            'l',
            'c',
            's',
            '1',
            '2',
            '0',
            'q',
            'h'
        }; // 8 inputs

        std::vector<std::string>        m_description{
            "Initialize Pico & PowerSupply",
            "Load config file (default: Merkel)",
            "Check System",
            "Show current Status",
            "Start Measurement",
            "Stop Measurement",
            "Force Stop",
            "Quit",
            "Print help!"
        }; // 8 inputs

        std::vector< std::pair<char, std::string> >   m_tuple;

    public:
        MyState(Database * database);
        ~MyState();

        // prints the help screen
        void printHelp();
        // prints a welcome screen
        void printWelcome();
        // prints the status screen
        void printStatus();
        // demands user input
        char getKey();
        // check the user input and react accordingly
        void doMenu();

        void showStatus();

        void runPico();

};


#endif //STATEMACHINE_H
