//
// Pthread implementation of Chapel threading interface
//

#include "chplrt.h"
#include "chplthreads.h"
#include "chplmem.h"
#include "error.h"
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

//
// task: a function and an argument to the function
//
typedef struct _chpl_task_struct* task_p;
typedef struct _chpl_task_struct {
  _chpl_threadfp_t  fun;
  _chpl_threadarg_t arg;
} task_t;

//
// task pool: linked list of tasks
//
typedef struct _chpl_pool_struct* task_pool_p;
typedef struct _chpl_pool_struct {
  task_p task;
  task_pool_p next;
} task_pool_t;


static _chpl_mutex_t   threading_lock; // critical section lock
static _chpl_condvar_t wakeup_signal;  // signal a waiting thread
static _chpl_condvar_t exit_signal;    // local threads completed
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
    _printInternalError("pthread_cond_init() failed");
  return cv;
}


// Mutex

static void _chpl_mutex_init(_chpl_mutex_p mutex) {
  // WAW: how to explicitly specify blocking-type?
  if (pthread_mutex_init(mutex, NULL))
    _printInternalError("pthread_mutex_init() failed");
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
    _printInternalError("pthread_mutex_lock() failed");
  return return_value;
}

void _chpl_mutex_unlock(_chpl_mutex_p mutex) {
  if (pthread_mutex_unlock(mutex))
    _printInternalError("pthread_mutex_unlock() failed");
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
      _printInternalError("pthread_cond_wait() failed");
  }
  return return_value;
}

int _chpl_sync_wait_empty_and_lock(_chpl_sync_aux_t *s, int32_t lineno, _string filename) {
  int return_value = _chpl_mutex_lock(s->lock);
  while (return_value == 0 && s->is_full) {
    if ((return_value = pthread_cond_wait(s->signal_empty, s->lock)))
      _printInternalError("pthread_cond_wait() failed");
  }
  return return_value;
}

void _chpl_sync_mark_and_signal_full(_chpl_sync_aux_t *s) {
  s->is_full = true;
  _chpl_sync_unlock(s);
  if (pthread_cond_signal(s->signal_full))
    _printInternalError("pthread_cond_signal() failed");
}

void _chpl_sync_mark_and_signal_empty(_chpl_sync_aux_t *s) {
  s->is_full = false;
  _chpl_sync_unlock(s);
  if (pthread_cond_signal(s->signal_empty))
    _printInternalError("pthread_cond_signal() failed");
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
      _printInternalError("invalid mutex in _chpl_single_wait_full");
  }
  return return_value;
}

