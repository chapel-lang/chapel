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

#include "system.hpp"
#include "throttle_decl.hpp"
#include "plugin.hpp"
#include "config.hpp"

namespace nanos {
   namespace ext {

      //TODO: only works with 1 scheduling group

      class IdleThreadsThrottle: public ThrottlePolicy
      {
         private:
            int _limit;

            IdleThreadsThrottle ( const IdleThreadsThrottle & );
            const IdleThreadsThrottle & operator= ( const IdleThreadsThrottle & );
         public:
            //used in the plugin: must be public
            static const int _defaultLimit;

            IdleThreadsThrottle( int actualLimit = _defaultLimit ) : _limit( actualLimit ) {}

            void init() {}

            void setMaxCutoff( int mi ) { _limit = mi; }

            bool throttle();

            ~IdleThreadsThrottle() {}
      };

      const int IdleThreadsThrottle::_defaultLimit = 100;

      bool IdleThreadsThrottle::throttle()
      {
         //checking if the number of idle threads is higher than the allowed maximum
         if ( sys.getIdleNum() > _limit )  {
            return false;
         }

         return true;
      }

      //factory
      static IdleThreadsThrottle * createIdleThrottle( int actualLimit )
      {
         return NEW IdleThreadsThrottle( actualLimit );
      }


      class IdleThreadsThrottlePlugin : public Plugin
      {
         private:
            int _actualLimit;

         public:
            IdleThreadsThrottlePlugin() : Plugin( "Idle Threads Throttling Plugin",1 ), _actualLimit( IdleThreadsThrottle::_defaultLimit ) {}

            virtual void config( Config &config )
            {
               config.setOptionsSection( "Idle threads throttle",
                                         "Throttle policy based on idle threads" );
                                         
               config.registerConfigOption ( "throttle-limit",
                  NEW Config::PositiveVar( "throttle-limit", _actualLimit),
                  "Defines maximum number of Idle Threads" );
               config.registerArgOption ( "throttle-limit", "throttle-limit" );
            }

            virtual void init() {
               sys.setThrottlePolicy( createIdleThrottle( _actualLimit )); 
            }
      };

   }
}

nanos::ext::IdleThreadsThrottlePlugin NanosXPlugin;
