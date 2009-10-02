#ifndef _chpl_threads_generic_h_
#define _chpl_threads_generic_h_

#include <stdint.h>


//
// The generic implementation of threading is a least-common-denominator
// version of threading whose purpose is to minimize the work needed to
// get Chapel tasking working on top of some new threading layer.  The
// generic implementation supplies all of the types in the public (to the
// rest of Chapel) threading interface, plus versions of all of the
// functions in that interface, with "_generic" appended to their names.
// If the threading layer #defines GENERIC_THREADING_SATISFIES_INTERFACE,
// these generic versions of the functions satisfy the public interface.
// Otherwise, the threading layer must supply these functions itself,
// with "_" and the expansion of CHPL_THREADS appended to their names,
// and these versions satisfy the interface.
//
// The threading layer only has to supply a small amount of support:
// mutexes, functions to retrieve thread identifiers and to cancel and
// join with specific threads, and some supplementary types and callback
// functions.  The complete list is:
//
// For mutexes
//   type(s)
//     chpl_mutex_t
//   functions
//     CHPL_MUTEX_INIT()
//     CHPL_MUTEX_NEW()
//     CHPL_MUTEX_LOCK()
//     CHPL_MUTEX_UNLOCK()
//
// For thread management
//   type(s)
//     <none>
//   functions
//     CHPL_THREAD_ID()
//     CHPL_THREAD_CANCEL()
//     CHPL_THREAD_JOIN()
//
// For sync variables
//   type(s)
//     threadlayer_sync_aux_t
//   functions
//     threadlayer_sync_suspend()
//     threadlayer_sync_awaken()
//     threadlayer_init_sync()
//     threadlayer_destroy_sync()
//
// For single variables
//   type(s)
//     threadlayer_single_aux_t
//   functions
//     threadlayer_single_suspend()
//     threadlayer_single_awaken()
//     threadlayer_init_single()
//     threadlayer_destroy_single()
//
// For task management
//   type(s)
//     <none>
//   functions
//     threadlayer_init()
//     threadlayer_thread_create()
//     threadlayer_pool_suspend()
//     threadlayer_pool_awaken()
//     threadlayer_get_thread_private_data()
//     threadlayer_set_thread_private_data()
//
// The mutex type, mutex functions, and thread management functions are
// fundamental and are declared in chplthreads.h.  The other types are
// declared in the chpl_cfg_threads.h file for each specific threading
// layer, and the callback functions are declared here.  The interfaces
// and requirements for these other types and callback functions are
// described elsewhere in this file.
//
// Although the above list may seem long, in practice many of the
// functions are quite simple, and with luck also easily extrapolated
// from what is done for other threading layers.  For an example of a
// minimal implementation, see "pthreads-minimal" threading.
//
// Also note that if a threading layer defines the public functions
// itself, it need not supply the callbacks needed by the generic
// implementation.
//
// Generally speaking the chpl_cfg_threads.h for the threading layer
// will define at least the types listed above, then #include this file,
// and then add any further declarations needed.  Note that among other
// things this file defines the chpl_sync_aux_t and chpl_single_aux_t
// types, so anything needing those declarations in chpl_cfg_threads.h
// must come after the point at which this file is #include'd into that
// one.
//


//
// Sync variables
//
// The threading layer's threadlayer_sync_aux_t may include any
// additional members the layer needs to support the suspend/awaken
// callbacks efficiently.  The generic threading code itself does
// not refer to this type or the tl_aux member at all.
//
typedef struct {
  volatile chpl_bool is_full;
  chpl_mutex_t* lock;
  threadlayer_sync_aux_t tl_aux;
} chpl_sync_aux_t;

void      chpl_sync_lock_generic(chpl_sync_aux_t *);
void      chpl_sync_unlock_generic(chpl_sync_aux_t *);
void      chpl_sync_wait_full_and_lock_generic(chpl_sync_aux_t *,
                                               int32_t, chpl_string);
void      chpl_sync_wait_empty_and_lock_generic(chpl_sync_aux_t *,
                                                int32_t, chpl_string);
