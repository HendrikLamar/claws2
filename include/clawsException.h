// =====================================================================================
// 
//       Filename:  ClawsException.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03.08.2017 18:42:57
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Hendrik Windel (hw), hwindel@mpp.mpg.de
//        Company:  Max Planck Institute for Physics
//        License:  GPLv3 - 2017, Hendrik Windel 
// 
// =====================================================================================

#ifndef CLAWSEXCEPTION_H
#define CLAWSEXCEPTION_H

#include <string>


/** The base class of the Claws specific exception handling.
 * 
 */
class ClawsException
{
    protected:
        const std::string       m_errorIn;
        std::string             m_errorOut;

    public:
        /** Standard Claws Exception
         * 
         */
        ClawsException(std::string error) :
            m_errorIn(error)
        {}

        /** In what() you need to specify which subexception was thrown.
         * Example: "Exception in SCPI: 'm_errorIn'"
         * 
         */
        virtual const char* what()
        {
            m_errorOut = "ClawsException: " + m_errorIn;
            return m_errorOut.c_str();
        };
        
};

///////////////////////////////////////////////////////////////////////////////

/** SCPI specific exception class derived from the ClawsException class.
 * 
 */
class SCPIException : public ClawsException
{
    public:
        SCPIException(std::string error) :
            ClawsException(error)
        {}

        const char* what()
        {
            m_errorOut = "SCPIException: " + m_errorIn;
            return m_errorOut.c_str();
        };
    
};




///////////////////////////////////////////////////////////////////////////////

/** Pico specific exception class derived from the ClawsException class.
 * 
 */

class PicoException : public ClawsException
{
    public:
        PicoException( std::string error) : ClawsException(error)
        {}

        const char* what()
        {
            m_errorOut = "PicoException: " + m_errorIn;
            return m_errorOut.c_str();
        }
};





///////////////////////////////////////////////////////////////////////////////

/** Channel specific exception class derived from the ClawsException class.
 * 
 */

class ChannelException : public ClawsException
{
    public:
        ChannelException( std::string error) : ClawsException(error)
        {}

        const char* what()
        {
            m_errorOut = "ChannelException: " + m_errorIn;
            return m_errorOut.c_str();
        }
};





///////////////////////////////////////////////////////////////////////////////

/** Utility specific exception class derived from the ClawsException class.
 * 
 */

class UtilityException: public ClawsException
{
    public:
        UtilityException( std::string error) : ClawsException(error)
        {}

        const char* what()
        {
            m_errorOut = "UtilityException: " + m_errorIn;
            return m_errorOut.c_str();
        }
};



#endif // CLAWSEXCEPTION_H
