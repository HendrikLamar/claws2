// =====================================================================================
// 
//       Filename:  map.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  09.08.2017 17:19:16
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include <map>
#include <string>
#include <iostream>


class KPS_Channels
{
    private:
        //! Hold the settings for a single channel.
        typedef std::map< std::string, std::string >        KPS_Ch;
        //! Holds multiple channels (e.g. Ch1, Ch2, ... )
        typedef std::map< std::string, KPS_Ch >           KPS_MultiCh;
        //! Holds for multiple channels several run modes.
        typedef std::map< std::string, KPS_MultiCh >      KPS_Mode;

        //! 
        KPS_Mode        m_channels;

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
            m_channels[mode][channel][key] = value;
        }

        //! Returns the value to the key parameters.
        std::string getKey(
                std::string mode, 
                std::string channel, 
                std::string key)
        {
            return m_channels[mode][channel][key];
        }


};



int main()
{
    
    //! std::map for one N6700 channel.
    typedef std::map< std::string, std::string > KPS_Channel;
    //! std::map for all channels of the N6700
    typedef std::map< std::string, KPS_Channel > KPS_MultiChannel;
    //! std::map containing additional gain information
    //! Usage:
    //!         KPS_Gain[high_gain][ch
    typedef std::map< std::string, KPS_MultiChannel > KPS_Gain;

    KPS_Channel     mostInnerCh1;
    mostInnerCh1.insert( std::make_pair( "Active", "true") );
    mostInnerCh1.insert( std::make_pair( "Volt", "10" ) );

    KPS_Channel     mostInnerCh2;
    mostInnerCh2.insert( std::make_pair( "Active", "false" ) );
    mostInnerCh2.insert( std::make_pair( "Volt", "15" ) );

    std::cout << "Channel1: " << mostInnerCh1["Volt"] << std::endl;
    std::cout << "Channel1: " << mostInnerCh2["Volt"] << std::endl;

    KPS_MultiChannel    Channels;
    Channels.insert( std::make_pair( "Ch1", mostInnerCh1 ) );
    Channels.insert( std::make_pair( "Ch2", mostInnerCh2 ) );

    std::cout << "Channel1: " << Channels["Ch1"]["Active"] << std::endl;

    KPS_Gain    PowerSupply;
    PowerSupply.insert( std::make_pair( "High", Channels ) );

    std::cout << "Channel2: " << PowerSupply["High"]["Ch2"]["Volt"] << std::endl;

    PowerSupply["High"]["Ch1"]["Volt"] = "11";
    std::cout << "Channel1: " << mostInnerCh1["Volt"] << std::endl;
    std::cout << "Channel1: " << PowerSupply["High"]["Ch1"]["Volt"] << std::endl;


    KPS_Channels psu;
    psu.setKey("high", "ch1", "volt", "15");
    psu.setKey("high", "ch2", "volt", "16");

    std::cout << "\nhighGain, Ch1, Volt: " << psu.getKey("high", "ch1", "volt") << "\n";
    std::cout << "\nhighGain, Ch2, Volt: " << psu.getKey("high", "ch2", "volt") << "\n";

/*     KPS_MultiChannel     lowGain;
 *     lowGain.insert( "Ch1", mostInnerCh1 );
 *     lowGain.insert( "Ch2", mostInnerCh2);
 */




    return 0;
}
