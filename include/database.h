// =====================================================================================
// 
//       Filename:  database.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  24.07.2017 11:43:22
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <memory>

#include "readini.h"
#include "utility.h"
#include "n6700_channels.h"


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////



class Database
{


    public:
        Database();
        virtual ~Database();

        void            setStop( bool switcher );
        bool            getStop();

        
        std::shared_ptr<ReadIni>        getInitReader();

        //! Sets the number of picos initialized. To be called after initialization.
        void            setNoOfPicosInitialized( unsigned int numb );



        ///////////////////////////////////////////////////////////////////////


        ///////////////////////////////////////////////////////////////////////
        //                  Power Supply
        //   All settings for the powersupply are send as strings. 
        ///////////////////////////////////////////////////////////////////////
        

        //! Void function which reads in the powersupply settings for low and
        //! high gain mode.
        void N6700_readChSettings();
        
        //! Reads in the n6700.ini file.
        void N6700_readConnectSettings();

        //! Returns a nested pair-construct with high-/low gain settings for
        //! all four channels.
        N6700_Channels N6700_getChannels() const;

        //! Returns a struct with ip, id, and port for the n6700.
        Utility::N6700_connect N6700_getConnect() const;




        ///////////////////////////////////////////////////////////////////////
        //                 Picoscope 
        //
        ///////////////////////////////////////////////////////////////////////


        //! Reads in the pico settings for a specific run mode. The
        //! intermediate settings are read-in standardly.
        void Pico_readSettings( Utility::Pico_RunMode mode );

        std::unique_ptr<std::vector< std::shared_ptr<Utility::Pico_Conf_Pico> > >     m_picoData;     


        ///////////////////////////////////////////////////////////////////
        //
        //                START general clawsRun 
        //
        ///////////////////////////////////////////////////////////////////


        //! Return the current run number.
        std::shared_ptr<unsigned long>  Claws_getCounter();
        //! Increments the run number.
        void                            Claws_incrCounter();
        //! Reads/writes the run number, either to/from the standard file defined
        //! by initilizer.ini or to a file you define with the id you define.
        //! @
        void Claws_rwCounter(
                char rw,                                //!< 'r' for read, 'w' for write.
                std::string file = "",                  //!< when blank, the argument from initializer.ini is used
                std::string id = "Settings.runNumber"   //!< when blank, the standard "Settings.runNumber" is used
                );


        std::shared_ptr<Utility::Steering_Data>     Claws_getConfig();
        void                                        Claws_readConfig();








        ///////////////////////////////////////////////////////////////////
        //
        //                END general clawsRun 
        //
        ///////////////////////////////////////////////////////////////////
    
    private:
        bool                        m_stopSwitch;

        std::shared_ptr<ReadIni>    m_initReader;

        std::shared_ptr<Utility::Steering_Data> m_steeringData;

        // number of picos initialized. This value is set by clawsRun.
        unsigned int                         m_MaxNoOfPicos;


        ///////////////////////////////////////////////////////////////////////
        //                  Power Supply
        //   All settings for the powersupply are send as strings. 
        ///////////////////////////////////////////////////////////////////////

        std::shared_ptr<N6700_Channels> m_N6700_Channels;
        
        Utility::N6700_connect      m_n6700_connect;




        ///////////////////////////////////////////////////////////////////
        //
        //                 START Pico Stuff 
        //
        ///////////////////////////////////////////////////////////////////

        std::vector< int >      m_picoSuccessfulReadinSettings{1,1,1,1};



        // Help functions for Database::Pico_readSettings
        void Pico_readChannelsSettings( 
                Utility::Pico_RunMode mode, 
                int picoNo
                );
        void Pico_readAquisitionSettings( 
                Utility::Pico_RunMode mode, 
                int picoNo
                );
        void Pico_readTriggerSimpleSettings( 
                Utility::Pico_RunMode mode, 
                int picoNo
                );
        void Pico_readTriggerAdvSettings( 
                Utility::Pico_RunMode mode, 
                int picoNo
                );

        void Pico_readIntermediateSettings( int picoNo );

        std::shared_ptr<Utility::Pico_Conf_HL_Gain> Pico_getHLGainStruct( 
                Utility::Pico_RunMode mode,
                int picoNo
                );


        std::string Pico_returnPathToRunMode( Utility::Pico_RunMode mode );
        


        ///////////////////////////////////////////////////////////////////
        //
        //                 STOP Pico Stuff 
        //
        ///////////////////////////////////////////////////////////////////





        







        ///////////////////////////////////////////////////////////////////
        //
        //                START general clawsRun 
        //
        ///////////////////////////////////////////////////////////////////


        std::shared_ptr<unsigned long> m_runNumber;






        ///////////////////////////////////////////////////////////////////
        //
        //                END general clawsRun 
        //
        ///////////////////////////////////////////////////////////////////
};


#endif // DATABASE_H
