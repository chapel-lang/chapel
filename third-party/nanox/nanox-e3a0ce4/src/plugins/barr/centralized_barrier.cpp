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

#include "barrier.hpp"
#include "system.hpp"
#include "atomic.hpp"
#include "schedule.hpp"
#include "plugin.hpp"
#include "synchronizedcondition.hpp"

namespace nanos {
   namespace ext {

      /*! \class CentralizedBarrier
       *  \brief implements a single semaphore barrier
       */
      class CentralizedBarrier: public Barrier
      {

         private:
            Atomic<int> _sem;
            Atomic<bool> _flag;
            MultipleSyncCond<EqualConditionChecker<bool> > _syncCondTrue;
            MultipleSyncCond<EqualConditionChecker<bool> > _syncCondFalse;
            int _numParticipants;

         public:
            CentralizedBarrier () : Barrier(), _sem(0), _flag(false),
               _syncCondTrue( EqualConditionChecker<bool>( &(_flag.override()), true ), 1 ),
               _syncCondFalse( EqualConditionChecker<bool>( &(_flag.override()), false ), 1 ) {}
            CentralizedBarrier ( const CentralizedBarrier& barrier ) : Barrier(barrier), _sem(0), _flag(false),
               _syncCondTrue( EqualConditionChecker<bool>( &(_flag.override()), true ), barrier._numParticipants ),
               _syncCondFalse( EqualConditionChecker<bool>( &(_flag.override()), false ), barrier._numParticipants )
               { init( barrier._numParticipants ); }

            const CentralizedBarrier & operator= ( const CentralizedBarrier & barrier );

            virtual ~CentralizedBarrier() { }

            void init ( int numParticipants );
            void resize ( int numThreads );

            void barrier ( int participant );
      };

      const CentralizedBarrier & CentralizedBarrier::operator= ( const CentralizedBarrier & barrier )
      {
         // self-assignment
         if ( &barrier == this ) return *this;

         Barrier::operator=(barrier);
         _sem = 0;
         _flag = false;

         if ( barrier._numParticipants != _numParticipants )
            resize(barrier._numParticipants);
         
         return *this;
      }

      void CentralizedBarrier::init( int numParticipants ) 
      {
         _numParticipants = numParticipants;
         _syncCondTrue.resize( numParticipants );
         _syncCondFalse.resize( numParticipants );
      }

      void CentralizedBarrier::resize( int numParticipants ) 
      {
         _numParticipants = numParticipants;
         _syncCondTrue.resize( numParticipants );
         _syncCondFalse.resize( numParticipants );
      }


      void CentralizedBarrier::barrier( int participant )
      {
         int val;


         val = ++_sem;

         /* the last process incrementing the semaphore sets the flag
         releasing all other threads waiting in the next block */
         if ( val == _numParticipants ) {
            _flag=true;
            _syncCondTrue.signal();
         } else {
            _syncCondTrue.wait();
         }

         val = --_sem;

         /* the last thread decrementing the sem for the second time resets the flag.
         A thread passing in the next barrier will be blocked until this is performed */
         if ( val == 0 ) {
            _flag=false;
            _syncCondFalse.signal();
         } else {
            _syncCondFalse.wait();
         }
      }


      static Barrier * createCentralizedBarrier()
      {
         return NEW CentralizedBarrier();
      }


      /*! \class CentralizedBarrierPlugin
       *  \brief plugin of the related centralizedBarrier class
       *  \see centralizedBarrier
       */
      class CentralizedBarrierPlugin : public Plugin
      {

         public:
            CentralizedBarrierPlugin() : Plugin( "Centralized Barrier Plugin",1 ) {}

            virtual void config( Config &config ) {}

            virtual void init() {
               sys.setDefaultBarrFactory( createCentralizedBarrier );
            }
      };

   }
}

nanos::ext::CentralizedBarrierPlugin NanosXPlugin;

