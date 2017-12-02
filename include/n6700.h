// =====================================================================================
// 
//       Filename:  n6700.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  09.08.2017 14:10:25
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================


#ifndef N6700_H
#define N6700_H

#include <vector>
#include <string>
#include <map>
#include <memory>

#include "database.h"
//#include "n6700_channels.h"
#include "utility.h"
#include "scpi.h"


class N6700 : public SCPI
{
    public:

        /** Standard initializer with default port 5025 for SCPI.
         *      - ipAdress is the regular IP adress as string
         *      - identity is the output of *IDN? 
         *      - the port is only needed if a different port than 5025 is demanded
         * 
         */
//        N6700(std::string ipAdress, std::string identity, unsigned short port = 5025);
        N6700( std::shared_ptr<Database> database );

        virtual ~N6700();

        //! Checks if the device is available and respons the device name we expect
        void checkDevice();

        //! Load config from config file.
        void loadConfig();

        //! Sends the configuration stored in the database to the PSU.
        //! This function has a processing time of 1 sec.
        void sendConf( Utility::Claws_Gain gain );


        //! Start power. This function has a processing time of 4 sec.
        void start();
        
        //! Stop power. This function has a processing time of 1 sec.
        void stop();

        //! Returns the current measured voltage of all channels in a vector
        //! of doubles. The value for Channel 1 is at vector.at(0) and so on...
        std::vector < double > getVolt();

        //! Returns the current measured current of all channels in a vector
        //! of doubles. The value for Channel 1 is at vector.at(0) and so on...
        std::vector < double > getCurr();


//        void run();

    private:
        std::string::size_type  m_sz;
        std::shared_ptr<Database> m_database;




        const std::string       m_ID;           ///< Name of the device.
                                                ///< It must be the answer to the 
                                                ///< "*IDN?" command.
                                                
        std::shared_ptr<Utility::PSU_Config>    m_config_high;
        std::shared_ptr<Utility::PSU_Config>    m_config_low;

        std::shared_ptr<Utility::PSU_Config>    m_config_current;

        //! Sends the command to the PSU.
        void sendCommand( std::string cmd );
                                                
};


#endif // N6700_H
