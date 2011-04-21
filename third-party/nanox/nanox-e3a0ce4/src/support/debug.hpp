/*************************************************************************************/
/*      Copyright 2009 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/

#ifndef _NANOS_LIB_DEBUG
#define _NANOS_LIB_DEBUG

#include <stdexcept>
//Having system.hpp here generate too many circular dependences
//but it's not really needed so we can delay it most times until the actual usage
//#include "system.hpp"
#include "xstring.hpp"
#include <iostream>

namespace nanos
{

   class FatalError : public  std::runtime_error
   {

      public:
         FatalError ( const std::string &value, int peId=-1 ) :
               runtime_error( std::string( "FATAL ERROR: [" ) + toString<int>( peId ) + "] " + value ) {}

   };

   class FailedAssertion : public  std::runtime_error
   {

      public:
         FailedAssertion ( const char *file, const int line, const std::string &value,
                           const std::string msg, int peId=-1 ) :
               runtime_error(
                  std::string( "ASSERT failed: [" )+ toString<int>( peId ) + "] "
                  + value + ":" + msg
                  + " (" + file + ":" + toString<int>( line )+ ")" ) {}

   };

#define fatal(msg)  throw nanos::FatalError(msg,getMyThreadSafe()->getId());
#define fatal0(msg)  throw nanos::FatalError(msg);
#define fatal_cond(cond,msg) if ( cond ) fatal(msg);
#define fatal_cond0(cond,msg) if ( cond ) fatal0(msg);

#define warning(msg) { std::cerr << "WARNING: [" << getMyThreadSafe()->getId() << "]" << msg << std::endl; }
#define warning0(msg) { std::cerr << "WARNING: [?]" << msg << std::endl; }

#define message(msg) \
   std::cerr << "MSG: [" << getMyThreadSafe()->getId() << "] " << msg << std::endl;
#define message0(msg) \
   std::cerr << "MSG: [?] " << msg << std::endl;

#ifdef NANOS_DEBUG_ENABLED
#define ensure(cond,msg) if ( !(cond) ) throw nanos::FailedAssertion(__FILE__, __LINE__ , #cond, msg, getMyThreadSafe()->getId());
#define ensure0(cond,msg) if ( !(cond) ) throw nanos::FailedAssertion(__FILE__, __LINE__, #cond, msg );

#define verbose(msg) \
   if (sys.getVerbose()) std::cerr << "[" << getMyThreadSafe()->getId() << "]" << msg << std::endl;
#define verbose0(msg) \
   if (sys.getVerbose()) std::cerr << "[?]" << msg << std::endl;

#define debug(msg) \
   if (sys.getVerbose()) std::cerr << "DBG: [" << getMyThreadSafe()->getId() << "]" << msg << std::endl;
#define debug0(msg) \
   if (sys.getVerbose()) std::cerr << "DBG: [?]" << msg << std::endl;
#else
#define ensure(cond,msg)
#define ensure0(cond,msg)
#define verbose(msg)
#define verbose0(msg)
#define debug(msg)
#define debug0(msg)
#endif

};


#endif
