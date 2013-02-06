#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
#define NDEBUG
#endif

#include "chplrt.h"
#include "chpl-mem.h"
#include "chplcast.h"
#include "chpl-tasks.h"
#include "chplcgfns.h" // for chpl_ftable
#include "config.h"
#include "error.h"
#include <assert.h>
#include <stdint.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

//
// task pool: linked list of tasks
//
typedef struct chpl_pool_struct* chpl_task_pool_p;

typedef struct chpl_pool_struct {
  chpl_taskID_t id;       // task identifier
  chpl_fn_p fun;          // function to call for task
  void*     arg;          // argument to the function
  chpl_bool serial_state; // whether new tasks can be created while executing fun
  c_subloc_t sublocale;  // subloc id associated with the current task.
  chpl_task_pool_p next;
} task_pool_t;

static chpl_task_pool_p     task_pool_head; // head of task pool
static chpl_task_pool_p     task_pool_tail; // tail of task pool
static int                  queued_cnt;     // number of tasks in the task pool

static chpl_bool launch_next_task(void);


// Sync variables

void chpl_sync_lock(chpl_sync_aux_t *s) { }
void chpl_sync_unlock(chpl_sync_aux_t *s) { }

void chpl_sync_waitFullAndLock(chpl_sync_aux_t *s,
                                  int32_t lineno, chpl_string filename) {
  // while blocked, try running tasks from the task pool
  while (!*s && launch_next_task())
    /* do nothing! */;
  if (!*s)
    chpl_error("sync var empty (running in CHPL_TASKS=none mode)",
               lineno, filename);
}

void chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *s,
                                   int32_t lineno, chpl_string filename) {
  // while blocked, try running tasks from the task pool
  while (*s && launch_next_task())
    /* do nothing! */;
  if (*s)
    chpl_error("sync var full (running in CHPL_TASKS=none mode)",
               lineno, filename);
}

void chpl_sync_markAndSignalFull(chpl_sync_aux_t *s) {
  *s = true;
}

void chpl_sync_markAndSignalEmpty(chpl_sync_aux_t *s) {
  *s = false;
}

chpl_bool chpl_sync_isFull(void *val_ptr, chpl_sync_aux_t *s,
                            chpl_bool simple_sync_var) {
  return *s;
}

void chpl_sync_initAux(chpl_sync_aux_t *s) {
  *s = false;
}

void chpl_sync_destroyAux(chpl_sync_aux_t *s) { }


// Tasks

static chpl_taskID_t next_taskID = chpl_nullTaskID + 1;
static chpl_taskID_t curr_taskID;
static chpl_bool serial_state;
static c_subloc_t sublocale;
static uint64_t taskCallStackSize = 0;

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

int chpl_task_createCommTask(chpl_fn_p fn, void* arg) {
  pthread_t thread;
  return pthread_create(&thread, NULL, (void* (*)(void*)) fn, arg);
}

void chpl_task_callMain(void (*chpl_main)(void)) {
  serial_state = false;
  chpl_main();
}

void chpl_task_addToTaskList(chpl_fn_int_t fid,
                           void* arg,
                           chpl_task_list_p *task_list,
                           int32_t task_list_node,
                           chpl_bool call_chpl_begin,
                           int lineno,
                           chpl_string filename) {
  chpl_task_begin(chpl_ftable[fid], arg, false, false, NULL);
}

void chpl_task_processTaskList(chpl_task_list_p task_list) { }

void chpl_task_executeTasksInList(chpl_task_list_p task_list) { }

void chpl_task_freeTaskList(chpl_task_list_p task_list) { }

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

chpl_taskID_t chpl_task_getId(void) { return curr_taskID; }

void chpl_task_yield(void) {
}

void chpl_task_sleep(int secs) {
  sleep(secs);
}

chpl_bool chpl_task_getSerial(void) { return serial_state; }

void chpl_task_setSerial(chpl_bool new_state) {
  serial_state = new_state;
}

c_subloc_t chpl_task_getSubLoc(void) { return sublocale; }

void chpl_task_setSubLoc(c_subloc_t new_subloc)
{ sublocale = new_subloc; }

uint64_t chpl_task_getCallStackSize(void) {
  return taskCallStackSize;
}


uint32_t chpl_task_getNumQueuedTasks(void) { return queued_cnt; }

uint32_t chpl_task_getNumRunningTasks(void) { return 1; }

int32_t  chpl_task_getNumBlockedTasks(void) { return 0; }


// Internal utility functions for task management

static chpl_bool
launch_next_task(void) {
  chpl_taskID_t saved_taskID;
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
    // set state to reflect new state
    //
    saved_taskID = curr_taskID;
    saved_serial_state = chpl_task_getSerial();
    chpl_task_setSerial(task->serial_state);

    (*task->fun)(task->arg);
    chpl_mem_free(task, 0, 0);

    //
    // restore state
    //
    chpl_task_setSerial(saved_serial_state);
    curr_taskID = saved_taskID;

    return true;
  } else {
    return false;  // task pool was empty!
  }
}


// Threads

uint32_t chpl_task_getNumThreads(void) { return 1; }

uint32_t chpl_task_getNumIdleThreads(void) { return 0; }
