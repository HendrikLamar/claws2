// =====================================================================================
// 
//       Filename:  main.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  24.07.2017 15:33:05
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>


#include <chrono>
#include <thread>
#include <memory>
#include <mutex>

#include <TROOT.h>
#include <TH1I.h>
#include <TFile.h>



int main()
{

    ROOT::EnableThreadSafety();
    gROOT->SetBatch();
//    gROOT->ProcessLine("gErrorIgnoreLevel = 6000");

    int nData = 16;
    unsigned int nDataLength = 100000;
    int loops = 100;
    
    for( int runNum = 0; runNum < loops; ++runNum )
    {
        std::shared_ptr<std::vector<std::shared_ptr<TH1I>>> finalData = 
            std::make_shared<std::vector<std::shared_ptr<TH1I>>>();
    
        std::cout << "Starting run \t\t\t#" << runNum << std::endl;
    
        std::shared_ptr<std::vector<
            std::shared_ptr<std::vector<int>>>> myRawData = 
            std::make_shared<std::vector<std::shared_ptr<std::vector<int>>>>();
    
        for( int i = 0; i < nData; ++i )
        {
    
            std::shared_ptr<std::vector<int>> tmpV = std::make_shared<std::vector<int>>();
            for( unsigned int ii = 0; ii < nDataLength; ++ii )
            {
                tmpV->push_back(1);
            }

            std::string name = 
                "event-" + 
                std::to_string(i);
            finalData->push_back(std::make_shared<TH1I>(
                    name.c_str(), 
                    name.c_str(),
                    tmpV->size(),
                    0,
                    tmpV->size()));
    
            myRawData->push_back(tmpV);
        }
    
//        std::cout << "Length RawDataVec: " << myRawData->size() << std::endl;


        for( int workID = 0; workID < nData; ++workID )
        {

        }
    
        // create work
        auto workItem = [](
//                int workID,
                std::shared_ptr<std::vector<int>> rawData,
//                std::shared_ptr<std::vector<std::shared_ptr<TH1I>>> data,
                std::mutex *mymutex,
                std::shared_ptr<TH1I> thist)
            {
//                std::shared_ptr<TH1I> thist = std::make_shared<TH1I>(
//                        name.c_str(),
//                        name.c_str(),
//                        rawData->size(),
//                        0,
//                        rawData->size());

                for( unsigned int tint = 0; tint < rawData->size(); ++tint )
                {
                    thist->SetBinContent(tint+1,rawData->at(tint));
                };
//                std::cout << "Mutex address: " << mymutex << std::endl;
//                std::cout << "InThread address rawData: " << rawData << std::endl;
//                std::cout << "InThread address data: " << data << std::endl;

//                std::this_thread::sleep_for(std::chrono::seconds(2));
//                std::lock_guard<std::mutex> guard(*mymutex);
//                data->push_back(thist);
//                std::cout << "In workID: " << &runNum << std::endl;
            };
    
        // transform to TH1I
        std::vector<std::thread> workers;
        std::mutex finalData_mutex;
        std::cout << "Starting threads\n";
        for( int workID = 0; workID < nData; ++workID )
        {
//            std::cout << "OutThread address rawData: " << myRawData->at(workID) << std::endl;
//            std::cout << "OutThread address data: " << finalData << std::endl;
//            std::cout << "Out workID: " << &workID << std::endl;
//            std::cout << "Out Mutex address: " << &finalData_mutex << std::endl;
            workers.emplace_back(
                    workItem, 
//                    workID, 
                    myRawData->at(workID), 
//                    finalData,
                    &finalData_mutex,
                    finalData->at(workID));
        }

        std::cout << "Waiting for threads\n";
        for( auto & tthread : workers ) tthread.join();

        std::string tpath  = "/mnt/claws_data/data/test/";
        std::string name = "event-" + std::to_string(runNum);
        std::string fpath = tpath + name + ".root";

        std::shared_ptr<TFile> tfile = 
            std::make_shared<TFile>(fpath.c_str(), "RECREATE");
        tfile->cd();

        for( auto& tmp : *finalData )
        {
            tmp->Write();
        }
//        std::cout << "Flushing TFile...\n";
//        tfile->Flush();
        tfile->Write();

        std::cout << "Closing TFile...\n";
        tfile->Close();

        std::cout << "Cleaning data vector..." << std::flush;
        finalData->clear();
        std::cout << "done!\n";
    }
    
    return 0;
};
