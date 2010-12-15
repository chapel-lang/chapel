//
// FIFO implementation of Chapel tasking interface
//

#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
#define NDEBUG
#endif

#include "chpl_rt_utils_static.h"
#include "chplcomm.h"
#include "chplexit.h"
#include "chpl_mem.h"
#include "chplrt.h"
#include "chpltasks.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>


//
// task pool: linked list of tasks
//
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


// This constitutes one entry on our list of threads.
typedef struct thread_list* thread_list_p;
struct thread_list {
  threadlayer_threadID_t thread;
  thread_list_p          next;
};


typedef struct lockReport {
  const char*        filename;
  int                lineno;
  uint64_t           prev_progress_cnt;
  chpl_bool          maybeLocked;
  struct lockReport* next;
} lockReport_t;


// This is the data that is private to each thread.
typedef struct {
  task_pool_p   ptask;
  lockReport_t* lockRprt;
} thread_private_data_t;


static chpl_bool        initialized = false;

static threadlayer_mutex_t threading_lock;     // critical section lock
static threadlayer_mutex_t extra_task_lock;    // critical section lock
static threadlayer_mutex_t task_id_lock;       // critical section lock
static threadlayer_mutex_t task_list_lock;     // critical section lock
static task_pool_p         task_pool_head;     // head of task pool
static task_pool_p         task_pool_tail;     // tail of task pool
static threadlayer_mutex_t thread_list_lock;   // critical section lock
static thread_list_p       thread_list_head;   // head of thread_list
static thread_list_p       thread_list_tail;   // tail of thread_list
static int                 queued_cnt;         // number of tasks in the task pool
static int                 waking_cnt;         // number of threads signaled to wakeup
static int                 running_cnt;        // number of running threads 
static int                 threads_cnt;        // number of threads (total)
static int                 blocked_thread_cnt; // number of threads waiting for something
static int                 idle_cnt;           // number of threads that are idle
static int64_t             extra_task_cnt;     // number of threads executing more than one task
static uint64_t            progress_cnt;       // counts unblock operations, as a
                                               //   proxy for progress
static threadlayer_mutex_t block_report_lock;  // critical section lock
static threadlayer_mutex_t taskTable_lock;     // critical section lock

static lockReport_t* lockReportHead = NULL;
static lockReport_t* lockReportTail = NULL;

static chpl_string idleTaskName = "|idle|";

static chpl_taskID_t           get_next_task_id(void);
static thread_private_data_t*  get_thread_private_data(void);
static task_pool_p             get_current_ptask(void);
static void                    set_current_ptask(task_pool_p);
static void                    add_me_to_thread_list(void);
static void                    report_locked_threads(void);
static void                    report_all_tasks(void);
static void                    SIGINT_handler(int sig);
static void                    initializeLockReportForThread(void);
static chpl_bool               set_block_loc(int, chpl_string);
static void                    unset_block_loc(void);
static void                    check_for_deadlock(void);
static void*                   chpl_begin_helper(void*);
static void                    launch_next_task_in_new_thread(void);
static void                    schedule_next_task(int);
static task_pool_p             add_to_task_pool(chpl_fn_p,
                                                void*,
                                                chpl_bool,
                                                chpl_task_list_p);
static int taskMaxThreadsPerLocale = -1;


// Sync variables

static void sync_wait_and_lock(chpl_sync_aux_t *s,
                               chpl_bool want_full,
                               int32_t lineno, chpl_string filename) {
  threadlayer_mutex_lock(&s->lock);
  while (s->is_full != want_full) {
    if (set_block_loc(lineno, filename)) {
      // all other tasks appear to be blocked
      struct timeval deadline, now;
      chpl_bool timed_out;
      gettimeofday(&deadline, NULL);
      deadline.tv_sec += 1;
      do {
        timed_out = threadlayer_sync_suspend(s, &deadline);
        if (s->is_full != want_full && !timed_out)
            gettimeofday(&now, NULL);
      } while (s->is_full != want_full
               && !timed_out
               && (now.tv_sec < deadline.tv_sec
                   || (now.tv_sec == deadline.tv_sec
                       && now.tv_usec < deadline.tv_usec)));
      if (s->is_full != want_full)
        check_for_deadlock();
    }
    else {
      do {
        (void) threadlayer_sync_suspend(s, NULL);
      } while (s->is_full != want_full);
    }
    unset_block_loc();
  }

  if (blockreport)
    progress_cnt++;
}

void CHPL_SYNC_LOCK(chpl_sync_aux_t *s) {
  threadlayer_mutex_lock(&s->lock);
}

void CHPL_SYNC_UNLOCK(chpl_sync_aux_t *s) {
  threadlayer_mutex_unlock(&s->lock);
}

void CHPL_SYNC_WAIT_FULL_AND_LOCK(chpl_sync_aux_t *s,
                                  int32_t lineno, chpl_string filename) {
  sync_wait_and_lock(s, true, lineno, filename);
}

