//
// Pthread implementation of Chapel threading interface
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
#include <pthread.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/time.h>

//
// task pool: linked list of tasks
//
typedef struct chpl_pool_struct {
  chpl_fn_p        fun;             // function to call for task
  void*            arg;             // argument to the function
  chpl_bool        serial_state;    // whether new threads can be created while executing fun
  chpl_bool        begun;           // whether execution of this task has begun
  chpl_task_list_p task_list_entry; // points to the task list entry, if there is one
  chpl_task_pool_p next;
  chpl_string filename;
  int lineno;
} task_pool_t;

// This struct is intended for use in a circular linked list where the pointer
// to the list actually points to the tail of the list, i.e., the last entry
// inserted into the list, making it easier to append items to the end of the list.
// Since it is part of a circular list, the last entry will, of course,
// point to the first entry in the list.
struct chpl_task_list {
  chpl_fn_p fun;
  void* arg;
  volatile chpl_task_pool_p task_pool_entry; // when null, execution of the associated task has begun
  chpl_task_list_p next;
  chpl_string filename;
  int lineno;
};


static chpl_mutex_t     threading_lock;     // critical section lock
static chpl_mutex_t     extra_task_lock;    // critical section lock
static chpl_mutex_t     task_list_lock;     // critical section lock
static chpl_condvar_t   wakeup_signal;      // signal a waiting thread
static pthread_key_t    serial_key;         // per-thread serial state
static chpl_task_pool_p task_pool_head;     // head of task pool
static chpl_task_pool_p task_pool_tail;     // tail of task pool
static int              queued_cnt;         // number of tasks in the task pool
static int              waking_cnt;         // number of threads signaled to wakeup
static int              running_cnt;        // number of running threads 
static int              threads_cnt;        // number of threads (total)
static int              blocked_thread_cnt; // number of threads waiting for something
static int              idle_cnt;           // number of threads that are idle
static int              extra_task_cnt;     // number of threads executing more than one task
static chpl_bool*       maybe_deadlocked;   // whether all existing threads are blocked
static chpl_mutex_t     report_lock;        // critical section lock
static pthread_key_t    lock_report_key;


typedef struct lockReport {
  const char* filename;
  int lineno;
  chpl_bool maybeLocked, maybeDeadlocked;
  struct lockReport* next;
} lockReport;


lockReport* lockReportHead = NULL;
lockReport* lockReportTail = NULL;

static void  traverseLockedThreads(int sig);
static void  tasksReport(int sig);
static chpl_bool setBlockingLocation(int lineno, chpl_string filename);
static void  unsetBlockingLocation(void);
static void  initializeLockReportForThread(void);
static chpl_string idleThreadName = "|idle|";

// Condition variables

static chpl_condvar_p chpl_condvar_new(void) {
  chpl_condvar_p cv;
  cv = (chpl_condvar_p) chpl_alloc(sizeof(chpl_condvar_t), CHPL_RT_MD_COND_VAR, 0, 0);
  if (pthread_cond_init(cv, NULL))
    chpl_internal_error("pthread_cond_init() failed");
  return cv;
}


// Mutex

void chpl_mutex_init(chpl_mutex_p mutex) {
  // WAW: how to explicitly specify blocking-type?
  if (pthread_mutex_init(mutex, NULL))
    chpl_internal_error("pthread_mutex_init() failed");
}

static chpl_mutex_p chpl_mutex_new(void) {
  chpl_mutex_p m;
  m = (chpl_mutex_p) chpl_alloc(sizeof(chpl_mutex_t), CHPL_RT_MD_MUTEX, 0, 0);
  chpl_mutex_init(m);
  return m;
}

int chpl_mutex_lock(chpl_mutex_p mutex) {
  int return_value;
  if ((return_value = pthread_mutex_lock(mutex)))
    chpl_internal_error("pthread_mutex_lock() failed");
  return return_value;
}

