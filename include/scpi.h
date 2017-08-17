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
#include <vector>
//#include <sys/socket.h>
//#include <sys/types.h>
#include <arpa/inet.h>




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

        const unsigned short    m_port;         //!< Holds the correct port number.
                                                //!< The default for SCPI is 5025.
        
        struct sockaddr_in      m_server;       //!< Holds the data of the server/device.

        int                     m_socket;       //!< Hold the a file descriptor
                                                //!< that can be used in later
                                                //!< function calls that operate
                                                //!< on this socket.

        /** Standard initializer with default port 5025 for SCPI.
         *      - ipAdress is the regular IP adress as string
         *      - the port is only needed if a different port than 5025 is demanded
         * 
         */
        SCPI(std::string ipAdress, unsigned short port = 5025);

        virtual ~SCPI();

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

        /// Splits a string (e.g. the return value of a voltage measurement of
        /// one or multiple channels) by comma and returns each channel in
        /// consecutive in a vector<double>.
        std::vector< double > splitStringbyComma( std::string text );
        
    private:
        bool isClosed();
        
};


///////////////////////////////////////////////////////////////////////////////



#endif // SCPI_H