void CHPL_SYNC_WAIT_EMPTY_AND_LOCK(chpl_sync_aux_t *s,
                                   int32_t lineno, chpl_string filename) {
  sync_wait_and_lock(s, false, lineno, filename);
}

void CHPL_SYNC_MARK_AND_SIGNAL_FULL(chpl_sync_aux_t *s) {
  s->is_full = true;
  threadlayer_sync_awaken(s);
  CHPL_SYNC_UNLOCK(s);
}

void CHPL_SYNC_MARK_AND_SIGNAL_EMPTY(chpl_sync_aux_t *s) {
  s->is_full = false;
  threadlayer_sync_awaken(s);
  CHPL_SYNC_UNLOCK(s);
}

chpl_bool CHPL_SYNC_IS_FULL(void *val_ptr,
                            chpl_sync_aux_t *s,
                            chpl_bool simple_sync_var) {
  return s->is_full;
}

void CHPL_SYNC_INIT_AUX(chpl_sync_aux_t *s) {
  s->is_full = false;
  threadlayer_mutex_init(&s->lock);
  threadlayer_sync_init(s);
}

void CHPL_SYNC_DESTROY_AUX(chpl_sync_aux_t *s) {
  threadlayer_sync_destroy(s);
}


// Single variables

void CHPL_SINGLE_LOCK(chpl_single_aux_t *s) {
  threadlayer_mutex_lock(&s->lock);
}

void CHPL_SINGLE_UNLOCK(chpl_single_aux_t *s) {
  threadlayer_mutex_unlock(&s->lock);
}

void CHPL_SINGLE_WAIT_FULL(chpl_single_aux_t *s,
                           int32_t lineno, chpl_string filename) {
  threadlayer_mutex_lock(&s->lock);
  while (!s->is_full) {
    if (set_block_loc(lineno, filename)) {
      // all other tasks appear to be blocked
      struct timeval deadline, now;
      chpl_bool timed_out;
      gettimeofday(&deadline, NULL);
      deadline.tv_sec += 1;
      do {
        timed_out = threadlayer_single_suspend(s, &deadline);
        if (!s->is_full && !timed_out)
            gettimeofday(&now, NULL);
      } while (!s->is_full
               && !timed_out
               && (now.tv_sec < deadline.tv_sec
                   || (now.tv_sec == deadline.tv_sec
                       && now.tv_usec < deadline.tv_usec)));
      if (!s->is_full)
        check_for_deadlock();
    }
    else {
      do {
        (void) threadlayer_single_suspend(s, NULL);
      } while (!s->is_full);
    }
    unset_block_loc();
  }

  if (blockreport)
    progress_cnt++;
}

void CHPL_SINGLE_MARK_AND_SIGNAL_FULL(chpl_single_aux_t *s) {
  s->is_full = true;
  threadlayer_single_awaken(s);
  CHPL_SINGLE_UNLOCK(s);
}

chpl_bool CHPL_SINGLE_IS_FULL(void *val_ptr,
                              chpl_single_aux_t *s,
                              chpl_bool simple_single_var) {
  return s->is_full;
}

void CHPL_SINGLE_INIT_AUX(chpl_single_aux_t *s) {
  s->is_full = false;
  threadlayer_mutex_init(&s->lock);
  threadlayer_single_init(s);
}

void CHPL_SINGLE_DESTROY_AUX(chpl_single_aux_t *s) {
  threadlayer_single_destroy(s);
}


// Tasks

void CHPL_TASKING_INIT(int32_t maxThreadsPerLocale, uint64_t callStackSize) {
  thread_private_data_t *tp;

  // Tuck maxThreadsPerLocale away in a static global for use by other routines
  taskMaxThreadsPerLocale = maxThreadsPerLocale;

  threadlayer_mutex_init(&threading_lock);
  threadlayer_mutex_init(&extra_task_lock);
  threadlayer_mutex_init(&task_id_lock);
  threadlayer_mutex_init(&task_list_lock);
  threadlayer_mutex_init(&thread_list_lock);
  queued_cnt = 0;
  running_cnt = 0;                     // only main thread running
  waking_cnt = 0;
  threads_cnt = 0;
  blocked_thread_cnt = 0;
  idle_cnt = 0;
  extra_task_cnt = 0;
  task_pool_head = task_pool_tail = NULL;
  thread_list_head = thread_list_tail = NULL;

  threadlayer_init(callStackSize);


  tp = (thread_private_data_t*) chpl_alloc(sizeof(thread_private_data_t),
                                           CHPL_RT_MD_THREAD_PRIVATE_DATA,
                                           0, 0);

  tp->ptask = (task_pool_p) chpl_alloc(sizeof(task_pool_t),
                                       CHPL_RT_MD_TASK_POOL_DESCRIPTOR,
                                       0, 0);
  tp->ptask->id           = get_next_task_id();
  tp->ptask->fun          = NULL;
  tp->ptask->arg          = NULL;
  tp->ptask->serial_state = false;
  tp->ptask->ltask        = NULL;
  tp->ptask->begun        = true;
  tp->ptask->filename     = "main program";
  tp->ptask->lineno       = 0;
  tp->ptask->next         = NULL;

  threadlayer_set_thread_private_data(tp);

  if (taskreport) {
    threadlayer_mutex_init(&taskTable_lock);
    chpldev_taskTable_add(tp->ptask->id,
                          tp->ptask->lineno, tp->ptask->filename,
                          (uint64_t) (intptr_t) tp->ptask);
    chpldev_taskTable_set_active(tp->ptask->id);
  }

  if (blockreport) {
    progress_cnt = 0;
    threadlayer_mutex_init(&block_report_lock);
    initializeLockReportForThread();
  }

  if (blockreport || taskreport) {
    signal(SIGINT, SIGINT_handler);
  }

  initialized = true;
}


