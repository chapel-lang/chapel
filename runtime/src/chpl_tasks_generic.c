//
// Generic implementation of Chapel tasking interface
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


//
// This file is effectively empty unless the thread-specific configuration
// file says it is needed.
//
#ifdef USE_GENERIC_TASK_MANAGEMENT


//
// task pool: linked list of tasks
//
typedef struct task_pool_struct* task_pool_p;

typedef struct task_pool_struct {
  chpl_fn_p        fun;          // function to call for task
  void*            arg;          // argument to the function
  chpl_bool        serial_state; // whether new threads can be created while executing fun
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

static void        report_locked_threads(void);
static void        report_all_tasks(void);
static void        SIGINT_handler(int sig);
static void        initializeLockReportForThread(void);
static chpl_string idleThreadName = "|idle|";


// Threads

int32_t  chpl_threads_getMaxThreads_generic(void) { return 0; }

int32_t  chpl_threads_maxThreadsLimit_generic(void) { return 0; }

// take the main thread into account
uint32_t chpl_numThreads_generic(void) { return threads_cnt + 1; }

uint32_t chpl_numIdleThreads_generic(void) {
  int numIdleThreads;

  // begin critical section
  chpl_mutex_lock(&threading_lock);

  numIdleThreads = idle_cnt - waking_cnt;

  // end critical section
  chpl_mutex_unlock(&threading_lock);

  assert(numIdleThreads >= 0);
  return numIdleThreads;
}

uint32_t chpl_numQueuedTasks_generic(void) { return queued_cnt; }

uint32_t chpl_numRunningTasks_generic(void) {
  int numRunningTasks;

  // begin critical section
  chpl_mutex_lock(&threading_lock);
  chpl_mutex_lock(&extra_task_lock);

  // take the main thread into account
  numRunningTasks = running_cnt + extra_task_cnt + 1;

  // end critical section
  chpl_mutex_unlock(&threading_lock);
  chpl_mutex_unlock(&extra_task_lock);

  return numRunningTasks;
}

int32_t  chpl_numBlockedTasks_generic(void) {
  if (blockreport) {
    int numBlockedTasks;

    // begin critical section
    chpl_mutex_lock(&threading_lock);
    chpl_mutex_lock(&report_lock);

    numBlockedTasks = blocked_thread_cnt - idle_cnt;

    // end critical section
    chpl_mutex_unlock(&threading_lock);
    chpl_mutex_unlock(&report_lock);

    assert(numBlockedTasks >= 0);
    return numBlockedTasks;
  }
  else
    return -1;
}


void initChplThreads_generic() {
  thread_private_data_t *tp;

  chpl_mutex_init(&threading_lock);
  chpl_mutex_init(&extra_task_lock);
  chpl_mutex_init(&task_list_lock);
  queued_cnt = 0;
  running_cnt = 0;                     // only main thread running
  waking_cnt = 0;
  threads_cnt = 0;
  blocked_thread_cnt = 0;
  idle_cnt = 0;
  extra_task_cnt = 0;
  maybe_deadlocked = NULL;
  task_pool_head = task_pool_tail = NULL;

  threadlayer_init();

  tp = chpl_alloc(sizeof(thread_private_data_t), CHPL_RT_MD_THREAD_PRIVATE_DATA, 0, 0);
  threadlayer_set_thread_private_data(tp);

  chpl_thread_init_generic();

  if (blockreport)
    chpl_mutex_init(&report_lock);

  if (blockreport || taskreport) {
    signal(SIGINT, SIGINT_handler);
  }
}


void exitChplThreads_generic() {
  chpl_bool debug = false;
  if (debug)
    fprintf(stderr, "A total of %d threads were created; waking_cnt = %d\n", threads_cnt, waking_cnt);

  // calls chpl_thread_cancel() and chpl_thread_join() for all threads
  chpldev_endAllThreads(); 

  threadlayer_exit();
}


void chpl_thread_init_generic(void) {
  if (blockreport)
    initializeLockReportForThread();
}


chpl_bool chpl_get_serial_generic(void) {
  thread_private_data_t* tp;

  tp = (thread_private_data_t*) threadlayer_get_thread_private_data();
  if (tp == NULL)
    chpl_internal_error("no thread private data");

  return tp->serial_state;
}

void chpl_set_serial_generic(chpl_bool state) {
  thread_private_data_t* tp;

  tp = (thread_private_data_t*) threadlayer_get_thread_private_data();
  if (tp == NULL)
    chpl_internal_error("no thread private data");

  tp->serial_state = state;
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
      else if (rep->lineno == 0 && !strcmp(rep->filename, idleThreadName))
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
    printf("Executing tasks:\n");
    chpldev_taskTable_print();
}


//
// This is a signal handler that does thread and task reporting.
//
static void SIGINT_handler(int sig) {
  signal(sig, SIG_IGN);

  if (blockreport)
    report_locked_threads();

  if(taskreport)
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
// (indirectly from the task wrapper function via chpl_thread_init()) as
// each task starts.  We never remove these from the list or deallocate
// them.  If we do traverse the list while reporting a deadlock, we just
// skip the leaked ones, because they don't say "blocked".
//
static void initializeLockReportForThread(void) {
  lockReport_t* newLockReport;
  thread_private_data_t* tp;

  newLockReport = chpl_alloc(sizeof(lockReport_t), CHPL_RT_MD_LOCK_REPORT_DATA,
                             0, 0);
  newLockReport->next = NULL;
  newLockReport->maybeLocked = false;
  newLockReport->maybeDeadlocked = false;

  tp = (thread_private_data_t*) threadlayer_get_thread_private_data();
  if (tp == NULL)
    chpl_internal_error("no thread private data");
  tp->lockRprt = newLockReport;

  // Begin critical section
  chpl_mutex_lock(&report_lock);
  if (lockReportHead) {
    lockReportTail->next = newLockReport;
    lockReportTail = newLockReport;
  } else {
    lockReportHead = newLockReport;
    lockReportTail = newLockReport;
  }
  // End critical section
  chpl_mutex_unlock(&report_lock);
}


//
// This function removes tasks at the beginning of the task pool
// that have already started executing.
// assumes threading_lock has already been acquired!
//
static void remove_begun_tasks_from_pool (void) {
  while (task_pool_head && task_pool_head->begun) {
    task_pool_p ptask = task_pool_head;
    task_pool_head = task_pool_head->next;
    chpl_free(ptask, 0, 0);
    if (task_pool_head == NULL)  // task pool is now empty
      task_pool_tail = NULL;
  }
}

//
// When we create a thread it runs this wrapper function, which just
// executes tasks out of the pool as they become available.
//
static void*
chpl_begin_helper(void* ptask_void) {
  task_pool_p ptask = (task_pool_p) ptask_void;
  thread_private_data_t *tp;

  tp = chpl_alloc(sizeof(thread_private_data_t),
                  CHPL_RT_MD_THREAD_PRIVATE_DATA, 0, 0);
  threadlayer_set_thread_private_data(tp);

  if (blockreport)
    initializeLockReportForThread();

  // add incoming task to task-table structure in ChapelRuntime
  if(taskreport) {
      chpl_mutex_lock(&threading_lock);
      chpldev_taskTable_add(
        chpl_thread_id(), ptask->lineno, ptask->filename);
      chpl_mutex_unlock(&threading_lock);
  }

  while (true) {
    //
    // reset serial state
    //
    tp->serial_state = ptask->serial_state;

    (*ptask->fun)(ptask->arg);

    // begin critical section
    chpl_mutex_lock(&threading_lock);

    //
    // We have to wait to free the ptask until we hold the lock, in
    // order to make sure launch_next_task_in_new_thread() is done
    // manipulating the ptask before anyone else could re-allocate it.
    // If we want to do the free before grabbing the lock then we'll
    // have to arrange for launch_next_task_in_new_thread() to do the
    // pool manipulations before calling threadlayer_thread_create(),
    // but then we'll also have to be prepared to undo all those
    // manipulations if we cannot create a thread.
    //
    chpl_free(ptask, 0, 0);

    if(taskreport) {
        chpldev_taskTable_remove(chpl_thread_id());
    }

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
        if (chpl_thread_set_block_loc(0, idleThreadName)) {
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
            chpl_thread_check_for_deadlock();
            timed_out = true;
          }
        }
        else {
          do {
            (void) threadlayer_pool_suspend(&threading_lock, NULL);
          } while (!task_pool_head);
        }
        chpl_thread_unset_block_loc();
      }

      // remove any tasks that have already started executing
      remove_begun_tasks_from_pool();
    } while (!task_pool_head);

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
    ptask->begun = true;
    if(taskreport) {
        chpldev_taskTable_add(chpl_thread_id(), ptask->lineno,
                              ptask->filename);
    }
    task_pool_head = task_pool_head->next;
    if (task_pool_head == NULL)  // task pool is now empty
      task_pool_tail = NULL;
    else if (waking_cnt > 0)
      // Our technique for informing the threading layer that there is
      // nothing, and then something, to do is inherently racy.  If we
      // have more to do than just this one task, tell the threading
      // layer so.  This may result in the threading layer being overly
      // optimistic about the amount of available work, but if so that
      // will correct itself, and we certainly don't want the threading
      // layer failing to pick up work when we have it.
      threadlayer_pool_awaken();

    // end critical section
    chpl_mutex_unlock(&threading_lock);
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
  task_pool_p      ptask;
  static chpl_bool warning_issued = false;
  chpl_threadID_t  thread;

  if (warning_issued)  // If thread creation failed previously, don't try again
    return;

  // remove any tasks that have already started executing
  remove_begun_tasks_from_pool();

  if ((ptask = task_pool_head)) {
    if (threadlayer_thread_create(&thread, chpl_begin_helper, ptask)) {
      char msg[256];
      if (maxThreads)
        sprintf(msg,
                "maxThreads is %"PRId32", but unable to create more than %d threads",
                maxThreads, threads_cnt);
      else
        sprintf(msg,
                "maxThreads is unbounded, but unable to create more than %d threads",
                threads_cnt);
      chpl_warning(msg, 0, 0);
      warning_issued = true;
    } else {
      assert(queued_cnt > 0);
      queued_cnt--;

      // remember thread ID for later use
      chpldev_taskTable_addThread(thread); // requires threading lock

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

  for (; howMany && (maxThreads == 0 || threads_cnt + 1 < maxThreads); howMany--)
    launch_next_task_in_new_thread();
}


// create a task from the given function pointer and arguments
// and append it to the end of the task pool
// assumes threading_lock has already been acquired!
static task_pool_p add_to_task_pool(chpl_fn_p fp,
                                    void* a,
                                    chpl_bool serial,
                                    chpl_task_list_p ltask) {
  task_pool_p ptask = (task_pool_p)chpl_alloc(sizeof(task_pool_t),
                                              CHPL_RT_MD_TASK_POOL_DESCRIPTOR,
                                              0, 0);
  if (ltask) {
    ptask->filename = ltask->filename;
    ptask->lineno = ltask->lineno;
  } else {  /* Believe this happens only when an on-clause starts the task */
    ptask->filename = "<unknown>";
    ptask->lineno = 0;
  }
  ptask->fun = fp;
  ptask->arg = a;
  ptask->serial_state = serial;
  ptask->ltask = ltask;
  ptask->begun = false;
  ptask->next = NULL;

  if (task_pool_tail)
    task_pool_tail->next = ptask;
  else
    task_pool_head = ptask;
  task_pool_tail = ptask;

  queued_cnt++;

  return ptask;
}


void chpl_add_to_task_list(chpl_fn_int_t fid, void* arg,
                           chpl_task_list_p *task_list,
                           int32_t task_list_locale,
                           chpl_bool call_chpl_begin,
                           int lineno,
                           chpl_string filename) {
  if (task_list_locale == chpl_localeID) {
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
      chpl_begin(fp, arg, false, false, ltask);
    }

    // begin critical section - not needed for cobegin or coforall statements
    if (call_chpl_begin)
      chpl_mutex_lock(&task_list_lock);

    if (*task_list) {
      ltask->next = (*task_list)->next;
      (*task_list)->next = ltask;
    }
    else
      ltask->next = ltask;
    *task_list = ltask;

    // end critical section - not needed for cobegin or coforall statements
    if (call_chpl_begin)
      chpl_mutex_unlock(&task_list_lock);
  }
  else {
    // call_chpl_begin should be true here because if task_list_locale !=
    // chpl_localeID, then this function could not have been called from
    // the context of a cobegin or coforall statement, which are the only
    // contexts in which chpl_begin() should not be called.
    chpl_fn_p fp = chpl_ftable[fid];
    assert(call_chpl_begin);
    chpl_begin(fp, arg, false, false, NULL);
  }
}


void chpl_process_task_list(chpl_task_list_p task_list) {
  // task_list points to the last entry on the list; task_list->next is
  // actually the first element on the list.
  chpl_task_list_p ltask = task_list, next_task;
  chpl_bool serial = chpl_get_serial();
  // This function is not expected to be called if a cobegin contains fewer
  // than two statements; a coforall, however, may generate just one task,
  // or even none at all.
  if (ltask == NULL)
    return;
  assert(ltask->next);
  next_task = ltask->next;  // next_task now points to the head of the list

  if (serial)
    do {
      ltask = next_task;

      if(taskreport) {
          chpl_mutex_lock(&threading_lock);
          chpldev_taskTable_add(chpl_thread_id(), ltask->lineno,
                                ltask->filename);
          chpl_mutex_unlock(&threading_lock);
      }

      (*ltask->fun)(ltask->arg);

      if(taskreport) {
          chpl_mutex_lock(&threading_lock);
          chpldev_taskTable_remove(chpl_thread_id());
          chpl_mutex_unlock(&threading_lock);
      }

      next_task = ltask->next;
    } while (ltask != task_list);

  else {
    int task_cnt = 0;
    chpl_task_list_p first_task = next_task;
    next_task = next_task->next;

    if (first_task != task_list) {
      // there are at least two tasks in task_list

      // begin critical section
      chpl_mutex_lock(&threading_lock);

      do {
        ltask = next_task;
        ltask->ptask = add_to_task_pool(ltask->fun, ltask->arg, serial, ltask);
        assert(ltask->ptask == NULL
               || ltask->ptask->ltask == ltask);
        next_task = ltask->next;
        task_cnt++;
      } while (ltask != task_list);

      schedule_next_task(task_cnt);

      // end critical section
      chpl_mutex_unlock(&threading_lock);
    }

    // Execute the first task on the list, since it has to run to completion
    // before continuing beyond the cobegin or coforall it's in.

    if(taskreport) {
        chpl_mutex_lock(&threading_lock);
        chpldev_taskTable_add(chpl_thread_id(), first_task->lineno,
                              first_task->filename);
        chpl_mutex_unlock(&threading_lock);
    }

    // begin critical section
    chpl_mutex_lock(&extra_task_lock);

    extra_task_cnt++;

    // end critical section
    chpl_mutex_unlock(&extra_task_lock);

    (*first_task->fun)(first_task->arg);

    // begin critical section
    chpl_mutex_lock(&extra_task_lock);

    extra_task_cnt--;

    // end critical section
    chpl_mutex_unlock(&extra_task_lock);

    if(taskreport) {
        chpl_mutex_lock(&threading_lock);
        chpldev_taskTable_remove(chpl_thread_id());
        chpl_mutex_unlock(&threading_lock);
    }
  }
}


void chpl_execute_tasks_in_list(chpl_task_list_p task_list) {
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
  if (!chpl_get_serial()) do {
    ltask = next_task;
    next_task = ltask->next;

    // don't lock unless it looks like we will find a task to execute
    // if we do so
    if (ltask->ptask) {
      chpl_fn_p task_to_run_fun = NULL;
      void* task_to_run_arg = NULL;

      // begin critical section
      chpl_mutex_lock(&threading_lock);

      if (ltask->ptask) {
        assert(!ltask->ptask->begun);
        task_to_run_fun = ltask->ptask->fun;
        task_to_run_arg = ltask->ptask->arg;
        ltask->ptask->begun = true;
        ltask->ptask->ltask = NULL;
        // there is no longer any need to access the corresponding task
        // pool entry so avoid any potential of accessing a node that
        // will eventually be freed
        ltask->ptask = NULL;
        if (waking_cnt > 0)
          waking_cnt--;
        assert(queued_cnt > 0);
        queued_cnt--;
      }

      // end critical section
      chpl_mutex_unlock(&threading_lock);

      if (task_to_run_fun) {
        // begin critical section
        chpl_mutex_lock(&extra_task_lock);

        extra_task_cnt++;

        // end critical section
        chpl_mutex_unlock(&extra_task_lock);

        (*task_to_run_fun)(task_to_run_arg);

        // begin critical section
        chpl_mutex_lock(&extra_task_lock);

        extra_task_cnt--;

        // end critical section
        chpl_mutex_unlock(&extra_task_lock);
      }
    }

  } while (ltask != task_list);
}


void chpl_free_task_list(chpl_task_list_p task_list) {
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
void chpl_begin(chpl_fn_p fp, void* a,
                chpl_bool ignore_serial,  // always add task to pool
                chpl_bool serial_state,
                chpl_task_list_p ltask) {
  if (!ignore_serial && chpl_get_serial()) {
    (*fp)(a);
  } else {
    task_pool_p ptask = NULL;

    // begin critical section
    chpl_mutex_lock(&threading_lock);

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
    chpl_mutex_unlock(&threading_lock);
  }
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



// Deadlock detection

chpl_bool chpl_thread_set_block_loc(int lineno, chpl_string filename) {
  thread_private_data_t* tp;
  chpl_bool isLastUnblockedThread;

  if (!blockreport)
    return false;

  isLastUnblockedThread = false;

  tp = (thread_private_data_t*) threadlayer_get_thread_private_data();
  if (tp == NULL)
    chpl_internal_error("no thread private data");
  tp->lockRprt->filename = filename;
  tp->lockRprt->lineno = lineno;
  tp->lockRprt->maybeLocked = true;

  // Begin critical section
  chpl_mutex_lock(&report_lock);

  blocked_thread_cnt++;
  if (blocked_thread_cnt > threads_cnt) {
    isLastUnblockedThread = true;
    tp->lockRprt->maybeDeadlocked = true;
    if (maybe_deadlocked)
      *maybe_deadlocked = false;
    maybe_deadlocked = &(tp->lockRprt->maybeDeadlocked);
  }

  // End critical section
  chpl_mutex_unlock(&report_lock);

  return isLastUnblockedThread;
}


void chpl_thread_unset_block_loc() {
  thread_private_data_t* tp;

  if (!blockreport)
    return;

  tp = (thread_private_data_t*) threadlayer_get_thread_private_data();
  if (tp == NULL)
    chpl_internal_error("no thread private data");
  tp->lockRprt->maybeLocked = false;

  // Begin critical section
  chpl_mutex_lock(&report_lock);

  blocked_thread_cnt--;
  if (maybe_deadlocked)
    *maybe_deadlocked = false;
  maybe_deadlocked = NULL;

  // End critical section
  chpl_mutex_unlock(&report_lock);
}


void chpl_thread_check_for_deadlock() {
  thread_private_data_t* tp;

  // Blockreport should be true here here, because this can't be called
  // unless chpl_thread_set_block_loc() returns true, and it can't do
  // that unless blockreport is true.  So this is just a check for
  // ongoing internal consistency.
  assert(blockreport);

  tp = (thread_private_data_t*) threadlayer_get_thread_private_data();
  if (tp == NULL)
    chpl_internal_error("no thread private data");

  if (!tp->lockRprt->maybeDeadlocked)
    return;

  fflush(stdout);
  fprintf(stderr, "Program is deadlocked!\n");

  report_locked_threads();

  if(taskreport)
    report_all_tasks();

  chpl_exit_any(1);
}


#endif  // USE_GENERIC_TASK_MANAGEMENT
