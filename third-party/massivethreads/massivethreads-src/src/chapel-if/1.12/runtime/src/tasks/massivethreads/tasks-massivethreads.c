#define _GNU_SOURCE

#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
#ifndef NDEBUG
#define NDEBUG
#endif
#endif

#if 1
#include "chplrt.h"
#include "chpl_rt_utils_static.h"
#include "chplcgfns.h"
#include "chpl-comm.h"
#include "chplexit.h"
#include "chpl-locale-model.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"
#include "chpl-tasks-callbacks-internal.h"
#include "chplsys.h"
#include "chpl-linefile-support.h"
#include "error.h"

#else

#include "chpl-mem.h"
#include "chpl-comm.h"
#include "chplcast.h"
#include "c hplrt.h"
#include "chpl-tasks.h"
#include "config.h"
#include "error.h"

#endif

#include <assert.h>
#include <stdint.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sched.h>
#include <stdio.h>
//#include <dlfcn.h>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>

#include "myth/myth.h"

void chpl_sync_lock(chpl_sync_aux_t *s);
void chpl_sync_unlock(chpl_sync_aux_t *s);
void chpl_sync_waitFullAndLock(chpl_sync_aux_t *s, int32_t lineno, int32_t filename);
void chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *s, int32_t lineno, int32_t filename);
void chpl_sync_markAndSignalFull(chpl_sync_aux_t *s);
void chpl_sync_markAndSignalEmpty(chpl_sync_aux_t *s);
chpl_bool chpl_sync_isFull(void *val_ptr, chpl_sync_aux_t *s);
void chpl_sync_initAux(chpl_sync_aux_t *s);
void chpl_sync_destroyAux(chpl_sync_aux_t *s);
void chpl_task_init(void);
void chpl_task_exit(void);
void chpl_task_callMain(void (*chpl_main)(void));
void chpl_task_stdModulesInitialized(void);
int chpl_task_createCommTask(chpl_fn_p fn, void* arg);
void chpl_task_addToTaskList(chpl_fn_int_t fid, void* arg, c_sublocid_t subloc,
                             void** p_task_list_void, int32_t task_list_locale,
                             chpl_bool is_begin_stmt, int lineno, int32_t filename);
void chpl_task_executeTasksInList(void** p_task_list_void);
void chpl_task_startMovedTask(chpl_fn_p fp, void* a, c_sublocid_t subloc,
                              chpl_taskID_t id, chpl_bool serial_state);
chpl_taskID_t chpl_task_getId(void);
void chpl_task_yield(void);
void chpl_task_sleep(double secs);
chpl_bool chpl_task_getSerial(void);
void chpl_task_setSerial(chpl_bool state);
uint32_t chpl_task_getMaxPar(void);
c_sublocid_t chpl_task_getNumSublocales(void);
chpl_task_prvData_t* chpl_task_getPrvData(void);
size_t chpl_task_getCallStackSize(void);
uint32_t chpl_task_getNumQueuedTasks(void);
uint32_t chpl_task_getNumRunningTasks(void);
int32_t  chpl_task_getNumBlockedTasks(void);
uint32_t chpl_task_getNumThreads(void);
uint32_t chpl_task_getNumIdleThreads(void);

static void enter(const char * function_name) {
  fprintf(stderr, "enter       %s:\n", function_name);
}

static void return_from(const char * function_name) {
  fprintf(stderr, "return_from %s:\n", function_name);
}

// Sync variables
void chpl_sync_lock(chpl_sync_aux_t *s) {
  //Simple mutex lock
  enter("chpl_sync_lock");
  myth_felock_lock(s->felock);
  return_from("chpl_sync_lock");
}

void chpl_sync_unlock(chpl_sync_aux_t *s) {
  //Simple mutex unlock
  enter("chpl_sync_unlock");
  myth_felock_unlock(s->felock);
  return_from("chpl_sync_unlock");
}

void chpl_sync_waitFullAndLock(chpl_sync_aux_t *s,
			       int32_t lineno, int32_t filename) {
  enter("chpl_sync_waitFullAndLock");
  //wait until F/E bit is empty, and acquire lock
  myth_felock_wait_and_lock(s->felock, 1);
  return_from("chpl_sync_waitFullAndLock");
}

void chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *s,
				int32_t lineno, int32_t filename) {
  enter("chpl_sync_waitEmptyAndLock");
  myth_felock_wait_and_lock(s->felock, 0);
  return_from("chpl_sync_waitEmptyAndLock");
}

void chpl_sync_markAndSignalFull(chpl_sync_aux_t *s) {
  //release lock and set F/E bit to full
  enter("chpl_sync_markAndSignalFull");
  myth_felock_mark_and_signal(s->felock, 1);
  return_from("chpl_sync_markAndSignalFull");
}

void chpl_sync_markAndSignalEmpty(chpl_sync_aux_t *s) {
  //release lock and set F/E bit to empty
  enter("chpl_sync_markAndSignalEmpty");
  myth_felock_mark_and_signal(s->felock, 0);
  return_from("chpl_sync_markAndSignalEmpty");
}

chpl_bool chpl_sync_isFull(void *val_ptr, chpl_sync_aux_t *s) {
  //return whether F/E bit is full or not
  chpl_bool x;
  enter("chpl_sync_isFull");
  x = myth_felock_status(s->felock);
  return_from("chpl_sync_isFull");
  return x;
}