void CHPL_TASKING_EXIT(void) {
  chpl_bool debug = false;
  thread_list_p tlp;

  if (!initialized)
    return;

  if (debug)
    fprintf(stderr, "A total of %d threads were created; waking_cnt = %d\n", threads_cnt, waking_cnt);

  // shut down all threads
  for (tlp = thread_list_head; tlp != NULL; tlp = tlp->next)
    threadlayer_thread_cancel(tlp->thread);
  while (thread_list_head != NULL) {
    threadlayer_thread_join(thread_list_head->thread);
    tlp = thread_list_head;
    thread_list_head = thread_list_head->next;
    chpl_free(tlp, 0, 0);
  }

  threadlayer_exit();
}


void CHPL_PER_PTHREAD_TASKING_INIT(void) {
  return;
}


void CHPL_ADD_TO_TASK_LIST(chpl_fn_int_t fid, void* arg,
                           chpl_task_list_p *task_list,
                           int32_t task_list_locale,
                           chpl_bool call_chpl_begin,
                           int lineno,
                           chpl_string filename) {
  if (task_list_locale == chpl_localeID) {
    chpl_task_list_p ltask;

    ltask = (chpl_task_list_p) chpl_alloc(sizeof(struct chpl_task_list),
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
      threadlayer_mutex_lock(&task_list_lock);

    if (*task_list) {
      ltask->next = (*task_list)->next;
      (*task_list)->next = ltask;
    }
    else
      ltask->next = ltask;
    *task_list = ltask;

    // end critical section - not needed for cobegin or coforall statements
    if (call_chpl_begin)
      threadlayer_mutex_unlock(&task_list_lock);
  }
  else {
    // call_chpl_begin should be true here because if task_list_locale !=
    // chpl_localeID, then this function could not have been called from
    // the context of a cobegin or coforall statement, which are the only
    // contexts in which CHPL_BEGIN() should not be called.
    chpl_fn_p fp = chpl_ftable[fid];
    assert(call_chpl_begin);
    CHPL_BEGIN(fp, arg, false, false, NULL);
  }
}


void CHPL_PROCESS_TASK_LIST(chpl_task_list_p task_list) {
  // task_list points to the last entry on the list; task_list->next is
  // actually the first element on the list.
  chpl_task_list_p ltask = task_list, next_task;
  task_pool_p curr_ptask;
  task_pool_t nested_task;

  // This function is not expected to be called if a cobegin contains fewer
  // than two statements; a coforall, however, may generate just one task,
  // or even none at all.
  if (ltask == NULL)
    return;
  assert(ltask->next);
  next_task = ltask->next;  // next_task now points to the head of the list

  curr_ptask = get_current_ptask();

  if (curr_ptask->serial_state) {
    do {
      ltask = next_task;
      (*ltask->fun)(ltask->arg);
      next_task = ltask->next;
    } while (ltask != task_list);
  } else {
    int task_cnt = 0;
    chpl_task_list_p first_task = next_task;
    next_task = next_task->next;

    if (first_task != task_list) {
      // there are at least two tasks in task_list

      // begin critical section
      threadlayer_mutex_lock(&threading_lock);

      do {
        ltask = next_task;
        ltask->ptask = add_to_task_pool(ltask->fun, ltask->arg,
                                        curr_ptask->serial_state, ltask);
        assert(ltask->ptask == NULL
               || ltask->ptask->ltask == ltask);
        next_task = ltask->next;
        task_cnt++;
      } while (ltask != task_list);

      schedule_next_task(task_cnt);

      // end critical section
      threadlayer_mutex_unlock(&threading_lock);
    }

    // Execute the first task on the list, since it has to run to completion
    // before continuing beyond the cobegin or coforall it's in.
    nested_task.id           = get_next_task_id();
    nested_task.fun          = first_task->fun;
    nested_task.arg          = first_task->arg;
    nested_task.serial_state = false;
    nested_task.ltask        = first_task;
    nested_task.begun        = true;
    nested_task.filename     = first_task->filename;
    nested_task.lineno       = first_task->lineno;
    set_current_ptask(&nested_task);

    if (taskreport) {
      threadlayer_mutex_lock(&taskTable_lock);
      chpldev_taskTable_add(nested_task.id,
                            nested_task.lineno, nested_task.filename,
                            (uint64_t) (intptr_t) &nested_task);
      chpldev_taskTable_set_suspended(curr_ptask->id);
      chpldev_taskTable_set_active(nested_task.id);
      threadlayer_mutex_unlock(&taskTable_lock);
    }

    // begin critical section
    threadlayer_mutex_lock(&extra_task_lock);

    extra_task_cnt++;

    // end critical section
    threadlayer_mutex_unlock(&extra_task_lock);

    if (blockreport)
      initializeLockReportForThread();

    (*first_task->fun)(first_task->arg);

    // begin critical section
    threadlayer_mutex_lock(&extra_task_lock);

    extra_task_cnt--;

    // end critical section
    threadlayer_mutex_unlock(&extra_task_lock);

    if (taskreport) {
      threadlayer_mutex_lock(&taskTable_lock);
      chpldev_taskTable_set_active(curr_ptask->id);
      chpldev_taskTable_remove(nested_task.id);
      threadlayer_mutex_unlock(&taskTable_lock);
    }

    set_current_ptask(curr_ptask);
  }
}


void CHPL_EXECUTE_TASKS_IN_LIST(chpl_task_list_p task_list) {
  // task_list points to the last entry on the list; task_list->next is
  // actually the first element on the list.
  chpl_task_list_p ltask = task_list, next_task;
  // This function is not expected to be called if a cobegin contains fewer
  // than two statements; a coforall, however, may generate just one task,
  // or even none at all.
  if (ltask == NULL)
    return;
  assert(ltask->next);
  next_task = ltask->next;  // next_task now points to the head of the list

  // If the serial state is true, the tasks in task_list have already been
  // executed.
  if (!CHPL_GET_SERIAL()) do {
    ltask = next_task;
    next_task = ltask->next;

    // don't lock unless it looks like we will find a task to execute
    // if we do so
    if (ltask->ptask) {
      task_pool_p  curr_ptask;
      task_pool_p  nested_ptask = NULL;
      chpl_fn_p    task_to_run_fun = NULL;
      void*        task_to_run_arg = NULL;

      // begin critical section
      threadlayer_mutex_lock(&threading_lock);

      if (ltask->ptask) {
        assert(!ltask->ptask->begun);
        task_to_run_fun = ltask->ptask->fun;
        task_to_run_arg = ltask->ptask->arg;
        ltask->ptask->begun = true;
        ltask->ptask->ltask = NULL;
        // there is no longer any need to access the corresponding task
        // pool entry so avoid any potential of accessing a node that
        // will eventually be freed
        nested_ptask = ltask->ptask;
        ltask->ptask = NULL;
        if (waking_cnt > 0)
          waking_cnt--;
        assert(queued_cnt > 0);
        queued_cnt--;
        if (nested_ptask->prev == NULL) {
          if ((task_pool_head = nested_ptask->next) == NULL)
            task_pool_tail = NULL;
          else
            task_pool_head->prev = NULL;
        }
        else {
          nested_ptask->prev->next = nested_ptask->next;
          if (nested_ptask->next == NULL)
            task_pool_tail = nested_ptask->prev;
          else
            nested_ptask->next->prev = nested_ptask->prev;
        }
      }

      // end critical section
      threadlayer_mutex_unlock(&threading_lock);

      if (task_to_run_fun) {
        curr_ptask = get_current_ptask();
        set_current_ptask(nested_ptask);

        // begin critical section
        threadlayer_mutex_lock(&extra_task_lock);

        extra_task_cnt++;

        // end critical section
        threadlayer_mutex_unlock(&extra_task_lock);

        if (taskreport) {
          threadlayer_mutex_lock(&taskTable_lock);
          chpldev_taskTable_set_suspended(curr_ptask->id);
          chpldev_taskTable_set_active(nested_ptask->id);
          threadlayer_mutex_unlock(&taskTable_lock);
        }

        if (blockreport)
          initializeLockReportForThread();

        (*task_to_run_fun)(task_to_run_arg);

        if (taskreport) {
          threadlayer_mutex_lock(&taskTable_lock);
          chpldev_taskTable_set_active(curr_ptask->id);
          chpldev_taskTable_remove(nested_ptask->id);
          threadlayer_mutex_unlock(&taskTable_lock);
        }

        // begin critical section
        threadlayer_mutex_lock(&extra_task_lock);

        extra_task_cnt--;

        // end critical section
        threadlayer_mutex_unlock(&extra_task_lock);

        set_current_ptask(curr_ptask);
        chpl_free(nested_ptask, 0, 0);
      }
    }

  } while (ltask != task_list);
}


void CHPL_FREE_TASK_LIST(chpl_task_list_p task_list) {
  // task_list points to the last entry on the list; task_list->next is
  // actually the first element on the list.
  chpl_task_list_p ltask = task_list, next_task;
  // This function is not expected to be called if a cobegin contains fewer
  // than two statements; a coforall, however, may generate just one task,
  // or even none at all.
  if (ltask == NULL)
    return;
  assert(ltask->next);
  next_task = ltask->next;  // next_task now points to the head of the list

  do {
    ltask = next_task;
    next_task = ltask->next;
    chpl_free(ltask, 0, 0);
  } while (ltask != task_list);
}


//
// interface function with begin-statement
//
void CHPL_BEGIN(chpl_fn_p fp, void* a,
                chpl_bool ignore_serial,  // always add task to pool
                chpl_bool serial_state,
                chpl_task_list_p ltask) {
  if (!ignore_serial && CHPL_GET_SERIAL()) {
    (*fp)(a);
  } else {
    task_pool_p ptask = NULL;

    // begin critical section
    threadlayer_mutex_lock(&threading_lock);

    ptask = add_to_task_pool(fp, a, serial_state, ltask);
    // this task may begin executing before returning from this function,
    // so the task list node needs to be updated before there is any
    // possibility of launching this task
    if (ltask)
      ltask->ptask = ptask;

    schedule_next_task(1);

    assert(ptask->ltask == NULL
           || (ptask->ltask == ltask
               && ltask->ptask == ptask));

    // end critical section
    threadlayer_mutex_unlock(&threading_lock);
  }
}


chpl_taskID_t CHPL_TASK_ID(void) {
  return get_current_ptask()->id;
}


void CHPL_TASK_YIELD(void) {
  threadlayer_yield();
}


void CHPL_TASK_SLEEP(int secs) {
  sleep(secs);
}


chpl_bool CHPL_GET_SERIAL(void) {
  return get_thread_private_data()->ptask->serial_state;
}

void CHPL_SET_SERIAL(chpl_bool state) {
  get_thread_private_data()->ptask->serial_state = state;
}

uint64_t chpl_task_callstacksize(void) {
  return threadlayer_call_stack_size();
}

uint64_t CHPL_TASK_CALLSTACKSIZELIMIT(void) {
  return threadlayer_call_stack_size_limit();
}


uint32_t CHPL_NUMQUEUEDTASKS(void) { return queued_cnt; }

uint32_t CHPL_NUMRUNNINGTASKS(void) {
  int numRunningTasks;

  // begin critical section
  threadlayer_mutex_lock(&threading_lock);
  threadlayer_mutex_lock(&extra_task_lock);

  // take the main thread into account
  numRunningTasks = running_cnt + extra_task_cnt + 1;

  // end critical section
  threadlayer_mutex_unlock(&extra_task_lock);
  threadlayer_mutex_unlock(&threading_lock);

  return numRunningTasks;
}

int32_t  CHPL_NUMBLOCKEDTASKS(void) {
  if (blockreport) {
    int numBlockedTasks;

    // begin critical section
    threadlayer_mutex_lock(&threading_lock);
    threadlayer_mutex_lock(&block_report_lock);

    numBlockedTasks = blocked_thread_cnt - idle_cnt;

    // end critical section
    threadlayer_mutex_unlock(&block_report_lock);
    threadlayer_mutex_unlock(&threading_lock);

    assert(numBlockedTasks >= 0);
    return numBlockedTasks;
  }
  else
    return -1;
}


// Internal utility functions for task management

//
// Get a new task ID.
//
static chpl_taskID_t get_next_task_id(void) {
  static chpl_taskID_t       id = 0;

  chpl_taskID_t              next_id;

  threadlayer_mutex_lock(&task_id_lock);
  next_id = id++;
  threadlayer_mutex_unlock(&task_id_lock);

  return next_id;
}


//
// Get the the thread private data pointer for my thread.
//
static thread_private_data_t* get_thread_private_data(void) {
  thread_private_data_t* tp;

  tp = (thread_private_data_t*) threadlayer_get_thread_private_data();
  if (tp == NULL)
    chpl_internal_error("no thread private data");

  return tp;
}


//
// Set the descriptor for the task now running on my thread.
//
static task_pool_p get_current_ptask(void) {
  return get_thread_private_data()->ptask;
}


//
// Set the descriptor for the task now running on my thread.
//
static void set_current_ptask(task_pool_p ptask) {
  get_thread_private_data()->ptask = ptask;
}


//
// Add the current thread (which is new) to our list of active threads.
//
static void add_me_to_thread_list(void) {
    thread_list_p tlp;

    tlp = (thread_list_p) chpl_alloc(sizeof(struct thread_list),
                                     CHPL_RT_MD_THREAD_LIST_DESCRIPTOR, 0, 0);

    tlp->thread = threadlayer_thread_id();
    tlp->next   = NULL;

    threadlayer_mutex_lock(&thread_list_lock);
    if (thread_list_head == NULL)
      thread_list_head = tlp;
    else
      thread_list_tail->next = tlp;
    thread_list_tail = tlp;
    threadlayer_mutex_unlock(&thread_list_lock);
}


//
// Walk over the linked list of thread states and print the ones that
// are blocked/waiting.  This is used by both the deadlock reporting
// and the ^C signal handler.
//
static void report_locked_threads(void) {
  lockReport_t* rep;

  fflush(stdout);

  rep = lockReportHead;
  while (rep != NULL) {
    if (rep->maybeLocked) {
      if (rep->lineno > 0 && rep->filename)
        fprintf(stderr, "Waiting at: %s:%d\n", rep->filename, rep->lineno);
      else if (rep->lineno == 0 && !strcmp(rep->filename, idleTaskName))
        fprintf(stderr, "Waiting for more work\n");
    }
    rep = rep->next;
  }

  fflush(stdout);
}


//
// This signal handler prints an overall task report, containing
// pending tasks and those that are running.
//
static void report_all_tasks(void) {
    task_pool_p pendingTask = task_pool_head;

    printf("Task report\n");
    printf("--------------------------------\n");

    // print out pending tasks
    printf("Pending tasks:\n");
    while(pendingTask != NULL) {
        if(! pendingTask->begun) {
            printf("- %s:%d\n", pendingTask->filename,
                   (int)pendingTask->lineno);
        }
        pendingTask = pendingTask->next;
    }
    printf("\n");
    
    // print out running tasks
    printf("Known tasks:\n");
    chpldev_taskTable_print();
}


//
// This is a signal handler that does thread and task reporting.
//
static void SIGINT_handler(int sig) {
  signal(sig, SIG_IGN);

  if (blockreport)
    report_locked_threads();

  if (taskreport)
    report_all_tasks();

  chpl_exit_any(1);
}


//
// This function should be called exactly once per thread (not task!),
// including the main thread. It should be called before the first task
// this thread was created to do is started.
//
// Our handling of lock report list entries could be improved.  We
// allocate one each time this function is called, and this is called
// just before each task wrapper is called.  We never remove these
// from the list or deallocate them.  If we do traverse the list while
// reporting a deadlock, we just skip the leaked ones, because they
// don't say "blocked".
//
static void initializeLockReportForThread(void) {
  lockReport_t* newLockReport;

  newLockReport = (lockReport_t*) chpl_alloc(sizeof(lockReport_t),
                                             CHPL_RT_MD_LOCK_REPORT_DATA,
                                             0, 0);
  newLockReport->maybeLocked = false;
  newLockReport->next = NULL;

  get_thread_private_data()->lockRprt = newLockReport;

  // Begin critical section
  threadlayer_mutex_lock(&block_report_lock);
  if (lockReportHead) {
    lockReportTail->next = newLockReport;
    lockReportTail = newLockReport;
  } else {
    lockReportHead = newLockReport;
    lockReportTail = newLockReport;
  }
  // End critical section
  threadlayer_mutex_unlock(&block_report_lock);
}



// Deadlock detection

//
// Inform task management that the thread (task) is about to suspend
// waiting for a sync or single variable to change state or the task
// pool to become nonempty.  The return value is true if the program
// may be deadlocked, indicating that the thread should use a timeout
// deadline on its suspension if possible, and false otherwise.
//
static chpl_bool set_block_loc(int lineno, chpl_string filename) {
  thread_private_data_t* tp;
  chpl_bool isLastUnblockedThread;

  if (!blockreport)
    return false;

  isLastUnblockedThread = false;

  tp = get_thread_private_data();
  tp->lockRprt->filename = filename;
  tp->lockRprt->lineno = lineno;
  tp->lockRprt->prev_progress_cnt = progress_cnt;
  tp->lockRprt->maybeLocked = true;

  // Begin critical section
  threadlayer_mutex_lock(&block_report_lock);

  blocked_thread_cnt++;
  if (blocked_thread_cnt > threads_cnt) {
    isLastUnblockedThread = true;
  }

  // End critical section
  threadlayer_mutex_unlock(&block_report_lock);

  return isLastUnblockedThread;
}


//
// Inform task management that the thread (task) is no longer suspended.
//
static void unset_block_loc(void) {
  if (!blockreport)
    return;

  get_thread_private_data()->lockRprt->maybeLocked = false;

  // Begin critical section
  threadlayer_mutex_lock(&block_report_lock);

  blocked_thread_cnt--;

  // End critical section
  threadlayer_mutex_unlock(&block_report_lock);
}


//
// Check for and report deadlock, when a suspension deadline passes.
//
static void check_for_deadlock(void) {
  // Blockreport should be true here, because this can't be called
  // unless set_block_loc() returns true, and it can't do that unless
  // blockreport is true.  So this is just a check for ongoing
  // internal consistency.
  assert(blockreport);

  if (get_thread_private_data()->lockRprt->prev_progress_cnt < progress_cnt)
    return;

  fflush(stdout);
  fprintf(stderr, "Program is deadlocked!\n");

  report_locked_threads();

  if (taskreport)
    report_all_tasks();

  chpl_exit_any(1);
}


//
// When we create a thread it runs this wrapper function, which just
// executes tasks out of the pool as they become available.
//
static void*
chpl_begin_helper(void* ptask_void) {
  task_pool_p ptask = (task_pool_p) ptask_void;
  thread_private_data_t *tp;

  tp = (thread_private_data_t*) chpl_alloc(sizeof(thread_private_data_t),
                                           CHPL_RT_MD_THREAD_PRIVATE_DATA,
                                           0, 0);
  tp->ptask = ptask;
  threadlayer_set_thread_private_data(tp);

  // add new thread to our thread list
  add_me_to_thread_list();

  // the chpldev_taskTable_set_active() call can end up temporarily
  // waiting on a sync var, so if deadlock detection is enabled we
  // have to set that up first
  if (blockreport)
    initializeLockReportForThread();

  while (true) {
    if (taskreport) {
      threadlayer_mutex_lock(&taskTable_lock);
      chpldev_taskTable_set_active(ptask->id);
      threadlayer_mutex_unlock(&taskTable_lock);
    }

    (*ptask->fun)(ptask->arg);

    if (taskreport) {
      threadlayer_mutex_lock(&taskTable_lock);
      chpldev_taskTable_remove(ptask->id);
      threadlayer_mutex_unlock(&taskTable_lock);
    }

    // begin critical section
    threadlayer_mutex_lock(&threading_lock);

    //
    // We have to wait to free the ptask until we hold the lock, in
    // order to make sure launch_next_task_in_new_thread() is done
    // manipulating the ptask before anyone else could re-allocate it.
    // We could do the free before grabbing the lock if we arranged for
    // launch_next_task_in_new_thread() to do the pool manipulations
    // before calling threadlayer_thread_create(), but then we would
    // also have to be prepared to undo all those manipulations if we
    // were unable to create a thread.
    //
    tp->ptask = NULL;
    chpl_free(ptask, 0, 0);

    //
    // finished task; decrement running count and increment idle count
    //
    assert(running_cnt > 0);
    running_cnt--;
    idle_cnt++;

    //
    // wait for a not-yet-begun task to be present in the task pool
    //
    do {
      chpl_bool timed_out = false;
      while (!task_pool_head || timed_out) {
        timed_out = false;
        if (set_block_loc(0, idleTaskName)) {
          // all other tasks appear to be blocked
          struct timeval deadline, now;
          gettimeofday(&deadline, NULL);
          deadline.tv_sec += 1;
          do {
            timed_out = threadlayer_pool_suspend(&threading_lock, &deadline);
            if (!task_pool_head && !timed_out)
              gettimeofday(&now, NULL);
          } while (!task_pool_head
                   && !timed_out
                   && (now.tv_sec < deadline.tv_sec
                       || (now.tv_sec == deadline.tv_sec
                           && now.tv_usec < deadline.tv_usec)));
          if (!task_pool_head) {
            check_for_deadlock();
            timed_out = true;
          }
        }
        else {
          do {
            (void) threadlayer_pool_suspend(&threading_lock, NULL);
          } while (!task_pool_head);
        }
        unset_block_loc();
      }
    } while (!task_pool_head);

    if (blockreport)
      progress_cnt++;

    assert(task_pool_head && !task_pool_head->begun);

    if (waking_cnt > 0)
      waking_cnt--;

    //
    // start new task; increment running count and remove task from pool
    // also add to task to task-table (structure in ChapelRuntime that keeps
    // track of currently running tasks for task-reports on deadlock or 
    // Ctrl+C).
    //
    assert(queued_cnt > 0);
    queued_cnt--;
    idle_cnt--;
    running_cnt++;
    ptask = task_pool_head;
    if (ptask->ltask) {
      ptask->ltask->ptask = NULL;
      // there is no longer any need to access the corresponding task
      // list entry so avoid any potential of accessing a node that
      // will eventually be freed
      ptask->ltask = NULL;
    }
    tp->ptask = ptask;
    ptask->begun = true;
    task_pool_head = task_pool_head->next;
    if (task_pool_head == NULL)  // task pool is now empty
      task_pool_tail = NULL;
    else {
      task_pool_head->prev = NULL;
      if (waking_cnt > 0)
        // Our technique for informing the threading layer that there is
        // nothing, and then something, to do is inherently racy.  If we
        // have more to do than just this one task, tell the threading
        // layer so.  This may result in the threading layer being overly
        // optimistic about the amount of available work, but if so that
        // will correct itself, and it is better than activating too few
        // threads to do the available work.
        threadlayer_pool_awaken();
    }

    // end critical section
    threadlayer_mutex_unlock(&threading_lock);
  }

  return NULL;
}


//
// run task in a new thread
// assumes at least one task is in the pool and threading_lock has already
// been acquired!
//
static void
launch_next_task_in_new_thread(void) {
  task_pool_p             ptask;
  static chpl_bool        warning_issued = false;
  threadlayer_threadID_t  thread;

  if (warning_issued)  // If thread creation failed previously, don't try again
    return;

  if ((ptask = task_pool_head)) {
    if (threadlayer_thread_create(&thread, chpl_begin_helper, ptask)) {
      char msg[256];
      if (taskMaxThreadsPerLocale)
        sprintf(msg,
                "maxThreadsPerLocale is %"PRId32", but unable to create more than %d threads",
                taskMaxThreadsPerLocale, threads_cnt);
      else
        sprintf(msg,
                "maxThreadsPerLocale is unbounded, but unable to create more than %d threads",
                threads_cnt);
      chpl_warning(msg, 0, 0);
      warning_issued = true;
    } else {
      assert(queued_cnt > 0);
      queued_cnt--;
      threads_cnt++;
      running_cnt++;
      if (ptask->ltask) {
        ptask->ltask->ptask = NULL;
        // there is no longer any need to access the corresponding task
        // list entry so avoid any potential of accessing a node that
        // will eventually be freed
        ptask->ltask = NULL;
      }
      ptask->begun = true;
      task_pool_head = task_pool_head->next;
      if (task_pool_head == NULL)  // task pool is now empty
        task_pool_tail = NULL;
      else
        task_pool_head->prev = NULL;
    }
  }
}


// Schedule one or more tasks either by signaling an existing thread or by
// launching new threads if available
static void schedule_next_task(int howMany) {
  //
  // Reduce the number of new threads to be started, by the number that
  // are already looking for work and will find it very soon.  Try to
  // launch each remaining task in a new thread, up to the maximum number
  // of threads we are supposed to have.  (And keep in mind that the main
  // thread is not included in threads_cnt, but is included in idle_cnt
  // if it is idle.)
  //
  if (idle_cnt > waking_cnt) {
    // increment waking_cnt by the number of idle threads
    if (idle_cnt - waking_cnt >= howMany) {
      waking_cnt += howMany;
      howMany = 0;
    } else {
      howMany -= (idle_cnt - waking_cnt);
      waking_cnt = idle_cnt;
    }
    threadlayer_pool_awaken();
  }

  for (; howMany && (taskMaxThreadsPerLocale == 0 || threads_cnt + 1 < taskMaxThreadsPerLocale); howMany--)
    launch_next_task_in_new_thread();
}


// create a task from the given function pointer and arguments
// and append it to the end of the task pool
// assumes threading_lock has already been acquired!
static task_pool_p add_to_task_pool(chpl_fn_p fp,
                                    void* a,
                                    chpl_bool serial,
                                    chpl_task_list_p ltask) {
  task_pool_p ptask = (task_pool_p) chpl_alloc(sizeof(task_pool_t),
                                               CHPL_RT_MD_TASK_POOL_DESCRIPTOR,
                                               0, 0);
  ptask->id           = get_next_task_id();
  ptask->fun          = fp;
  ptask->arg          = a;
  ptask->serial_state = serial;
  ptask->ltask        = ltask;
  ptask->begun        = false;

  if (ltask) {
    ptask->filename = ltask->filename;
    ptask->lineno = ltask->lineno;
  } else {  /* Believe this happens only when an on-clause starts the task */
    ptask->filename = "<unknown>";
    ptask->lineno = 0;
  }

  ptask->next = NULL;

  if (task_pool_tail)
    task_pool_tail->next = ptask;
  else
    task_pool_head = ptask;
  ptask->prev = task_pool_tail;
  task_pool_tail = ptask;

  queued_cnt++;

  if (taskreport) {
    threadlayer_mutex_lock(&taskTable_lock);
    chpldev_taskTable_add(ptask->id,
                          ptask->lineno, ptask->filename,
                          (uint64_t) (intptr_t) ptask);
    threadlayer_mutex_unlock(&taskTable_lock);
  }

  return ptask;
}


// Threads

int32_t  CHPL_THREADS_GETMAXTHREADS(void) { return 0; }

int32_t  CHPL_THREADS_MAXTHREADSLIMIT(void) { return 0; }

// take the main thread into account
uint32_t CHPL_NUMTHREADS(void) { return threads_cnt + 1; }

uint32_t CHPL_NUMIDLETHREADS(void) {
  int numIdleThreads;

  // begin critical section
  threadlayer_mutex_lock(&threading_lock);

  numIdleThreads = idle_cnt - waking_cnt;

  // end critical section
  threadlayer_mutex_unlock(&threading_lock);

  assert(numIdleThreads >= 0);
  return numIdleThreads;
}


//
// Is the task pool empty?
//
// This can be used by a thread layer's threadlayer_pool_suspend() to
// tell if the pool has become nonempty before the deadline passes.
//
chpl_bool chpl_pool_is_empty(void) {
  return task_pool_head == NULL;
}
