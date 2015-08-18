/**************************************************************************
*  Copyright 2011 Sandia Corporation. Under the terms of Contract
*  DE-AC04-94AL85000, there is a non-exclusive license for use of this work by
*  or on behalf of the U.S. Government. Export of this program may require a
*  license from the United States Government
**************************************************************************/

#ifndef _tasks_qthreads_h_
#define _tasks_qthreads_h_

#include <stdint.h>

#include <assert.h>
#include <qthread.h>
#include <stdio.h>
#include <pthread.h>

#include "chpltypes.h"
#include "chpl-tasks-prvdata.h"

#define CHPL_COMM_YIELD_TASK_WHILE_POLLING
void chpl_task_yield(void);

// For mutexes
//   type(s)
//     threadlayer_mutex_t
//     threadlayer_mutex_p
//   functions
//     threadlayer_mutex_init()
//     threadlayer_mutex_new()
//     threadlayer_mutex_lock()
//     threadlayer_mutex_unlock()
//
// For thread management
//   type(s)
//     <none>
//   functions
//     threadlayer_thread_id()
//     threadlayer_thread_cancel()
//     threadlayer_thread_join()
//
// For sync variables
//   type(s)
//     threadlayer_sync_aux_t
//   functions
//     threadlayer_sync_suspend()
//     threadlayer_sync_awaken()
//     threadlayer_sync_init()
//     threadlayer_sync_destroy()
//
// For task management
//   type(s)
//     <none>
//   functions
//     threadlayer_init()
//     threadlayer_thread_create()
//     threadlayer_pool_suspend()
//     threadlayer_pool_awaken()
//     threadlayer_get_thread_private_data()
//     threadlayer_set_thread_private_data()
//
// The types are declared in the threads-*.h file for each specific
// threading layer, and the callback functions are declared here.  The
// interfaces and requirements for these other types and callback
// functions are described elsewhere in this file.
//
// Although the above list may seem long, in practice many of the
// functions are quite simple, and with luck also easily extrapolated
// from what is done for other threading layers.  For an example of an
// implementation, see "pthreads" threading.
//

//
// Type (and default value) used to communicate task identifiers
// between C code and Chapel code in the runtime.
//
typedef unsigned int chpl_taskID_t;
#define chpl_nullTaskID QTHREAD_NULL_TASK_ID

//
// Mutexes
//
typedef syncvar_t chpl_mutex_t;

//
// Sync variables
//
// The threading layer's threadlayer_sync_aux_t may include any
// additional members the layer needs to support the suspend/awaken
// callbacks efficiently.  The FIFO tasking code itself does not
// refer to this type or the tl_aux member at all.
//
typedef struct {
    aligned_t lockers_in;
    aligned_t lockers_out;
    uint_fast32_t uncontested_locks;
    int       is_full;
    syncvar_t signal_full;
    syncvar_t signal_empty;
} chpl_sync_aux_t;

#define chpl_sync_reset(x) qthread_syncvar_empty(&(x)->sync_aux.signal_full)

#define chpl_read_FE(x) ({                                                           \
                             uint64_t y;                                             \
                             qthread_syncvar_readFE(&y, &(x)->sync_aux.signal_full); \
                             y; })

#define chpl_read_FF(x) ({                                                           \
                             uint64_t y;                                             \
                             qthread_syncvar_readFF(&y, &(x)->sync_aux.signal_full); \
                             y; })

#define chpl_read_XX(x) ((x)->sync_aux.signal_full.u.s.data)

#define chpl_write_EF(x, y) do {                                 \
        uint64_t z = (uint64_t)(y);                              \
        qthread_syncvar_writeEF(&(x)->sync_aux.signal_full, &z); \
} while(0)

#define chpl_write_FF(x, y) do {                                    \
        uint64_t z, dummy;                                          \
        z = (uint64_t)(y);                                          \
        qthread_syncvar_readFE(&dummy, &(x)->sync_aux.signal_full); \
        qthread_syncvar_writeF(&(x)->sync_aux.signal_full, &z);     \
} while(0)

#define chpl_write_XF(x, y) do {                                \
        uint64_t z = (uint64_t)(y);                             \
        qthread_syncvar_writeF(&(x)->sync_aux.signal_full, &z); \
} while(0)

#define chpl_single_reset(x) qthread_syncvar_empty(&(x)->single_aux.signal_full)

#define chpl_single_read_FF(x) ({                                                             \
                                    uint64_t y;                                               \
                                    qthread_syncvar_readFF(&y, &(x)->single_aux.signal_full); \
                                    y; })

#define chpl_single_write_EF(x, y) do {                            \
        uint64_t z = (uint64_t)(y);                                \
        qthread_syncvar_writeEF(&(x)->single_aux.signal_full, &z); \
} while(0)