void _chpl_single_mark_and_signal_full(_chpl_single_aux_t *s) {
  s->is_full = true;
  _chpl_mutex_unlock(s->lock);
  if (pthread_cond_signal(s->signal_full))
    _printInternalError("pthread_cond_signal() failed");
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

static void serial_delete(_chpl_bool *p) {
  if (NULL != p) {
    _chpl_free(p, 0, 0);
  }
}

void initChplThreads() {
  _chpl_mutex_init(&threading_lock);
  running_cnt = 0;                     // only main thread running
  threads_cnt = 0;
  task_pool_head = task_pool_tail = NULL;
  pthread_cond_init(&exit_signal, NULL);

  _chpl_mutex_init(&_memtrack_lock);
  _chpl_mutex_init(&_memstat_lock);
  _chpl_mutex_init(&_memtrace_lock);

  if (pthread_key_create(&serial_key, (void(*)(void*))serial_delete))
    _printInternalError("serial key not created");
  _chpl_thread_init();
}


void exitChplThreads() {
  // begin critical section
  _chpl_mutex_lock(&threading_lock);
  if (running_cnt > 0 || task_pool_head) {
    // block until everyone else is finished
    pthread_cond_wait(&exit_signal, &threading_lock);
  }
  _chpl_mutex_unlock(&threading_lock);
  // end critical section

  pthread_key_delete(serial_key);
}


void _chpl_thread_init(void) {}  // No need to do anything!


uint64_t _chpl_thread_id(void) {
  return (intptr_t) pthread_self();
}


_chpl_bool _chpl_get_serial(void) {
  _chpl_bool *p;
  p = (_chpl_bool*) pthread_getspecific(serial_key);
  return p == NULL ? false : *p;
}

void _chpl_set_serial(_chpl_bool state) {
  _chpl_bool *p;
  p = (_chpl_bool*) pthread_getspecific(serial_key);
  if (p == NULL) {
    if (state) {
      p = (_chpl_bool*) _chpl_alloc(sizeof(_chpl_bool), "serial flag", 0, 0);
      *p = state;
      if (pthread_setspecific(serial_key, p))
        _printInternalError("serial state not created");
    }
  }
  else *p = state;
}


//
// appends the given task to the end of the task pool
//
static void
add_to_task_pool (task_p task) {
  task_pool_p pool;

  pool = (task_pool_p)_chpl_malloc(1, sizeof(task_pool_t), "task pool entry", 0, 0);
  pool->task = task;
  pool->next = NULL;

  // begin critical section
  _chpl_mutex_lock(&threading_lock);

  if (task_pool_tail) {
    task_pool_tail->next = pool;
  } else {
    task_pool_head = pool;
  }
  task_pool_tail = pool;

  // end critical section
  _chpl_mutex_unlock(&threading_lock);
}


//
// thread wrapper function runs the user function, waits for more
// tasks, and runs those as they become available
//
static void
_chpl_begin_helper (task_p task) {
  task_pool_p pool;

  while (1) {
    (*task->fun)(task->arg);
    _chpl_free(task, 0, 0);

    // begin critical section
    _chpl_mutex_lock(&threading_lock);

    //
    // finished task; decrement running count
    //
    running_cnt--;

    //
    // signal exit if there are no other running threads and no tasks
    // in the task pool
    //
    if (running_cnt == 0 && !task_pool_head)
      pthread_cond_signal(&exit_signal);

    //
    // wait for a task to be added to the task pool
    //
    while (!task_pool_head) {
      pthread_cond_wait(&wakeup_signal, &threading_lock);
    }

    //
    // start new task; increment running count and remove task from
    // pool
    //
    running_cnt++;
    pool = task_pool_head;
    if (task_pool_tail == task_pool_head)
      task_pool_tail = task_pool_head = NULL;
    else
      task_pool_head = task_pool_head->next;

    // end critical section
    _chpl_mutex_unlock(&threading_lock);

    //
    // reset task pointer and serial state
    //
    task = pool->task;
    _chpl_free(pool, 0, 0);
    _chpl_set_serial(false);
  }
}


//
// run task in task pool in an existing thread if one is waiting or in
// a new thread if the number of maximum threads has not yet been reached
//
static void
launch_next_task(void) {
  pthread_t      thread;
  task_pool_p pool;

  // begin critical section
  _chpl_mutex_lock(&threading_lock);

  if (task_pool_head && threads_cnt > running_cnt) {
    //
    // signal thread to wake up and grab new task if a task exists in
    // the pool and there is a waiting thread
    //
    pthread_cond_signal(&wakeup_signal);

  } else if (task_pool_head && (maxThreads == 0 || running_cnt < maxThreads)) {
    //
    // start a new thread if a task exists in the pool, there are no
    // waiting threads, and the number of running threads is less than
    // the maximum
    //
    pool = task_pool_head;
    if (pthread_create(&thread, NULL, (_chpl_threadfp_t)_chpl_begin_helper, pool->task)) {
      char msg[256];
      sprintf(msg, "pthread_create failed with %d running threads", running_cnt);
      _printInternalError(msg);
    }
    threads_cnt++;
    running_cnt++;
    pthread_detach(thread);
    if (task_pool_tail == task_pool_head)
      task_pool_tail = task_pool_head = NULL;
    else task_pool_head = task_pool_head->next;
    _chpl_free(pool, 0, 0);
  }

  // end critical section
  _chpl_mutex_unlock(&threading_lock);
}


//
// interface function with begin-statement
//
int
_chpl_begin (_chpl_threadfp_t fp, _chpl_threadarg_t a) {
  task_p task;

  if (_chpl_get_serial()) {
    (*fp)(a);
  } else {
    task = (task_p)_chpl_malloc(1, sizeof(task_t), "_chpl_begin helper arg", 0, 0);
    task->fun = fp;
    task->arg = a;
    add_to_task_pool(task);
    launch_next_task();
  }
  return 0;
}
