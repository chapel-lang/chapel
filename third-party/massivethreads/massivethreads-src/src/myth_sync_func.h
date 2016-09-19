/* 
 * myth_sync_func.h
 */
#pragma once
#ifndef MYTH_SYNC_FUNC_H_
#define MYTH_SYNC_FUNC_H_

#include "myth/myth.h"

#include "myth_config.h"
#include "myth_context.h"
#include "myth_worker.h"
#include "myth_thread.h"

#include "myth_context_func.h"
#include "myth_desc_func.h"
#include "myth_sleep_queue_func.h"
#include "myth_sched_func.h"

#include "myth_spinlock_func.h"

//#define ALIGN_MUTEX

/* ----------- common procedure for handling blocking/wake up ----------- */

/* put this thread in the q of waiting threads */
static inline long myth_sleep_queue_enq_th(myth_sleep_queue_t * q, 
					   myth_thread_t t) {
  return myth_sleep_queue_enq(q, (myth_sleep_queue_item_t)t);
}

/* dequeue a thread from the q of waiting threads;
   it returns null if q is found empty. */
static inline myth_thread_t myth_sleep_queue_deq_th(myth_sleep_queue_t * q) {
  return (myth_thread_t)myth_sleep_queue_deq(q);
}

/* put this thread in the q of waiting threads */
static inline long myth_sleep_stack_push_th(myth_sleep_stack_t * s, 
					    myth_thread_t t) {
  return myth_sleep_stack_push(s, (myth_sleep_queue_item_t)t);
}

/* dequeue a thread from the q of waiting threads;
   it returns null if q is found empty. */
static inline myth_thread_t myth_sleep_stack_pop_th(myth_sleep_stack_t * s) {
  return (myth_thread_t)myth_sleep_stack_pop(s);
}

/* called back right before switching context
   from the caller who is going to block to
   the next context to run. see the call to 
   myth_swap_context_withcall in myth_block_on_queue.
   myth_swap_context_withcall saves the context
   of the current thread, call myth_mutex_lock_1
   and then jump to the next context (another
   thread or the scheduler).  */

static inline int myth_mutex_unlock_body(myth_mutex_t * mutex);

MYTH_CTX_CALLBACK void myth_block_on_queue_cb(void *arg1,void *arg2,void *arg3) {
  myth_sleep_queue_t * q = arg1;
  myth_thread_t cur = arg2;
  myth_mutex_t * m = arg3;
  /* put the current thread to the sleep
     queue here.  it is important not to have
     done this until this point (i.e., after
     current thread's conetxt has been
     saved).  if we put it in the queue
     before calling context switch
     (myth_swap_context_withcall), another
     thread might unlock the mutex right
     after it enters the queue and access
     cur data structure before the context
     has been saved  */
  myth_sleep_queue_enq_th(q, cur);
  if (m) {
    myth_mutex_unlock_body(m);
  }
}

/* block the current thread on sleep_queue q */
static inline void myth_block_on_queue(myth_sleep_queue_t * q,
				       myth_mutex_t * m) {
  myth_running_env_t env = myth_get_current_env();
  myth_thread_t cur = env->this_thread;
  /* pop next thread to run */
  myth_thread_t next = myth_queue_pop(&env->runnable_q);
  /* next context to run. either another thread
     or the scheduler */
  myth_context_t next_ctx;
  env->this_thread = next;
  if (next) {
    /* a runnable thread */
    next->env = env;
    next_ctx = &next->context;
  } else {
    /* no runnable thread -> scheduler */
    next_ctx = &env->sched.context;
  }
  /* now save the current context, myth_sleep_queue_enq_th(q, cur)
     to put cur in the q, and jump to next_ctx */
  myth_swap_context_withcall(&cur->context, next_ctx,
			     myth_block_on_queue_cb, q, cur, m);
}


