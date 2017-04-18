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
using namespace std;
#include <sys/types.h>
#include <string.h>

#include <libps6000-1.4/ps6000Api.h>
#include <libps6000-1.4/PicoStatus.h>



int main()
{
    PICO_STATUS ret;   
    int16_t psHndl;   
    bool psOpen;
    ret = ps6000OpenUnit(&psHndl,nullptr);
    if(ret==PICO_OK)
    {
        cout<<"Picoscope opened.\n";
        psOpen=true;
    }
    else{
        cout<<"Problem! "<<ret<<"\n";
        return ret;
    };

    cout << "Press any button to continue..." << "\n";
    string dummy;
    cin >> dummy;
    cout << "Closing Pico" << "\n";

    ret = ps6000CloseUnit(psHndl);

    if(ret){
        cout << "Bombe!" << "\n";
    }



    return 0;
};
