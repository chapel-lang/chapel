//
// Pthread implementation of Chapel threading interface
//

#include "chplrt.h"
#include "chplthreads.h"
#include "chplmem.h"
#include "error.h"
#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>

//
// task pool: linked list of tasks
//
typedef struct _chpl_pool_struct* task_pool_p;
typedef struct _chpl_pool_struct {
  _chpl_threadfp_t  fun;  // function to call for task
  _chpl_threadarg_t arg;  // argument to the function
  _Bool             serial_state; // whether new threads can be created while executing fun
  task_pool_p next;
} task_pool_t;


static _chpl_mutex_t   threading_lock; // critical section lock
static _chpl_condvar_t wakeup_signal;  // signal a waiting thread
static pthread_key_t   serial_key;     // per-thread serial state
static task_pool_p     task_pool_head; // head of task pool
static task_pool_p     task_pool_tail; // tail of task pool
static int             running_cnt;    // number of running threads 
static int             threads_cnt;    // number of threads

// Condition variables

static _chpl_condvar_p _chpl_condvar_new(void) {
  _chpl_condvar_p cv;
  cv = (_chpl_condvar_p) _chpl_alloc(sizeof(_chpl_condvar_t), "condition var", 0, 0);
  if (pthread_cond_init(cv, NULL))
    _chpl_internal_error("pthread_cond_init() failed");
  return cv;
}


// Mutex

static void _chpl_mutex_init(_chpl_mutex_p mutex) {
  // WAW: how to explicitly specify blocking-type?
  if (pthread_mutex_init(mutex, NULL))
    _chpl_internal_error("pthread_mutex_init() failed");
}

static _chpl_mutex_p _chpl_mutex_new(void) {
  _chpl_mutex_p m;
  m = (_chpl_mutex_p) _chpl_alloc(sizeof(_chpl_mutex_t), "mutex", 0, 0);
  _chpl_mutex_init(m);
  return m;
}

int _chpl_mutex_lock(_chpl_mutex_p mutex) {
  int return_value;
  if ((return_value = pthread_mutex_lock(mutex)))
    _chpl_internal_error("pthread_mutex_lock() failed");
  return return_value;
}

void _chpl_mutex_unlock(_chpl_mutex_p mutex) {
  if (pthread_mutex_unlock(mutex))
    _chpl_internal_error("pthread_mutex_unlock() failed");
}


// Sync variables

int _chpl_sync_lock(_chpl_sync_aux_t *s) {
  return _chpl_mutex_lock(s->lock);
}

void _chpl_sync_unlock(_chpl_sync_aux_t *s) {
  _chpl_mutex_unlock(s->lock);
}

int _chpl_sync_wait_full_and_lock(_chpl_sync_aux_t *s, int32_t lineno, _string filename) {
  int return_value = _chpl_mutex_lock(s->lock);
  while (return_value == 0 && !s->is_full) {
    if ((return_value = pthread_cond_wait(s->signal_full, s->lock)))
      _chpl_internal_error("pthread_cond_wait() failed");
  }
  return return_value;
}

int _chpl_sync_wait_empty_and_lock(_chpl_sync_aux_t *s, int32_t lineno, _string filename) {
  int return_value = _chpl_mutex_lock(s->lock);
  while (return_value == 0 && s->is_full) {
    if ((return_value = pthread_cond_wait(s->signal_empty, s->lock)))
      _chpl_internal_error("pthread_cond_wait() failed");
  }
  return return_value;
}

void _chpl_sync_mark_and_signal_full(_chpl_sync_aux_t *s) {
  s->is_full = true;
  _chpl_sync_unlock(s);
  if (pthread_cond_signal(s->signal_full))
    _chpl_internal_error("pthread_cond_signal() failed");
}

void _chpl_sync_mark_and_signal_empty(_chpl_sync_aux_t *s) {
  s->is_full = false;
  _chpl_sync_unlock(s);
  if (pthread_cond_signal(s->signal_empty))
    _chpl_internal_error("pthread_cond_signal() failed");
}

_chpl_bool _chpl_sync_is_full(void *val_ptr, _chpl_sync_aux_t *s, _chpl_bool simple_sync_var) {
  return s->is_full;
}

void _chpl_init_sync_aux(_chpl_sync_aux_t *s) {
  s->is_full = false;
  s->lock = _chpl_mutex_new();
  s->signal_full = _chpl_condvar_new();
  s->signal_empty = _chpl_condvar_new();
}


// Single variables

int _chpl_single_lock(_chpl_single_aux_t *s) {
  return _chpl_mutex_lock(s->lock);
}

int _chpl_single_wait_full(_chpl_single_aux_t *s, int32_t lineno, _string filename) {
  int return_value = _chpl_mutex_lock(s->lock);
  while (return_value == 0 && !s->is_full) {
    if ((return_value = pthread_cond_wait(s->signal_full, s->lock)))
      _chpl_internal_error("invalid mutex in _chpl_single_wait_full");
  }
  return return_value;
}

void _chpl_single_mark_and_signal_full(_chpl_single_aux_t *s) {
  s->is_full = true;
  _chpl_mutex_unlock(s->lock);
  if (pthread_cond_signal(s->signal_full))
    _chpl_internal_error("pthread_cond_signal() failed");
}

_chpl_bool _chpl_single_is_full(void *val_ptr, _chpl_single_aux_t *s, _chpl_bool simple_single_var) {
  return s->is_full;
}

void _chpl_init_single_aux(_chpl_single_aux_t *s) {
  s->is_full = false;
  s->lock = _chpl_mutex_new();
  s->signal_full = _chpl_condvar_new();
}