#define chpl_single_read_XX(x) ((x)->single_aux.signal_full.u.s.data)

// Tasks

//
// Handy services for threading layer callback functions.
//
// The FIFO tasking implementation also provides the following service
// routines that can be used by threading layer callback functions.
//

//
// The remaining declarations are all for callback functions to be
// provided by the threading layer.
//

//
// These are called once each, from CHPL_TASKING_INIT() and
// CHPL_TASKING_EXIT().
//
void threadlayer_init(void);
void threadlayer_exit(void);

//
// Mutexes
//
/*void qthread_mutex_init(qthread_mutex_p);
 * qthread_mutex_p qthread_mutex_new(void);
 * void qthread_mutex_lock(qthread_mutex_p);
 * void qthread_mutex_unlock(qthread_mutex_p);*/

//
// Sync variables
//
// The CHPL_SYNC_WAIT_{FULL,EMPTY}_AND_LOCK() functions should call
// threadlayer_sync_suspend() when a sync variable is not in the desired
// full/empty state.  The call will be made with the sync variable's
// mutex held.  (Thus, threadlayer_sync_suspend() can dependably tell
// that the desired state must be the opposite of the state it initially
// sees the variable in.)  It should return (with the mutex again held)
// as soon as it can once either the sync variable changes to the
// desired state, or (if the given deadline pointer is non-NULL) the
// deadline passes.  It can return also early, before either of these
// things occur, with no ill effects.  If a deadline is given and it
// does pass, then threadlayer_sync_suspend() must return true;
// otherwise false.
//
// The less the function can execute while waiting for the sync variable
// to change state, and the quicker it can un-suspend when the variable
// does change state, the better overall performance will be.  Obviously
// the sync variable's mutex must be unlocked while the routine waits
// for the variable to change state or the deadline to pass, or livelock
// may result.
//
// The CHPL_SYNC_MARK_AND_SIGNAL_{FULL,EMPTY}() functions will call
// threadlayer_sync_awaken() every time they are called, not just when
// they change the state of the sync variable.
//
// Threadlayer_sync_{init,destroy}() are called to initialize or
// destroy, respectively, the contents of the tl_aux member of the
// chpl_sync_aux_t for the specific threading layer.
//
/*chpl_bool threadlayer_sync_suspend(chpl_sync_aux_t *s,
 *                                 struct timeval *deadline);
 * void threadlayer_sync_awaken(chpl_sync_aux_t *s);
 * void threadlayer_sync_init(chpl_sync_aux_t *s);
 * void threadlayer_sync_destroy(chpl_sync_aux_t *s);*/

//
// Task management
//

//
// The interface for thread creation may need to be extended eventually
// to allow for specifying such things as stack sizes and/or locations.
//
/*int threadlayer_thread_create(threadlayer_threadID_t*, void*(*)(void*), void*);*/

//
// Threadlayer_pool_suspend() is called when a thread finds nothing in
// the pool of unclaimed tasks, and so has no work to do.  The call will
// be made with the pointed-to mutex held.  It should return (with the
// mutex again held) as soon as it can once either the task pool is no
// longer empty or (if the given deadline pointer is non-NULL) the
// deadline passes.  It can return also early, before either of these
// things occur, with no ill effects.  If a deadline is given and it
// does pass, then threadlayer_pool_suspend() must return true;
// otherwise false.
//
// The less the function can execute while waiting for the pool to
// become nonempty, and the quicker it can un-suspend when that happens,
// the better overall performance will be.
//
// The mutex passed to threadlayer_pool_suspend() is the one that
// provides mutual exclusion for changes to the task pool.  Allowing
// access to this mutex simplifies the implementation for certain
// threading layers, such as those based on pthreads condition
// variables.  However, it also introduces a complication in that it
// allows a threading layer to create deadlock or livelock situations if
// it is not careful.  Certainly the mutex must be unlocked while the
// routine waits for the task pool to fill or the deadline to pass, or
// livelock may result.
//
/*chpl_bool threadlayer_pool_suspend(chpl_mutex_t*, struct timeval*);
 * void threadlayer_pool_awaken(void);*/

//
// Thread private data
//
// These set and get a pointer to thread private data associated with
// each thread.  This is for the use of the FIFO tasking implementation
// itself.  If the threading layer also needs to store some data private
// to each thread, it must make other arrangements to do so.
//
/*void  threadlayer_set_thread_private_data(void*);
 * void* threadlayer_get_thread_private_data(void);*/


#ifndef QTHREAD_MULTINODE
extern
#ifdef __cplusplus
"C"
#endif
volatile int chpl_qthread_done_initializing;
#endif

typedef struct {
    c_string task_filename;
    int task_lineno;
    chpl_taskID_t id;
    chpl_bool is_executeOn;
    c_sublocid_t requestedSubloc;  // requested sublocal for task
    chpl_task_prvData_t prvdata;
} chpl_task_prvDataImpl_t;

