#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

//
// The "pthreads-minimal" threading implementation is for demonstration
// purposes only.  It shows use of the generic threading implementation
// with the minimum use of capabilities from an underlying threading
// layer.
//

#include <pthread.h>


//
// This implementation uses both the generic sync/single variable support
// and the generic task management.
//
#define USE_GENERIC_SYNC_SINGLE_SUPPORT 1
#define USE_GENERIC_TASK_MANAGEMENT 1


//
// Declare required types and bring in chpl_threads_generic.h.
//

typedef pthread_mutex_t chpl_mutex_t;

// type (and default value) used to communicate thread identifiers
// between C code and Chapel code in the runtime.
typedef pthread_t chpl_threadID_t;
#define chpl_nullThreadID 0

typedef struct { } threadlayer_sync_aux_t;

typedef struct { } threadlayer_single_aux_t;

#include "chpl_threads_generic.h"


//
// Rename all the generic functions to the corresponding public names,
// so that they implement the public interface directly.
//

#define chpl_sync_lock_generic  \
        chpl_sync_lock
#define chpl_sync_unlock_generic  \
        chpl_sync_unlock
#define chpl_sync_wait_full_and_lock_generic  \
        chpl_sync_wait_full_and_lock
#define chpl_sync_wait_empty_and_lock_generic  \
        chpl_sync_wait_empty_and_lock
#define chpl_sync_mark_and_signal_full_generic  \
        chpl_sync_mark_and_signal_full
#define chpl_sync_mark_and_signal_empty_generic  \
        chpl_sync_mark_and_signal_empty
#define chpl_sync_is_full_generic  \
        chpl_sync_is_full
#define chpl_init_sync_aux_generic  \
        chpl_init_sync_aux
#define chpl_destroy_sync_aux_generic  \
        chpl_destroy_sync_aux

#define chpl_single_lock_generic  \
        chpl_single_lock
#define chpl_single_unlock_generic  \
        chpl_single_unlock
#define chpl_single_wait_full_generic  \
        chpl_single_wait_full
#define chpl_single_mark_and_signal_full_generic  \
        chpl_single_mark_and_signal_full
#define chpl_single_is_full_generic  \
        chpl_single_is_full
#define chpl_init_single_aux_generic  \
        chpl_init_single_aux
#define chpl_destroy_single_aux_generic  \
        chpl_destroy_single_aux

#define chpl_threads_getMaxThreads_generic  \
        chpl_threads_getMaxThreads
#define chpl_threads_maxThreadsLimit_generic  \
        chpl_threads_maxThreadsLimit
#define chpl_numThreads_generic  \
        chpl_numThreads
#define chpl_numIdleThreads_generic  \
        chpl_numIdleThreads
#define chpl_numQueuedTasks_generic  \
        chpl_numQueuedTasks
#define chpl_numRunningTasks_generic  \
        chpl_numRunningTasks
#define chpl_numBlockedTasks_generic  \
        chpl_numBlockedTasks

#define initChplThreads_generic  \
        initChplThreads
#define exitChplThreads_generic  \
        exitChplThreads

#define chpl_thread_init_generic  \
        chpl_thread_init
#define chpl_get_serial_generic  \
        chpl_get_serial
#define chpl_set_serial_generic  \
        chpl_set_serial

#define chpl_add_to_task_list_generic  \
        chpl_add_to_task_list
#define chpl_process_task_list_generic  \
        chpl_process_task_list
#define chpl_execute_tasks_in_list_generic  \
        chpl_execute_tasks_in_list
#define chpl_free_task_list_generic   \
        chpl_free_task_list

#define chpl_begin_generic  \
        chpl_begin


#endif // _chpl_cfg_threads_h_
