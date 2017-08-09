// =====================================================================================
// 
//       Filename:  n6700.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  09.08.2017 10:17:41
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================
#include <string>

#include "readini.h"
#include "scpi.h"
#include "utility.h"


///////////////////////////////////////////////////////////////////////////////



class KN6700 : 
    public SCPI,
    public ReadIni
{
    public:

        /** Standard initializer with default port 5025 for SCPI.
         *      - ipAdress is the regular IP adress as string
         *      - identity is the output of *IDN? 
         *      - the port is only needed if a different port than 5025 is demanded
         * 
         */
        KN6700(std::string ipAdress, std::string identity, unsigned short port = 5025);

        virtual ~KN6700();

        // checks if the device is available and respons the device name we expect
        void checkDevice();
        
    private:
        const std::string       m_ID;           ///< Name of the device.
                                                ///< It must be the answer to the 
                                                ///< "*IDN?" command.
                                                
        //! Holds the all the channel settings in a special Agi_Channel struct.
        std::vector< Utility::K_Channel >     m_K_Channels;

        //! Reads in the ini-file for the powersupply and stores the channel
        //! settings in the channel vector m_Agi_Channels;
        void K_readIni();




};
