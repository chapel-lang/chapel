#define _GNU_SOURCE

#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
#define NDEBUG
#endif

#include "chplrt.h"
#include "chplcast.h"
#include "chplcgfns.h" // for chpl_ftable
#include "chpl-comm.h"
#include "chpl-locale-model.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"
#include "error.h"
#include <assert.h>
#include <stdint.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sched.h>
#include <stdio.h>
#include <dlfcn.h>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>

#include "myth.h"

// When a task (=user-level thread) migrates with acquiring a
// pthread_mutex lock, the following pthread_mutex_unlock sometimes
// does not work correctly.
// For a workaround, MassiveThreads tasking layer suppresses task migrations
// when defined the following macro.
#define PTHREAD_MUTEX_OVERRIDE

typedef struct {
  c_sublocid_t requestedSubloc;  // requested sublocal for task
  chpl_bool    serial_state;     // true: serialize execution
}task_private_data_t;

typedef struct {
  // A task which acquires pthread_mutex_lock must not migrate to
  // another worker thread.
  int in_mutex_flag;
} thread_local_data;

static int tasking_layer_active = 0;
static int worker_in_cs_beforeinit = 0;
static thread_local_data* s_tld;

static const task_private_data_t s_def_chpl_data=
             { c_sublocid_any, false };

// task-private data of main task
static task_private_data_t s_main_chpl_data =
             { c_sublocid_any, false };

static const uint64_t c_def_stack_size = 32 * 1024 * sizeof(size_t);

static inline task_private_data_t* getTaskPrivateData(void) {
  if (tasking_layer_active){
    if (myth_wsapi_get_hint_size(myth_self())==sizeof(task_private_data_t)){
      return (task_private_data_t*)myth_wsapi_get_hint_ptr(myth_self());
    }
  }
  return &s_main_chpl_data;
}

#ifdef PTHREAD_MUTEX_OVERRIDE

static int is_worker_in_cs(void) {
  if (tasking_layer_active) {
    int rank = myth_get_worker_num();
    assert(s_tld[rank].in_mutex_flag>=0);
    return s_tld[rank].in_mutex_flag>0;
  } else{
    assert(worker_in_cs_beforeinit>=0);
    return worker_in_cs_beforeinit>0;
  }
}

// #define CS_WARNING

static void worker_enter_cs(void) {
  if (tasking_layer_active) {
    int rank = myth_get_worker_num();
    s_tld[rank].in_mutex_flag++;
#ifdef CS_WARNING
    if (s_tld[rank].in_mutex_flag>1){
      fprintf(stderr,"warning:duplicated enter to critical section worker: %d\n",rank);
    }
#endif
  } else {
    worker_in_cs_beforeinit++;
#ifdef CS_WARNING
    if (worker_in_cs_beforeinit<0){
      fprintf(stderr,"warning:duplicated enter to critical section \n");
    }
#endif
  }
}

static void worker_exit_cs(void) {
  if (tasking_layer_active) {
    int rank = myth_get_worker_num();
    s_tld[rank].in_mutex_flag--;
    if (s_tld[rank].in_mutex_flag<0){
#ifdef CS_WARNING
      fprintf(stderr,"warning:exit from empty critical section worker: %d\n",rank);
#endif
      s_tld[rank].in_mutex_flag=0;
    }
  } else {
    worker_in_cs_beforeinit--;
    if (worker_in_cs_beforeinit<0){
#ifdef CS_WARNING
      fprintf(stderr,"warning:exit from empty critical section \n");
#endif
      worker_in_cs_beforeinit=0;
    }
  }
}

#include <pthread.h>

static int (*pthread_mutex_lock_fp)(pthread_mutex_t *) = NULL;
static int (*pthread_mutex_trylock_fp)(pthread_mutex_t *) = NULL;
static int (*pthread_mutex_unlock_fp)(pthread_mutex_t *) = NULL;

