//
// Generic implementation of Chapel threading interface
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

#include <nanos.h>

//
// // This is the data that is private to each thread.
// typedef struct {
//   chpl_bool     serial_state;
//   lockReport_t* lockRprt;
// } thread_private_data_t;
// 

// Sync variables

static void sync_wait_and_lock(chpl_sync_aux_t *s,
                               chpl_bool want_full,
                               int32_t lineno,
                               chpl_string filename) {
  nanos_wait_on_bool(&s->is_full,want_full);
}

void chpl_sync_lock_generic(chpl_sync_aux_t *s) {
  CHPL_MUTEX_LOCK(s->lock);
}

void chpl_sync_unlock_generic(chpl_sync_aux_t *s) {
  CHPL_MUTEX_UNLOCK(s->lock);
}

void chpl_sync_wait_full_and_lock_generic(chpl_sync_aux_t *s,
                                          int32_t lineno,
                                          chpl_string filename) {
  sync_wait_and_lock(s, true, lineno, filename);
}

void chpl_sync_wait_empty_and_lock_generic(chpl_sync_aux_t *s,
                                           int32_t lineno,
                                           chpl_string filename) {
  sync_wait_and_lock(s, false, lineno, filename);
}

void chpl_sync_mark_and_signal_full_generic(chpl_sync_aux_t *s) {
  s->is_full = true;
  threadlayer_sync_awaken(s);
  chpl_sync_unlock_generic(s);
}

void chpl_sync_mark_and_signal_empty_generic(chpl_sync_aux_t *s) {
  s->is_full = false;
  threadlayer_sync_awaken(s);
  chpl_sync_unlock_generic(s);
}

chpl_bool chpl_sync_is_full_generic(void *val_ptr,
                                    chpl_sync_aux_t *s,
                                    chpl_bool simple_sync_var) {
  return s->is_full;
}

void chpl_init_sync_aux_generic(chpl_sync_aux_t *s) {
  s->is_full = false;
  s->lock = CHPL_MUTEX_NEW();
  threadlayer_init_sync(s);
}

void chpl_destroy_sync_aux_generic(chpl_sync_aux_t *s) {
  chpl_free(s->lock, 0, 0);
  threadlayer_destroy_sync(s);
}


// Single variables

void chpl_single_lock_generic(chpl_single_aux_t *s) {
  CHPL_MUTEX_LOCK((chpl_mutex_t*)s->lock);
}

void chpl_single_unlock_generic(chpl_single_aux_t *s) {
  CHPL_MUTEX_UNLOCK((chpl_mutex_t*)s->lock);
}

void chpl_single_wait_full_generic(chpl_single_aux_t *s,
                                   int32_t lineno, chpl_string filename) {

  nanos_wait_on_bool(&s->is_full,1);
}

void chpl_single_mark_and_signal_full_generic(chpl_single_aux_t *s) {
  s->is_full = true;
  threadlayer_single_awaken(s);
  chpl_single_unlock_generic(s);
}

chpl_bool chpl_single_is_full_generic(void *val_ptr,
                                      chpl_single_aux_t *s,
                                      chpl_bool simple_single_var) {
  return s->is_full;
}

void chpl_init_single_aux_generic(chpl_single_aux_t *s) {
  s->is_full = false;
  s->lock = CHPL_MUTEX_NEW();
  threadlayer_init_single(s);
}

void chpl_destroy_single_aux_generic(chpl_single_aux_t *s) {
  chpl_free(s->lock, 0, 0);
  threadlayer_destroy_single(s);
}


// Tasks

void chpl_tasking_init_generic() {
  if (blockreport || taskreport) {
    chpl_internal_error("blockreport/taskreport not implemented");
  }


/*  tp = chpl_alloc(sizeof(thread_private_data_t), CHPL_RT_MD_THREAD_PRIVATE_DATA, 0, 0);
  threadlayer_set_thread_private_data(tp);
  tp->serial_state = false;*/
}


void chpl_tasking_exit_generic() {
}


void chpl_add_to_task_list_generic(chpl_fn_int_t fid, void* arg,
                                   chpl_task_list_p *task_list,
                                   int32_t task_list_locale,
                                   chpl_bool call_chpl_begin,
                                   int lineno,
                                   chpl_string filename) {

   
    chpl_fn_p fp = chpl_ftable[fid];
    CHPL_BEGIN(fp, arg, false, false, NULL);
}

void chpl_process_task_list_generic(chpl_task_list_p task_list) {
}

void chpl_execute_tasks_in_list_generic(chpl_task_list_p task_list) {
 
}


void chpl_free_task_list_generic(chpl_task_list_p task_list) {
}


//
// interface function with begin-statement
//
void chpl_begin_generic(chpl_fn_p fp, void* a,
                        chpl_bool ignore_serial,  // always add task to pool
                        chpl_bool serial_state,
                        chpl_task_list_p ltask) {

   assert(!ltask);
   
   nanos_wd_t wd=0;
   nanos_smp_args_t nanos_smp_arg = { (void (*) (void *))fp};
   nanos_device_t nanos_devices[] =
       { {nanos_smp_factory, &nanos_smp_arg } };

   nanos_wd_props_t props = {
     .mandatory_creation=true,
     .tie_to = 0,
     .tied = 0 };

   NANOS_SAFE(nanos_create_wd ( &wd, 1, nanos_devices , 0 ,
                     &a, nanos_current_wd(), &props ));

   NANOS_SAFE(nanos_submit(wd,0,0));
}


//TODO
chpl_bool chpl_get_serial_generic(void) {
  return 0;
}

void chpl_set_serial_generic(chpl_bool state) {
}

uint32_t chpl_numQueuedTasks_generic(void) {
   chpl_internal_error("chpl_numQueuedTasks_generic not implemented");
   return 0;
}

uint32_t chpl_numRunningTasks_generic(void) {
  int num;
  NANOS_SAFE(nanos_get_num_running_tasks(&num));
  return num;
}

int32_t  chpl_numBlockedTasks_generic(void) {
  chpl_internal_error("chpl_numBlockedTasks_generic not implemented");
  return 0;
}

// Threads

int32_t  chpl_threads_getMaxThreads_generic(void) { return 0; }

int32_t  chpl_threads_maxThreadsLimit_generic(void) { return 0; }

// take the main thread into account
uint32_t chpl_numThreads_generic(void) {
    chpl_internal_error("chpl_numThreads_generic not implemented");
    return 0;
}

uint32_t chpl_numIdleThreads_generic(void) {
    chpl_internal_error("chpl_numThreads_generic not implemented");
    return 0;
}