MYTH_CTX_CALLBACK void myth_block_on_stack_cb(void *arg1,void *arg2,void *arg3) {
  myth_sleep_stack_t * s = arg1;
  myth_thread_t cur = arg2;
  myth_mutex_t * m = arg3;
  /* put the current thread to the sleep
     queue here.  it is important not to have
     done this until this point (i.e., after
     current thread's conetxt has been
     saved).  if we put it in the queue
     before calling context switch
     (myth_swap_context_withcall), another
     thread might unlock the mutex right
     after it enters the queue and access
     cur data structure before the context
     has been saved  */
  myth_sleep_stack_push_th(s, cur);
  if (m) {
    myth_mutex_unlock_body(m);
  }
}


/* block the current thread on sleep_queue q */
static inline void myth_block_on_stack(myth_sleep_stack_t * s,
				       myth_mutex_t * m) {
  myth_running_env_t env = myth_get_current_env();
  myth_thread_t cur = env->this_thread;
  /* pop next thread to run */
  myth_thread_t next = myth_queue_pop(&env->runnable_q);
  /* next context to run. either another thread
     or the scheduler */
  myth_context_t next_ctx;
  env->this_thread = next;
  if (next) {
    /* a runnable thread */
    next->env = env;
    next_ctx = &next->context;
  } else {
    /* no runnable thread -> scheduler */
    next_ctx = &env->sched.context;
  }
  /* now save the current context, myth_sleep_queue_enq_th(q, cur)
     to put cur in the q, and jump to next_ctx */
  myth_swap_context_withcall(&cur->context, next_ctx,
			     myth_block_on_stack_cb, s, cur, m);
}


typedef void * (*callback_on_wakeup_t)(void *);

/* wake up exactly one thread from the queue.
   you must gurantee there is one in the queue,
   or an attempt is perhaps concurrently made
   to insert one. wait until the queue becomes
   non-empty. */

static inline int myth_wake_one_from_queue(myth_sleep_queue_t * q,
					   callback_on_wakeup_t callback,
					   void * arg) {
  myth_running_env_t env = myth_get_current_env();
  /* wait until the queue becomes non-empty.
     necessary for example when lock/unlock
     are called almost at the same time on 
     a mutex. if lock was slightly ahead, the
     unlocker may observe the queue before
     the locker enters the queue */
  myth_thread_t to_wake = 0;
  while (!to_wake) {
    to_wake = myth_sleep_queue_deq_th(q);
  }
  /* wake up this guy */
  to_wake->env = env;
  /* do any action after dequeueing from the sleep queue
     but before really putting it in the run queue.
     (for mutex, we need to clear the locked flag
     exactly at this moment; (i) doing so before
     dequeueing from the sleep queue allows other
     thread to lock the mutex, unlock it, and dequeue
     the thread I am trying to wake; (ii) doing so
     after putting it in the run queue will allow
     the to_wake to run (by another worker), observe
     the lock bit still set, and sleep again. */
  if (callback) {
    callback(arg);
  }
  /* put the thread to wake up in run queue */
  myth_queue_push(&env->runnable_q, to_wake);
  return 1;
}

/* wake up exactly n threads from the q.
   used e.g., by barrier, where the thread
   that enters the barrier last knows how many
   threads should wake up */
