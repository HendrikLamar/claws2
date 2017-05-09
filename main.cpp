// =====================================================================================
// 
//       Filename:  main.cpp
// 
//    Description:  Main File for the CLAWS DAQ2 
// 
//        Version:  1.0
//        Created:  25.04.2017 16:51:44
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================


#include "pico.h"
#include "roofi.h"

#include <iostream>
#include <string>

#include <TH1I.h>

int main(){

    std::cout << "Welcome to the CLAWS DAQ2!\n\n";

    Roofi::Initstruct myInit = Roofi::start_readIni();

    std::cout << myInit.Pico1Serial << "\t" << myInit.Pico1Alias << "\n";


/*     int16_t handle;
 *     int8_t serial[]{"CX743/024"};
 * //    int8_t serial;
 * 
 *     // initialize pico
 *     Pico *pico1 = new Pico(&handle, serial);
 *     
 *     std::cout << "enabled?: " << pico1->getCh(0)->returnEnabled() << std::endl;
 *     pico1->getCh(0)->setEnabled(true);
 *     pico1->getCh(1)->setEnabled(true);
 *     pico1->getCh(2)->setEnabled(true);
 *     pico1->getCh(3)->setEnabled(true);
 * 
 *     std::cout << pico1->getCh(0) << std::endl;
 * //
 * 
 *     pico1->getTimebase();
 * //        std::cout << timeInterval << "\n";
 *     std::cout << "\nThe chosen time interval is: " << pico1->returnTimeInterval() \
 *         << "\n";
 *     std::cout << "The number of samples we can collect: " << pico1->returnMaxSamples()\
 *         << "\n\n";
 * 
 *     pico1->setSimpleTrigger(true, PS6000_TRIGGER_AUX, -1024, PS6000_FALLING, 0, 10000);
 */


/*     for(auto kk = 1; kk < 10 + 1; ++kk){
 *         pico1->runBlock();
 *         pico1->getValues();
 * 
 * 
 *         char tmpPath[8];
 *         if(kk<10){
 *             sprintf(tmpPath, "00%d",kk);
 *         }
 *         else if(kk<100){
 *             sprintf(tmpPath, "0%d",kk);
 *         }
 *         else if(kk<=1000){
 *             sprintf(tmpPath, "%d",kk);
 *         }
 *         
 *         std::string pathFinalA = std::string("output/Event-") + tmpPath + "-ChA-10M.root";
 *         std::string pathFinalB = std::string("output/Event-") + tmpPath + "-ChB-10M.root";
 *         std::string pathFinalC = std::string("output/Event-") + tmpPath + "-ChC-10M.root";
 *         std::string pathFinalD = std::string("output/Event-") + tmpPath + "-ChD-10M.root";
 *         
 *         TH1I *hChA = new TH1I("hChA", tmpPath, pico1->returnNoOfSamples(), 0, \
 *                 pico1->returnNoOfSamples());
 *         TH1I *hChB = new TH1I("hChB", tmpPath, pico1->returnNoOfSamples(), 0, \
 *                 pico1->returnNoOfSamples());
 *         TH1I *hChC = new TH1I("hChC", tmpPath, pico1->returnNoOfSamples(), 0, \
 *                 pico1->returnNoOfSamples());
 *         TH1I *hChD = new TH1I("hChD", tmpPath, pico1->returnNoOfSamples(), 0, \
 *                 pico1->returnNoOfSamples());
 * 
 *         for(uint32_t ii = 0; ii < pico1->returnNoOfSamples(); ++ii){
 *             hChA->Fill(ii, pico1->getCh(0)->getBuffer()->at(ii));
 *             hChB->Fill(ii, pico1->getCh(1)->getBuffer()->at(ii));
 *             hChC->Fill(ii, pico1->getCh(2)->getBuffer()->at(ii));
 *             hChD->Fill(ii, pico1->getCh(3)->getBuffer()->at(ii));
 * //            if(ii%100==0){
 * //                std::cout << pico1->getCh(0)->getBuffer()->at(ii) << "\n";
 * //            }
 *         }
 * 
 * 
 * 
 * 
 * //        for(uint32_t ii = 0; ii < noSamples; ++ii){
 * //            hChA->Fill(ii,buffA[ii]);
 * //            if(ii%100==0){
 * //                std::cout << buffA[ii] << "\n";
 * //            }
 * //        }
 * 
 * 
 * 
 *         hChA->SaveAs(pathFinalA.c_str());
 *         hChB->SaveAs(pathFinalB.c_str());
 *         hChC->SaveAs(pathFinalC.c_str());
 *         hChD->SaveAs(pathFinalD.c_str());
 *         delete hChA;
 *         hChA = nullptr;
 *         delete hChB;
 *         hChB = nullptr;
 *         delete hChC;
 *         hChC = nullptr;
 *         delete hChD;
 *         hChD = nullptr;
 *     }
 */



/*     std::cout << "Okay, closing unit now!\n";
 *     pico1->closeUnit();
 *     std::cout << "Pico closed!" << std::endl;
 * 
 */

    return 0;
}

