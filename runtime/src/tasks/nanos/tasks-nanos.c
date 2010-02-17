//
// Nanos implementation of Chapel tasking interface
//

#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
#define NDEBUG
#endif

#include "chpl_rt_utils.h"
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

#include <nanos.h>

//
// // This is the data that is private to each thread.
// typedef struct {
//   chpl_bool     serial_state;
//   lockReport_t* lockRprt;
// } thread_private_data_t;
// 


// Mutexes

void CHPL_MUTEX_INIT(chpl_mutex_t* m) {
    threadlayer_mutex_init((threadlayer_mutex_t*) m);
}

chpl_mutex_t* CHPL_MUTEX_NEW(void) {
    return (chpl_mutex_t*) threadlayer_mutex_new();
}

void CHPL_MUTEX_LOCK(chpl_mutex_t* m) {
    threadlayer_mutex_lock((threadlayer_mutex_t*) m);
}

void CHPL_MUTEX_UNLOCK(chpl_mutex_t* m) {
    threadlayer_mutex_unlock((threadlayer_mutex_t*) m);
}


// Sync variables

static void sync_wait_and_lock(chpl_sync_aux_t *s,
                               chpl_bool want_full,
                               int32_t lineno, chpl_string filename) {
  nanos_wait_on_bool(&s->is_full,want_full);
}

void CHPL_SYNC_LOCK(chpl_sync_aux_t *s) {
  CHPL_MUTEX_LOCK(s->lock);
}

void CHPL_SYNC_UNLOCK(chpl_sync_aux_t *s) {
  CHPL_MUTEX_UNLOCK(s->lock);
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
  s->lock = CHPL_MUTEX_NEW();
  threadlayer_sync_init(s);
}

void CHPL_SYNC_DESTROY_AUX(chpl_sync_aux_t *s) {
  chpl_free(s->lock, 0, 0);
  threadlayer_sync_destroy(s);
}


// Single variables

void CHPL_SINGLE_LOCK(chpl_single_aux_t *s) {
  CHPL_MUTEX_LOCK((chpl_mutex_t*)s->lock);
}

void CHPL_SINGLE_UNLOCK(chpl_single_aux_t *s) {
  CHPL_MUTEX_UNLOCK((chpl_mutex_t*)s->lock);
}

void CHPL_SINGLE_WAIT_FULL(chpl_single_aux_t *s,
                           int32_t lineno, chpl_string filename) {

  nanos_wait_on_bool(&s->is_full,1);
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
  s->lock = CHPL_MUTEX_NEW();
  threadlayer_single_init(s);
}

void CHPL_SINGLE_DESTROY_AUX(chpl_single_aux_t *s) {
  chpl_free(s->lock, 0, 0);
  threadlayer_single_destroy(s);
}


// Tasks

void CHPL_TASKING_INIT() {
  if (blockreport || taskreport) {
    chpl_internal_error("blockreport/taskreport not implemented");
  }


/*  tp = chpl_alloc(sizeof(thread_private_data_t), CHPL_RT_MD_THREAD_PRIVATE_DATA, 0, 0);
  threadlayer_set_thread_private_data(tp);
  tp->serial_state = false;*/
}


void CHPL_TASKING_EXIT() {
}


void CHPL_ADD_TO_TASK_LIST(chpl_fn_int_t fid, void* arg,
                           chpl_task_list_p *task_list,
                           int32_t task_list_locale,
                           chpl_bool call_chpl_begin,
                           int lineno, chpl_string filename) {
    chpl_fn_p fp = chpl_ftable[fid];
    CHPL_BEGIN(fp, arg, false, false, NULL);
}

void CHPL_PROCESS_TASK_LIST(chpl_task_list_p task_list) {
}

void CHPL_EXECUTE_TASKS_IN_LIST(chpl_task_list_p task_list) {
 
}


void CHPL_FREE_TASK_LIST(chpl_task_list_p task_list) {
}


//
// interface function with begin-statement
//
void CHPL_BEGIN(chpl_fn_p fp, void* a,
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
chpl_taskID_t CHPL_TASK_ID(void) {
  return 0;
}

//TODO
void CHPL_TASK_SLEEP(int secs) {
  sleep(secs);
}

//TODO
chpl_bool CHPL_GET_SERIAL(void) {
  return 0;
}

//TODO
void CHPL_SET_SERIAL(chpl_bool state) {
}

uint32_t CHPL_NUMQUEUEDTASKS(void) {
   chpl_internal_error("CHPL_NUMQUEUEDTASKS not implemented");
   return 0;
}

uint32_t CHPL_NUMRUNNINGTASKS(void) {
  int num;
  NANOS_SAFE(nanos_get_num_running_tasks(&num));
  return num;
}

int32_t  CHPL_NUMBLOCKEDTASKS(void) {
  chpl_internal_error("CHPL_NUMBLOCKEDTASKS not implemented");
  return 0;
}

// Threads

int32_t  CHPL_THREADS_GETMAXTHREADS(void) { return 0; }

int32_t  CHPL_THREADS_MAXTHREADSLIMIT(void) { return 0; }

// take the main thread into account
uint32_t CHPL_NUMTHREADS(void) {
    chpl_internal_error("CHPL_NUMTHREADS not implemented");
    return 0;
}

uint32_t CHPL_NUMIDLETHREADS(void) {
    chpl_internal_error("CHPL_NUMIDLETHREADS not implemented");
    return 0;
}

