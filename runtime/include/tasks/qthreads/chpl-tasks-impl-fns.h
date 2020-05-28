/**************************************************************************
*  Copyright 2011 Sandia Corporation. Under the terms of Contract
*  DE-AC04-94AL85000, there is a non-exclusive license for use of this work by
*  or on behalf of the U.S. Government. Export of this program may require a
*  license from the United States Government
**************************************************************************/

/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Qthreads tasking layer interface function shortcuts.
//

#ifndef _chpl_tasks_impl_fns_h_
#define _chpl_tasks_impl_fns_h_

#include "chpl-locale-model.h"
#include "chpltypes.h"

#include <assert.h>
#include "qthread.h"
#include "qthread-chapel.h"

#include <pthread.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern
#ifdef __cplusplus
"C"
#endif
volatile int chpl_qthread_done_initializing;

#define CHPL_TASK_STD_MODULES_INITIALIZED chpl_task_stdModulesInitialized
void chpl_task_stdModulesInitialized(void);

//
// Task private data
//

//
// Structure of task-local storage.
//
// Note: Granted this is a type and chpl-task-impl.h defines those, but
// it's a type only used by the functions below and it requires another
// type (chpl_task_infoRuntime_t) defined in chpl-tasks.h proper, before
// we are included, so we define it here.
//
typedef struct chpl_qthread_tls_s {
  struct chpl_task_bundle* bundle;
  // The below fields could move to chpl_task_bundleData_t
  // That would reduce the size of the task local storage,
  // but increase the size of executeOn bundles.
  chpl_task_infoRuntime_t infoRuntime;
} chpl_qthread_tls_t;

extern pthread_t chpl_qthread_process_pthread;
extern pthread_t chpl_qthread_comm_pthread;

extern chpl_qthread_tls_t chpl_qthread_process_tls;
extern chpl_qthread_tls_t chpl_qthread_comm_task_tls;

// Wrap qthread_get_tasklocal().
static inline chpl_qthread_tls_t* chpl_qthread_get_tasklocal(void)
{
    chpl_qthread_tls_t* tls;

    if (chpl_qthread_done_initializing) {
        tls = (chpl_qthread_tls_t*)
               qthread_get_tasklocal(sizeof(chpl_qthread_tls_t));
        if (tls == NULL) {
            pthread_t me = pthread_self();
            if (pthread_equal(me, chpl_qthread_comm_pthread))
                tls = &chpl_qthread_comm_task_tls;
            else if (pthread_equal(me, chpl_qthread_process_pthread))
                tls = &chpl_qthread_process_tls;
        }
    }
    else
        tls = NULL;

    return tls;
}

#ifdef CHPL_TASK_GET_INFO_RUNTIME_IMPL_DECL
#error "CHPL_TASK_GET_INFO_RUNTIME_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_GET_INFO_RUNTIME_IMPL_DECL 1
#endif
static inline chpl_task_infoRuntime_t* chpl_task_getInfoRuntime(void)
{
    chpl_qthread_tls_t * data = chpl_qthread_get_tasklocal();
    if (data) {
        return &data->infoRuntime;
    }
    return NULL;
}

#ifdef CHPL_TASK_GET_INFO_CHAPEL_IMPL_DECL
#error "CHPL_TASK_GET_INFO_CHAPEL_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_GET_INFO_CHAPEL_IMPL_DECL 1
#endif
static inline chpl_task_infoChapel_t* chpl_task_getInfoChapel(void)
{
    chpl_qthread_tls_t * data = chpl_qthread_get_tasklocal();
    if (data && data->bundle) {
        return &data->bundle->infoChapel;
    }
    assert(data && data->bundle);
    return NULL;
}


//
// Sublocale support
//
#ifdef CHPL_TASK_GETREQUESTEDSUBLOC_IMPL_DECL
#error "CHPL_TASK_GETREQUESTEDSUBLOC_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_GETREQUESTEDSUBLOC_IMPL_DECL 1
#endif
static inline
c_sublocid_t chpl_task_getRequestedSubloc(void)
{
    chpl_qthread_tls_t * data = chpl_qthread_get_tasklocal();
    if (data && data->bundle) {
        return data->bundle->requestedSubloc;
    }
    return c_sublocid_any;
}

#ifdef CHPL_TASK_GETSUBLOC_IMPL_DECL
#error "CHPL_TASK_GETSUBLOC_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_GETSUBLOC_IMPL_DECL 1
#endif
static inline
c_sublocid_t chpl_task_getSubloc(void)
{
    return chpl_localeModel_sublocMerge(chpl_task_getRequestedSubloc(),
                                        (c_sublocid_t) qthread_shep());
}

#ifdef CHPL_TASK_SETSUBLOC_IMPL_DECL
#error "CHPL_TASK_SETSUBLOC_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_SETSUBLOC_IMPL_DECL 1
#endif
static inline
void chpl_task_setSubloc(c_sublocid_t full_subloc)
{
    qthread_shepherd_id_t curr_shep;

    assert(isActualSublocID(full_subloc) || full_subloc == c_sublocid_any);

    // Only change sublocales if the caller asked for a particular one,
    // which is not the current one, and we're a (movable) task.
    //
    // Note: It's likely that this won't work in all cases where we need
    //       it.  In particular, we envision needing to move execution
    //       from sublocale to sublocale while initializing the memory
    //       layer, in order to get the NUMA domain affinity right for
    //       the subparts of the heap.  But this will be happening well
    //       before tasking init and in any case would be done from the
    //       main thread of execution, which doesn't have a shepherd.
    //       The code below wouldn't work in that situation.
    if ((curr_shep = qthread_shep()) != NO_SHEPHERD) {
        chpl_qthread_tls_t * data = chpl_qthread_get_tasklocal();
        c_sublocid_t execution_subloc =
          chpl_localeModel_sublocToExecutionSubloc(full_subloc);
        if (data && data->bundle) {
            data->bundle->requestedSubloc = full_subloc;
        }

        if (execution_subloc != c_sublocid_any &&
            (qthread_shepherd_id_t) execution_subloc != curr_shep) {
            qthread_migrate_to((qthread_shepherd_id_t) execution_subloc);
        }
    }
}

#define CHPL_TASK_IMPL_RESET_SPAWN_ORDER() qthread_chpl_reset_spawn_order()

#define CHPL_TASK_IMPL_GET_FIXED_NUM_THREADS() \
    chpl_task_impl_getFixedNumThreads()
uint32_t chpl_task_impl_getFixedNumThreads(void);

#define CHPL_TASK_IMPL_IS_FIXED_THREAD() (qthread_shep() != NO_SHEPHERD)

#define CHPL_TASK_IMPL_CAN_MIGRATE_THREADS() CHPL_QTHREAD_TASKS_CAN_MIGRATE_THREADS

#ifdef __cplusplus
} // end extern "C"
#endif

#endif // ifndef _chpl_tasks_impl_fns_h_
/* vim:set expandtab: */