void chpl_mutex_unlock(chpl_mutex_p mutex) {
  if (pthread_mutex_unlock(mutex))
    chpl_internal_error("pthread_mutex_unlock() failed");
}


// Sync variables

int chpl_sync_lock(chpl_sync_aux_t *s) {
  return chpl_mutex_lock(s->lock);
}

void chpl_sync_unlock(chpl_sync_aux_t *s) {
  chpl_mutex_unlock(s->lock);
}

int chpl_sync_wait_full_and_lock(chpl_sync_aux_t *s, int32_t lineno, chpl_string filename) {
  int return_value = chpl_mutex_lock(s->lock);
  while (return_value == 0 && !s->is_full) {
    if (setBlockingLocation(lineno, filename)) {
      // all other threads appear to be blocked
      struct timeval now;
      struct timespec wakep_time;
      gettimeofday (&now, NULL);
      wakep_time.tv_sec = now.tv_sec + 2;
      wakep_time.tv_nsec = 0;
      if ((return_value = pthread_cond_timedwait(s->signal_full, s->lock, &wakep_time))
          == ETIMEDOUT) {
        lockReport* lockRprt = (lockReport*)pthread_getspecific(lock_report_key);
        if (lockRprt->maybeDeadlocked) {
          fflush(stdout);
          fprintf(stderr, "Program is deadlocked!\n");
          traverseLockedThreads(SIGINT);
        }
        return_value = 0;
      }
    }
    else if ((return_value = pthread_cond_wait(s->signal_full, s->lock)))
      chpl_internal_error("pthread_cond_wait() failed");
    unsetBlockingLocation();
  }
  return return_value;
}

int chpl_sync_wait_empty_and_lock(chpl_sync_aux_t *s, int32_t lineno, chpl_string filename) {
  int return_value = chpl_mutex_lock(s->lock);
  while (return_value == 0 && s->is_full) {
    if (setBlockingLocation(lineno, filename)) {
      // all other threads appear to be blocked
      struct timeval now;
      struct timespec wakep_time;
      gettimeofday (&now, NULL);
      wakep_time.tv_sec = now.tv_sec + 2;
      wakep_time.tv_nsec = 0;
      if ((return_value = pthread_cond_timedwait(s->signal_empty, s->lock, &wakep_time))
          == ETIMEDOUT) {
        lockReport* lockRprt = (lockReport*)pthread_getspecific(lock_report_key);
        if (lockRprt->maybeDeadlocked) {
          fflush(stdout);
          fprintf(stderr, "Program is deadlocked!\n");
          traverseLockedThreads(SIGINT);
        }
        return_value = 0;
      }
    }
    else if ((return_value = pthread_cond_wait(s->signal_empty, s->lock)))
      chpl_internal_error("pthread_cond_wait() failed");
    unsetBlockingLocation();
  }
  return return_value;
}

void chpl_sync_mark_and_signal_full(chpl_sync_aux_t *s) {
  s->is_full = true;
  if (pthread_cond_signal(s->signal_full))
    chpl_internal_error("pthread_cond_signal() failed");
  chpl_sync_unlock(s);
}

void chpl_sync_mark_and_signal_empty(chpl_sync_aux_t *s) {
  s->is_full = false;
  if (pthread_cond_signal(s->signal_empty))
    chpl_internal_error("pthread_cond_signal() failed");
  chpl_sync_unlock(s);
}

chpl_bool chpl_sync_is_full(void *val_ptr, chpl_sync_aux_t *s, chpl_bool simple_sync_var) {
  return s->is_full;
}

void chpl_init_sync_aux(chpl_sync_aux_t *s) {
  s->is_full = false;
  s->lock = chpl_mutex_new();
  s->signal_full = chpl_condvar_new();
  s->signal_empty = chpl_condvar_new();
}

void chpl_destroy_sync_aux(chpl_sync_aux_t *s) {
  chpl_free(s->lock, 0, 0);
  chpl_free(s->signal_full, 0, 0);
  chpl_free(s->signal_empty, 0, 0);
}