// Threads

static void serial_delete(_Bool *p) {
  if (p != NULL) {
    _chpl_free(p, 0, 0);
  }
}

int32_t _chpl_threads_getMaxThreads(void) { return 0; }
int32_t _chpl_threads_maxThreadsLimit(void) { return 0; }

void initChplThreads() {
  _chpl_mutex_init(&threading_lock);
  if (pthread_cond_init(&wakeup_signal, NULL))
    _chpl_internal_error("pthread_cond_init() failed in");
  running_cnt = 0;                     // only main thread running
  threads_cnt = 0;
  task_pool_head = task_pool_tail = NULL;

  _chpl_mutex_init(&_memtrack_lock);
  _chpl_mutex_init(&_memstat_lock);
  _chpl_mutex_init(&_memtrace_lock);

  if (pthread_key_create(&serial_key, (void(*)(void*))serial_delete))
    _chpl_internal_error("serial key not created");
  _chpl_thread_init();
}


void exitChplThreads() {
  pthread_key_delete(serial_key);
}


void _chpl_thread_init(void) {}  // No need to do anything!


uint64_t _chpl_thread_id(void) {
  return (intptr_t) pthread_self();
}


_chpl_bool _chpl_get_serial(void) {
  _Bool *p;
  p = (_Bool*) pthread_getspecific(serial_key);
  return p == NULL ? false : *p;
}

void _chpl_set_serial(_chpl_bool state) {
  _Bool *p;
  p = (_Bool*) pthread_getspecific(serial_key);
  if (p == NULL) {
    if (state) {
      p = (_Bool*) _chpl_alloc(sizeof(_Bool), "serial flag", 0, 0);
      *p = state;
      if (pthread_setspecific(serial_key, p)) {
        if (pthread_key_create(&serial_key, (void(*)(void*))serial_delete))
          _chpl_internal_error("serial key not created");
        else if (pthread_setspecific(serial_key, p))
          _chpl_internal_error("serial state not created");
      }
    }
  }
  else *p = state;
}


//
// thread wrapper function runs the user function, waits for more
// tasks, and runs those as they become available
//
static void
_chpl_begin_helper (task_pool_p task) {

  while (true) {
    //
    // reset serial state
    //
    _chpl_set_serial(task->serial_state);

    (*task->fun)(task->arg);

    // begin critical section
    _chpl_mutex_lock(&threading_lock);

    _chpl_free(task, 0, 0);  // make sure task_pool_head no longer points to this task!

    //
    // finished task; decrement running count
    //
    running_cnt--;

    //
    // wait for a task to be added to the task pool
    //
    while (!task_pool_head) {
      pthread_cond_wait(&wakeup_signal, &threading_lock);
    }

    //
    // start new task; increment running count and remove task from pool
    //
    running_cnt++;
    task = task_pool_head;
    task_pool_head = task_pool_head->next;
    if (task_pool_head == NULL)  // task pool is now empty
      task_pool_tail = NULL;

    // end critical section
    _chpl_mutex_unlock(&threading_lock);
  }
}


//
// run task in a new thread
// assumes at least one task is in the pool and threading_lock has already been acquired!
//
static void
launch_next_task(void) {
  pthread_t   thread;
  task_pool_p task = task_pool_head;
  if (pthread_create(&thread, NULL, (_chpl_threadfp_t)_chpl_begin_helper, task)) {
    static _Bool warning_issued = false;
    if (!warning_issued) {
      char msg[256];
      if (maxThreads)
        sprintf(msg, "maxThreads is %"PRId32", but unable to create more than %d threads",
                maxThreads, threads_cnt);
      else
        sprintf(msg, "maxThreads is unbounded, but unable to create more than %d threads",
                threads_cnt);
      _chpl_warning(msg, 0, 0);
      warning_issued = true;
    }
  } else {
    threads_cnt++;
    running_cnt++;
    pthread_detach(thread);
    task_pool_head = task_pool_head->next;
    if (task_pool_head == NULL)  // task pool is now empty
      task_pool_tail = NULL;
  }
}


//
// interface function with begin-statement
//
int
_chpl_begin (_chpl_threadfp_t fp, _chpl_threadarg_t a, _Bool serial_state) {

  // The thread that receives a request from another locale to start a new thread
  // (due to an on statement) always has the serial state set to false, causing
  // the specified task to always be placed in the task pool, rather than executed
  // immediately.

  if (_chpl_get_serial()) {
    (*fp)(a);
  } else {
    // create a task from the given function pointer and arguments
    // and append it to the end of the task pool
    task_pool_p task = (task_pool_p)_chpl_malloc(1, sizeof(task_pool_t), "task pool entry", 0, 0);
    task->fun = fp;
    task->arg = a;
    task->serial_state = serial_state;
    task->next = NULL;

    // begin critical section
    _chpl_mutex_lock(&threading_lock);

    if (task_pool_tail) {
      task_pool_tail->next = task;
    } else {
      task_pool_head = task;
    }
    task_pool_tail = task;

    // if there is an idle thread, send it a signal to wake up and grab a new task
    if (threads_cnt > running_cnt)
      pthread_cond_signal(&wakeup_signal);
    // otherwise, try to launch task in a new thread
    // if the maximum number threads has not yet been reached
    // take the main thread into account (but not when counting idle threads above)
    else if (maxThreads == 0 || threads_cnt + 1 < maxThreads)
      launch_next_task();

    // end critical section
    _chpl_mutex_unlock(&threading_lock);
  }
  return 0;
}