// Define PRV_DATA_IMPL_VAL to set up a chpl_task_prvData_t.
#define PRV_DATA_IMPL_VAL(_fn, _ln, _id, _is_execOn, _subloc, _serial) \
        { .task_filename = _fn, \
          .task_lineno = _ln, \
          .id = _id, \
          .is_executeOn = _is_execOn, \
          .requestedSubloc = _subloc, \
          .prvdata = { .serial_state = _serial } }

typedef struct {
    void                     *fn;
    void                     *args;
    chpl_bool                countRunning;
    chpl_task_prvDataImpl_t  chpl_data;
} chpl_qthread_wrapper_args_t;

// Structure of task-local storage
typedef struct chpl_qthread_tls_s {
    /* Task private data: serial state, etc. */
    chpl_task_prvDataImpl_t chpl_data;
    /* Reports */
    c_string    lock_filename;
    size_t      lock_lineno;
} chpl_qthread_tls_t;

extern pthread_t chpl_qthread_process_pthread;
extern pthread_t chpl_qthread_comm_pthread;

extern chpl_qthread_tls_t chpl_qthread_process_tls;
extern chpl_qthread_tls_t chpl_qthread_comm_task_tls;

#define CHPL_TASK_STD_MODULES_INITIALIZED chpl_task_stdModulesInitialized
void chpl_task_stdModulesInitialized(void);

// Wrap qthread_get_tasklocal() and assert that it is always available.
static inline chpl_qthread_tls_t * chpl_qthread_get_tasklocal(void)
{
    chpl_qthread_tls_t* tls;

    if (chpl_qthread_done_initializing) {
        tls = (chpl_qthread_tls_t *)
              qthread_get_tasklocal(sizeof(chpl_qthread_tls_t));
        if (tls == NULL) {
            pthread_t me = pthread_self();
            if (pthread_equal(me, chpl_qthread_comm_pthread))
                tls = &chpl_qthread_comm_task_tls;
            else if (pthread_equal(me, chpl_qthread_process_pthread))
                tls = &chpl_qthread_process_tls;
        }
        assert(tls);
    }
    else
        tls = NULL;

    return tls;
}

#ifdef CHPL_TASK_GET_PRVDATA_IMPL_DECL
#error "CHPL_TASK_GET_PRVDATA_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_GET_PRVDATA_IMPL_DECL 1
#endif
static inline chpl_task_prvData_t* chpl_task_getPrvData(void)
{
    chpl_qthread_tls_t * data = chpl_qthread_get_tasklocal();
    if (data) {
        return &data->chpl_data.prvdata;
    }
    assert(data);
    return NULL;
}

#ifdef CHPL_TASK_GETSUBLOC_IMPL_DECL
#error "CHPL_TASK_GETSUBLOC_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_GETSUBLOC_IMPL_DECL 1
#endif
static inline
c_sublocid_t chpl_task_getSubloc(void)
{
    return (c_sublocid_t) qthread_shep();
}

#ifdef CHPL_TASK_SETSUBLOC_IMPL_DECL
#error "CHPL_TASK_SETSUBLOC_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_SETSUBLOC_IMPL_DECL 1
#endif
static inline
void chpl_task_setSubloc(c_sublocid_t subloc)
{
    qthread_shepherd_id_t curr_shep;

    assert(subloc != c_sublocid_none);

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
        if (data) {
            data->chpl_data.requestedSubloc = subloc;
        }

        if (subloc != c_sublocid_any &&
            (qthread_shepherd_id_t) subloc != curr_shep) {
            qthread_migrate_to((qthread_shepherd_id_t) subloc);
        }
    }
}

#ifdef CHPL_TASK_GETREQUESTEDSUBLOC_IMPL_DECL
#error "CHPL_TASK_GETREQUESTEDSUBLOC_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_GETREQUESTEDSUBLOC_IMPL_DECL 1
#endif
static inline
c_sublocid_t chpl_task_getRequestedSubloc(void)
{
    chpl_qthread_tls_t * data = chpl_qthread_get_tasklocal();
    if (data) {
        return data->chpl_data.requestedSubloc;
    }
    return c_sublocid_any;
}

#ifdef CHPL_TASK_SUPPORTS_REMOTE_CACHE_IMPL_DECL
#error "CHPL_TASK_SUPPORTS_REMOTE_CACHE_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_SUPPORTS_REMOTE_CACHE_IMPL_DECL 1
#endif
extern int chpl_qthread_supports_remote_cache;
static inline
int chpl_task_supportsRemoteCache(void) {
  return chpl_qthread_supports_remote_cache;
}

#endif // ifndef _tasks_qthreads_h_
/* vim:set expandtab: */
