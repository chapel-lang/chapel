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

#include "system.hpp"
#include "chpl_nanos.h"

using namespace nanos;

// // TODO: include chpl headers?
// typedef char * chpl_string;
// typedef bool chpl_bool;
// typedef void (*chpl_fn_p) (void *);
// typedef void * chpl_task_list_p;
// typedef int chpl_fn_int_t;
// typedef int chpl_taskID_t;
// 
// extern chpl_fn_p chpl_ftable[];

// Sync variables interface

void chpl_sync_initAux(chpl_sync_aux_t *s)
{
   s->is_full = false;

   s->empty = (void * )NEW MultipleSyncCond<EqualConditionChecker<bool> >( EqualConditionChecker<bool>( &s->is_full, false ) , 4);
   s->full = (void *) NEW MultipleSyncCond<EqualConditionChecker<bool> >( EqualConditionChecker<bool>( &s->is_full, true ) , 4);

   s->lock = (void *) NEW Lock();
}

void chpl_sync_destroyAux(chpl_sync_aux_t *s)
{
   delete (GenericSyncCond *)s->empty;
   delete (GenericSyncCond *)s->full;
}

void chpl_sync_waitFullAndLock(chpl_sync_aux_t *s,
                               int32_t lineno, chpl_string filename)
{
   GenericSyncCond *sync = (GenericSyncCond *) s->full;
   Lock *l = (Lock *)s->lock;

   l->acquire();
   while ( !s->is_full ) {
      l->release();
      sync->wait();
      l->acquire();
   }
}

void chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *s,
                                int32_t lineno, chpl_string filename)
{
   GenericSyncCond *sync = (GenericSyncCond *) s->empty;
   Lock *l = (Lock *)s->lock;

   l->acquire();
   while ( s->is_full ) {
      l->release();
      sync->wait();
      l->acquire();
   }
  
}

void chpl_sync_markAndSignalFull(chpl_sync_aux_t *s)
{
   GenericSyncCond *sync = (GenericSyncCond *) s->full;
   Lock *l = (Lock *)s->lock;

   s->is_full = true;
   sync->signal_one();
   l->release();
}

void chpl_sync_markAndSignalEmpty(chpl_sync_aux_t *s)
{
   GenericSyncCond *sync = (GenericSyncCond *) s->empty;
   Lock *l = (Lock *)s->lock;

   s->is_full = false;
   sync->signal_one();
   l->release();
}

chpl_bool chpl_sync_isFull(void *val_ptr,
                           chpl_sync_aux_t *s,
                           chpl_bool simple_sync_var)
{
   return s->is_full;
}

void chpl_sync_lock(chpl_sync_aux_t *s)
{
   Lock *l = (Lock *)s->lock;
   l->acquire();
}

void chpl_sync_unlock(chpl_sync_aux_t *s)
{
   Lock *l = (Lock *)s->lock;
   l->release();
}
