// =====================================================================================
// 
//       Filename:  n6700_channels.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  16.08.2017 16:40:05
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#ifndef N6700_CHANNELS_H
#define N6700_CHANNELS_H

#include <vector>
#include <string>
#include <map>


class N6700_Channels
{
    protected:
        //! Hold the settings for a single channel.
        typedef std::map< std::string, std::string >        N6700_Ch;
        //! Holds multiple channels (e.g. Ch1, Ch2, ... )
        typedef std::map< std::string, N6700_Ch >           N6700_MultiCh;
        //! Holds for multiple channels several run modes.
        typedef std::map< std::string, N6700_MultiCh >      N6700_Mode;

        //! 
        N6700_Mode        m_powerSupply;



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
//            m_channels.insert( std::pair<std::string, N6700_MultiCh>(mode, std::pair<std::string, N6700_Ch>(channel, std::pair<std::string, std::string>(key, value))));
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

        //! Vector containing all possible channel.
        // \todo one could read this via ini-file to not recompile if modified
        const std::vector< std::string > Channels{"Ch1", "Ch2", "Ch3", "Ch4"};

        //! Vector containing all possible settings per channel.
        const std::vector< std::string > Settings{"Active", "CurrLimit", "Volt"};

        //! Vector containing all possible modes (e.g. High_Gain, Low_Gain).
        const std::vector< std::string > Gains{"High_Gain", "Low_Gain"};

};



#endif // N6700_CHANNELS_H