// Single variables

int chpl_single_lock(chpl_single_aux_t *s) {
  return chpl_mutex_lock(s->lock);
}

void chpl_single_unlock(chpl_single_aux_t *s) {
  chpl_mutex_unlock(s->lock);
}

int chpl_single_wait_full(chpl_single_aux_t *s, int32_t lineno, chpl_string filename) {
  int return_value = chpl_mutex_lock(s->lock);
  while (return_value == 0 && !s->is_full) {
    if (setBlockingLocation(lineno, filename)) {
      // all other threads appear to be blocked
      struct timeval now;
      struct timespec wakep_time;
      gettimeofday (&now, NULL);
      wakep_time.tv_sec = now.tv_sec + 2;
      wakep_time.tv_nsec = 0;
      if ((return_value = pthread_cond_timedwait(s->signal_full, s->lock, &wakep_time))
          == ETIMEDOUT) {
        lockReport* lockRprt = (lockReport*)pthread_getspecific(lock_report_key);
        if (lockRprt->maybeDeadlocked) {
          fflush(stdout);
          fprintf(stderr, "Program is deadlocked!\n");
          traverseLockedThreads(SIGINT);
        }
        return_value = 0;
      }
    }
    else if ((return_value = pthread_cond_wait(s->signal_full, s->lock)))
      chpl_internal_error("invalid mutex in chpl_single_wait_full");
    unsetBlockingLocation();
  }
  return return_value;
}

void chpl_single_mark_and_signal_full(chpl_single_aux_t *s) {
  s->is_full = true;
  chpl_mutex_unlock(s->lock);
  if (pthread_cond_signal(s->signal_full))
    chpl_internal_error("pthread_cond_signal() failed");
}

chpl_bool chpl_single_is_full(void *val_ptr, chpl_single_aux_t *s, chpl_bool simple_single_var) {
  return s->is_full;
}

void chpl_init_single_aux(chpl_single_aux_t *s) {
  s->is_full = false;
  s->lock = chpl_mutex_new();
  s->signal_full = chpl_condvar_new();
}


// Threads

static void serial_delete(chpl_bool *p) {
  if (p != NULL)
    chpl_free(p, 0, 0);
}

static void lock_report_delete(lockReport *p) {
  if (p != NULL)
    chpl_free(p, 0, 0);
}


int32_t  chpl_threads_getMaxThreads(void) { return 0; }

int32_t  chpl_threads_maxThreadsLimit(void) { return 0; }

// take the main thread into account
uint32_t chpl_numThreads(void) { return threads_cnt + 1; }

uint32_t chpl_numIdleThreads(void) {
  int numIdleThreads;

  // begin critical section
  chpl_mutex_lock(&threading_lock);

  numIdleThreads = idle_cnt - waking_cnt;

  // end critical section
  chpl_mutex_unlock(&threading_lock);

  assert(numIdleThreads >= 0);
  return numIdleThreads;
}

uint32_t chpl_numQueuedTasks(void) { return queued_cnt; }

