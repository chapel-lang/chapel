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

#include <assert.h>
#include <pthread.h>

#include "barrier.hpp"
#include "system.hpp"
#include "atomic.hpp"
#include "plugin.hpp"

namespace nanos {
   namespace ext {

      /*! \class PosixBarrier
       *  \brief implements a barrier according to a centralized scheme with a posix barrier
       */
      class PosixBarrier: public Barrier
      {

         private:
            pthread_barrier_t _pBarrier;

            PosixBarrier( const PosixBarrier &barrier );
            const PosixBarrier & operator= ( const PosixBarrier & );
         public:
            PosixBarrier() { }

            void init ( int numParticipants );
            void resize ( int numParticipants );

            void barrier ( int participant );

            ~PosixBarrier() { pthread_barrier_destroy( &_pBarrier ); }
       };


      void PosixBarrier::init ( int numParticipants )
      {
         /*! initialize the barrier to the current participant number */
         pthread_barrier_init ( &_pBarrier, NULL, numParticipants );
      }

      void PosixBarrier::resize ( int numParticipants )
      {
         pthread_barrier_destroy( &_pBarrier );
         pthread_barrier_init ( &_pBarrier, NULL, numParticipants );
      }

      void PosixBarrier::barrier ( int participant )
      {
         pthread_barrier_wait( &_pBarrier );
      }

      static Barrier * createPosixBarrier()
      {
         return NEW PosixBarrier();
      }

      /*! \class PosixBarrierPlugin
       *  \brief plugin of the related posixBarrier class
       *  \see posixBarrier
       */
      class PosixBarrierPlugin : public Plugin
      {

         public:
            PosixBarrierPlugin() : Plugin( "Posix Barrier Plugin",1 ) {}

            virtual void config( Config &config ) {}

            virtual void init() {
               sys.setDefaultBarrFactory( createPosixBarrier );
            }
      };

   }
}

nanos::ext::PosixBarrierPlugin NanosXPlugin;

