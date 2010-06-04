// Define the function names in the tasking interface.

//
// These build the desired function name.  ("TFN" is an acronym for
// "tasking function name".)
//

#define _TFN_CONCAT(name, suffix)    name##_##suffix
#define _TFN_INDIRECT(name, suffix)  _TFN_CONCAT(name, suffix)
#define _TFN(name)                   _TFN_INDIRECT(name, CHPL_TASKS)


//
// Define the names.
//

#define CHPL_SYNC_LOCK                   _TFN(chpl_sync_lock)
#define CHPL_SYNC_UNLOCK                 _TFN(chpl_sync_unlock)
#define CHPL_SYNC_WAIT_FULL_AND_LOCK     _TFN(chpl_sync_wait_full_and_lock)
#define CHPL_SYNC_WAIT_EMPTY_AND_LOCK    _TFN(chpl_sync_wait_empty_and_lock)
#define CHPL_SYNC_MARK_AND_SIGNAL_FULL   _TFN(chpl_sync_mark_and_signal_full)
#define CHPL_SYNC_MARK_AND_SIGNAL_EMPTY  _TFN(chpl_sync_mark_and_signal_empty)
#define CHPL_SYNC_IS_FULL                _TFN(chpl_sync_is_full)
#define CHPL_SYNC_INIT_AUX               _TFN(chpl_init_sync_aux)
#define CHPL_SYNC_DESTROY_AUX            _TFN(chpl_destroy_sync_aux)

#define CHPL_SINGLE_LOCK                 _TFN(chpl_single_lock)
#define CHPL_SINGLE_UNLOCK               _TFN(chpl_single_unlock)
#define CHPL_SINGLE_WAIT_FULL            _TFN(chpl_single_wait_full)
#define CHPL_SINGLE_MARK_AND_SIGNAL_FULL _TFN(chpl_single_mark_and_signal_full)
#define CHPL_SINGLE_IS_FULL              _TFN(chpl_single_is_full)
#define CHPL_SINGLE_INIT_AUX             _TFN(chpl_init_single_aux)
#define CHPL_SINGLE_DESTROY_AUX          _TFN(chpl_destroy_single_aux)

#define CHPL_TASKING_INIT                _TFN(chpl_tasking_init)
#define CHPL_TASKING_EXIT                _TFN(chpl_tasking_exit)

#define CHPL_ADD_TO_TASK_LIST            _TFN(chpl_add_to_task_list)
#define CHPL_PROCESS_TASK_LIST           _TFN(chpl_process_task_list)
#define CHPL_EXECUTE_TASKS_IN_LIST       _TFN(chpl_execute_tasks_in_list)
#define CHPL_FREE_TASK_LIST              _TFN(chpl_free_task_list)

#define CHPL_BEGIN                       _TFN(chpl_begin)

#define CHPL_TASK_ID                     _TFN(chpl_task_id)
#define CHPL_TASK_SLEEP                  _TFN(chpl_task_sleep)
#define CHPL_GET_SERIAL                  _TFN(chpl_get_serial)
#define CHPL_SET_SERIAL                  _TFN(chpl_set_serial)

#define CHPL_NUMQUEUEDTASKS              _TFN(chpl_numQueuedTasks)
#define CHPL_NUMRUNNINGTASKS             _TFN(chpl_numRunningTasks)
#define CHPL_NUMBLOCKEDTASKS             _TFN(chpl_numBlockedTasks)

#define CHPL_THREADS_GETMAXTHREADS       _TFN(chpl_threads_getMaxThreads)
#define CHPL_THREADS_MAXTHREADSLIMIT     _TFN(chpl_threads_maxThreadsLimit)
#define CHPL_NUMTHREADS                  _TFN(chpl_numThreads)
#define CHPL_NUMIDLETHREADS              _TFN(chpl_numIdleThreads)
