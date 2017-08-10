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

#include "scpi.h"



///////////////////////////////////////////////////////////////////////////////
//              START Channel class for the N6700
//
//              n6700 follows afterwards
///////////////////////////////////////////////////////////////////////////////


class KPS_Channels
{
    protected:
        //! Hold the settings for a single channel.
        typedef std::map< std::string, std::string >        KPS_Ch;
        //! Holds multiple channels (e.g. Ch1, Ch2, ... )
        typedef std::map< std::string, KPS_Ch >           KPS_MultiCh;
        //! Holds for multiple channels several run modes.
        typedef std::map< std::string, KPS_MultiCh >      KPS_Mode;

        //! 
        KPS_Mode        m_powerSupply;

        //! Vector containing all possible channel.
        // \todo one could read this via ini-file to not recompile if modified
        const std::vector< std::string > m_KPS_channels {"Ch1", "Ch2", "Ch3", "Ch4"};

        //! Vector containing all possible settings per channel.
        const std::vector< std::string > m_KPS_settings {"Active", "CurrLimit", "Volt"};

        //! Vector containing all possible modes (e.g. High_Gain, Low_Gain).
        const std::vector< std::string > m_KPS_mode {"High_Gain", "Low_Gain"};


    public:

        //! Inserts a setting to a the defined keys.
        //! No error checking yet!
        // \todo Add proper insert() function with some error checking.
        void setKey( 
                std::string mode, 
                std::string channel, 
                std::string key, 
                std::string value )
        {
//            m_channels.insert( std::pair<std::string, KPS_MultiCh>(mode, std::pair<std::string, KPS_Ch>(channel, std::pair<std::string, std::string>(key, value))));
            m_powerSupply[mode][channel][key] = value;
        }

        //! Returns the value to the key parameters.
        std::string getKey(
                std::string mode, 
                std::string channel, 
                std::string key)
        {
            return m_powerSupply[mode][channel][key];
        }


};


///////////////////////////////////////////////////////////////////////////////
//              END Channel class for the N6700
///////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////
//              START n6700 class
///////////////////////////////////////////////////////////////////////////////



class N6700 : public SCPI
{
    public:

        /** Standard initializer with default port 5025 for SCPI.
         *      - ipAdress is the regular IP adress as string
         *      - identity is the output of *IDN? 
         *      - the port is only needed if a different port than 5025 is demanded
         * 
         */
        N6700(std::string ipAdress, std::string identity, unsigned short port = 5025);

        virtual ~N6700();

        // checks if the device is available and respons the device name we expect
        void checkDevice();
        
    private:
        const std::string       m_ID;           ///< Name of the device.
                                                ///< It must be the answer to the 
                                                ///< "*IDN?" command.
                                                

};

///////////////////////////////////////////////////////////////////////////////
//              END n6700 class
///////////////////////////////////////////////////////////////////////////////

#endif // N6700_H
