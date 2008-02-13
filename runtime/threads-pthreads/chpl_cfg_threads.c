// Pthread implementation

#include "chplrt.h"
#include "chplthreads.h"
#include "chplmem.h"
#include "error.h"
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>


typedef struct {
  _chpl_threadfp_t  fun;                   // function to fork as a new thread
  _chpl_threadarg_t arg;                   // argument for the created thread
} _chpl_createarg_t;

struct task_pool {
  _chpl_createarg_t *nt;
  struct task_pool  *next;
};


// The global vars are to synchronize with threads created with 
// begin's which are not joined.  We need to wait on them before the
// main thread can call exit for the process.
static _chpl_mutex_t     _chpl_begin_cnt_lock; // protect _chpl_begin_cnt
static int               _chpl_begin_cnt;      // number of unjoined threads 
static _chpl_condvar_t   _chpl_can_exit;       // can main thread exit?
static pthread_key_t     _chpl_serial;         // per-thread serial state
static struct task_pool *task_pool_head, *task_pool_tail;
static unsigned int      task_pool_cnt;        // number of tasks in pool 


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

static void _chpl_serial_delete(_chpl_bool *p) {
  if (NULL != p) {
    _chpl_free(p, 0, 0);
  }
}

void initChplThreads() {
  _chpl_mutex_init(&_chpl_begin_cnt_lock);
  _chpl_begin_cnt = task_pool_cnt = 0;          // only main thread running
  task_pool_head = task_pool_tail = NULL;
  pthread_cond_init(&_chpl_can_exit, NULL);

  _chpl_mutex_init(&_memtrack_lock);
  _chpl_mutex_init(&_memstat_lock);
  _chpl_mutex_init(&_memtrace_lock);

  if (pthread_key_create(&_chpl_serial, (void(*)(void*))_chpl_serial_delete))
    _printInternalError("serial key not created");
  _chpl_thread_init();
}


void exitChplThreads() {
  _chpl_mutex_lock(&_chpl_begin_cnt_lock);
  if (_chpl_begin_cnt > 0) {
    // block until everyone else is finished
    pthread_cond_wait(&_chpl_can_exit, &_chpl_begin_cnt_lock);
  }
  _chpl_mutex_unlock(&_chpl_begin_cnt_lock);

  pthread_key_delete(_chpl_serial);
}


void _chpl_thread_init(void) {}  // No need to do anything!


uint64_t _chpl_thread_id(void) {
  return (intptr_t) pthread_self();
}


_chpl_bool _chpl_get_serial(void) {
  _chpl_bool *p;
  p = (_chpl_bool*) pthread_getspecific(_chpl_serial);
  return p == NULL ? false : *p;
}

void _chpl_set_serial(_chpl_bool state) {
  _chpl_bool *p;
  p = (_chpl_bool*) pthread_getspecific(_chpl_serial);
  if (p == NULL) {
    if (state) {
      p = (_chpl_bool*) _chpl_alloc(sizeof(_chpl_bool), "serial flag", 0, 0);
      *p = state;
      if (pthread_setspecific(_chpl_serial, p))
        _printInternalError("serial state not created");
    }
  }
  else *p = state;
}


// Appends the given task to the end of the task pool.
static void
add_to_task_pool (_chpl_createarg_t *nt) {
  struct task_pool *task =
    (struct task_pool *) _chpl_malloc(1, sizeof(struct task_pool), "task pool entry", 0, 0);
  task->nt = nt;
  task->next = NULL;
  if (task_pool_tail) {
    task_pool_tail->next = task;
  } else {
    task_pool_head = task;  // No tasks were in the task pool!
  }
  task_pool_tail = task;
  task_pool_cnt++;
}

static void _chpl_begin_helper (_chpl_createarg_t *);

// Returns true if next task from task pool was launched successfully, false otherwise.
static _chpl_bool
launch_next_task(void) {
  pthread_t      thread;
  struct task_pool *task = task_pool_head;
  if (pthread_create(&thread, NULL, (_chpl_threadfp_t) _chpl_begin_helper, task->nt))
    return false;
  else {
    _chpl_begin_cnt++;
    pthread_detach(thread);
    if (task_pool_tail == task_pool_head)  // Only one task in task pool!
      task_pool_tail = task_pool_head = NULL;
    else task_pool_head = task_pool_head->next;
    task_pool_cnt--;
    _chpl_free(task, 0, 0);
    return true;
  }
}

// Will call the real begin statement function. Only purpose of this
// helper thread is to wait on that function and coordinate the exiting
// of the main Chapel thread.
static void
_chpl_begin_helper (_chpl_createarg_t *nt) {

  (*nt->fun)(nt->arg);
  _chpl_free(nt, 0, 0);

  // decrement begin thread count and see if we can signal Chapel exit
  _chpl_mutex_lock(&_chpl_begin_cnt_lock);

  _chpl_begin_cnt--;
  if (task_pool_cnt) {
    if (!launch_next_task() && _chpl_begin_cnt == 0)  // No way to retry launching!
      _printInternalError("unable to launch task from task pool");
  } else if (_chpl_begin_cnt == 0) {
    pthread_cond_signal(&_chpl_can_exit);
  }

  _chpl_mutex_unlock(&_chpl_begin_cnt_lock);
}


int
_chpl_begin (_chpl_threadfp_t fp, _chpl_threadarg_t a) {
  pthread_t      thread;
  _chpl_createarg_t  *nt;

  if (_chpl_get_serial()) {
    (*fp)(a);
  } else {
    nt = (_chpl_createarg_t*) _chpl_malloc(1, sizeof(_chpl_createarg_t), "_chpl_begin helper arg", 0, 0);
    nt->fun = fp;
    nt->arg = a;

    _chpl_mutex_lock(&_chpl_begin_cnt_lock);
    if (_chpl_begin_cnt >= maxThreads
        || pthread_create(&thread, NULL, (_chpl_threadfp_t) _chpl_begin_helper, nt))
      add_to_task_pool(nt);
    else {
      _chpl_begin_cnt++;
      pthread_detach(thread);
    }
    _chpl_mutex_unlock(&_chpl_begin_cnt_lock);
  }
  return 0;
}
