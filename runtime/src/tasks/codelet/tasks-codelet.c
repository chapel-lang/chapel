//
// FIFO implementation of Chapel tasking interface
//

#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
#define NDEBUG
#endif

#include "chpl_rt_utils_static.h"
#include "chplcgfns.h"
#include "chpl-comm.h"
#include "chplexit.h"
#include "chpl-mem.h"
#include "chplrt.h"
#include "chpl-tasks.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>

#include <starpu.h>

//
// task pool: linked list of tasks
//

#if 0
typedef struct task_pool_struct* task_pool_p;
typedef struct task_pool_struct {
  chpl_taskID_t    id;           // task identifier
  chpl_fn_p        fun;          // function to call for task
  void*            arg;          // argument to the function
  chpl_bool        serial_state; // whether new tasks can be created while executing fun
  chpl_bool        begun;        // whether execution of this task has begun
  chpl_task_list_p ltask;        // points to the task list entry, if there is one
  chpl_string      filename;
  int              lineno;
  task_pool_p      next;
  task_pool_p      prev;
} task_pool_t;
#else
typedef struct chpl_pool_struct* chpl_task_pool_p;
typedef struct chpl_pool_struct {
  chpl_taskID_t id;       // task identifier
  chpl_fn_p fun;          // function to call for task
  void*     arg;          // argument to the function
  chpl_bool serial_state; // whether new tasks can be created while executing fun
  chpl_task_pool_p next;
} task_pool_t;
#endif




// This struct is intended for use in a circular linked list where the pointer
// to the list actually points to the tail of the list, i.e., the last entry
// inserted into the list, making it easier to append items to the end of the list.
// Since it is part of a circular list, the last entry will, of course,
// point to the first entry in the list.
#if 0
struct chpl_task_list {
  chpl_fn_p fun;
  void* arg;
  volatile task_pool_p ptask; // when null, execution of the associated task has begun
  chpl_string filename;
  int lineno;
  chpl_task_list_p next;
};
#endif

#if 0
typedef struct lockReport {
  const char*        filename;
  int                lineno;
  uint64_t           prev_progress_cnt;
  chpl_bool          maybeLocked;
  struct lockReport* next;
} lockReport_t;
#endif


// This is the data that is private to each thread.
#if 0
typedef struct {
  task_pool_p   ptask;
  lockReport_t* lockRprt;
} thread_private_data_t;
#endif


//static chpl_bool        initialized = false;

static chpl_thread_mutex_t threading_lock;     // critical section lock
static chpl_thread_mutex_t extra_task_lock;    // critical section lock
//static chpl_thread_mutex_t task_id_lock;       // critical section lock
//static chpl_thread_mutex_t task_list_lock;     // critical section lock
//static volatile task_pool_p
//                           task_pool_head;     // head of task pool
//static volatile task_pool_p
//                           task_pool_tail;     // tail of task pool
static chpl_task_pool_p     task_pool_head; // head of task pool
static chpl_task_pool_p     task_pool_tail; // tail of task pool


static int                 queued_task_cnt;    // number of tasks in task pool
static int                 running_task_cnt;   // number of running tasks
static int64_t             extra_task_cnt;     // number of tasks being run by
                                               //   threads occupied already
static int                 waking_thread_cnt;  // number of threads created but
                                               //   not yet running
//static int                 blocked_thread_cnt; // number of threads that
                                               //   cannot make progress
static int                 idle_thread_cnt;    // number of threads looking
                                               //   for work
static uint64_t            progress_cnt;       // number of unblock operations,
                                               //   as a proxy for progress


// Sync variables

static void sync_wait_and_lock(chpl_sync_aux_t *s,
                               chpl_bool want_full,
                               int32_t lineno, chpl_string filename) {
  chpl_thread_mutexLock(&s->lock);

  if (blockreport)
    progress_cnt++;
}

void chpl_sync_lock(chpl_sync_aux_t *s) {
  chpl_thread_mutexLock(&s->lock);
}

void chpl_sync_unlock(chpl_sync_aux_t *s) {
  chpl_thread_mutexUnlock(&s->lock);
}

void chpl_sync_waitFullAndLock(chpl_sync_aux_t *s,
                                  int32_t lineno, chpl_string filename) {
  sync_wait_and_lock(s, true, lineno, filename);
}

void chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *s,
                                   int32_t lineno, chpl_string filename) {
  sync_wait_and_lock(s, false, lineno, filename);
}

void chpl_sync_markAndSignalFull(chpl_sync_aux_t *s) {
  s->is_full = true;
  chpl_sync_unlock(s);
}

void chpl_sync_markAndSignalEmpty(chpl_sync_aux_t *s) {
  s->is_full = false;
  chpl_sync_unlock(s);
}

chpl_bool chpl_sync_isFull(void *val_ptr,
                            chpl_sync_aux_t *s,
                            chpl_bool simple_sync_var) {
  return s->is_full;
}

void chpl_sync_initAux(chpl_sync_aux_t *s) {
  s->is_full = false;
  chpl_thread_mutexInit(&s->lock);
}

void chpl_sync_destroyAux(chpl_sync_aux_t *s) { }


// Tasks