int pthread_mutex_lock(pthread_mutex_t *mutex) {
  worker_enter_cs();
  if (!pthread_mutex_lock_fp) {
    pthread_mutex_lock_fp = dlsym(RTLD_NEXT, "pthread_mutex_lock");
    assert(pthread_mutex_lock_fp);
  }
  pthread_mutex_lock_fp(mutex);
  return 0;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex) {
  int ret;
  if (!pthread_mutex_trylock_fp) {
    pthread_mutex_trylock_fp = dlsym(RTLD_NEXT, "pthread_mutex_trylock");
    assert(pthread_mutex_lock_fp);
  }
  ret = pthread_mutex_trylock_fp(mutex);
  if (ret==0) worker_enter_cs();
  return ret;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) {
  if (!pthread_mutex_unlock_fp) {
    pthread_mutex_unlock_fp = dlsym(RTLD_NEXT, "pthread_mutex_unlock");
    assert(pthread_mutex_unlock_fp);
  }
  pthread_mutex_unlock_fp(mutex);
  worker_exit_cs();
  return 0;
}


#else

static int is_worker_in_cs(void) {
  return 0;
}

static void worker_enter_cs(void) {
}

static void worker_exit_cs(void) {
}

#endif

// Sync variables
void chpl_sync_lock(chpl_sync_aux_t *s) {
  //Simple mutex lock
  assert(!is_worker_in_cs());
  {
    myth_felock_lock(s->lock);
  }
}
void chpl_sync_unlock(chpl_sync_aux_t *s) {
  assert(!is_worker_in_cs());
  //Simple mutex unlock
  myth_felock_unlock(s->lock);
}

void chpl_sync_waitFullAndLock(chpl_sync_aux_t *s, int32_t lineno,
    chpl_string filename) {
  assert(!is_worker_in_cs());
  {
    //wait until F/E bit is empty, and acquire lock
    myth_felock_wait_lock(s->lock, 1);
  }
}

void chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *s, int32_t lineno,
    chpl_string filename) {
  assert(!is_worker_in_cs());
  {
    myth_felock_wait_lock(s->lock, 0);
  }
}

void chpl_sync_markAndSignalFull(chpl_sync_aux_t *s) {
  //release lock and set F/E bit to full
  myth_felock_set_unlock(s->lock, 1);
}

void chpl_sync_markAndSignalEmpty(chpl_sync_aux_t *s) {
  //release lock and set F/E bit to empty
  myth_felock_set_unlock(s->lock, 0);
}

chpl_bool chpl_sync_isFull(void *val_ptr, chpl_sync_aux_t *s) {
  //return whether F/E bit is full or not
  return myth_felock_status(s->lock);
}

void chpl_sync_initAux(chpl_sync_aux_t *s) {
  //init sync variable
  s->lock = myth_felock_create();
}

void chpl_sync_destroyAux(chpl_sync_aux_t *s) {
  //destroy sync variable
  myth_felock_destroy(s->lock);
}

static cpu_set_t worker_cpusets[CPU_SETSIZE];
static int available_cores = -1;

static void get_process_affinity_info(void) {
  cpu_set_t cset;
  int i;
  sched_getaffinity(getpid(), sizeof(cpu_set_t), &cset);
  for (i = 0; i < CPU_SETSIZE; i++) {
    CPU_ZERO(&worker_cpusets[i]);
  }
  available_cores = 0;
  for (i = 0; i < CPU_SETSIZE; i++) {
    if (CPU_ISSET(i, &cset)) {
      CPU_SET(i, &worker_cpusets[available_cores]);
      available_cores++;
    }
  }
}

//Return the number of CPU cores
static int get_cpu_num(void) {
  assert(available_cores > 0);
  return available_cores;
}

static int32_t s_num_workers;
static uint64_t s_stack_size;

