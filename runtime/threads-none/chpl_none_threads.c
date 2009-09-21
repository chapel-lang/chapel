#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
#define NDEBUG
#endif

#include "chplrt.h"
#include "chplthreads.h"
#include "chpl_mem.h"
#include "error.h"
#include <assert.h>
#include <stdint.h>

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


// Mutex
void chpl_mutex_init(chpl_mutex_p mutex) { }
void chpl_mutex_lock(chpl_mutex_p mutex) { }
void chpl_mutex_unlock(chpl_mutex_p mutex) { }


// Sync variables

void chpl_sync_lock(chpl_sync_aux_t *s) { }
void chpl_sync_unlock(chpl_sync_aux_t *s) { }

void chpl_sync_wait_full_and_lock(chpl_sync_aux_t *s, int32_t lineno, chpl_string filename) {
  // while blocked, try running tasks from the task pool
  while (!*s && launch_next_task())
    /* do nothing! */;
  if (!*s)
    chpl_error("sync var empty (running in single-threaded mode)", lineno, filename);
}

void chpl_sync_wait_empty_and_lock(chpl_sync_aux_t *s, int32_t lineno, chpl_string filename) {
  // while blocked, try running tasks from the task pool
  while (*s && launch_next_task())
    /* do nothing! */;
  if (*s)
    chpl_error("sync var full (running in single-threaded mode)", lineno, filename);
}

void chpl_sync_mark_and_signal_full(chpl_sync_aux_t *s) {
  *s = true;
}

void chpl_sync_mark_and_signal_empty(chpl_sync_aux_t *s) {
  *s = false;
}

chpl_bool chpl_sync_is_full(void *val_ptr, chpl_sync_aux_t *s, chpl_bool simple_sync_var) {
  return *s;
}

void chpl_init_sync_aux(chpl_sync_aux_t *s) {
  *s = false;
}

void chpl_destroy_sync_aux(chpl_sync_aux_t *s) { }


// Single variables

void chpl_single_lock(chpl_single_aux_t *s) { }

void chpl_single_unlock(chpl_single_aux_t *s) { }

void chpl_single_wait_full(chpl_single_aux_t *s, int32_t lineno, chpl_string filename) {
  // while blocked, try running tasks from the task pool
  while (!*s && launch_next_task())
    /* do nothing! */;
  if (!*s)
    chpl_error("single var empty (running in single-threaded mode)", lineno, filename);
}

void chpl_single_mark_and_signal_full(chpl_single_aux_t *s) {
  *s = true;
}

chpl_bool chpl_single_is_full(void *val_ptr, chpl_single_aux_t *s, chpl_bool simple_single_var) {
  return *s;
}

void chpl_init_single_aux(chpl_single_aux_t *s) {
  *s = false;
}

void chpl_destroy_single_aux(chpl_single_aux_t *s) { }

// Tasks

static chpl_bool serial_state;

void chpl_tasking_init() {
  task_pool_head = task_pool_tail = NULL;
  serial_state = false;
  queued_cnt = 0;
}

void chpl_tasking_exit() { }

void chpl_add_to_task_list(chpl_fn_int_t fid,
                           void* arg,
                           chpl_task_list_p *task_list,
                           int32_t task_list_locale,
                           chpl_bool call_chpl_begin,
                           int lineno,
                           chpl_string filename) {
  chpl_begin(chpl_ftable[fid], arg, false, false, NULL);
}

void chpl_process_task_list (chpl_task_list_p task_list) { }

void chpl_execute_tasks_in_list (chpl_task_list_p task_list) { }

void chpl_free_task_list (chpl_task_list_p task_list) { }

void chpl_begin(chpl_fn_p fp, void* a, chpl_bool ignore_serial,
                chpl_bool serial_state, chpl_task_list_p task_list_entry) {
  if (!ignore_serial && chpl_get_serial()) {
    (*fp)(a);
  } else {
    // create a task from the given function pointer and arguments
    // and append it to the end of the task pool for later execution
    chpl_task_pool_p task = (chpl_task_pool_p)chpl_alloc(sizeof(task_pool_t),
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

chpl_bool chpl_get_serial(void) { return serial_state; }

void chpl_set_serial(chpl_bool new_state) {
  serial_state = new_state;
}

uint32_t chpl_numQueuedTasks(void) { return queued_cnt; }

uint32_t chpl_numRunningTasks(void) { return 1; }

int32_t  chpl_numBlockedTasks(void) { return 0; }


// Internal utility functions for task management

static chpl_bool
launch_next_task(void) {
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
    chpl_set_serial(task->serial_state);

    (*task->fun)(task->arg);
    chpl_free(task, 0, 0);

    return true;
  } else {
    return false;  // task pool was empty!
  }
}


// Threads

int32_t chpl_threads_getMaxThreads(void) { return 1; }

int32_t chpl_threads_maxThreadsLimit(void) { return 1; }

uint32_t chpl_numThreads(void) { return 1; }

uint32_t chpl_numIdleThreads(void) { return 0; }

chpl_threadID_t chpl_thread_id(void) { return 0; }

void chpl_thread_cancel(chpl_threadID_t threadID) {
  chpl_internal_error("chpl_thread_cancel() shouldn't be called in threads-none");
}

void chpl_thread_join(chpl_threadID_t threadID) {
  chpl_internal_error("chpl_thread_join() shouldn't be called in threads-none");
}
