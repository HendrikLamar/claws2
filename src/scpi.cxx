// =====================================================================================
// 
//       Filename:  scpi.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03.08.2017 20:26:37
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================
#include <iostream>

#include <string>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "clawsException.h"
#include "scpi.h"


///////////////////////////////////////////////////////////////////////////////


SCPI::SCPI(std::string ipAdress, unsigned short port) : 
    m_ipAdress(ipAdress),
    m_port(port)
{};


SCPI::~SCPI()
{};



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void    SCPI::initSocket()
{
    /*
     * Initialize socket with folling options and test if this is doable.
     */
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( m_socket == -1 )
    {
        throw SCPIException("Initialization failed. Socket not found!");
    }

    // Create a copy of the sockaddr_in-struct?! Not understood.

    // Define the familiy of this connection. AF_INET is for IPV4,
    // AF_INET6 equals IPV6.
    m_server.sin_family = AF_INET;

    // Define the used port.
    m_server.sin_port = htons(m_port);

    // Defines all the needed parameter for the connection.
    // From manual: 'inet_pton - convert IPv4 and IPv6 addresses from text to binary form'
    int result;
    result = inet_pton(AF_INET, m_ipAdress.c_str(), &m_server.sin_addr);
    if ( result != 1 )
    {
        throw SCPIException("Network adress is not successfully converted! -> inet_pton failed.");
    };
    
    return;
}


///////////////////////////////////////////////////////////////////////////////

void    SCPI::openSocket()
{
    int result = connect(m_socket, reinterpret_cast<sockaddr*>(&m_server), sizeof(m_server));
    if ( result == -1 )
    {
        throw SCPIException("Connection could not be established!");
    };

    return;
}

///////////////////////////////////////////////////////////////////////////////

void    SCPI::setCommand(std::string command)
{
    const std::string fcommand = command + "\n";
    const int size = fcommand.size();
    
    // the following code ensures that all of the message is send!
    int bytesSent = 0;
    while( bytesSent < size )
    {
        int result = send(m_socket, fcommand.c_str() + bytesSent, size-bytesSent, 0);
        if( result < 0 )
        {
            throw SCPIException("Command could not be sent!");
        }
        bytesSent += result;
    }

    return;
}

///////////////////////////////////////////////////////////////////////////////

std::string SCPI::getAnswer()
{
    std::stringstream line;
    for( char c; recv(m_socket, &c, 1, 0) > 0; line << c)
    {
        if( c == '\n' )
        {
            return line.str();
        };
    };
    throw SCPIException("Could not receive the entire data!");

    return "Unknown ERROR -Argh!";
}

///////////////////////////////////////////////////////////////////////////////

void SCPI::closeSocket()
{
    close(m_socket);

    /// \todo Ideally check if the socket is really closed. But currently it does not work
    //  -> see below.
//    isClosed();
}

///////////////////////////////////////////////////////////////////////////////

/* bool SCPI::isClosed()
 * {
 *    fd_set rfd;
 *   FD_ZERO(&rfd);
 *   FD_SET(m_socket, &rfd);
 *   timeval tv_usec { 0 };
 *   select(m_socket+1, &rfd, 0, 0, &tv);
 *   if (!FD_ISSET(m_socket, &rfd))
 *     return false;
 *   int n = 0;
 *   ioctl(m_socket, FIONREAD, &n);
 *   return n == 0;
 * } 
 */


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////