static inline int myth_wake_many_from_queue(myth_sleep_queue_t * q,
					    callback_on_wakeup_t callback,
					    void * arg,
					    long n) {
  /* the following works only when some measures are taken to 
     guarantee that thread to wake up do not reenter the queue.
     currently, this procedure is called from barrier wait,
     when the caller finds it is the last guy so needs to 
     wake up others. a race would arise if those waking threads
     call barrier_wait again on the same barrier and reenter
     the queue, possibly BEFORE threads that should wake up now.

     thread 1 .. n
        barrier_wait
     thread 0
        barrier_wait -> wake up 1 .. n, but a thread, say thread 1,
                        has not yet entered the queue
     thread 1
        woke up and barrier_wait again

     in our current implementation, barrier_wait ensures that
     threads that enter barrier_wait while the wake up is
     in progress waits until all threads have been dequeued
     from the sleep queue.  this is done by temporarily making
     the state of barrier invalid, and having threads that
     see invalid state wait for it to become valid again.

     alternatively, we could (i) dequeue all threads without
     waking them up and (ii) putting them in the run queue.
  */
  myth_running_env_t env = myth_get_current_env();
  /* wait until the queue becomes non-empty.
     necessary for example when lock/unlock
     are called almost at the same time on 
     a mutex. if lock was slightly ahead, the
     unlocker may observe the queue before
     the locker enters the queue */
  myth_thread_t to_wake_head = 0;
  myth_thread_t to_wake_tail = 0;
  long i;
  for (i = 0; i < n; i++) {
    myth_thread_t to_wake = 0;
    while (!to_wake) {
      to_wake = myth_sleep_queue_deq_th(q);
    }
    to_wake->env = env;
    to_wake->next = 0;
    if (to_wake_tail) {
      to_wake_tail->next = to_wake;
    } else {
      to_wake_head = to_wake;
    }
    to_wake_tail = to_wake;
  }
  /* do any action after dequeueing from the sleep queue
     but before really putting it in the run queue.
     (for mutex, we need to clear the locked flag
     exactly at this moment; (i) doing so before
     dequeueing from the sleep queue allows other
     thread to lock the mutex, unlock it, and dequeue
     the thread I am trying to wake; (ii) doing so
     after putting it in the run queue will allow
     the to_wake to run (by another worker), observe
     the lock bit still set, and sleep again. */
  if (callback) {
    callback(arg);
  }
  /* put the thread to wake up in run queue */
  myth_thread_t to_wake = to_wake_head;
  for (i = 0; i < n; i++) {
    assert(to_wake);
    myth_thread_t next = to_wake->next;
    myth_queue_push(&env->runnable_q, to_wake);
    to_wake = next;
  }
  return n;
}

/* wake up if any thread in the queue.
   used e.g., by a cond_signal which wakes up 
   if any thread is waiting.  */
static inline int myth_wake_if_any_from_queue(myth_sleep_queue_t * q,
					      callback_on_wakeup_t callback,
					      void * arg) {
  myth_running_env_t env = myth_get_current_env();
  myth_thread_t to_wake = myth_sleep_queue_deq_th(q);
  /* no threads sleeping, done */
  if (!to_wake) return 0;	/* I did not wake up any */
  to_wake->env = env;
  /* any action after dequeue but before really waking him up */
  if (callback) {
    callback(arg);
  }
  /* put the thread that just woke up to the run queue */
  myth_queue_push(&env->runnable_q, to_wake);
  return 1;			/* I woke up one */
}

/* wake up all threads waiting on q.
   return the number of threads that it has waken  */
static inline int myth_wake_all_from_queue(myth_sleep_queue_t * q,
					   callback_on_wakeup_t callback,
					   void * arg) {
  int n = 0;
  while (1) {
    if (myth_wake_if_any_from_queue(q, callback, arg) == 0) {
      break;
    }
    n++;
  }
  return n;			
}