void      chpl_sync_mark_and_signal_full_generic(chpl_sync_aux_t *);
void      chpl_sync_mark_and_signal_empty_generic(chpl_sync_aux_t *);
chpl_bool chpl_sync_is_full_generic(void *, chpl_sync_aux_t *, chpl_bool);
void      chpl_init_sync_aux_generic(chpl_sync_aux_t *);
void      chpl_destroy_sync_aux_generic(chpl_sync_aux_t *);


//
// Single variables
//
// The threading layer's threadlayer_single_aux_t may include any
// additional members the layer needs to support the suspend/awaken
// callbacks efficiently.  The generic threading code itself does not
// refer to this type or the tl_aux member at all.
//
typedef struct {
  volatile chpl_bool is_full;
  chpl_mutex_t* lock;
  threadlayer_single_aux_t tl_aux;
} chpl_single_aux_t;

void      chpl_single_lock_generic(chpl_single_aux_t *);
void      chpl_single_unlock_generic(chpl_single_aux_t *);
void      chpl_single_wait_full_generic(chpl_single_aux_t *,
                                        int32_t, chpl_string);
void      chpl_single_mark_and_signal_full_generic(chpl_single_aux_t *);
chpl_bool chpl_single_is_full_generic(void *, chpl_single_aux_t *, chpl_bool);
void      chpl_init_single_aux_generic(chpl_single_aux_t *);
void      chpl_destroy_single_aux_generic(chpl_single_aux_t *);


// Tasks

uint32_t chpl_numQueuedTasks_generic(void);
uint32_t chpl_numRunningTasks_generic(void);
int32_t  chpl_numBlockedTasks_generic(void);

void      chpl_tasking_init_generic(void);
void      chpl_tasking_exit_generic(void);

chpl_bool chpl_get_serial_generic(void);
void      chpl_set_serial_generic(chpl_bool);

typedef struct chpl_task_list* chpl_task_list_generic_p;

void chpl_add_to_task_list_generic(chpl_fn_int_t, void*,
                                   chpl_task_list_generic_p*, int32_t,
                                   chpl_bool, int, chpl_string);
void chpl_process_task_list_generic(chpl_task_list_generic_p);
void chpl_execute_tasks_in_list_generic(chpl_task_list_generic_p);
void chpl_free_task_list_generic(chpl_task_list_generic_p);

void chpl_begin_generic(chpl_fn_p, void*, chpl_bool, chpl_bool,
                        chpl_task_list_generic_p);


// Threads

int32_t  chpl_threads_getMaxThreads_generic(void);
int32_t  chpl_threads_maxThreadsLimit_generic(void);
uint32_t chpl_numThreads_generic(void);
uint32_t chpl_numIdleThreads_generic(void);


//
// Handy services for threading layer callback functions.
//
// The generic threading implementation also provides the following
// service routines that can be used by threading layer callback
// functions.
//

//
// Is the task pool empty?
//
chpl_bool chpl_pool_is_empty(void);


//
// The remaining declarations are all for callback functions to be
// provided by the threading layer.
//

//
// These are called once each, from chpl_tasking_init_generic() and
// chpl_tasking_exit_generic().
//
void threadlayer_init(void);
void threadlayer_exit(void);


//
// Sync variables
//
// The chpl_sync_wait_{full,empty}_and_lock_generic() functions call
// threadlayer_sync_suspend() when a sync variable is not in the desired
// full/empty state.  The call will be made with the sync variable's
// mutex held.  (Thus, threadlayer_sync_suspend() can dependably tell
// that the desired state must be the opposite of the state it initially
// sees the variable in.)  It should return (with the mutex again held)
// as soon as it can once either the sync variable changes to the
// desired state, or (if the given deadline pointer is non-NULL) the
// deadline passes.  It can return also early, before either of these
// things occur, with no ill effects.  If a deadline is given and it
// does pass, then threadlayer_sync_suspend() must return true;
// otherwise false.
//
// The less the function can execute while waiting for the sync variable
// to change state, and the quicker it can un-suspend when the variable
// does change state, the better overall performance will be.  Obviously
// the sync variable's mutex must be unlocked while the routine waits
// for the variable to change state or the deadline to pass, or livelock
// may result.
//
// The chpl_sync_mark_and_signal_{full,empty}() functions will call
// threadlayer_sync_awaken() every time they are called, not just when
// they change the state of the sync variable.
//
// Threadlayer_{init,destroy}_sync() are called to initialize or
// destroy, respectively, the contents of the tl_aux member of the
// chpl_sync_aux_t for the specific threading layer.
//
chpl_bool threadlayer_sync_suspend(chpl_sync_aux_t *s,
                                   struct timeval *deadline);