static int                  queued_cnt;     // number of tasks in the task pool
static uint64_t taskCallStackSize = 0;
static chpl_taskID_t curr_taskID;
static chpl_taskID_t next_taskID = chpl_nullTaskID + 1;
static chpl_bool serial_state;
void chpl_task_init(int32_t numThreadsPerLocale, int32_t maxThreadsPerLocale, 
                    int numCommTasks, uint64_t callStackSize) {
  //
  // If a value was specified for the call stack size config const, use
  // that (rounded up to a whole number of pages) to set the system
  // stack limit.
  //
  if (callStackSize != 0) {
    uint64_t      pagesize = (uint64_t) sysconf(_SC_PAGESIZE);
    struct rlimit rlim;
      
    callStackSize = (callStackSize + pagesize - 1) & ~(pagesize - 1);

    if (getrlimit(RLIMIT_STACK, &rlim) != 0)
      chpl_internal_error("getrlimit() failed");

    if (rlim.rlim_max != RLIM_INFINITY && callStackSize > rlim.rlim_max) {
      char warning[128];
      sprintf(warning, "callStackSize capped at %lu\n", 
              (unsigned long)rlim.rlim_max);
      chpl_warning(warning, 0, NULL);

      callStackSize = rlim.rlim_max;
    }

    rlim.rlim_cur = callStackSize;

    if (setrlimit(RLIMIT_STACK, &rlim) != 0)
      chpl_internal_error("setrlimit() failed");
  }
  taskCallStackSize = callStackSize;

  curr_taskID = next_taskID++;
  serial_state = true;  // Likely makes no difference, except for testing/debugging.

  task_pool_head = task_pool_tail = NULL;
  queued_cnt = 0;
}

void chpl_task_exit(void) { }

void chpl_task_callMain(void (*chpl_main)(void)) {
  serial_state = false;
  chpl_main();
}


void chpl_task_stdModulesInitialized(void) {
}


int chpl_task_createCommTask(chpl_fn_p fn, void* arg) {
  pthread_t thread;
  return pthread_create(&thread, NULL, (void* (*)(void*)) fn, arg);
}

void chpl_task_addToTaskList(chpl_fn_int_t fid,
                           void* arg,
                           chpl_task_list_p *task_list,
                           int32_t task_list_locale,
                           chpl_bool call_chpl_begin,
                           int lineno,
                           chpl_string filename) {
  chpl_task_begin(chpl_ftable[fid], arg, false, false, NULL);
}

void chpl_task_processTaskList(chpl_task_list_p task_list) { }

void chpl_task_executeTasksInList(chpl_task_list_p task_list) { }

void chpl_task_freeTaskList(chpl_task_list_p task_list) { }


//
// interface function with begin-statement
//
void chpl_task_begin(chpl_fn_p fp, void* a, chpl_bool ignore_serial,
                chpl_bool serial_state, chpl_task_list_p task_list_entry) {
  if (!ignore_serial && chpl_task_getSerial()) {
    //
    // save and restore current task's serial state before and after
    // invoking new task
    //
    chpl_taskID_t saved_taskID = curr_taskID;
    chpl_bool saved_serial_state = chpl_task_getSerial();
    (*fp)(a);
    chpl_task_setSerial(saved_serial_state);
    curr_taskID = saved_taskID;
  } else {
    // create a task from the given function pointer and arguments
    // and append it to the end of the task pool for later execution
    chpl_task_pool_p task;

    task = (chpl_task_pool_p)chpl_mem_alloc(sizeof(task_pool_t),
                                            CHPL_RT_MD_TASK_DESCRIPTOR,
                                            0, 0);
    task->id = next_taskID++;
    task->fun = fp;
    task->arg = a;
    task->serial_state = serial_state;
    task->next = NULL;

    if (task_pool_tail) {
      task_pool_tail->next = task;
    } else {
      task_pool_head = task;
    }
    task_pool_tail = task;

    queued_cnt++;
  }
}

chpl_taskID_t chpl_task_getId(void) { 
  return starpu_worker_get_id() + 1; 
}


void chpl_task_yield(void) {
  chpl_thread_yield();
}


void chpl_task_sleep(int secs) {
  sleep(secs);
}

chpl_bool chpl_task_getSerial(void) { return serial_state; }

void chpl_task_setSerial(chpl_bool new_state) {
  serial_state = new_state;
}

uint64_t chpl_task_getCallStackSize(void) {
  return chpl_thread_getCallStackSize();
}

uint32_t chpl_task_getNumQueuedTasks(void) { return queued_task_cnt; }

uint32_t chpl_task_getNumRunningTasks(void) {
  int numRunningTasks;

  // begin critical section
  chpl_thread_mutexLock(&threading_lock);
  chpl_thread_mutexLock(&extra_task_lock);

  numRunningTasks = running_task_cnt + extra_task_cnt;

  // end critical section
  chpl_thread_mutexUnlock(&extra_task_lock);
  chpl_thread_mutexUnlock(&threading_lock);

  return numRunningTasks;
}

int32_t  chpl_task_getNumBlockedTasks(void) { return 0; }


//
// Get a new task ID.
//

// Threads

uint32_t chpl_task_getNumThreads(void) {
  return chpl_thread_getNumThreads();
}

uint32_t chpl_task_getNumIdleThreads(void) {
  int numIdleThreads;

  // begin critical section
  chpl_thread_mutexLock(&threading_lock);

  numIdleThreads = idle_thread_cnt - waking_thread_cnt;

  // end critical section
  chpl_thread_mutexUnlock(&threading_lock);

  assert(numIdleThreads >= 0);
  return numIdleThreads;
}