static inline int myth_wake_many_from_stack(myth_sleep_stack_t * s,
					    callback_on_wakeup_t callback,
					    void * arg,
					    long n) {
  /* the following works only when some measures are taken to 
     guarantee that thread to wake up do not reenter the queue.
     currently, this procedure is called from barrier wait,
     when the caller finds it is the last guy so needs to 
     wake up others. a race would arise if those waking threads
     call barrier_wait again on the same barrier and reenter
     the queue, possibly BEFORE threads that should wake up now.

     thread 1 .. n
        barrier_wait
     thread 0
        barrier_wait -> wake up 1 .. n, but a thread, say thread 1,
                        has not yet entered the queue
     thread 1
        woke up and barrier_wait again

     in our current implementation, barrier_wait ensures that
     threads that enter barrier_wait while the wake up is
     in progress waits until all threads have been dequeued
     from the sleep queue.  this is done by temporarily making
     the state of barrier invalid, and having threads that
     see invalid state wait for it to become valid again.

     alternatively, we could (i) dequeue all threads without
     waking them up and (ii) putting them in the run queue.
  */
  myth_running_env_t env = myth_get_current_env();
  /* wait until the queue becomes non-empty.
     necessary for example when lock/unlock
     are called almost at the same time on 
     a mutex. if lock was slightly ahead, the
     unlocker may observe the queue before
     the locker enters the queue */
  myth_thread_t to_wake_head = 0;
  myth_thread_t to_wake_tail = 0;
  long i;
  for (i = 0; i < n; i++) {
    myth_thread_t to_wake = 0;
    while (!to_wake) {
      to_wake = myth_sleep_stack_pop_th(s);
    }
    to_wake->env = env;
    to_wake->next = 0;
    if (to_wake_tail) {
      to_wake_tail->next = to_wake;
    } else {
      to_wake_head = to_wake;
    }
    to_wake_tail = to_wake;
  }
  /* do any action after dequeueing from the sleep queue
     but before really putting it in the run queue.
     (for mutex, we need to clear the locked flag
     exactly at this moment; (i) doing so before
     dequeueing from the sleep queue allows other
     thread to lock the mutex, unlock it, and dequeue
     the thread I am trying to wake; (ii) doing so
     after putting it in the run queue will allow
     the to_wake to run (by another worker), observe
     the lock bit still set, and sleep again. */
  if (callback) {
    callback(arg);
  }
  /* put the thread to wake up in run queue */
  myth_thread_t to_wake = to_wake_head;
  for (i = 0; i < n; i++) {
    assert(to_wake);
    myth_thread_t next = to_wake->next;
    myth_queue_push(&env->runnable_q, to_wake);
    to_wake = next;
  }
  return n;
}

/* ----------- once ----------- */

static inline int myth_once_try_set(myth_once_t * once_control,
				    int old, int new) {
  return __sync_bool_compare_and_swap(&once_control->state, old, new);
}

static inline int myth_once_wait_until(myth_once_t * once_control,
				       int state) {
  int s = once_control->state;
  while (s != state) {
    myth_yield();
    s = once_control->state;
  }
  return 0;
}

static inline int
myth_once_body(myth_once_t * once_control, void (*init_routine)(void)) {
  int s = once_control->state;
  if (s == myth_once_state_init) {
   if (myth_once_try_set(once_control, myth_once_state_init,
			 myth_once_state_in_progress)) {
     init_routine();
     once_control->state = myth_once_state_completed;
     return 0;
   }
  }
  myth_once_wait_until(once_control, myth_once_state_completed);
  return 0;
}

/* ----------- mutex ----------- */

/* initialize mutex */
static inline int
myth_mutexattr_init_body(myth_mutexattr_t *attr);

static inline int myth_mutex_init_body(myth_mutex_t * mutex, 
				       const myth_mutexattr_t * attr) {
  myth_sleep_queue_init(mutex->sleep_q);
  mutex->state = 0;
  if (attr) {
    mutex->attr = *attr;
  } else {
    myth_mutexattr_init_body(&mutex->attr);
  }
  return 0;
}

/* destroy mutex */
static inline int myth_mutex_destroy_body(myth_mutex_t * mutex)
{
  myth_sleep_queue_destroy(mutex->sleep_q);
  return 0;
}

static inline int myth_mutex_trylock_body(myth_mutex_t * mutex) {
  /* TODO: spin block */
  while (1) {
    long s = mutex->state;
    /* check the lock bit */
    if (s & 1) {
      /* lock bit set. do nothing and go home */
      return EBUSY;
    } else if (__sync_bool_compare_and_swap(&mutex->state, s, s + 1)) {
      /* I set the lock bit */
      return 0;
    } else {
      continue;
    }
  }
}

/* lock mutex.

   state = 2 * (the number of threads blocked
                on the mutex)
         + locked_bit (1 if locked)

   note that a mutex may be not locked (the
   last bit is zero) even if there are
   blocked threads in the queue.  this
   happens right after unlocking a mutex that
   has more than one threads blocked on
   it. we unblock one of them AND CLEAR THE
   LOCKED BIT.  we could leave the bit set,
   so that the unblocked thread is guaranteed
   to be the next one to obtain the lock.
   however, this approach leads to a live
   lock if another thread repeats
   mutex_trylock on such a mutex.  we instead
   clear the locked bit and let the unblocked
   one compete with other threads trying to
   lock it.  this approach may lead to
   spuriously waking up many threads only one
   of whom may be able to acquire a lock.
 */