uint32_t chpl_numRunningTasks(void) {
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

int32_t  chpl_numBlockedTasks(void) {
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


void initChplThreads() {
  chpl_mutex_init(&threading_lock);
  chpl_mutex_init(&extra_task_lock);
  chpl_mutex_init(&task_list_lock);
  if (pthread_cond_init(&wakeup_signal, NULL))
    chpl_internal_error("pthread_cond_init() failed in");
  queued_cnt = 0;
  running_cnt = 0;                     // only main thread running
  waking_cnt = 0;
  threads_cnt = 0;
  blocked_thread_cnt = 0;
  idle_cnt = 0;
  extra_task_cnt = 0;
  maybe_deadlocked = NULL;
  task_pool_head = task_pool_tail = NULL;

  if (pthread_key_create(&serial_key, (void(*)(void*))serial_delete))
    chpl_internal_error("serial key not created");
  if (pthread_key_create(&lock_report_key, (void(*)(void*))lock_report_delete))
    chpl_internal_error("lock report key not created");

  if (blockreport) {
    chpl_mutex_init(&report_lock);
    signal(SIGINT, traverseLockedThreads);
  }

  if (taskreport) {
    signal(SIGINT, tasksReport);
  }

  chpl_thread_init();
}



void exitChplThreads() {
  chpl_bool debug = false;
  if (debug)
    fprintf(stderr, "A total of %d threads were created; waking_cnt = %d\n", threads_cnt, waking_cnt);

  // calls cancel and join (below) for all threads
  chpldev_endAllThreads(); 

  pthread_key_delete(serial_key);
}

void chpl_thread_cancel(chpl_threadID_t threadID) {
  pthread_t thread = (pthread_t) threadID;
  if (0 != pthread_cancel(thread)) {
    chpl_internal_error("thread cancel failed");
  }
}    

void chpl_thread_join(chpl_threadID_t threadID) {
  pthread_t thread = (pthread_t) threadID;
  if (0 != pthread_join(thread, NULL)) {
    chpl_internal_error("thread join failed");
  }
}    

void chpl_thread_init(void) {
  if (blockreport)
    initializeLockReportForThread();
}


chpl_threadID_t chpl_thread_id(void) {
  return (uintptr_t) pthread_self();
}


chpl_bool chpl_get_serial(void) {
  chpl_bool *p;
  p = (chpl_bool*) pthread_getspecific(serial_key);
  return p == NULL ? false : *p;
}

void chpl_set_serial(chpl_bool state) {
  chpl_bool *p;
  p = (chpl_bool*) pthread_getspecific(serial_key);
  if (p == NULL) {
    if (state) {
      p = (chpl_bool*) chpl_alloc(sizeof(chpl_bool), CHPL_RT_MD_SERIAL_FLAG, 0, 0);
      *p = state;
      if (pthread_setspecific(serial_key, p))
        chpl_internal_error("serial key got corrupted");
    }
  }
  else *p = state;
}


//
// This signal handler walks over a linked list with one node per thread.
// If a thread is waiting on a sync or single variable, it sets its
// maybeLocked field first. When the signal is caught, print the locations
// of all threads that have the maybeLocked field set.
//
static void traverseLockedThreads(int sig) {
  lockReport* rep;
  assert(blockreport); // Error: this should only be called as a signal handler
                       // and it should only be handled if blockreport is on
  signal(sig, SIG_IGN);
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

  if(taskreport) {
    tasksReport(SIGINT);
  }

  chpl_exit_any(1);
}

// called to print thread report
static void tasksReport(int sig) {
    chpl_task_pool_p pendingTask = task_pool_head;

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

    chpl_exit_any(1);
}

static chpl_bool setBlockingLocation(int lineno, chpl_string filename) {
  lockReport* lockRprt;
  chpl_bool isLastUnblockedThread = false;
  if (blockreport) {
    lockRprt = (lockReport*)pthread_getspecific(lock_report_key);
    lockRprt->filename = filename;
    lockRprt->lineno = lineno;
    lockRprt->maybeLocked = true;

    // Begin critical section
    chpl_mutex_lock(&report_lock);
    blocked_thread_cnt++;
    if (blocked_thread_cnt > threads_cnt) {
      isLastUnblockedThread = true;
      lockRprt->maybeDeadlocked = true;
      if (maybe_deadlocked)
        *maybe_deadlocked = false;
      maybe_deadlocked = &(lockRprt->maybeDeadlocked);
    }
    // End critical section
    chpl_mutex_unlock(&report_lock);
  }
  return isLastUnblockedThread;
}


static void unsetBlockingLocation() {
  lockReport* lockRprt;
  if (!blockreport)
    return;
  lockRprt = (lockReport*)pthread_getspecific(lock_report_key);
  lockRprt->maybeLocked = false;

  // Begin critical section
  chpl_mutex_lock(&report_lock);
  blocked_thread_cnt--;
  if (maybe_deadlocked)
    *maybe_deadlocked = false;
  maybe_deadlocked = NULL;
  // End critical section
  chpl_mutex_unlock(&report_lock);
}


//
// This function should be called exactly once per pthread (not task!),
// including the main thread. It should be called before the first task
// this thread was created to do is started.
//
static void initializeLockReportForThread() {
  lockReport* newLockReport;
  newLockReport = chpl_alloc(sizeof(lockReport), CHPL_RT_MD_LOCK_REPORT_DATA, 0, 0);
  newLockReport->next = NULL;
  newLockReport->maybeLocked = false;
  newLockReport->maybeDeadlocked = false;
  if (pthread_setspecific(lock_report_key, newLockReport))
    chpl_internal_error("lock report key got corrupted");

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
static void skip_over_begun_tasks (void) {
  while (task_pool_head && task_pool_head->begun) {
    chpl_task_pool_p task = task_pool_head;
    task_pool_head = task_pool_head->next;
    chpl_free(task, 0, 0);
    if (task_pool_head == NULL)  // task pool is now empty
      task_pool_tail = NULL;
  }
}


//
// used to release threading lock when a task thread is cancelled
// during a cond_wait.
//
static void chpl_task_thread_cleanup(void* unused) {
  chpl_mutex_unlock(&threading_lock);
}

//
// thread wrapper function runs the user function, waits for more
// tasks, and runs those as they become available
//
static void
chpl_begin_helper (chpl_task_pool_p task) {

  // disable cancellation immediately
  // enable only while waiting for new work
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); 
  
  if (blockreport)
    initializeLockReportForThread();

  // add incoming task to task-table structure in ChapelRuntime
  if(taskreport) {
      chpl_mutex_lock(&threading_lock);
      chpldev_taskTable_add(
        chpl_thread_id(), task->lineno, task->filename);
      chpl_mutex_unlock(&threading_lock);
  }

  while (true) {
    //
    // reset serial state
    //
    chpl_set_serial(task->serial_state);

    (*task->fun)(task->arg);

    // begin critical section
    chpl_mutex_lock(&threading_lock);
    pthread_cleanup_push( chpl_task_thread_cleanup, NULL );

    chpl_free(task, 0, 0);  // make sure task_pool_head no longer points to this task!

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
    // wait for a task to be added to the task pool
    //
    do {
      chpl_bool timed_out = false;

      while (!task_pool_head || timed_out) {

        timed_out = false;
        if (setBlockingLocation(0, idleThreadName)) {
          // all other threads appear to be blocked
          struct timeval now;
	  struct timespec wakep_time;
	  int last_cancel_state, cond_timedwait_result;

          gettimeofday (&now, NULL);
          wakep_time.tv_sec = now.tv_sec + 2;
          wakep_time.tv_nsec = 0;
	  
	  // enable cancellation with cleanup handler and wait for wakeup signal
	  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &last_cancel_state);
	  assert(last_cancel_state == PTHREAD_CANCEL_DISABLE); // sanity check

	  cond_timedwait_result = pthread_cond_timedwait(&wakeup_signal, &threading_lock, &wakep_time);

	  pthread_setcancelstate(last_cancel_state, NULL);

          if (cond_timedwait_result == ETIMEDOUT) {
            lockReport* lockRprt = (lockReport*)pthread_getspecific(lock_report_key);
            if (lockRprt->maybeDeadlocked) {
              fflush(stdout);
              fprintf(stderr, "Program is deadlocked!\n");
              traverseLockedThreads(SIGINT);
            }
            timed_out = true;
          }

        }
        else {
	  // enable cancellation with cleanup handler and wait for wakeup signal
	  int last_cancel_state;
	  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &last_cancel_state);
	  assert(last_cancel_state == PTHREAD_CANCEL_DISABLE);
	
          pthread_cond_wait(&wakeup_signal, &threading_lock);

	  pthread_setcancelstate(last_cancel_state, NULL);
	}
        unsetBlockingLocation();
      }
      // skip over any tasks that have already started executing
      skip_over_begun_tasks();
    } while (!task_pool_head);

    assert(task_pool_head && !task_pool_head->begun);

    if (waking_cnt > 0)
      waking_cnt--;

    //
    // start new task; increment running count and remove task from pool
    // also add to task to task-table (structure in ChapelRuntime that keeps
    // track of currently running tasks for task-reports on deadlock or 
    // Ctrl+C.
    //
    assert(queued_cnt > 0);
    queued_cnt--;
    idle_cnt--;
    running_cnt++;
    task = task_pool_head;
    if (task->task_list_entry) {
      task->task_list_entry->task_pool_entry = NULL;
      // there is no longer any need to access the corresponding task list entry
      // so avoid any potential of accessing a node that will eventually be freed
      task->task_list_entry = NULL;
    }
    task->begun = true;
    if(taskreport) {
        chpldev_taskTable_add(chpl_thread_id(), task->lineno, task->filename);
    }
    task_pool_head = task_pool_head->next;
    if (task_pool_head == NULL)  // task pool is now empty
      task_pool_tail = NULL;
    else if (waking_cnt > 0)
      // schedule another task if one is waiting; this must be done in
      // case, for example, 2 signals were performed by chpl_begin()
      // back-to-back before any thread was woken up from the
      // pthread_cond_wait just above.  In that case, the thread which
      // does eventually wake up is responsible for making sure the other
      // signal is handled (either by an existing thread or by creating
      // a new thread)
      pthread_cond_signal(&wakeup_signal);

    // end critical section
    pthread_cleanup_pop(0); // remove cleanup handler now that cancellation is disabled.
    chpl_mutex_unlock(&threading_lock);
  }
}


