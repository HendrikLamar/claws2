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


#include <unistd.h> // for usleep
#include <iostream>
#include <sys/types.h>
#include <string>
#include <array>

// pico related stuff
#include <libps6000-1.4/ps6000Api.h>
#include <libps6000-1.4/PicoStatus.h>

// ROOT stuff
#include <TH1I.h>
#include <TCanvas.h>

extern bool g_ready = false;

void checkStatus(PICO_STATUS ret, int i){
    if(ret!=PICO_OK){
        std::cout << "Houston, we have a problem at #" << i << "!" << "\n";
        std::cout << "It's error code: " << ret << "\n";
    }
}

void CallBackBlock(int16_t handle, PICO_STATUS ret, void *pParameter){
    if(ret != PICO_CANCELLED){
        
        g_ready = true;
    }
}

int main()
{
    std::cout << "Starting Picotest..." << "\n";

    PICO_STATUS ret;   

    // Enumerate Units
    int16_t counts;
    int16_t serialLth = 10;
    int8_t serials[serialLth];
    ret = ps6000EnumerateUnits(&counts, serials, &serialLth);

    std::cout << "No of Picos found: " << counts << "\n";
    std::cout << "List of serial numbers: " << serials << "\n";
    std::cout << "Length of the string: " << serialLth << "\n\n";

    int16_t psHndl;   
    bool psOpen;
    ret = ps6000OpenUnit(&psHndl,serials);
    if(ret==PICO_OK)
    {
        std::cout<<"Picoscope opened.\n";
        psOpen=true;
    }
    else{
        std::cout<<"Problem! "<<ret<<"\n";
        return ret;
    };




/*     std::cout << "Press any button + ENTER to continue..." << "\n";
 *     std::string dummy;
 *     std::cin >> dummy;
 */

                                                                                
    ret = ps6000SetEts(psHndl, PS6000_ETS_OFF, 0, 0, NULL); // Turn off ETS

    for (auto kk = 0; kk < 5; ++kk){
        
        // set all the trigger stuff to some default value
        ret = ps6000SetSimpleTrigger(psHndl,0,PS6000_CHANNEL_A,0,PS6000_FALLING,0,500);
        checkStatus(ret, 1);

        // Turn on all Channels to some default values
        float analogueOffset1{0.001};
        ret = ps6000SetChannel(psHndl,PS6000_CHANNEL_A,true,PS6000_DC_1M,\
                PS6000_50MV,analogueOffset1,PS6000_BW_FULL);   
        checkStatus(ret, 2);
        ret = ps6000SetChannel(psHndl,PS6000_CHANNEL_B,false,PS6000_DC_1M,\
                PS6000_50MV,analogueOffset1,PS6000_BW_FULL);   
        checkStatus(ret, 3);
        ret = ps6000SetChannel(psHndl,PS6000_CHANNEL_C,false,PS6000_DC_1M,\
                PS6000_50MV,analogueOffset1,PS6000_BW_FULL);   
        checkStatus(ret, 4);
        ret = ps6000SetChannel(psHndl,PS6000_CHANNEL_D,false,PS6000_DC_1M,\
                PS6000_50MV,analogueOffset1,PS6000_BW_FULL);   
        checkStatus(ret, 5);

        float analogueOffset2{0.000};
        ret = ps6000SetChannel(psHndl,PS6000_CHANNEL_A,true,PS6000_DC_1M,\
                PS6000_50MV,analogueOffset2,PS6000_BW_FULL);   
        checkStatus(ret, 6);

        // pass a proper data buffer to store the data
        uint32_t bufferLthA{210};
        int16_t *buffA = new int16_t[bufferLthA];
        int32_t timebase{2};
        float samplingTime;

        uint32_t maxSamples;
        uint32_t segmentIndex{0};
        ret = ps6000GetTimebase2(psHndl,timebase,bufferLthA,&samplingTime,0,\
                &maxSamples,segmentIndex);
        checkStatus(ret, 7);
        ret = ps6000SetDataBuffer(psHndl,PS6000_CHANNEL_A,buffA,bufferLthA,\
                PS6000_RATIO_MODE_NONE);
        checkStatus(ret, 8);

        std::cout << "The chosen sampling time is " << samplingTime << " ns.\n\n";
        std::cout << "Starting capturing data..." << "\n\n";

        int32_t aqTime;
        uint32_t preTriggers    {100};
        uint32_t postTriggers   {100};
        ret = ps6000RunBlock(psHndl,preTriggers,postTriggers,timebase,1,&aqTime,\
                segmentIndex,CallBackBlock,nullptr);
        checkStatus(ret, 9);

        std::cout << "Waiting for trigger..." << "\n";

        int counter = 0;
        while(!g_ready){
            usleep(0);
            ++counter;
        }

        std::cout << "RunBlock is done: " << g_ready << "\n";
        std::cout << "and took " << counter << " loops." << std::endl;

        ret = ps6000Stop(psHndl);
        checkStatus(ret, 10);
        
//        int16_t psReady;
//        PICO_STATUS ps6000IsReady(psHndl,&psReady);
        
        uint32_t noOfSamples = preTriggers + postTriggers;
        ret = ps6000GetValues(psHndl,0,&noOfSamples,1,PS6000_RATIO_MODE_NONE,0,NULL);

        std::cout << noOfSamples << " samples collected! Saving now!" << "\n";

/*         for(uint32_t ii = 0; ii < bufferLthA; ++ii ){
 *             std::cout << ii << "\t" <<  buffA[ii] << "\n";
 *         }
 */

        char bufNaming1[8];
        sprintf(bufNaming1, "A%d", kk);

        TH1I *hist = new TH1I(bufNaming1, bufNaming1, preTriggers+postTriggers,0,\
                preTriggers + postTriggers);

        for(uint32_t ii = 0; ii < noOfSamples; ++ii){
            hist->Fill(ii,buffA[ii]);
        }

        TCanvas *c1 = new TCanvas("001", "001", 800,600);
        hist->Draw("hist");
        char buff1[127];
        sprintf(buff1, "First_Waveform_No%d.pdf", kk);
        c1->SaveAs(buff1);

        // delete old stuff
        delete hist;
        hist = nullptr;
        delete c1;
        c1 = nullptr;

        char bufNaming2[8];
        sprintf(bufNaming2, "B%d", kk);
        TH1I *hist2 = new TH1I(bufNaming2,bufNaming2,9, -1024-256,1024+256);
        for(uint32_t ii = 0; ii < noOfSamples; ++ii){
            hist2->Fill(buffA[ii]);
        }
        

        TCanvas *c2 = new TCanvas("001", "001", 800,600);
        hist2->Draw("hist");

        char buff2[127];
        sprintf(buff2, "First_Histogram_No%d.pdf", kk);
        c2->SaveAs(buff2);

        delete hist2;
        hist2 = nullptr;
        delete c2;
        c2 = nullptr;


        // wait one second before start the next run
        sleep(2);
    }


/*     std::cout << "Press any button + ENTER to continue..." << "\n";
 *     std::cin >> dummy;
 */



    std::cout << "Closing Pico" << "\n";
    ret = ps6000CloseUnit(psHndl);

    if(ret){
        std::cout << "Bombe!" << "\n";
    }



    return 0;
};