/* 
   0 : lock is free; no one in the queue
   1 : lock is held; no one in the queue
   2 : lock is free; some in the queue
   3 : lock is held; some in the queue
 */
static inline int myth_mutex_lock_body(myth_mutex_t * mutex) {
  /* TODO: spin block */
  while (1) {
    long s = mutex->state;
    assert(s >= 0);
    /* check lock bit */
    if ((s & 1) == 0) {
      /* lock bit clear -> try to become the one who set it */
      if (__sync_bool_compare_and_swap(&mutex->state, s, s + 1)) {
	break;
      }
    } else {
      /* lock bit set. indicate I am going to block on it.
	 I am competing with a thread who is trying to unlock it */
      if (__sync_bool_compare_and_swap(&mutex->state, s, s + 2)) {
	/* OK, I reserved a seat in the queue. even if the mutex is
	   unlocked by another thread right after the above cas, 
	   he will learn I am going to be in the queue soon, so should
	   wake me up */
	myth_block_on_queue(mutex->sleep_q, 0);
      }
    }
  }
  return 0;
}
  
/* timedlock
   the current implementation repeats trylock and then yield,
   until it succeds trylock.
   we want to make it block on the mutex, so that it will
   promptly wake up when the mutex is unlocked.
*/
static inline int
myth_mutex_timedlock_body(myth_mutex_t * mutex,
			  const struct timespec *restrict abstime) {
  if (myth_mutex_trylock_body(mutex) == 0) {
    return 0;
  } else {
    struct timespec tp[1];
    while (1) {
      int err = clock_gettime(CLOCK_REALTIME, tp);
      assert(err == 0);
      if (myth_timespec_gt(tp, abstime)) return ETIMEDOUT;
      if (myth_mutex_trylock_body(mutex) == 0) {
	return 0;
      } else {
	myth_yield_ex_body(myth_yield_option_local_first);
      }
    }
  }
}

/* clear lock bit, done after dequeueing
   a thread to wake up from the sleep queue and
   before putting it in the run queue */
static void * myth_mutex_clear_lock_bit(void * mutex_) {
  myth_mutex_t * mutex = mutex_;
  assert(mutex->state & 1);
  __sync_fetch_and_sub(&mutex->state, 1);
  return 0;
}

/* unlock a mutex */
static inline int myth_mutex_unlock_body(myth_mutex_t * mutex) {
  while (1) {
    long s = mutex->state;
    /* the mutex must be locked now (by me). 
       TODO: a better diagnosis message */
    if (!(s & 1)) {
      /* lock bit clear. the programmer must have called this
	 on a mutex that is not locked */
      fprintf(stderr, "myth_mutex_unlock : called on unlocked mutex, abort.\n");
      /* TODO: call myth_abort() rather than directly calling exit
	 show more useful info */
      exit(1);
    }
    if (s > 1) {
      /* some threads are blocked (or just have decided to block) 
	 on the queue. decrement it (while still keeping the lock bit)
	 wake up one, and then clear the lock bit */
      if (__sync_bool_compare_and_swap(&mutex->state, s, s - 2)) {
	// myth_mutex_wake_one_from_queue(mutex);
	myth_wake_one_from_queue(mutex->sleep_q, 
				 myth_mutex_clear_lock_bit, mutex);
	break;
      }
    } else {
      /* nobody waiting. clear the lock bit and done */
      assert(s == 1);
      if (__sync_bool_compare_and_swap(&mutex->state, 1, 0)) {
	break;
      }
    }
  }
  return 0;
}

static inline int
myth_mutexattr_init_body(myth_mutexattr_t *attr) {
  attr->type = MYTH_MUTEX_DEFAULT;
  return 0;
}

static inline int
myth_mutexattr_destroy_body(myth_mutexattr_t *attr) {
  (void)attr;
  return 0;
}

static inline int
myth_mutexattr_gettype_body(const myth_mutexattr_t *restrict attr,
			    int *restrict type) {
  *type = attr->type;
  return 0;
}

