#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
#define NDEBUG
#endif

#include "chpl_mem.h"
#include "chplcast.h"
#include "chplrt.h"
#include "chpltasks.h"
#include "config.h"
#include "error.h"
#include <assert.h>
#include <stdint.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>

//
// task pool: linked list of tasks
//
typedef struct chpl_pool_struct* chpl_task_pool_p;

typedef struct chpl_pool_struct {
  chpl_fn_p fun;          // function to call for task
  void*     arg;          // argument to the function
  chpl_bool serial_state; // whether new threads can be created while executing fun
  chpl_task_pool_p next;
} task_pool_t;

static chpl_task_pool_p     task_pool_head; // head of task pool
static chpl_task_pool_p     task_pool_tail; // tail of task pool
static int                  queued_cnt;     // number of tasks in the task pool

static chpl_bool launch_next_task(void);


// Sync variables

void CHPL_SYNC_LOCK(chpl_sync_aux_t *s) { }
void CHPL_SYNC_UNLOCK(chpl_sync_aux_t *s) { }

void CHPL_SYNC_WAIT_FULL_AND_LOCK(chpl_sync_aux_t *s,
                                  int32_t lineno, chpl_string filename) {
  // while blocked, try running tasks from the task pool
  while (!*s && launch_next_task())
    /* do nothing! */;
  if (!*s)
    chpl_error("sync var empty (running in single-threaded mode)",
               lineno, filename);
}

void CHPL_SYNC_WAIT_EMPTY_AND_LOCK(chpl_sync_aux_t *s,
                                   int32_t lineno, chpl_string filename) {
  // while blocked, try running tasks from the task pool
  while (*s && launch_next_task())
    /* do nothing! */;
  if (*s)
    chpl_error("sync var full (running in single-threaded mode)",
               lineno, filename);
}

void CHPL_SYNC_MARK_AND_SIGNAL_FULL(chpl_sync_aux_t *s) {
  *s = true;
}

void CHPL_SYNC_MARK_AND_SIGNAL_EMPTY(chpl_sync_aux_t *s) {
  *s = false;
}

chpl_bool CHPL_SYNC_IS_FULL(void *val_ptr, chpl_sync_aux_t *s,
                            chpl_bool simple_sync_var) {
  return *s;
}

void CHPL_SYNC_INIT_AUX(chpl_sync_aux_t *s) {
  *s = false;
}

void CHPL_SYNC_DESTROY_AUX(chpl_sync_aux_t *s) { }


// Single variables

void CHPL_SINGLE_LOCK(chpl_single_aux_t *s) { }

void CHPL_SINGLE_UNLOCK(chpl_single_aux_t *s) { }

void CHPL_SINGLE_WAIT_FULL(chpl_single_aux_t *s,
                           int32_t lineno, chpl_string filename) {
  // while blocked, try running tasks from the task pool
  while (!*s && launch_next_task())
    /* do nothing! */;
  if (!*s)
    chpl_error("single var empty (running in single-threaded mode)",
               lineno, filename);
}

void CHPL_SINGLE_MARK_AND_SIGNAL_FULL(chpl_single_aux_t *s) {
  *s = true;
}

chpl_bool CHPL_SINGLE_IS_FULL(void *val_ptr, chpl_single_aux_t *s,
                              chpl_bool simple_single_var) {
  return *s;
}

void CHPL_SINGLE_INIT_AUX(chpl_single_aux_t *s) {
  *s = false;
}

void CHPL_SINGLE_DESTROY_AUX(chpl_single_aux_t *s) { }

// Tasks

static chpl_bool serial_state;
static uint64_t taskCallStackSize = 0;

void CHPL_TASKING_INIT(int32_t maxThreadsPerLocale, uint64_t callStackSize) {
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

  task_pool_head = task_pool_tail = NULL;
  serial_state = false;
  queued_cnt = 0;
}

void CHPL_TASKING_EXIT(void) { }

void CHPL_TASKING_CALL_MAIN(void (*chpl_main)(void)) {
  chpl_main();
}

void CHPL_ADD_TO_TASK_LIST(chpl_fn_int_t fid,
                           void* arg,
                           chpl_task_list_p *task_list,
                           int32_t task_list_locale,
                           chpl_bool call_chpl_begin,
                           int lineno,
                           chpl_string filename) {
  CHPL_BEGIN(chpl_ftable[fid], arg, false, false, NULL);
}

void CHPL_PROCESS_TASK_LIST(chpl_task_list_p task_list) { }

void CHPL_EXECUTE_TASKS_IN_LIST(chpl_task_list_p task_list) { }

void CHPL_FREE_TASK_LIST(chpl_task_list_p task_list) { }

void CHPL_BEGIN(chpl_fn_p fp, void* a, chpl_bool ignore_serial,
                chpl_bool serial_state, chpl_task_list_p task_list_entry) {
  if (!ignore_serial && CHPL_GET_SERIAL()) {
    //
    // save and restore current task's serial state before and after
    // invoking new task
    //
    chpl_bool saved_serial_state = CHPL_GET_SERIAL();
    (*fp)(a);
    CHPL_SET_SERIAL(saved_serial_state);
  } else {
    // create a task from the given function pointer and arguments
    // and append it to the end of the task pool for later execution
    chpl_task_pool_p task;

    task = (chpl_task_pool_p)chpl_alloc(sizeof(task_pool_t),
                                        CHPL_RT_MD_TASK_DESCRIPTOR,
                                        0, 0);
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

chpl_taskID_t CHPL_TASK_ID(void) { return 0; }

void CHPL_TASK_YIELD(void) {
}

void CHPL_TASK_SLEEP(int secs) {
  sleep(secs);
}

chpl_bool CHPL_GET_SERIAL(void) { return serial_state; }

void CHPL_SET_SERIAL(chpl_bool new_state) {
  serial_state = new_state;
}


uint64_t chpl_task_callstacksize(void) {
  return taskCallStackSize;
}


uint32_t CHPL_NUMQUEUEDTASKS(void) { return queued_cnt; }

uint32_t CHPL_NUMRUNNINGTASKS(void) { return 1; }

int32_t  CHPL_NUMBLOCKEDTASKS(void) { return 0; }


// Internal utility functions for task management

static chpl_bool
launch_next_task(void) {
  chpl_bool saved_serial_state;

  if (task_pool_head) {
    // retrieve the first task from the task pool
    chpl_task_pool_p task = task_pool_head;
    task_pool_head = task_pool_head->next;
    if (task_pool_head == NULL)  // task pool is now empty
      task_pool_tail = NULL;

    assert(queued_cnt > 0);
    queued_cnt--;

    //
    // reset serial state
    //
    saved_serial_state = CHPL_GET_SERIAL();
    CHPL_SET_SERIAL(task->serial_state);

    (*task->fun)(task->arg);
    chpl_free(task, 0, 0);

    //
    // restore serial state
    //
    CHPL_SET_SERIAL(saved_serial_state);

    return true;
  } else {
    return false;  // task pool was empty!
  }
}


// Threads

int32_t CHPL_THREADS_GETMAXTHREADS(void) { return 1; }

int32_t CHPL_THREADS_MAXTHREADSLIMIT(void) { return 1; }

uint32_t CHPL_NUMTHREADS(void) { return 1; }

uint32_t CHPL_NUMIDLETHREADS(void) { return 0; }
