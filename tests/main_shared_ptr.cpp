// =====================================================================================
// 
//       Filename:  main_shr_ptr.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  17.10.2017 23:01:53
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#include <memory>
#include <iostream>
#include <string>
#include <vector>

#include <TH1I.h>

int main()
{
    typedef std::vector< std::shared_ptr< std::pair< std::string , std::shared_ptr<TH1I> > > > pico_data_hist;

//    std::shared_ptr< int > test{std::make_shared<int>()};
    std::shared_ptr< int > ptr1{std::make_shared< int >(1)};

    std::shared_ptr< int > ptr2{ptr1};

    ptr1 = std::make_shared< int >(5);

    std::cout << "Ptr1: " << *ptr1 << std::endl;
    std::cout << "Ptr2: " << *ptr2 << std::endl;



    std::shared_ptr<TH1I> hist{std::make_shared< TH1I >("hist", "", 
            12, 
            0, 
            12
            )};

    std::shared_ptr< std::vector< std::shared_ptr< TH1I> > > vHist{ new std::vector< std::shared_ptr< TH1I > > };

    vHist->push_back(hist);

    std::shared_ptr<TH1I> thist{nullptr};
    for( std::shared_ptr<TH1I> tmp : *vHist )
    {
        std::cout << tmp << std::endl;
        thist = tmp;
    }

    std::cout << thist<< std::endl;
/*     std::shared_ptr< std::pair< std::string, std::shared_ptr<TH1I> > > test_data{std::make_shared<std::pair< std::string, std::shared_ptr<TH1I> >>(std::make_pair( "Histogramm", hist ))};
 * 
 *     std::cout << "First: " << test_data->first << "\tSecond: " << test_data->second << std::endl;
 * 
 * 
 *     std::shared_ptr< std::pair< std::string, std::shared_ptr< std::pair< std::string, std::shared_ptr<TH1I> > > > > pair_of_pico_and_pico_data{std::make_shared<std::pair< std::string, std::shared_ptr< std::pair< std::string, std::shared_ptr<TH1I> > > > >("Upper", test_data)};
 * 
 *     std::shared_ptr < 
 *         std::vector< 
 *             std::shared_ptr< 
 *                 std::pair< 
 *                     std::string, std::shared_ptr< std::pair< std::string, std::shared_ptr<TH1I> > >
 *                     > 
 *                 > 
 *             >
 *         >  m_picos_hist;
 * 
 *     m_picos_hist->push_back(pair_of_pico_and_pico_data);
 */


//    // very very long way (thanks to shr_ptr to construct a pair of pico_data_hist
//    std::shared_ptr< std::pair< std::string, std::shared_ptr< TH1I > > > tipair{
//        std::make_shared< std::pair< std::string, std::shared_ptr< TH1I > > >
//            ( std::make_pair( "PS6000_A", hist ) )};
//
//    // another wrong and not easy to read way to construct one unit of m_picos_hist
//    std::shared_ptr< std::pair< std::string, std::shared_ptr< pico_data_hist > > > topair
//    {
//        std::make_shared< std::pair< std::string, std::shared_ptr< pico_data_hist > > >
//        (
//            std::make_pair( "Upper", tipair )
//        )
//    };

    return 0;
}