void threadlayer_sync_awaken(chpl_sync_aux_t *s);
void threadlayer_init_sync(chpl_sync_aux_t *s);
void threadlayer_destroy_sync(chpl_sync_aux_t *s);


//
// Single variables
//
// Analogous to the sync case, the chpl_single_wait_full_generic()
// function calls threadlayer_single_suspend() when a single variable is
// not full.  The call will be made with the single variable's mutex
// held.  It should return (with the mutex again held) as soon as it can
// once either the single variable becomes full, or (if the given
// deadline pointer is non-NULL) the deadline passes.  It can return also
// early, before either of these things occur, with no ill effects.  If a
// deadline is given and it does pass, then threadlayer_single_suspend()
// must return true; otherwise false.
//
// The less the function can execute while waiting for the single
// variable to become full, and the quicker it can un-suspend when the
// variable does become full, the better overall performance will be.
// Obviously the single variable's mutex must be unlocked while the
// routine waits for the variable to become full or the deadline to
// pass, or livelock may result.
//
// The chpl_single_mark_and_signal_full_generic() function will call
// threadlayer_single_awaken() every time it is called, not just when it
// fills the single variable.
//
// Threadlayer_{init,destroy}_single() are called to initialize or
// destroy, respectively, the contents of the tl_aux member of the
// chpl_single_aux_t for the specific threading layer.
//
chpl_bool threadlayer_single_suspend(chpl_single_aux_t *s,
                                     struct timeval *deadline);
void threadlayer_single_awaken(chpl_single_aux_t *s);
void threadlayer_init_single(chpl_single_aux_t *s);
void threadlayer_destroy_single(chpl_single_aux_t *s);


//
// Task management
//

//
// The interface for thread creation may need to be extended eventually
// to allow for specifying such things as stack sizes and/or locations.
//
int threadlayer_thread_create(void*, void*(*)(void*), void*);

//
// Threadlayer_pool_suspend() is called when a thread finds nothing in
// the pool of unclaimed tasks, and so has no work to do.  The call will
// be made with the pointed-to mutex held.  It should return (with the
// mutex again held) as soon as it can once either the task pool is no
// longer empty or (if the given deadline pointer is non-NULL) the
// deadline passes.  It can return also early, before either of these
// things occur, with no ill effects.  If a deadline is given and it
// does pass, then threadlayer_pool_suspend() must return true;
// otherwise false.
//
// The less the function can execute while waiting for the pool to
// become nonempty, and the quicker it can un-suspend when that happens,
// the better overall performance will be.
//
// The mutex passed to threadlayer_pool_suspend() is the one that
// provides mutual exclusion for changes to the task pool.  Allowing
// access to this mutex simplifies the implementation for certain
// threading layers, such as those based on pthreads condition
// variables.  However, it also introduces a complication in that it
// allows a threading layer to create deadlock or livelock situations if
// it is not careful.  Certainly the mutex must be unlocked while the
// routine waits for the task pool to fill or the deadline to pass, or
// livelock may result.
//
// Note that the generic implementation provides chpl_pool_is_empty(),
// which the suspend callback can use to tell when the pool becomes
// nonempty.
//
chpl_bool threadlayer_pool_suspend(chpl_mutex_t*, struct timeval*);
void threadlayer_pool_awaken(void);


//
// Thread private data
//
// These set and get a pointer to thread private data associated with
// each thread.  This is for the use of the generic implementation of
// threading itself.  If the threading layer also needs to store some
// data private to each thread, it must make other arrangements to do
// so.
//
void  threadlayer_set_thread_private_data(void*);
void* threadlayer_get_thread_private_data(void);

#endif