static inline int
myth_mutexattr_settype_body(myth_mutexattr_t *attr, int type) {
  attr->type = type;
  return 0;
}

/* ----------- reader-writer lock ----------- */

static inline int unimplemented(void) {
  assert(0);
  return -1;
}

static inline int
myth_rwlockattr_init_body(myth_rwlockattr_t *attr);

static inline int
myth_rwlock_init_body(myth_rwlock_t *restrict rwlock,
		      const myth_rwlockattr_t *restrict attr) {
  myth_sleep_queue_init(rwlock->sleep_q);
  rwlock->state = 0;
  if (attr) {
    rwlock->attr = *attr;
  } else {
    myth_rwlockattr_init_body(&rwlock->attr);
  }
  return 0;
}
// myth_rwlock_t rwlock = MYTH_RWLOCK_INITIALIZER;

static inline int
myth_rwlock_destroy_body(myth_rwlock_t *rwlock) {
  myth_sleep_queue_destroy(rwlock->sleep_q);
  return 0;
}

/* read-lock a reader-writer lock */
static inline int
myth_rwlock_rdlock_body(myth_rwlock_t *rwlock) {
  (void)rwlock;
  unimplemented();
  return 0;
}

/* try to read-lock a reader-writer lock */
static inline int
myth_rwlock_tryrdlock_body(myth_rwlock_t *rwlock) {
  (void)rwlock;
  unimplemented();
  return 0;
}

/* try to read-lock a reader-writer lock with time out */
static inline int
myth_rwlock_timedrdlock_body(myth_rwlock_t *restrict rwlock,
			     const struct timespec *restrict abstime) {
  (void)rwlock;
  (void)abstime;
  unimplemented();
  return 0;
}

/* write-lock a reader-writer lock */
static inline int
myth_rwlock_wrlock_body(myth_rwlock_t *rwlock) {
  (void)rwlock;
  unimplemented();
  return 0;
}

/* try to write-lock a reader-writer lock */
static inline int
myth_rwlock_trywrlock_body(myth_rwlock_t *rwlock) {
  (void)rwlock;
  unimplemented();
  return 0;
}

/* try to write-lock a reader-writer lock with timeout */
static inline int
myth_rwlock_timedwrlock_body(myth_rwlock_t *restrict rwlock,
			     const struct timespec *restrict abstime) {
  (void)rwlock;
  (void)abstime;
  unimplemented();
  return 0;
}

static inline int
myth_rwlock_unlock_body(myth_rwlock_t *rwlock) {
  (void)rwlock;
  unimplemented();
  return 0;
}

static inline int
myth_rwlockattr_init_body(myth_rwlockattr_t *attr) {
  attr->kind = MYTH_RWLOCK_PREFER_READER;
  return 0;
}

static inline int
myth_rwlockattr_destroy_body(myth_rwlockattr_t *attr) {
  (void)attr;
  return 0;
}

static inline int
myth_rwlockattr_getkind_body(const myth_rwlockattr_t *attr, int *pref) {
  *pref = attr->kind;
  return 0;
}

static inline int
myth_rwlockattr_setkind_body(myth_rwlockattr_t *attr, int pref) {
  attr->kind = pref;
  return 0;
}

/* ----------- condition variable ----------- */

static inline int
myth_condattr_init_body(myth_condattr_t *attr);

  static inline int myth_cond_init_body(myth_cond_t * cond, 
				      const myth_condattr_t * attr) {
  myth_sleep_queue_init(cond->sleep_q);
  if (attr) {
    cond->attr = *attr;
  } else {
    myth_condattr_init_body(&cond->attr);
  }
  return 0;
}

static inline int myth_cond_destroy_body(myth_cond_t * cond) {
  myth_sleep_queue_destroy(cond->sleep_q);
  return 0;
}

static inline int myth_cond_broadcast_body(myth_cond_t * cond) {
  myth_wake_all_from_queue(cond->sleep_q, 0, 0);
  return 0;
}

