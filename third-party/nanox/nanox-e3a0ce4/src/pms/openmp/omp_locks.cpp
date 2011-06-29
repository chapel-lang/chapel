/*************************************************************************************/
/*      Copyright 2010 Barcelona Supercomputing Center                               */
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

#include "omp.h"
#include "nanos.h"
#include "atomic.hpp"

extern "C"
{
   using namespace nanos;

   void omp_init_lock( omp_lock_t *arg )
   {
      // NOTE: This assumes Lock is the same size than Void * so nothing has to be allocated
      Lock *lock = (Lock *) arg;

      new (lock) Lock;
   }

   void omp_destroy_lock( omp_lock_t *arg )
   {
   }

   void omp_set_lock( omp_lock_t *arg )
   {
      Lock &lock = *(Lock *) arg;
      lock++;
   }

   void omp_unset_lock( omp_lock_t *arg )
   {
      Lock &lock = *(Lock *) arg;
      lock--;
   }

   int omp_test_lock( omp_lock_t *arg )
   {
      Lock &lock = *(Lock *) arg;
      return lock.tryAcquire();
   }

   struct __omp_nest_lock {
      Lock lock;
      nanos_wd_t owner;
      short count;
   };

   void omp_init_nest_lock( omp_nest_lock_t *arg ) {
      struct __omp_nest_lock *nlock = NEW struct __omp_nest_lock();
      nlock->owner = NULL;
      nlock->count = 0;
      *arg = nlock;
   }

   void omp_destroy_nest_lock( omp_nest_lock_t *arg ) {
      struct __omp_nest_lock *nlock=( struct __omp_nest_lock * )*arg;
      delete nlock;
   }

   void omp_set_nest_lock( omp_nest_lock_t *arg ) {

      struct __omp_nest_lock *nlock=( struct __omp_nest_lock * )*arg;

      if ( nlock->owner == nanos_current_wd() ) {
         // count >=1 is assumed because only the owner can set it
         nlock->count++;
      } else {
         nlock->lock++;
         // count == 0 is assumed because we just acquired the lock
         nlock->owner = nanos_current_wd();
         nlock->count++;
      }
   }

   void omp_unset_nest_lock( omp_nest_lock_t *arg ) {

      struct __omp_nest_lock *nlock=( struct __omp_nest_lock * )*arg;

      nlock->count--;
      if ( nlock->count == 0 ) {
         nlock->owner = NULL;
         nlock->lock--;
      }
   }

   int omp_test_nest_lock( omp_nest_lock_t *arg ) {
      struct __omp_nest_lock *nlock=( struct __omp_nest_lock * )*arg;
     
      if ( nlock->owner == nanos_current_wd() ) {
         // count >=1 is assumed because only the owner can set it
         nlock->count++;
         return 1;
      } else {
         int result = nlock->lock.tryAcquire();
         if ( result != 0 ) {
            // count == 0 is assumed because we just acquired the lock
            nlock->owner = nanos_current_wd();
            nlock->count++;
         }
         return result;
      }
   }
}

