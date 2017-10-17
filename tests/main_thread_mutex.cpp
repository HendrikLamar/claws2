// =====================================================================================
// 
//       Filename:  main_thread_mutex.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  17.10.2017 23:34:04
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

void foo( int i, std::mutex& mu ){
    std::lock_guard<std::mutex> iolock(mu);
    std::cout << "Thread No: " << i << std::endl;
}

int main()
{

    std::cout << "Number of CPUs: " << std::thread::hardware_concurrency() << std::endl;
    
    std::mutex iomutex;
    std::vector< std::thread > noThread(11);

    std::cout << noThread.size() << std::endl;

    for( int i = 0; i < 11; ++i )
    {
        noThread.at(i) = std::thread([&iomutex,i]{
                {   
                std::lock_guard<std::mutex>  mylock(iomutex);
                std::cout << "Thread No: " << i << "\n";
                }
                     // Simulate important work done by the tread by sleeping for a bit...
                     std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
                });

    }

    for( auto& tmp : noThread )
    {
        tmp.join();
    }



    return 0;
}


/* // Sample of launching C++11 threads and checking how many CPUs the host system
 * // has.
 * //
 * // Eli Bendersky [http://eli.thegreenplace.net]
 * // This code is in the public domain.
 * #include <algorithm>
 * #include <chrono>
 * #include <iostream>
 * #include <mutex>
 * #include <thread>
 * #include <vector>
 * 
 * int main(int argc, const char** argv) {
 *   unsigned num_cpus = std::thread::hardware_concurrency();
 *   std::cout << "Launching " << num_cpus << " threads\n";
 * 
 *   // A mutex ensures orderly access to std::cout from multiple threads.
 *   std::mutex iomutex;
 *   std::vector<std::thread> threads(num_cpus);
 *   for (unsigned i = 0; i < num_cpus; ++i) {
 *     threads[i] = std::thread([&iomutex, i] {
 *       {
 *         // Use a lexical scope and lock_guard to safely lock the mutex only for
 *         // the duration of std::cout usage.
 *         std::lock_guard<std::mutex> iolock(iomutex);
 *         std::cout << "Thread #" << i << " is running\n";
 *       }
 * 
 *       // Simulate important work done by the tread by sleeping for a bit...
 *       std::this_thread::sleep_for(std::chrono::milliseconds(200));
 * 
 *     });
 *   }
 * 
 *   for (auto& t : threads) {
 *     t.join();
 *   }
 *   return 0;
 * }
 */