static inline int myth_cond_signal_body(myth_cond_t * cond) {
  myth_wake_if_any_from_queue(cond->sleep_q, 0, 0);
  return 0;
}

static inline int myth_cond_wait_body(myth_cond_t * cond, myth_mutex_t * mutex) {
  myth_block_on_queue(cond->sleep_q, mutex);
  return myth_mutex_lock(mutex);
}

static inline int
myth_cond_timedwait_body(myth_cond_t * cond, myth_mutex_t * mutex,
			 const struct timespec *restrict abstime) {
  (void)cond;
  (void)mutex;
  (void)abstime;
  unimplemented();
  myth_block_on_queue(cond->sleep_q, mutex);
  return myth_mutex_lock(mutex);
}

static inline int
myth_condattr_init_body(myth_condattr_t *attr) {
  (void)attr;
  return 0;
}

static inline int
myth_condattr_destroy_body(myth_condattr_t *attr) {
  (void)attr;
  return 0;
}

/* ----------- barrier ----------- */

static inline int myth_barrierattr_init_body(myth_barrierattr_t *attr);

  static inline int myth_barrier_init_body(myth_barrier_t * barrier, 
					 const myth_barrierattr_t * attr, 
					 long n_threads) {
  //myth_sleep_queue_init(barrier->sleep_q);
  myth_sleep_stack_init(barrier->sleep_s);
  /* 2 *(number of threads that reached) + invalid */
  barrier->state = 0;
  barrier->n_threads = n_threads;
  if (attr) {
    barrier->attr = *attr;
  } else {
    myth_barrierattr_init_body(&barrier->attr);
  }
  return 0;
}

static inline int myth_barrier_destroy_body(myth_barrier_t * barrier) {
  assert(barrier->state == 0);
  //myth_sleep_queue_destroy(barrier->sleep_q);
  myth_sleep_stack_destroy(barrier->sleep_s);
  return 0;
}

static inline int myth_barrier_wait_body(myth_barrier_t * barrier) {
  while (1) {
    long c = barrier->state;
    if (c >= barrier->n_threads) {
      /* TODO: set errno and return */
      fprintf(stderr, 
	      "myth_barrier_wait : excess threads (> %ld) enter barrier_wait\n",
	      barrier->n_threads);
      exit(1);
    }
    if (! __sync_bool_compare_and_swap(&barrier->state, c, c + 1)) {
      continue;
    }
    if (c == barrier->n_threads - 1) {
      /* I am the last one. wake up all guys.
	 TODO: spin block */
      barrier->state = 0;	/* reset state */
      //myth_wake_many_from_queue(barrier->sleep_q, 0, 0, c);
      myth_wake_many_from_stack(barrier->sleep_s, 0, 0, c);
      return MYTH_BARRIER_SERIAL_THREAD;
    } else {
      //myth_block_on_queue(barrier->sleep_q, 0);
      myth_block_on_stack(barrier->sleep_s, 0);
      return 0;
    }
  }
}

static inline int myth_barrierattr_init_body(myth_barrierattr_t *attr) {
  (void)attr;
  return 0;
}

static inline int myth_barrierattr_destroy_body(myth_barrierattr_t *attr) {
  (void)attr;
  return 0;
}

/* ----------- join counter ----------- */

/* calc number of bits enough to represent x
   (1 -> 1, 2 -> 2, 3 -> 2, 4 -> 3, etc.)
*/
static inline int calc_bits(long x) {
  int b = 0;
  while (x >= (1L << b)) {
    b++;
  }
  assert(x < (1L << b));
  return b;
}

static inline int myth_join_counterattr_init_body(myth_join_counterattr_t * attr);

static inline int
myth_join_counter_init_body(myth_join_counter_t * jc, 
			    const myth_join_counterattr_t * attr, 
			    long n_threads) {
  //Create a join counter with initial value val
  //val must be positive
  int b = calc_bits(n_threads);
  long mask = (1L << b) - 1;
  myth_sleep_queue_init(jc->sleep_q);
  jc->n_threads = n_threads;
  jc->n_threads_bits = b;
  jc->state_mask = mask;
  assert((n_threads & mask) == n_threads);
  /* number of waiters|number of decrements so far */
  jc->state = 0;
  if (attr) {
    jc->attr = *attr;
  } else {
    myth_join_counterattr_init_body(&jc->attr);
  }
  return 0;
}

