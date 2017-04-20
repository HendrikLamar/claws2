// =====================================================================================
// 
//       Filename:  pico.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  12.04.2017 16:26:42
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================


#include <iostream>
#include <sys/types.h>
#include <string>
#include <array>

#include <libps6000-1.4/ps6000Api.h>
#include <libps6000-1.4/PicoStatus.h>



int main()
{
    std::cout << "Starting Picotest..." << "\n";

    PICO_STATUS ret;   
    int16_t psHndl;   
    bool psOpen;
    ret = ps6000OpenUnit(&psHndl,nullptr);
    if(ret==PICO_OK)
    {
        std::cout<<"Picoscope opened.\n";
        psOpen=true;
    }
    else{
        std::cout<<"Problem! "<<ret<<"\n";
        return ret;
    };

    std::cout << "How many times the LED should blink?" << "\n";
    int16_t blink;
    std::cin >> blink;
    ret = ps6000FlashLed(psHndl, blink);




    std::cout << "Press any button + ENTER to continue..." << "\n";
    std::string dummy;
    std::cin >> dummy;
    std::cout << "Closing Pico" << "\n";
    ret = ps6000CloseUnit(psHndl);

    if(ret){
        std::cout << "Bombe!" << "\n";
    }



    return 0;
};