// Tasks
void chpl_task_init(void) {
  //Initialize tasking layer
  //initializing change the number of workers
  int32_t numThreadsPerLocale;
  int numCommTasks = chpl_comm_numPollingTasks();
  uint64_t callStackSize;
  char *env;
  int i;
  void *chpl_data_ptr=&s_main_chpl_data;
  size_t chpl_data_size=sizeof(task_private_data_t);

  //
  // This threading layer does not have any inherent limit on the number
  // of threads.  Its limit is the lesser of any limits imposed by the
  // comm layer and the user.
  //
  uint32_t lim;
  // Exit from MassiveThreads, if activated.
  myth_fini();
  // get process affinity mask
  get_process_affinity_info();
  // Default, number of CPU cores
  numThreadsPerLocale = get_cpu_num();
  // override by MYTH_WORKER_NUM
  env = getenv("MYTH_WORKER_NUM");
  if (env) {
    if ((lim = atoi(env)) > 0)
      numThreadsPerLocale = lim;
  }
  // override by --numThreadsPerLocale
  if ((lim=chpl_task_getenvNumThreadsPerLocale()) > 0)
    numThreadsPerLocale = lim;
  // limit by comm layer limit
  if ((lim = chpl_comm_getMaxThreads()) > 0){
    numThreadsPerLocale = (numThreadsPerLocale > lim)?lim:numThreadsPerLocale;
  }

  s_num_workers = numThreadsPerLocale;
  // If callstack size is not specified by argument,
  //  try to read from environmental variable
  if ((callStackSize = chpl_task_getenvCallStackSize()) == 0) {
    env = getenv("MYTH_DEF_STKSIZE");
    if (env) {
      int i_stk = atoi(env);
      if (i_stk > 0)
        callStackSize = i_stk;
    }
  }
  //Otherwise use the default size
  if (callStackSize == 0) {
    callStackSize = c_def_stack_size;
  }
  s_stack_size = callStackSize;
  assert(!is_worker_in_cs());
  s_tld = chpl_mem_allocMany(numThreadsPerLocale + numCommTasks,
      sizeof(thread_local_data), 0, 0, "");
  for (i = 0; i < numThreadsPerLocale + numCommTasks; i++) {
    s_tld[i].in_mutex_flag = 0;
  }
  tasking_layer_active = 1;
  myth_init_withparam((int) (numThreadsPerLocale + numCommTasks), (size_t) callStackSize);
  // Assign task-private data to this main task
  s_main_chpl_data = s_def_chpl_data;
  myth_wsapi_set_hint(myth_self(),&chpl_data_ptr,&chpl_data_size);
}

int chpl_task_createCommTask(chpl_fn_p fn, void* arg) {
  const size_t stacksize_for_comm_task = 8 * 1024 * 1024;
  myth_thread_option opt;
  myth_thread_t th;
  //chpl_fn_p is defined as "typedef void (*chpl_fn_p)(void*);" in chpltypes.h at line 85.
  //Since return value is always ignored, this cast is legal unless the definition is changed.
  opt.stack_size = stacksize_for_comm_task;
  opt.switch_immediately = 0;
  opt.custom_data_size = sizeof(task_private_data_t);
  opt.custom_data = (void*)&s_def_chpl_data;
  th = myth_create_ex((void*(*)(void*)) fn, arg, &opt);
  assert(th);
  myth_detach(th);
  return 0;
}

void chpl_task_exit(void) {
  //Cleanup tasking layer
  assert(!is_worker_in_cs());
  myth_fini();
  tasking_layer_active = 0;
  chpl_mem_free(s_tld, 0, "");
}

void chpl_task_callMain(void(*chpl_main)(void)) {
  //Call main function
  chpl_main();
}