void chpl_sync_initAux(chpl_sync_aux_t *s) {
  //init sync variable
  enter("chpl_sync_initAux");
  myth_felock_init(s->felock, 0);
  return_from("chpl_sync_initAux");
}

void chpl_sync_destroyAux(chpl_sync_aux_t *s) {
  //destroy sync variable
  enter("chpl_sync_destroyAux");
  myth_felock_destroy(s->felock);
  return_from("chpl_sync_destroyAux");
}

// Tasks
void chpl_task_init() {
  enter("chpl_task_init");
  myth_init();
  return_from("chpl_task_init");
}

void chpl_task_exit(void) {
  //Cleanup tasking layer
  enter("chpl_task_exit");
  myth_fini();
  return_from("chpl_task_exit");
}

void chpl_task_callMain(void (*chpl_main)(void)) {
  //Call main function
  enter("chpl_task_callMain");
  chpl_main();
  return_from("chpl_task_callMain");
}

void chpl_task_stdModulesInitialized(void) {
  enter("chpl_task_stdModulesInitialized");
  return_from("chpl_task_stdModulesInitialized");
}

int chpl_task_createCommTask(chpl_fn_p fn, void* arg) {
  enter("chpl_task_createCommTask");
  {
    myth_thread_t th = myth_create((void*(*)(void*))fn,arg);
    myth_detach(th);
  }
  return_from("chpl_task_createCommTask");
  return 0;
}

void chpl_task_addToTaskList(chpl_fn_int_t fid, void* arg, c_sublocid_t subloc,
                             void** p_task_list_void, int32_t task_list_locale,
                             chpl_bool is_begin_stmt,
			     int lineno, int32_t filename) {
  enter("chpl_task_addToTaskList");
  (*chpl_ftable[fid])(arg);
  return_from("chpl_task_addToTaskList");
}

void chpl_task_executeTasksInList(void ** task_list) {
  //Nothing to do because chpl_task_list is not used.
  enter("chpl_task_executeTasksInList");
  return_from("chpl_task_executeTasksInList");
}

void chpl_task_startMovedTask(chpl_fn_p fp, void* a, c_sublocid_t subloc,
                              chpl_taskID_t id, chpl_bool serial_state) {
  enter("chpl_task_startMovedTask");
  return_from("chpl_task_startMovedTask");
}

chpl_taskID_t chpl_task_getId(void) {
  myth_thread_t th;
  enter("chpl_task_getId");
  //get task private ID
  th = myth_self();
  return_from("chpl_task_getId");
  return (chpl_taskID_t)th;
}

void chpl_task_yield(void) {
  //yield execution to other tasks
  enter("chpl_task_yield");
  myth_yield(1);
  return_from("chpl_task_yield");
}

static double cur_time(void) {
  struct timeval tp[1];
  gettimeofday(tp, 0);
  return tp->tv_sec + 1.0e-6 * tp->tv_usec;
}

void chpl_task_sleep(double secs) {
  enter("chpl_task_sleep");
  {
    double end_t = cur_time() + secs;
    while (cur_time() < end_t) {
      myth_yield(1);
    }
  }
  return_from("chpl_task_sleep");
}

chpl_bool chpl_task_getSerial(void) {
  //get dynamic serial state
  enter("chpl_task_getSerial");
  return_from("chpl_task_getSerial");
  return 1;
}

void chpl_task_setSerial(chpl_bool new_state) {
  enter("chpl_task_setSerial");
  return_from("chpl_task_setSerial");
}

uint32_t chpl_task_getMaxPar(void) {
  enter("chpl_task_getMaxPar");
  return_from("chpl_task_getMaxPar");
  return 4;
}

c_sublocid_t chpl_task_getNumSublocales(void) {
  enter("chpl_task_getNumSublocales");
  return_from("chpl_task_getNumSublocales");
  return 0;
}

static chpl_task_prvData_t the_prvData[1];

chpl_task_prvData_t* chpl_task_getPrvData(void) {
  enter("chpl_task_getPrvData");
  return_from("chpl_task_getPrvData");
  return the_prvData;
}

uint64_t chpl_task_getCallStackSize(void) {
  //return call stack size
  uint64_t s;
  enter("chpl_task_getCallStackSize");
  s = 128 * 1024;
  return_from("chpl_task_getCallStackSize");
  return s;
}

uint32_t chpl_task_getNumQueuedTasks(void) {
  //return the number of queued tasks
  enter("chpl_task_getNumQueuedTasks");
  return_from("chpl_task_getNumQueuedTasks");
  return 0;
}

uint32_t chpl_task_getNumRunningTasks(void) {
  //return the number of running tasks
  enter("chpl_task_getNumRunningTasks");
  return_from("chpl_task_getNumRunningTasks");
  return 0;
}

int32_t  chpl_task_getNumBlockedTasks(void) {
  //return the number of blocked tasks
  enter("chpl_task_getNumBlockedTasks");
  return_from("chpl_task_getNumBlockedTasks");
  return 0;
}

uint32_t chpl_task_getNumThreads(void) {
  //return the number of threads (excluding a thread for comm)
  enter("chpl_task_getNumThreads");
  return_from("chpl_task_getNumThreads");
  return 1;
}

uint32_t chpl_task_getNumIdleThreads(void) {
  //return the number of idle threads
  enter("chpl_task_getNumIdleThreads");
  return_from("chpl_task_getNumIdleThreads");
  return 0;
}

