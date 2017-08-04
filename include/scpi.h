// =====================================================================================
// 
//       Filename:  SCPI.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03.08.2017 16:20:11
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#ifndef SCPI_H
#define SCPI_H

#include <string>
/* #include <sys/socket.h>
 * #include <sys/types.h>
 * #include <arpa/inet.h>
 */

#include "clawsException.h"


/** Top class needed by every device which is accessed over tcp/ip speaking 
 * the SCPI language.
 *
 * Only support for IPV4.
 * 
 */
class SCPI
{
    protected:
        const std::string       m_ipAdress;     ///< Holds the IP Adress

        const std::string       m_ID;           ///< Name of the device.
                                                ///< It must be the answer to the 
                                                ///< "*IDN?" command.
                                                
        const unsigned short    m_port;         //!< Holds the correct port number.
                                                //!< The default for SCPI is 5025.
        
        struct sockaddr_in      m_server;       //!< Holds the data of the server/device.

        int                     m_socket;       //!< Hold the a file descriptor
                                                //!< that can be used in later
                                                //!< function calls that operate
                                                //!< on this socket.

        /** Standard initializer with default port 5025 for SCPI.
         * 
         */
        SCPI(std::string ipAdress, std::string identification, unsigned short port = 5025) : 
            m_ipAdress(ipAdress),
            m_ID(identification),
            m_port(port)
        {
            initSocket();
            openSocket();
            setCommand("*IDN?");
            closeSocket();
        }

        /// Initializes the device. 
        void initSocket();

        /// Opens the connection for possible commands. This functions needs to
        /// be followed by the close() member function. 
        void openSocket();

        /// Closes the connection.
        void closeSocket();

        /// Pushes a command trough the open socket. The newline character '\n'
        /// is appended in the function.
        void setCommand(std::string command);

        /// Receives and returns the answer from the device.
        std::string getAnswer(); 

        
    private:
        bool isClosed();
        
};


#endif // SCPI_H