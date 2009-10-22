//
// Generic implementation of Chapel threading interface
//

#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
#define NDEBUG
#endif

#include "chapel_code.h"
#include "chplcomm.h"
#include "chplexit.h"
#include "chpl_mem.h"
#include "chplrt.h"
#include "chplthreads.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>

#include <nanos.h>

//
// task pool: linked list of tasks
//
typedef struct task_pool_struct* task_pool_p;

typedef struct task_pool_struct {
  chpl_fn_p        fun;          // function to call for task
  void*            arg;          // argument to the function
  chpl_bool        serial_state; // whether new tasks can be created while executing fun
  chpl_bool        begun;        // whether execution of this task has begun
  chpl_task_list_p ltask;        // points to the task list entry, if there is one
  chpl_string      filename;
  int              lineno;
  task_pool_p      next;
} task_pool_t;


// This struct is intended for use in a circular linked list where the pointer
// to the list actually points to the tail of the list, i.e., the last entry
// inserted into the list, making it easier to append items to the end of the list.
// Since it is part of a circular list, the last entry will, of course,
// point to the first entry in the list.
struct chpl_task_list {
  chpl_fn_p fun;
  void* arg;
  volatile task_pool_p ptask; // when null, execution of the associated task has begun
  chpl_string filename;
  int lineno;
  chpl_task_list_p next;
};


typedef struct lockReport {
  const char*        filename;
  int                lineno;
  chpl_bool          maybeLocked, maybeDeadlocked;
  struct lockReport* next;
} lockReport_t;


// This is the data that is private to each thread.
typedef struct {
  chpl_bool     serial_state;
  lockReport_t* lockRprt;
} thread_private_data_t;


static chpl_bool        initialized = false;

static chpl_mutex_t     threading_lock;     // critical section lock
static chpl_mutex_t     extra_task_lock;    // critical section lock
static chpl_mutex_t     task_list_lock;     // critical section lock
static task_pool_p      task_pool_head;     // head of task pool
static task_pool_p      task_pool_tail;     // tail of task pool
static int              queued_cnt;         // number of tasks in the task pool
static int              waking_cnt;         // number of threads signaled to wakeup
static int              running_cnt;        // number of running threads 
static int              threads_cnt;        // number of threads (total)
static int              blocked_thread_cnt; // number of threads waiting for something
static int              idle_cnt;           // number of threads that are idle
static int64_t          extra_task_cnt;     // number of threads executing more than one task
static chpl_bool*       maybe_deadlocked;   // whether all existing threads are blocked
static chpl_mutex_t     report_lock;        // critical section lock

static lockReport_t* lockReportHead = NULL;
static lockReport_t* lockReportTail = NULL;

static chpl_string idleTaskName = "|idle|";

static void        report_locked_threads(void);
static void        report_all_tasks(void);
static void        SIGINT_handler(int sig);
static void        initializeLockReportForThread(void);
static chpl_bool   set_block_loc(int, chpl_string);
static void        unset_block_loc(void);
static void        check_for_deadlock(void);
static void        remove_begun_tasks_from_pool (void);
static void*       chpl_begin_helper(void*);
static void        launch_next_task_in_new_thread(void);
static void        schedule_next_task(int);
static task_pool_p add_to_task_pool(chpl_fn_p,
                                    void*,
                                    chpl_bool,
                                    chpl_task_list_p);


// Sync variables

static void sync_wait_and_lock(chpl_sync_aux_t *s,
                               chpl_bool want_full,
                               int32_t lineno,
                               chpl_string filename) {
  nanos_wait_on_bool(&s->is_full,want_full);
}

void chpl_sync_lock_generic(chpl_sync_aux_t *s) {
  CHPL_MUTEX_LOCK(s->lock);
}

void chpl_sync_unlock_generic(chpl_sync_aux_t *s) {
  CHPL_MUTEX_UNLOCK(s->lock);
}

void chpl_sync_wait_full_and_lock_generic(chpl_sync_aux_t *s,
                                          int32_t lineno,
                                          chpl_string filename) {
  sync_wait_and_lock(s, true, lineno, filename);
}

void chpl_sync_wait_empty_and_lock_generic(chpl_sync_aux_t *s,
                                           int32_t lineno,
                                           chpl_string filename) {
  sync_wait_and_lock(s, false, lineno, filename);
}

void chpl_sync_mark_and_signal_full_generic(chpl_sync_aux_t *s) {
  s->is_full = true;
  threadlayer_sync_awaken(s);
  chpl_sync_unlock_generic(s);
}

void chpl_sync_mark_and_signal_empty_generic(chpl_sync_aux_t *s) {
  s->is_full = false;
  threadlayer_sync_awaken(s);
  chpl_sync_unlock_generic(s);
}

chpl_bool chpl_sync_is_full_generic(void *val_ptr,
                                    chpl_sync_aux_t *s,
                                    chpl_bool simple_sync_var) {
  return s->is_full;
}

void chpl_init_sync_aux_generic(chpl_sync_aux_t *s) {
  s->is_full = false;
  s->lock = CHPL_MUTEX_NEW();
  threadlayer_init_sync(s);
}

void chpl_destroy_sync_aux_generic(chpl_sync_aux_t *s) {
  chpl_free(s->lock, 0, 0);
  threadlayer_destroy_sync(s);
}


// Single variables

void chpl_single_lock_generic(chpl_single_aux_t *s) {
  CHPL_MUTEX_LOCK((chpl_mutex_t*)s->lock);
}