//
// run task in a new thread
// assumes at least one task is in the pool and threading_lock has already been acquired!
//
static void
launch_next_task(void) {
  pthread_t        thread;
  chpl_task_pool_p task;
  static chpl_bool warning_issued = false;

  if (warning_issued)  // If thread creation failed previously, don't try again!
    return;

  // skip over any tasks that have already started executing
  skip_over_begun_tasks();

  if ((task = task_pool_head)) {
    if (pthread_create(&thread, NULL, (void*(*)(void*))chpl_begin_helper, task)) {
      char msg[256];
      if (maxThreads)
        sprintf(msg, "maxThreads is %"PRId32", but unable to create more than %d threads",
                maxThreads, threads_cnt);
      else
        sprintf(msg, "maxThreads is unbounded, but unable to create more than %d threads",
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
      if (task->task_list_entry) {
        task->task_list_entry->task_pool_entry = NULL;
        // there is no longer any need to access the corresponding task list entry
        // so avoid any potential of accessing a node that will eventually be freed
        task->task_list_entry = NULL;
      }
      task->begun = true;

      task_pool_head = task_pool_head->next;
      if (task_pool_head == NULL)  // task pool is now empty
        task_pool_tail = NULL;
    }
  }
}


// Schedule one or more tasks either by signaling an existing thread or by
// launching new threads if available
static void schedule_next_task(int howMany) {
  // if there is an idle thread, send it a signal to wake up and grab
  // a new task
  if (idle_cnt > waking_cnt) {
    // increment waking_cnt by the number of idle threads
    if (idle_cnt - waking_cnt >= howMany) {
      waking_cnt += howMany;
      howMany = 0;
    } else {
      howMany -= (idle_cnt - waking_cnt);
      waking_cnt = idle_cnt;
    }
    pthread_cond_signal(&wakeup_signal);
  }

  //
  // try to launch each remaining task in a new thread
  // if the maximum number threads has not yet been reached
  // take the main thread into account (but not when counting idle
  // threads above)
  //
  for (; howMany && (maxThreads == 0 || threads_cnt + 1 < maxThreads); howMany--)
    launch_next_task();
}


// create a task from the given function pointer and arguments
// and append it to the end of the task pool
// assumes threading_lock has already been acquired!
static chpl_task_pool_p add_to_task_pool (
    chpl_fn_p fp,
    void* a,
   chpl_bool serial,
    chpl_task_list_p task_list_entry)
{
  chpl_task_pool_p task = (chpl_task_pool_p)chpl_alloc(sizeof(task_pool_t),
                                                       CHPL_RT_MD_TASK_POOL_DESCRIPTOR,
                                                       0, 0);
  if (task_list_entry) {
    task->filename = task_list_entry->filename;
    task->lineno = task_list_entry->lineno;
  } else {  /* Believe this happens only when an on-clause starts the task */
    task->filename = "<unknown>";
    task->lineno = 0;
  }
  task->fun = fp;
  task->arg = a;
  task->serial_state = serial;
  task->task_list_entry = task_list_entry;
  task->begun = false;
  task->next = NULL;

  if (task_pool_tail)
    task_pool_tail->next = task;
  else
    task_pool_head = task;
  task_pool_tail = task;

  queued_cnt++;

  return task;
}


//
// interface function with begin-statement
//
void chpl_begin (chpl_fn_p fp, void* a,
                 chpl_bool ignore_serial,  // always add task to pool
                 chpl_bool serial_state, chpl_task_list_p task_list_entry) {
  if (!ignore_serial && chpl_get_serial()) {
    (*fp)(a);
  } else {
    chpl_task_pool_p task_pool_entry = NULL;

    // begin critical section
    chpl_mutex_lock(&threading_lock);

    task_pool_entry = add_to_task_pool(fp, a, serial_state, task_list_entry);
    // this task may begin executing before returning from this function, so the task list
    // node needs to be updated before there is any possibility of launching this task
    if (task_list_entry)
      task_list_entry->task_pool_entry = task_pool_entry;

    schedule_next_task(1);

    assert(task_pool_entry->task_list_entry == NULL
           || (task_pool_entry->task_list_entry == task_list_entry
               && task_list_entry->task_pool_entry == task_pool_entry));

    // end critical section
    chpl_mutex_unlock(&threading_lock);
  }
}

void chpl_add_to_task_list (chpl_fn_int_t fid, void* arg,
                            chpl_task_list_p *task_list,
                            int32_t task_list_locale,
                            chpl_bool call_chpl_begin,
                            int lineno,
                            chpl_string filename) {
  if (task_list_locale == chpl_localeID) {
    chpl_task_list_p task = (chpl_task_list_p)chpl_alloc(sizeof(struct chpl_task_list),
                                                         CHPL_RT_MD_TASK_LIST_DESCRIPTOR, 0, 0);
    task->filename = filename;
    task->lineno = lineno;
    task->fun = chpl_ftable[fid];
    task->arg = arg;
    task->task_pool_entry = NULL;
    if (call_chpl_begin) {
      chpl_fn_p fp = chpl_ftable[fid];
      chpl_begin(fp, arg, false, false, task);
    }

    // begin critical section - not needed for cobegin or coforall statements
    if (call_chpl_begin)
      chpl_mutex_lock(&task_list_lock);

    if (*task_list) {
      task->next = (*task_list)->next;
      (*task_list)->next = task;
    }
    else task->next = task;
    *task_list = task;

    // end critical section - not needed for cobegin or coforall statements
    if (call_chpl_begin)
      chpl_mutex_unlock(&task_list_lock);
  }
  else {
    // call_chpl_begin should be true here because if task_list_locale != chpl_localeID, then
    // this function could not have been called from the context of a cobegin or coforall
    // statement, which are the only contexts in which chpl_begin() should not be called.
    chpl_fn_p fp = chpl_ftable[fid];
    assert(call_chpl_begin);
    chpl_begin(fp, arg, false, false, NULL);
  }
}

void chpl_process_task_list (chpl_task_list_p task_list) {
  // task_list points to the last entry on the list; task_list->next is actually
  // the first element on the list.
  chpl_task_list_p task = task_list, next_task;
  chpl_bool serial = chpl_get_serial();
  // This function is not expected to be called if a cobegin contains fewer
  // than two statements; a coforall, however, may generate just one task,
  // or even none at all.
  if (task == NULL)
    return;
  assert(task->next);
  next_task = task->next;  // next_task now points to the head of the list

  if (serial)
    do {
      task = next_task;

      if(taskreport) {
          chpl_mutex_lock(&threading_lock);
          chpldev_taskTable_add(chpl_thread_id(), task->lineno, task->filename);
          chpl_mutex_unlock(&threading_lock);
      }

      (*task->fun)(task->arg);

      if(taskreport) {
          chpl_mutex_lock(&threading_lock);
          chpldev_taskTable_remove(chpl_thread_id());
          chpl_mutex_unlock(&threading_lock);
      }

      next_task = task->next;
    } while (task != task_list);

  else {
    int task_cnt = 0;
    chpl_task_list_p first_task = next_task;
    next_task = next_task->next;

    if (first_task != task_list) {
      // there are at least two tasks in task_list

      // begin critical section
      chpl_mutex_lock(&threading_lock);

      do {
        task = next_task;
        task->task_pool_entry = add_to_task_pool(task->fun, task->arg, serial, 
                                                 task);
        assert(task->task_pool_entry == NULL
               || task->task_pool_entry->task_list_entry == task);
        next_task = task->next;
        task_cnt++;
      } while (task != task_list);

      schedule_next_task(task_cnt);

      // end critical section
      chpl_mutex_unlock(&threading_lock);
    }

    // Execute the first task on the list, since it has to run to completion
    // before continuing beyond the cobegin or coforall it's in.

    if(taskreport) {
        chpl_mutex_lock(&threading_lock);
        chpldev_taskTable_add(
            chpl_thread_id(), first_task->lineno, first_task->filename);
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

void chpl_execute_tasks_in_list (chpl_task_list_p task_list) {
  // task_list points to the last entry on the list; task_list->next is actually
  // the first element on the list.
  chpl_task_list_p task = task_list, next_task;
  // This function is not expected to be called if a cobegin contains fewer
  // than two statements; a coforall, however, may generate just one task,
  // or even none at all.
  if (task == NULL)
    return;
  assert(task->next);
  next_task = task->next;  // next_task now points to the head of the list

  // If the serial state is true, the tasks in task_list have already been executed.
  if (!chpl_get_serial()) do {

    task = next_task;
    next_task = task->next;

    // don't lock unnecessarily
    if (task->task_pool_entry) {
      chpl_fn_p task_to_run_fun = NULL;
      void* task_to_run_arg = NULL;

      // begin critical section
      chpl_mutex_lock(&threading_lock);

      if (task->task_pool_entry) {
        assert(!task->task_pool_entry->begun);
        task_to_run_fun = task->task_pool_entry->fun;
        task_to_run_arg = task->task_pool_entry->arg;
        task->task_pool_entry->begun = true;
        task->task_pool_entry->task_list_entry = NULL;
        // there is no longer any need to access the corresponding task pool entry
        // so avoid any potential of accessing a node that will eventually be freed
        task->task_pool_entry = NULL;
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

  } while (task != task_list);
}

void chpl_free_task_list (chpl_task_list_p task_list) {
  // task_list points to the last entry on the list; task_list->next is actually
  // the first element on the list.
  chpl_task_list_p task = task_list, next_task;
  // This function is not expected to be called if a cobegin contains fewer
  // than two statements; a coforall, however, may generate just one task,
  // or even none at all.
  if (task == NULL)
    return;
  assert(task->next);
  next_task = task->next;  // next_task now points to the head of the list

  do {
    task = next_task;
    next_task = task->next;
    chpl_free(task, 0, 0);
  } while (task != task_list);
}