static inline int myth_join_counter_wait_body(myth_join_counter_t * jc) {
  while (1) {
    long s = jc->state;
    if ((s & jc->state_mask) == jc->n_threads) {
      return 0;
    }
    /* try to indicate I am going to sleep. */
    long new_s = s + (1L << jc->n_threads_bits);
    if (! __sync_bool_compare_and_swap(&jc->state, s, new_s)) {
      /* another thread may have just decrement it, so I may
	 have to keep going */
      continue;
    }
    myth_block_on_queue(jc->sleep_q, 0);
    assert((jc->state & jc->state_mask) == jc->n_threads);
  }
}

static inline int myth_join_counter_dec_body(myth_join_counter_t * jc) {
  while (1) {
    long s = jc->state;
    long n_decs = s & jc->state_mask;
    if (n_decs >= jc->n_threads) {
      /* TODO: set errno and return */
      fprintf(stderr, 
	      "myth_join_counter_dec : excess threads (> %ld) enter join_counter_dec\n",
	      jc->n_threads);
      exit(1);
    }
    assert(((s + 1) & jc->state_mask) == (n_decs + 1));
    if (!__sync_bool_compare_and_swap(&jc->state, s, s + 1)) {
      continue;
    }
    if (n_decs == jc->n_threads - 1) {
      /* I am the last one. wake up all guys.
	 TODO: spin block */
      long n_threads_to_wake = (s >> jc->n_threads_bits);
      myth_wake_many_from_queue(jc->sleep_q, 0, 0, n_threads_to_wake);
    }
    break;
  }
  return 0;
}

static inline int myth_join_counterattr_init_body(myth_join_counterattr_t * attr) {
  (void)attr;
  return 0;
}

static inline int myth_join_counterattr_destroy_body(myth_join_counterattr_t * attr) {
  (void)attr;
  return 0;
}


/* ----------- felock----------- */
static inline int myth_felock_init_body(myth_felock_t * fe,
					const myth_felockattr_t * attr) {
  myth_mutex_init_body(fe->mutex, 0);
  myth_cond_init_body(&fe->cond[0], 0);
  myth_cond_init_body(&fe->cond[1], 0);
  fe->status = 0;
  if (attr) {
    fe->attr = *attr;
  } else {
    myth_felockattr_init(&fe->attr);
  }
  return 0;
}

static inline int myth_felock_destroy_body(myth_felock_t * fe) {
  myth_mutex_destroy_body(fe->mutex);
  myth_cond_destroy_body(&fe->cond[0]);
  myth_cond_destroy_body(&fe->cond[1]);
  return 0;
}

static inline int myth_felock_lock_body(myth_felock_t * fe) {
  return myth_mutex_lock_body(fe->mutex);
}

static inline int myth_felock_unlock_body(myth_felock_t * fe) {
  return myth_mutex_unlock_body(fe->mutex);
}

static inline int myth_felock_wait_and_lock_body(myth_felock_t * fe, 
						 int status_to_wait) {
  myth_mutex_lock_body(fe->mutex);
  while (fe->status != status_to_wait) {
    myth_cond_wait(&fe->cond[status_to_wait], fe->mutex);
  }
  return 0;
}

static inline int myth_felock_mark_and_signal_body(myth_felock_t * fe,
						   int status_to_signal) {
  fe->status = status_to_signal;
  myth_cond_signal(&fe->cond[status_to_signal]);
  return myth_mutex_unlock_body(fe->mutex);
}

static inline int myth_felock_status_body(myth_felock_t * fe) {
  return fe->status;
}

static inline int myth_felockattr_init_body(myth_felockattr_t * attr) {
  (void)attr;
  return 0;
}

static inline int myth_felockattr_destroy_body(myth_felockattr_t * attr) {
  (void)attr;
  return 0;
}

#endif /* MYTH_SYNC_FUNC_H_ */