void chpl_single_unlock_generic(chpl_single_aux_t *s) {
  CHPL_MUTEX_UNLOCK((chpl_mutex_t*)s->lock);
}

void chpl_single_wait_full_generic(chpl_single_aux_t *s,
                                   int32_t lineno, chpl_string filename) {

  nanos_wait_on_bool(&s->is_full,1);
}

void chpl_single_mark_and_signal_full_generic(chpl_single_aux_t *s) {
  s->is_full = true;
  threadlayer_single_awaken(s);
  chpl_single_unlock_generic(s);
}

chpl_bool chpl_single_is_full_generic(void *val_ptr,
                                      chpl_single_aux_t *s,
                                      chpl_bool simple_single_var) {
  return s->is_full;
}

void chpl_init_single_aux_generic(chpl_single_aux_t *s) {
  s->is_full = false;
  s->lock = CHPL_MUTEX_NEW();
  threadlayer_init_single(s);
}

void chpl_destroy_single_aux_generic(chpl_single_aux_t *s) {
  chpl_free(s->lock, 0, 0);
  threadlayer_destroy_single(s);
}


// Tasks

void chpl_tasking_init_generic() {
  thread_private_data_t *tp;
  
  if (blockreport || taskreport) {
    chpl_internal_error("blockreport/taskreport not implemented");
  }


/*  tp = chpl_alloc(sizeof(thread_private_data_t), CHPL_RT_MD_THREAD_PRIVATE_DATA, 0, 0);
  threadlayer_set_thread_private_data(tp);
  tp->serial_state = false;*/
}


void chpl_tasking_exit_generic() {
}


void chpl_add_to_task_list_generic(chpl_fn_int_t fid, void* arg,
                                   chpl_task_list_p *task_list,
                                   int32_t task_list_locale,
                                   chpl_bool call_chpl_begin,
                                   int lineno,
                                   chpl_string filename) {
   
  if (false && task_list_locale == chpl_localeID) {
    chpl_task_list_p ltask;

    ltask =  (chpl_task_list_p)chpl_alloc(sizeof(struct chpl_task_list),
                                          CHPL_RT_MD_TASK_LIST_DESCRIPTOR,
                                          0, 0);
    ltask->filename = filename;
    ltask->lineno   = lineno;
    ltask->fun      = chpl_ftable[fid];
    ltask->arg      = arg;
    ltask->ptask    = NULL;
    if (call_chpl_begin) {
      chpl_fn_p fp = chpl_ftable[fid];
      CHPL_BEGIN(fp, arg, false, false, ltask);
    }

    // begin critical section - not needed for cobegin or coforall statements
    if (call_chpl_begin)
      CHPL_MUTEX_LOCK(&task_list_lock);

    if (*task_list) {
      ltask->next = (*task_list)->next;
      (*task_list)->next = ltask;
    }
    else
      ltask->next = ltask;
    *task_list = ltask;

    // end critical section - not needed for cobegin or coforall statements
    if (call_chpl_begin)
      CHPL_MUTEX_UNLOCK(&task_list_lock);
  }
  else {
    // call_chpl_begin should be true here because if task_list_locale !=
    // chpl_localeID, then this function could not have been called from
    // the context of a cobegin or coforall statement, which are the only
    // contexts in which CHPL_BEGIN() should not be called.
    chpl_fn_p fp = chpl_ftable[fid];
    //assert(call_chpl_begin);
    CHPL_BEGIN(fp, arg, false, false, NULL);
  }
}

void chpl_process_task_list_generic(chpl_task_list_p task_list) {
}

void chpl_execute_tasks_in_list_generic(chpl_task_list_p task_list) {
 
}


void chpl_free_task_list_generic(chpl_task_list_p task_list) {
}


//
// interface function with begin-statement
//
void chpl_begin_generic(chpl_fn_p fp, void* a,
                        chpl_bool ignore_serial,  // always add task to pool
                        chpl_bool serial_state,
                        chpl_task_list_p ltask) {

   assert(!ltask);
   nanos_wd_t wd=0;
   nanos_smp_args_t nanos_device_arg = { (void (*) (void *))fp};
   nanos_device_t nanos_devices[] =
       { {nanos_smp_factory, &nanos_device_arg } };

   nanos_wd_props_t props;
   props.mandatory_creation=true;
   props.tie_to = 0;
   props.tied = 0;

   nanos_create_wd ( &wd, 1, nanos_devices , 0 ,
                     &a, nanos_current_wd(), &props );

   nanos_submit(wd,0,0);
}


//TODO
chpl_bool chpl_get_serial_generic(void) {
  return 0;
}

void chpl_set_serial_generic(chpl_bool state) {
}


uint32_t chpl_numQueuedTasks_generic(void) {
   chpl_internal_error("chpl_numQueuedTasks_generic not implemented");
   return 0;
}

uint32_t chpl_numRunningTasks_generic(void) {
  int num;
  nanos_get_num_running_tasks(&num);
  return num;
}

int32_t  chpl_numBlockedTasks_generic(void) {
  chpl_internal_error("chpl_numBlockedTasks_generic not implemented");
  return 0;
}

// Internal utility functions for task management




// Threads

int32_t  chpl_threads_getMaxThreads_generic(void) { return 0; }

int32_t  chpl_threads_maxThreadsLimit_generic(void) { return 0; }

// take the main thread into account
uint32_t chpl_numThreads_generic(void) {
    chpl_internal_error("chpl_numThreads_generic not implemented");
    return 0;
}

uint32_t chpl_numIdleThreads_generic(void) {
    chpl_internal_error("chpl_numThreads_generic not implemented");
    return 0;
}