void chpl_task_addToTaskList(chpl_fn_int_t fid, void* arg, c_sublocid_t subLoc,
    chpl_task_list_p *task_list, int32_t task_list_locale,
    chpl_bool is_begin_stmt, int lineno, chpl_string filename) {
  //Create a new task directly
  myth_thread_option opt;
  myth_thread_t th;
  chpl_bool serial_state = getTaskPrivateData()->serial_state;

  assert(subLoc == 0 || subLoc == c_sublocid_any || subLoc == c_sublocid_curr);

  if (serial_state) {
    (*chpl_ftable[fid])(arg);
    return;
  }
  //Create one task
  //chpl_fn_p is defined as "typedef void (*chpl_fn_p)(void*);" in chpltypes.h at line 85.
  //So this cast is OK unless the definition is changed.
  opt.stack_size = 0;
  opt.switch_immediately = (is_worker_in_cs())?0:1;
  opt.custom_data_size = sizeof(task_private_data_t);
  opt.custom_data = getTaskPrivateData();
  th = myth_create_ex((void*(*)(void*)) chpl_ftable[fid], arg, &opt);
  assert(th);
  myth_detach(th);
}

void chpl_task_processTaskList(chpl_task_list_p task_list) {
  //Nothing to do because chpl_task_list is actually not used.
}

void chpl_task_executeTasksInList(chpl_task_list_p task_list) {
  //Nothing to do because chpl_task_list is actually not used.
}

void chpl_task_freeTaskList(chpl_task_list_p task_list) {
  //Nothing to do because chpl_task_list is actually not used.
}

void chpl_task_startMovedTask(chpl_fn_p fp,
                              void* a,
                              c_sublocid_t subloc,
                              chpl_taskID_t id,
                              chpl_bool serial_state)
{
  myth_thread_t th;
  myth_thread_option opt;
  task_private_data_t chpl_data={subloc, serial_state};

  assert(subloc == 0 || subloc == c_sublocid_any);
  assert(id == chpl_nullTaskID);

  // Do the same as chpl_task_addToTaskList
  opt.stack_size = 0;
  opt.switch_immediately = (is_worker_in_cs())?0:1;
  opt.custom_data_size = sizeof(task_private_data_t);
  opt.custom_data = (void*)&chpl_data;
  th = myth_create_ex((void*(*)(void*)) fp, a, &opt);
  assert(th);
  myth_detach(th);
}

c_sublocid_t chpl_task_getSubloc(void) {
  return 0;
}

void chpl_task_setSubloc(c_sublocid_t subloc) {
  assert(subloc == 0
      || subloc == c_sublocid_any
      || subloc == c_sublocid_curr);
  getTaskPrivateData()->requestedSubloc=subloc;
}

c_sublocid_t chpl_task_getRequestedSubloc(void) {
  return getTaskPrivateData()->requestedSubloc;
}

chpl_taskID_t chpl_task_getId(void) {
  //get task private ID
  return (chpl_taskID_t) myth_self();
}

void chpl_task_yield(void) {
  //yield execution to other tasks
  myth_yield(1);
}

void chpl_task_sleep(int secs) {
  //sleep specified seconds
  sleep(secs);
}

chpl_bool chpl_task_getSerial(void)
{
  //get dynamic serial state
  return getTaskPrivateData()->serial_state;
}

void chpl_task_setSerial(chpl_bool new_state) {
  //set dynamic serial state
  getTaskPrivateData()->serial_state = new_state;
}

c_sublocid_t chpl_task_getNumSublocales(void)
{
#ifdef CHPL_LOCALE_MODEL_NUM_SUBLOCALES
  return CHPL_LOCALE_MODEL_NUM_SUBLOCALES;
#else
  return 0;
#endif
}

uint64_t chpl_task_getCallStackSize(void) {
  //return call stack size
  return s_stack_size;
}

uint32_t chpl_task_getNumQueuedTasks(void) {
  //return the number of queued tasks
  return 0;
}

uint32_t chpl_task_getNumRunningTasks(void) {
  //return the number of running tasks
  return 0;
}

int32_t chpl_task_getNumBlockedTasks(void) {
  //return the number of blocked tasks
  return 0;
}

uint32_t chpl_task_getNumThreads(void) {
  //return the number of threads (excluding a thread for comm)
  return s_num_workers;
}

uint32_t chpl_task_getNumIdleThreads(void) {
  //return the number of idle threads
  return 0;
}
